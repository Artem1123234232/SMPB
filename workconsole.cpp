#include "config.h"

#ifdef WORK_CONSOLE_ENABLE
#include "workconsole.h"

WorkConsole::WorkConsole(QObject *parent) : BaseConsole(parent) {
    // Задаем параметры
    setName("WorkConsole");
}
#endif