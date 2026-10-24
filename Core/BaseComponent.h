#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

// For string manipulation, console output, and other basic operations
#include <iostream>
#include <format>
#include <cstdint>

// For errors macros
#include <cerrno>

// For unique_ptr
#include <memory>

// Core class
class Core;

// Class of base task
class BaseTask;

class BaseComponent
{
private:
    Core* core = nullptr;

protected:
    std::string componentName; // Component name

public:
    BaseComponent(std::string name); // Constructor
    virtual ~BaseComponent(); // Destructor

    const std::string& getName() const; // Get component name
    std::string getPrefix() const; // Get component prefix
    int setCore(Core* t_core); // Set core

    // Core api functions
    int Core_setTPS(uint32_t tps); // Set core ticks per second
    int Core_setDebugModeEnabled(bool n_debugModeEnabled); // Set core debug mode enabled
    void Core_stop(); // Stop core
    uint32_t Core_getTPS() const; // Get core ticks per second
    bool Core_isDebugModeEnabled() const; // Is debug mode enabled in core?
    void Core_appendComponent(std::unique_ptr<BaseComponent> component); // Append Component to core


    int sendTask(std::unique_ptr<BaseTask> task, const std::string& workerName); // Send task into core
    virtual int processTask(std::unique_ptr<BaseTask> task);

    virtual int init(); // Init function
    virtual int tick(long long tickCounter); // Tick function
    virtual void death(); // Death function
};

#endif // BASECOMPONENT_H
