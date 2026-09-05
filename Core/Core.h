#ifndef CORE_H
#define CORE_H

// For string manipulation, console output, and other basic operations
#include <iostream>
#include <format>
#include <cstdint>

// For delays between processing ticks
#include <thread>
#include <chrono>

// For displaying CPU load
#include <iomanip>

// For unique_ptr
#include <memory>

// For math
#include <cmath>

// For errors macros
#include <cerrno>

enum class PermissionLevel : uint8_t {
    None   = 0,   // No access
    Medium = 127,  // Have acces to work with other components and lower-level rights
    Full   = 255  // Have acces to edit core config, modify permission other components and lower-level rights
};

// Class of base task
class BaseTask;

// Class of base component
class BaseComponent;

class Core
{
public:
    Core(); // Constructor
    ~Core(); // Destructor

    // Disable copying
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;

    bool isLocked() const; // Core is locked?

    int setTPS(uint32_t tps); // Set ticks per second
    int setDebugModeEnabled(bool n_debugModeEnabled); // Set debug mode enabled

    uint32_t getTPS() const; // Get ticks per second
    bool isDebugModeEnabled() const; // Is debug mode enabled?

    void appendComponent(std::unique_ptr<BaseComponent> component); // Append component
    int findComponentIdxByName(std::string& name);
    int processTask(BaseTask& task); // Process task
    int init(); // Init components
    int tick(long long tickCounter); // Tick components
    int cycle(); // Run cycle
private:
    // Core is locked? If false -> edit core configuration permitted
    bool locked = false;

    uint32_t ticksPerSecond = 5; // Ticks per second (TPS)
    bool debugModeEnabled = false; // Debug mode for more information in console

    // Tick duration in milliseconds
    std::chrono::milliseconds target_duration{200};

    // Components list
    std::vector<std::unique_ptr<BaseComponent>> componentsList;
    //std::vector<std::vector<std::unique_ptr<BaseTask>>> componentsTasksList;
};

#endif // CORE_H
