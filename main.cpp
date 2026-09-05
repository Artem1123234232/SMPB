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
//#include "Console/TuiMenu.h"


int main(int argc, char *argv[]) {
    //TuiMenu tuiMenu = TuiMenu();

    Core core = Core();
    core.appendComponent(std::make_unique<Config>(argc, argv));

    core.init();

    return core.cycle();
}