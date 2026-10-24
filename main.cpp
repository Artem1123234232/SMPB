//#include <QCoreApplication>

//#include "smpbcore.h"

#include <iostream>
#include <format>
#include <cstdint>

// Математика
#include <cmath>

// Ядро и его компоненты
#include <smpb/Config.h>
#include <smpb/Core.h>

int main(int argc, char *argv[]) {
    Core core = Core();
    core.setDebugModeEnabled(true);
    core.appendComponent(std::make_unique<Config>(argc, argv));

    core.init();

    return core.cycle();
}
