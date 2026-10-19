#include "BaseComponent.h"

// Core class
#include "Core.h"

// Class of base task
#include "BaseTask.h"

BaseComponent::BaseComponent(std::string name) : componentName(name) {
    // Constructor
    std::clog << "[" << componentName << "] " << "My constructor executed!\n";
}

BaseComponent::~BaseComponent() = default; // Destructor

const std::string& BaseComponent::getName() const {
    // Get component name
    return componentName;
}

const std::string BaseComponent::getPrefix() const {
    // Get component prefix
    return "[" + componentName + "] ";
}

int BaseComponent::setCore(Core* t_core) {
    // Set core
    core = t_core;
    return 0;
}

// Core api functions
int BaseComponent::Core_setTPS(uint32_t tps) {
    // Set core ticks per second
    return(core->setTPS(tps)); 
} 
int BaseComponent::Core_setDebugModeEnabled(bool n_debugModeEnabled) {
    // Set core debug mode enabled
    return(core->setDebugModeEnabled(n_debugModeEnabled));
} 
void BaseComponent::Core_stop() {
    // Stop core
    core->stop();
}
uint32_t BaseComponent::Core_getTPS() const {
    // Get core ticks per second
    return(core->getTPS());
} 
bool BaseComponent::Core_isDebugModeEnabled() const {
    // Is debug mode enabled in core?
    return(core->isDebugModeEnabled()); 
}
void BaseComponent::Core_appendComponent(std::unique_ptr<BaseComponent> component) {
    // Append Component to core
    core->appendComponent(std::move(component));
}
int BaseComponent::sendTask(std::unique_ptr<BaseTask> task, std::string workerName) {
    // Send task into core
    if (!task) return -EINVAL;
    task->setCreatorIdx(core->findComponentIdxByName(componentName));
    task->setWorkerIdx(core->findComponentIdxByName(workerName));
    return core->processTask(std::move(task));
}
int BaseComponent::proccessTask(std::unique_ptr<BaseTask> task) {
    // Proccess task function
    std::cerr << getPrefix() << "Error in " << componentName << "::processTask: This function has not been initialized." << std::endl ;
    return -ENOSYS;
}

int BaseComponent::init() {
    // Init function
    if (Core_isDebugModeEnabled()) std::clog << getPrefix() << "I\'m init!\n";
    return 0;
}

int BaseComponent::tick(long long tickCounter) {
    // Tick function
    if (Core_isDebugModeEnabled()) std::clog << getPrefix() << "I\'m tick!.\n";
    return 0;
}

void BaseComponent::death() {
    // Death function
    if (Core_isDebugModeEnabled()) std::clog << getPrefix() << "I\'m death!\n";
}