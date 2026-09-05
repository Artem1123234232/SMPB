#ifndef CONFIG_H
#define CONFIG_H

// Базовый компонент
#include "BaseComponent.h"

// Для работы со строками, вывода в консоль и другие базовые операции
#include <iostream>
#include <format>

class Config: public BaseComponent
{
public:
    Config(int argc, char *argv[]);
    uint32_t getTPS();
    bool isDirty();
    int init() override;
private:
    bool dirty = true;
    uint32_t ticksPerSecond = 24;
};

#endif // CONFIG_H
