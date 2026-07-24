#include "consoleapi.h"

ConsoleReader::ConsoleReader(QObject *parent) : QObject(parent) {
    #ifdef _WIN32
    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    // Отключаем строчный ввод (LINE_INPUT) и эхо
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

    notifier = new QWinEventNotifier(hStdin, this);
    connect(notifier, &QWinEventNotifier::activated, this, &ConsoleReader::onInputEvent);
    #else
    tcgetattr(STDIN_FILENO, &oldSettings);
    termios newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    notifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read, this);
    connect(notifier, &QSocketNotifier::activated, this, &ConsoleReader::onInputEvent);
    #endif
}

ConsoleReader::~ConsoleReader() {
    // Деструктор, нужен чтобы в линукс правильно очистить объект
    #ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    #endif
}

void ConsoleReader::onInputEvent(){
    #ifdef _WIN32

    INPUT_RECORD irInBuf[128];
    DWORD cNumRead;

    if (PeekConsoleInput(hStdin, irInBuf, 128, &cNumRead) && cNumRead > 0) {
        if (ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead)) {
            for (DWORD i = 0; i < cNumRead; i++) {
                if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown) {
                    WORD vKey = irInBuf[i].Event.KeyEvent.wVirtualKeyCode;
                    WCHAR ch = irInBuf[i].Event.KeyEvent.uChar.UnicodeChar;

                    // Перехватываем стрелочки на Windows и маппим их в ANSI-последовательности
                    if (vKey == VK_UP)    { emit keyReceived("\033[A"); continue; }
                    if (vKey == VK_DOWN)  { emit keyReceived("\033[B"); continue; }
                    if (vKey == VK_RIGHT) { emit keyReceived("\033[C"); continue; }
                    if (vKey == VK_LEFT)  { emit keyReceived("\033[D"); continue; }

                    // Обычные символы
                    if (ch != 0) {
                        emit keyReceived(QString(QChar(ch)));
                    }
                }
            }
        }
    }
    #else
    // На Linux/macOS стрелочки отправляют 3 байта сразу. Читаем буфер целиком.
    char buf[16];
    int bytesRead = read(STDIN_FILENO, buf, sizeof(buf));

    if (bytesRead > 0) {
        QString input = QString::fromUtf8(buf, bytesRead);

        emit keyReceived(input);
    }
    #endif
}