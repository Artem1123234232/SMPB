#ifndef CONFIG_H
#define CONFIG_H

// Для работы со строками, вывода в консоль и другие базовые операции
#include <iostream>
#include <format>

class Config
{
public:
    Config(int argc, char *argv[]);
    int getTPS();
    bool isDirty();
private:
    bool dirty = true;
    int ticksPerSecond = 24;
};

#endif // CONFIG_H
