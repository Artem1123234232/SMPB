#include "Core.h"
#include "Config.h" // Core configuraton component
#include "../Console/TuiMenu.h" // Tui Menu component


Core::Core(Config &t_config, TuiMenu &t_tuiMenu) {
    // Constructor
    std::clog << "Core constructor runned..." << std::endl;

    m_config = &t_config;
    m_tuiMenu = &t_tuiMenu;

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
    if (m_tuiMenu) m_tuiMenu->tick(tickCounter, ticksPerSecond);

    return 0;
}

int Core::cycle(){
    // Run cycle
    bool error = false;
    while (work){
        if (!error){
            if (m_config && m_config->isDirty()) applyConfiguration();

            // Засекаем время начала
            auto start_time = std::chrono::steady_clock::now();
            int tickErrorCode = tick();
            // При ошибке выходим из цикла
            if (tickErrorCode != 0) {
                std::cerr << "Critical Error: Core::tick return error code " << std::to_string(tickErrorCode) << std::endl;
                return tickErrorCode;
            }
            // Засекаем время окончания
            auto end_time = std::chrono::steady_clock::now();
            // Вычисляем разницу в миллисекундах (с дробной частью)
            std::chrono::duration<double, std::milli> duration = end_time - start_time;
            // Выводим результат в лог
            std::clog << "Tick #" << tickCounter
                      << " TPS: " << ticksPerSecond
                      << ", duration: " << duration.count() << " ms"
                      << " (util: " << std::fixed << std::setprecision(3)
                      << (duration.count() / 1000.0) * ticksPerSecond * 100.0 << "%)"
                      << std::endl;
        }
        tickCounter++;

        next_tick_time += target_duration;
        if (std::chrono::steady_clock::now() >= next_tick_time) {
            if (!error) {
                std::cerr << "Critical Error: Tick sleep error, no sleeping\n" << std::endl;
            }
            error = true;
            //std::this_thread::yield(); // Уступаем квант времени другим потокам
        } else {
            error = false;
            std::this_thread::sleep_until(next_tick_time); // Спим штатно
        }
    }
    std::clog << "Program finished cleanly." << std::endl;
    return 0;
}