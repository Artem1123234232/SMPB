// Core class
#include "Core.h"

// Class of base task
#include "BaseTask.h"

// Class of base component
#include "BaseComponent.h"

#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_RST "\033[0m"

Core::Core() {
    // Constructor
}

Core::~Core() {
    // Destructor
}

bool Core::isLocked() const {
    // Core is locked?
    return locked;
}

int Core::setTPS(uint32_t tps) {
    // Set ticks per second
    if (isLocked()) {
        return -EPERM;
    }
    ticksPerSecond = tps;

    // Tick duration in milliseconds
    target_duration = std::chrono::milliseconds( (ticksPerSecond > 0) ? (1000 / ticksPerSecond) : 0 );
    return 0;
}
int Core::setDebugModeEnabled(bool n_debugModeEnabled) {
    // Set debug mode enabled
    if (isLocked()) {
        return -EPERM;
    }
    debugModeEnabled = n_debugModeEnabled;
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
    componentsList.push_back(std::move(component));
}

int Core::findComponentIdxByName(std::string& name) {
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

int Core::processTask(BaseTask& task) {
    // Process task
    int creatorIdx = task.getCreatorIdx();
    int workerIdx = task.getWorkerIdx();
    if (creatorIdx >= 0 && creatorIdx < componentsList.size() && workerIdx >= 0 && workerIdx < componentsList.size()) {
        int errorCode = componentsList[workerIdx]->proccessTask(task);
        if (errorCode != 0){
            std::cerr << "Error in Core::processTask: " << componentsList[workerIdx]->getName() << "::processTask return error code " << std::to_string(errorCode) << ". Task from " << componentsList[creatorIdx]->getName() << " to " << componentsList[workerIdx]->getName() << " with name " << task.getName() << std::endl ;
            return errorCode;
        }
        return 0;
    }
    return -ENOENT;
}

int Core::init(){
    for (auto it = componentsList.begin(); it != componentsList.end(); ) {
        int errorCode1 = (*it)->setCore(this);

        if (errorCode1 != 0) {
            std::cerr << "Critical Error in Core::init: " << (*it)->getName() << "::setCore return error code " << std::to_string(errorCode1) << ", delete this component" << std::endl;
            it = componentsList.erase(it);
        }
        else {
            int errorCode2 = (*it)->init();
            if (errorCode2 != 0) {
                std::cerr << "Critical Error in Core::init: " << (*it)->getName() << "::init return error code " << std::to_string(errorCode2) << ", delete this component" << std::endl;
                it = componentsList.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    return 0;
}

int Core::tick(long long tickCounter){
    for (auto it = componentsList.begin(); it != componentsList.end(); ) {
        int errorCode = (*it)->tick(tickCounter);

        if (errorCode != 0) {
            std::cerr << "Critical Error in Core::tick: " << (*it)->getName() << "::tick return error code " << std::to_string(errorCode) << ", delete this component" << std::endl;
            it = componentsList.erase(it);
        } else {
            ++it;
        }
    }

    return 0;
}

int Core::cycle(){
    // Run cycle
    bool error = false;

    long long tickCounter = 0;

    // Position of the next tick point in time
    std::chrono::steady_clock::time_point next_tick_time = std::chrono::steady_clock::now();

    while (1){
        auto start_time = std::chrono::steady_clock::now();

        int errorCode = tick(tickCounter);

        if (errorCode != 0) {
            std::cerr << "Critical Error in Core::cycle: Core::tick return error code " << std::to_string(errorCode) << std::endl;
            return errorCode;
        }

        auto end_time = std::chrono::steady_clock::now();

        std::chrono::duration<double, std::milli> duration = end_time - start_time;

        std::clog << "Tick #" << tickCounter
                  << " end; Expected TPS: " << ticksPerSecond
                  << "; Tick duration: " << duration.count() << " ms, " << duration.count() * ticksPerSecond / 1000.0f << " ticks"
                  << " (util: " << std::fixed << std::setprecision(3)
                  << (duration.count() / 1000.0) * ticksPerSecond * 100.0 << "%)\n\n";

        tickCounter++;

        next_tick_time += target_duration;

        auto now_time = std::chrono::steady_clock::now();
        if (now_time >= next_tick_time) {
            std::chrono::duration<double, std::milli> lag = now_time - next_tick_time;
            double passTicks = lag.count() * ticksPerSecond / 1000.0f;

            next_tick_time += std::chrono::duration_cast<std::chrono::steady_clock::duration>( target_duration * passTicks );

            std::cerr << ANSI_YELLOW << "Warning: " << passTicks << " ticks are being skipped. Performance issues!" << ANSI_RST << "\n";
        }

        std::this_thread::sleep_until(next_tick_time); // Sleep
    }
    std::clog << "Program finished cleanly." << std::endl;
    return 0;
}