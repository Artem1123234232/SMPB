#ifndef TUIMENU_H
#define TUIMENU_H

// Для работы со строками, списками, вывода в консоль и другие базовые операции
#include <iostream>
#include <format>
#include <utility>
#include <array>
#include <vector>
#include <algorithm>

// Для математики
#include <cmath>
#include <cfloat> // Для FLT_MAX

// 3D Движок
#include "Test3D.h"

// Консольная палитра
//#ifndef RGB
//const std::string PALETTE = " .:!/r(l1Z4H9W8$@";
//const std::string PALETTE = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
//const std::string PALETTE = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$#FX%WM&8@KOB0S523456789";
//#endif

class TuiMenu
{
public:
    TuiMenu(); // Constructor
    ~TuiMenu(); // Destructor

    Test3D test3D = Test3D(true); // 3D Engie

    void tick(long long tickCounter, int ticksPerSecond);
private:
    std::pair<int, int> consoleSize;
    std::vector<std::string> oldStringsBuffer;
    std::pair<int, int> oldConsoleSize;

    bool eggRotation = false;
};

#endif // TUIMENU_H
