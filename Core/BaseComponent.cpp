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

std::string BaseComponent::getName() const {
    // Get component name
    return componentName;
}

std::string BaseComponent::getPrefix() const {
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
int BaseComponent::sendTask(BaseTask& task, std::string workerName) {
    // Send task into core
    task.setCreatorIdx(core->findComponentIdxByName(componentName));
    task.setWorkerIdx(core->findComponentIdxByName(workerName));
    return core->processTask(task);
}

int BaseComponent::proccessTask(BaseTask& task) {
    // Proccess task function
    std::cerr << "Error in " << componentName << "::processTask: This function has not been initialized." << std::endl ;
    return -ENOSYS;
}

int BaseComponent::init() {
    // Init function
    std::clog << getPrefix() << "I\'m init!\n";
    return 0;
}

int BaseComponent::tick(long long tickCounter) {
    // Tick function
    std::clog << getPrefix() << "I\'m tick!.\n";
    return 0;
}