// Core class
#include "Core.h"

// Class of base task
#include "BaseTask.h"

// Class of base component
#include "BaseComponent.h"

#include <csignal>
#include <exception>

#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_RST "\033[0m"

namespace {

std::atomic<int> receivedSignal{0};
static_assert(std::atomic<int>::is_always_lock_free);

void coreSignalHandler(int signal) {
    receivedSignal.store(signal);
    std::signal(signal, SIG_DFL);
}

class SignalHandlersGuard {
public:
    SignalHandlersGuard()
        : previousSigint(std::signal(SIGINT, coreSignalHandler)),
          previousSigterm(std::signal(SIGTERM, coreSignalHandler)) {}

    ~SignalHandlersGuard() {
        if (previousSigint != SIG_ERR) std::signal(SIGINT, previousSigint);
        if (previousSigterm != SIG_ERR) std::signal(SIGTERM, previousSigterm);
    }

    SignalHandlersGuard(const SignalHandlersGuard&) = delete;
    SignalHandlersGuard& operator=(const SignalHandlersGuard&) = delete;

private:
    void (*previousSigint)(int);
    void (*previousSigterm)(int);
};

template <typename Function>
int callComponent(const BaseComponent& component, const char* method, Function&& function) {
    try {
        return function();
    } catch (const std::exception& e) {
        std::cerr << "Critical Error in Core: " << component.getName() << "::" << method << " threw exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Critical Error in Core: " << component.getName() << "::" << method << " threw unknown exception" << std::endl;
    }
    return -ENOTRECOVERABLE;
}

}

Core::Core() {
    // Constructor
}

Core::~Core() {
    // Destructor
    destroyComponents();
}

int Core::setTPS(uint32_t tps) {
    // Set ticks per second
    ticksPerSecond = tps;
    // Tick duration in milliseconds
    target_duration = std::chrono::duration<double, std::milli>( (ticksPerSecond > 0) ? (1000.0 / ticksPerSecond) : 0.0 );
    return 0;
}
int Core::setDebugModeEnabled(bool n_debugModeEnabled) {
    // Set debug mode enabled
    debugModeEnabled = n_debugModeEnabled;
    return 0;
}

int Core::stop() {
    // Stop work
    stopFlag = true;
    return 0;
}

uint32_t Core::getTPS() const {
    // Get ticks per second
    return ticksPerSecond;
}
bool Core::isDebugModeEnabled() const {
    // Is debug mode enabled?
    return debugModeEnabled;
}

void Core::appendComponent(std::unique_ptr<BaseComponent> component) {
    // Append Component
    if (!component) {
        std::cerr << "Error in Core::appendComponent: component is nullptr" << std::endl;
        return;
    }
    componentsQueue.push_back(std::move(component));
}

int Core::findComponentIdxByName(const std::string& name) const {
    // Find component index by name
    auto it = std::find_if(componentsList.begin(), componentsList.end(),
        [&name](const std::unique_ptr<BaseComponent>& component) {
            return component && component->getName() == name; // Проверка на nullptr + сравнение строк
        });

    if (it != componentsList.end()) {
        return std::distance(componentsList.begin(), it);
    }
    return -1; 
}

int Core::findComponentIdxByPtr(const BaseComponent* component) const {
    if (!component) return -1;
    for (size_t i = 0; i < componentsList.size(); i++) {
        if (componentsList[i].get() == component) return static_cast<int>(i);
    }
    return -1;
}

int Core::processTask(std::unique_ptr<BaseTask> task) {
    // Process task
    int creatorIdx = task->getCreatorIdx();
    int workerIdx = task->getWorkerIdx();
    uint32_t taskIdx = task->getIdx();
    const int n = static_cast<int>(componentsList.size());
if (creatorIdx >= 0 && creatorIdx < n && workerIdx >= 0 && workerIdx < n && componentsList[creatorIdx] && componentsList[workerIdx]) {
        BaseComponent& worker = *componentsList[workerIdx];
        int errorCode = callComponent(worker, "processTask", [&] { return worker.processTask(std::move(task)); });
        if (errorCode != 0){
            std::cerr << "Error in Core::processTask: " << componentsList[workerIdx]->getName() << "::processTask return error code " << errorCode << ". Task from " << componentsList[creatorIdx]->getName() << " to " << componentsList[workerIdx]->getName() << " with idx " << taskIdx << std::endl;
            return errorCode;
        }
        return 0;
    }
    std::cerr << "Error in Core::processTask: The creator Idx or worker Idx is outside the bounds of the component list. Creator idx: " << creatorIdx << ". Worker idx: " << workerIdx << std::endl;
    return -ENOENT;
}

int Core::processComponentsQueue(){
    while (!componentsQueue.empty()) {
        std::vector<std::unique_ptr<BaseComponent>> pending = std::move(componentsQueue);
        componentsQueue.clear();

        for (auto& component : pending) {
            int errorCode1 = component->setCore(this);

            if (errorCode1 != 0) {
                std::cerr << "Critical Error in Core::processComponentsQueue: " << component->getName() << "::setCore return error code " << std::to_string(errorCode1) << ", delete this component" << std::endl;
                continue;
            }

            componentsList.push_back(std::move(component));
            BaseComponent* initialized = componentsList.back().get();

            int errorCode2 = callComponent(*initialized, "init", [&] { return initialized->init(); });
            if (errorCode2 != 0) {
                std::cerr << "Critical Error in Core::processComponentsQueue: " << initialized->getName() << "::init return error code " << std::to_string(errorCode2) << ", delete this component" << std::endl;
                callComponent(*initialized, "death", [&] { initialized->death(); return 0; });
                componentsList.pop_back();
            }
        }
    }

    return 0;
}

void Core::destroyComponents() {
    for (auto& component : componentsList) {
        if (!component) continue;
        callComponent(*component, "death", [&] { component->death(); return 0; });
        component.reset();
    }
    componentsList.clear();

    while (!componentsQueue.empty()) {
        std::vector<std::unique_ptr<BaseComponent>> pending = std::move(componentsQueue);
        componentsQueue.clear();
    }
}

int Core::init(){
    return processComponentsQueue();
}

int Core::tick(long long tickCounter){
    processComponentsQueue();

    for (auto it = componentsList.begin(); it != componentsList.end(); ) {
        BaseComponent& component = **it;
        int errorCode = callComponent(component, "tick", [&] { return component.tick(tickCounter); });

        if (errorCode != 0) {
            std::cerr << "Critical Error in Core::tick: " << (*it)->getName() << "::tick return error code " << std::to_string(errorCode) << ", delete this component" << std::endl;
            callComponent(component, "death", [&] { component.death(); return 0; });
            it = componentsList.erase(it);
        } else {
            ++it;
        }
    }

    return 0;
}

int Core::cycle(){
    // Run cycle
    long long tickCounter = 0;
    skippedTicksAccumulator = 0.0;

    receivedSignal.store(0);
    SignalHandlersGuard signalHandlersGuard;

    // Position of the next tick point in time
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double, std::nano>> next_tick_time = std::chrono::steady_clock::now();

    while (!stopFlag && receivedSignal.load() == 0){
        auto start_time = std::chrono::steady_clock::now();

        int errorCode = tick(tickCounter);

        if (errorCode != 0) {
            std::cerr << "Critical Error in Core::cycle: Core::tick return error code " << std::to_string(errorCode) << std::endl;
            return errorCode;
        }

        auto end_time = std::chrono::steady_clock::now();

        std::chrono::duration<double, std::milli> duration = end_time - start_time;

        if (debugModeEnabled) {
            std::clog << std::format("Tick #{} end; Expected TPS: {}; Tick duration: {:.3f} ms, {:.3f} ticks (util: {:.3f}%)\n\n",
                tickCounter, ticksPerSecond, duration.count(),
                duration.count() * ticksPerSecond / 1000.0,
                duration.count() / 1000.0 * ticksPerSecond * 100.0);
        }

        tickCounter++;

        next_tick_time += target_duration;

        auto now_time = std::chrono::steady_clock::now();
        if (now_time >= next_tick_time) {
            std::chrono::duration<double, std::milli> lag = now_time - next_tick_time;
            double passTicks = lag.count() * ticksPerSecond / 1000.0;

            next_tick_time += target_duration * passTicks;

            skippedTicksAccumulator += passTicks;
            if (skippedTicksAccumulator >= 1.0) {
                long long skippedTicks = static_cast<long long>(skippedTicksAccumulator);
                skippedTicksAccumulator -= skippedTicks;

                std::cerr << ANSI_YELLOW << "Warning: " << skippedTicks << " ticks are being skipped. Performance issues!" << ANSI_RST << "\n";
            }
        }

        std::this_thread::sleep_until(next_tick_time); // Sleep
    }

    // Stopping
    int stopSignal = receivedSignal.load();
    if (stopSignal != 0) std::clog << "Core received signal " << stopSignal << ", stopping" << std::endl;

    destroyComponents();
    stopFlag = false;
    std::clog << "Core is stop" << std::endl;
    return 0;
}