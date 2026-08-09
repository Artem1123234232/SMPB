#ifndef CORE_H
#define CORE_H

// Для работы со строками, вывода в консоль и другие базовые операции
#include <iostream>
#include <format>
#include <cstdint>

// Для задержки между тиками обработки
#include <thread>
#include <chrono>

class Config; // Core configuraton component
class Test3D; // 3D test component

class Core
{
public:
    Core(Config &t_config, Test3D &t_test3D); // Constructor
    ~Core(); // Destructor
    int tick(); // One tick
    int cycle(); // Run cycle
private:
    Config* m_config; // Core configuraton component
    Test3D* m_test3D; // 3D test component

    // By this bool == true, cycle running
    bool work = true;

    // Tick counter
    long long tickCounter = 0;

    // Ticks per second (TPS)
    uint32_t ticksPerSecond = 0;

    // Tick duration in milliseconds
    std::chrono::milliseconds  target_duration{0};

    // Position of the next tick point in time
    std::chrono::steady_clock::time_point next_tick_time;

    void applyConfiguration(); // The function for apply configuration from m_config
};

#endif // CORE_H
