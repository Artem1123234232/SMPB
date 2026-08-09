#include "Config.h"

Config::Config(int argc, char *argv[]) {
    // Constructor
    std::clog << "Config constructor runned..." << std::endl;

    std::clog << "Args:" << std::endl;
    for (int i = 0; i < argc; i++){
        std::clog << "\t" << std::to_string(i) << ": " << argv[i] << std::endl;
    }

    if (ticksPerSecond <= 0) {
        std::cerr << "Critical Error: Ticks per second <= 0!" << std::endl;
    }
}

int Config::getTPS(){
    return(ticksPerSecond);
}

bool Config::isDirty(){
    if (dirty){
        dirty = false;
        return true;
    }
    return false;
}