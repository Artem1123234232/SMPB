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

int Config::init() {
    auto config = readFile("core.config");

    switch (config.second) {
        case 0:
            break;
        case ENOENT:
            std::cerr << "Critical Error: Config::init, in readFile(\"core.config\"), file core.config not found!" << std::endl;
            break;
        default:
            std::cerr << "Unexcepted Critical Error: Config::init, in readFile(\"core.config\")!" << std::endl;
            break;
    }

    std::vector<std::pair<std::string, std::string>> commands;
    std::string command[2] = {"", ""};
    std::string s;
    uint32_t p = 0;

    config.first.push_back('\n');

    for (char& ch: config.first) {
        if (ch == ' ' || ch == '\n') {
            if (p < 2) command[p] = s;
            if (!s.empty()) p++;
            s = "";
        }
        else {
            s += ch;
        }
        
        if (ch == '\n') {
            if (!command[0].empty() && !command[1].empty()) commands.push_back({command[0], command[1]});
            s = ""; p = 0; command[0] = ""; command[1] = "";
        }
    }

    for (auto& command: commands) {
        std::clog << getPrefix() << command.first << " " << command.second << " - ";
        if (command.first == "SET_TPS") {
            uint32_t tps = 0;
            const auto& s = command.second;

            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), tps);

            if (ec != std::errc{} || ptr != s.data() + s.size() || Core_setTPS(tps) != 0) {
                std::clog << "no valid value!";
            } else {
                std::clog << "executed!";
            }
        } else {
            std::clog << "unknow command!";
        }
        std::clog << "\n";
    }
    return 0;
}