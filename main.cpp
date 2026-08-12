//#include <QCoreApplication>

//#include "smpbcore.h"

#include <iostream>
#include <format>
#include <cstdint>

// Математика
#include <cmath>

// Ядро и его компоненты
#include "Core/Config.h"
#include "Core/Core.h"
#include "Console/TuiMenu.h"


int main(int argc, char *argv[]) {
    Config config = Config(argc, argv);
    TuiMenu tuiMenu = TuiMenu();

    Core core = Core(config, tuiMenu);

    return core.cycle();
}