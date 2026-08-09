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
#include "Core/Test3D.h"


int main(int argc, char *argv[]) {
    Config config = Config(argc, argv);
    Test3D test3D = Test3D();

    Core core = Core(config, test3D);

    return core.cycle();
}