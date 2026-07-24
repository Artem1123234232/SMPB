#include "config.h"

#ifdef DEBUG_CONSOLE_ENABLE
#include "debugconsole.h"

DebugConsole::DebugConsole(QObject *parent) : BaseConsole(parent) {
    // Здесь можно инициализировать что-то при создании объекта
}
#endif