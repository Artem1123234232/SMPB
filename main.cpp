#include <QCoreApplication>

#include <QProcess>
#include <QFileInfo>
#include <QStandardPaths> // Заменили QProcess::findExecutable на этот класс
#include <QDebug>
#include <iostream>

#include "consoleparser.h"
#include "smpbcore.h"

#ifdef _WIN32
#include <windows.h>
#include <cstdio>
#else
#include <sys/ioctl.h>
#include <unistd.h> // Для isatty на Linux
#endif

// Единая функция инициализации консоли для всех ОС.
// Возвращает true, если нужно продолжать выполнение,
// и false, если в Linux запустился терминал и текущий процесс нужно завершить.
bool initConsole(int argc, char *argv[]) {
#ifdef _WIN32
    // --- Логика для Windows ---
    // Пытаемся прикрепиться к консоли родителя (если запустили из cmd/PowerShell)
    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        // Если родительской консоли нет (запустили кликом), создаем свою
        if (AllocConsole()) {
            FILE* fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
            freopen_s(&fp, "CONOUT$", "w", stderr);
            freopen_s(&fp, "CONIN$", "r", stdin);
            std::ios::sync_with_stdio();
        }
    }
    return true; // В Windows никогда не завершаем процесс досрочно
#else
    // --- Логика для Linux ---
    // Если вывод уже идет в терминал, ничего делать не нужно
    if (isatty(STDOUT_FILENO)) {
        return true;
    }

    // Защита от бесконечного цикла перезапусков
    QString marker = "--restarted-in-terminal";
    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == marker) {
            return true;
        }
    }

    // Получаем путь к нашему исполняемому файлу
    QString appPath = QFileInfo(QString::fromLocal8Bit(argv[0])).absoluteFilePath();

    // Собираем аргументы командной строки
    QStringList appArgs;
    for (int i = 1; i < argc; ++i) {
        appArgs << QString::fromLocal8Bit(argv[i]);
    }
    appArgs << marker;

    // Команда для выполнения: запускает программу, а после завершения ждет Enter
    QString command = QString("\"%1\" %2; echo -e '\\n[Программа завершена. Нажмите Enter для выхода]'; read")
                          .arg(appPath, appArgs.join(" "));

    // Список популярных терминалов в Linux
    QStringList terminals = {"x-terminal-emulator", "gnome-terminal", "konsole", "xfce4-terminal", "alacritty", "xterm"};
    bool started = false;

    for (const QString &term : terminals) {
        // Безопасный поиск утилиты через QStandardPaths (работает во всех версиях Qt6)
        if (!QStandardPaths::findExecutable(term).isEmpty()) {
            QStringList termArgs;

            if (term == "gnome-terminal" || term == "xfce4-terminal") {
                termArgs << "--" << "sh" << "-c" << command;
            } else { // konsole, x-terminal-emulator, xterm
                termArgs << "-e" << "sh" << "-c" << command;
            }

            started = QProcess::startDetached(term, termArgs);
            if (started) break;
        }
    }

    if (started) {
        return false; // Терминал успешно открыт, текущий невидимый процесс закрываем
    }

    std::cerr << "Критическая ошибка: не найден эмулятор терминала в Linux!" << std::endl;
    return true; // Если терминалов в системе нет, продолжаем работать "вслепую"
#endif
}

// Структура для возврата размера консоли.
struct ConsoleSize {
    int cols = 80; // Значение по умолчанию (ширина)
    int rows = 24; // Значение по умолчанию (высота)
};

// Единая функция получения размера консоли для всех ОС.
ConsoleSize getConsoleSize() {
    ConsoleSize size;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        // Вычисляем ширину и высоту видимого окна
        size.cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        size.rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
#else
    struct winsize w;
    // Запрашиваем размеры терминала у стандартного вывода
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        size.cols = w.ws_col;
        size.rows = w.ws_row;
    }
#endif
    return size;
}

int main(int argc, char *argv[]) {
    // Инициализация консоли
    if (!initConsole(argc, argv)) return 0;

    QCoreApplication a(argc, argv);

    SmpbCore smpbCore;

    // Запуск таймеров ядра SMPB
    smpbCore.startTimers();

    // Запуск цикла обработки событий Qt (без него QTimer не работает)
    return a.exec();
}