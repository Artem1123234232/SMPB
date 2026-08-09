#include "Core.h"
#include "Config.h" // Core configuraton component
#include "Test3D.h" // 3D Test


Core::Core(Config &t_config, Test3D &t_test3D) {
    // Constructor
    std::clog << "Core constructor runned..." << std::endl;

    m_config = &t_config;
    m_test3D = &t_test3D;

    // Position of the next tick point in time
    next_tick_time = std::chrono::steady_clock::now();
}

Core::~Core() {
    // Destructor
}

void Core::applyConfiguration(){
    // The function for apply configuration from m_config

    if (m_config) ticksPerSecond = m_config->getTPS();

    // Tick duration in milliseconds
    target_duration = std::chrono::milliseconds( (ticksPerSecond > 0) ? (1000 / ticksPerSecond) : 0 );
}

int Core::tick(){
    // Засекаем время начала
    auto start_time = std::chrono::steady_clock::now();

    std::cout << "\033[H";
    if (m_test3D) m_test3D->tick(tickCounter, ticksPerSecond);

    // Засекаем время окончания
    auto end_time = std::chrono::steady_clock::now();

    // Вычисляем разницу в миллисекундах (с дробной частью)
    std::chrono::duration<double, std::milli> duration = end_time - start_time;

    // Выводим результат в лог
    std::clog << "Tick #" << tickCounter << " TPS: "
              << ticksPerSecond << ", duration: "
              << duration.count() << " ms" << std::endl;

    tickCounter++;

    return 0;
}

int Core::cycle(){
    // Run cycle
    while (work){
        if (m_config && m_config->isDirty()) applyConfiguration();
        int tickErrorCode = tick();

        if (tickErrorCode != 0) {
            std::cerr << "Critical Error: Core::tick return error code " << std::to_string(tickErrorCode) << std::endl;
            return tickErrorCode;
        }

        next_tick_time += target_duration;
        if (std::chrono::steady_clock::now() >= next_tick_time) {
            std::cerr << "Critical Error: Tick sleep error, no sleeping\n" << std::endl;
            std::this_thread::yield(); // Уступаем квант времени другим потокам
        } else {
            std::this_thread::sleep_until(next_tick_time); // Спим штатно
        }
    }
    std::clog << "Program finished cleanly." << std::endl;
    return 0;
}