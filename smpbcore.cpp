#include "smpbcore.h"

SmpbCore::SmpbCore(QObject *parent) : QObject(parent) {
    // Подключаем библиотеку команд и ядро SMPB к парсеру консоли
    consoleParser.connectCommandsLib(commandsLibrary);
    consoleParser.connectSmpbCore(*this);

    // Настраиваем таймер консоли
    consoleTimer.setInterval(100);

    // Подключаем таймер консоли к функции парсера консоли
    QObject::connect(&consoleTimer, &QTimer::timeout, &consoleParser, &ConsoleParser::inputFunction);
}

void SmpbCore::startTimers(){
    // Запускаем таймер консоли
    consoleTimer.start();
}