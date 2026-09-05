#include "BaseComponent.h"

// Core class
#include "Core.h"

// Class of base task
#include "BaseTask.h"

BaseComponent::BaseComponent(std::string name) : componentName(name) {
    // Constructor
    std::clog << componentName << " constructor executed...\n";
}

BaseComponent::~BaseComponent() = default; // Destructor

std::string BaseComponent::getName() {
    // Get component name
    return componentName;
}

int BaseComponent::setCore(Core* t_core) {
    // Set core
    core = t_core;
    return 0;
}

int BaseComponent::proccessTask(BaseTask& task) {
    // Proccess task function
    std::cerr << "Error in " << componentName << "::processTask: This function has not been initialized." << std::endl ;
    return -ENOSYS;
}

int BaseComponent::init() {
    // Init function
    std::clog << componentName << " initialization...\n";
    return 0;
}

int BaseComponent::tick(long long tickCounter) {
    // Tick function
    std::clog << componentName << " tick...\n";
    return 0;
}

int BaseComponent::reload() {
    // Reload function
    std::clog << componentName << " reload...\n";
    return 0;
}