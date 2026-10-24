#ifndef CONFIG_H
#define CONFIG_H

// Базовый компонент
#include "BaseComponent.h"

// Для работы со строками, вывода в консоль и другие базовые операции
#include <iostream>
#include <format>

// For file reading
#include "tools.h"

class Config: public BaseComponent
{
public:
    Config(int argc, char *argv[]);
    int init() override;
};

#endif // CONFIG_H
