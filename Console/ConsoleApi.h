#ifndef CONSOLEAPI_H
#define CONSOLEAPI_H

#include <utility>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// Единая функция получения размера консоли для всех ОС.
inline std::pair<int, int> getConsoleSize() {
    std::pair<int, int> size;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        // Вычисляем ширину и высоту видимого окна
        size.first = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        size.second = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
#else
    struct winsize w;
    // Запрашиваем размеры терминала у стандартного вывода
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        size.first = w.ws_col;
        size.second = w.ws_row;
    }
#endif
    return size;
}

#endif // CONSOLEAPI_H
