#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

// For string manipulation, console output, and other basic operations
#include <iostream>
#include <format>
#include <cstdint>

// For errors macros
#include <cerrno>

// Core class
class Core;

// Class of base task
class BaseTask;

class BaseComponent
{
protected:
    std::string componentName; // Component name
    Core* core;

public:
    BaseComponent(std::string name); // Constructor
    virtual ~BaseComponent(); // Destructor

    std::string getName(); // Get component name
    int setCore(Core* t_core); // Set core

    int proccessTask(BaseTask& task);

    virtual int init(); // Init function
    virtual int tick(long long tickCounter); // Tick function
    virtual int reload(); // Reload function
};

#endif // BASECOMPONENT_H
