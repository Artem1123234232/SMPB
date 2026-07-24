#include <QCoreApplication>

#include "consoleapi.h"
#include "smpbcore.h"


int main(int argc, char *argv[]) {
    // Инициализация консоли
    if (!initConsole(argc, argv)) return 0;

    QCoreApplication a(argc, argv);

    SmpbCore smpbCore;

    // Запуск цикла обработки событий Qt (без него QTimer не работает)
    return a.exec();
}