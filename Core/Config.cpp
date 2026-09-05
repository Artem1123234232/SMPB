#include "Config.h"

// Core class
#include "Core.h"

Config::Config(int argc, char *argv[]) : BaseComponent("Config") {
    // Constructor
    std::clog << "Args:" << std::endl;
    for (int i = 0; i < argc; i++){
        std::clog << "\t" << std::to_string(i) << ": " << argv[i] << std::endl;
    }

    if (ticksPerSecond <= 0) {
        std::cerr << "Critical Error: Ticks per second <= 0!" << std::endl;
    }
}

uint32_t Config::getTPS() {
    return(ticksPerSecond);
}

bool Config::isDirty() {
    if (dirty){
        dirty = false;
        return true;
    }
    return false;
}

int Config::init() {
    int errorCode = 0;
    if (core){
        errorCode = core->setTPS(getTPS());
        if (errorCode == -EPERM) {
            std::cerr << "Critical Error: Config::init insufficient privileges to apply the configuration" << std::endl;
            return errorCode;
        }
    }
    else if (errorCode != 0) {
        std::cerr << "Critical Error: Config::init core is nullptr" << std::endl;
        return -404;
    }
    return errorCode;
}