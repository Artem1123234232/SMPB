#pragma once

#include <QString>
#include <climits>

#ifndef CONFIG_H
#define CONFIG_H

#define BASE_CONSOLE_ENABLE
#define WORK_CONSOLE_ENABLE
#define DEBUG_CONSOLE_ENABLE

#define DEBUG_FUNC_ENABLE
#define SUPER_DEBUG_MODE

#define ANSI_RED QString("\033[31m")
#define ANSI_GREEN QString("\033[32m")
#define ANSI_BLUE QString("\033[34m")
#define ANSI_YELLOW QString("\033[33m")
#define ANSI_PURPLE QString("\033[35m")
#define ANSI_CYAN QString("\033[36m")
#define ANSI_RST QString("\033[0m")

#endif // CONFIG_H