#pragma once

#include <QTimer>
#include <QObject>
#include <QString>
#include <QStringList>
#include <iostream>

#include "consoleparser.h"
#include "commands.h"

class SmpbCore : public QObject {
    Q_OBJECT // Обязательный макрос для работы сигналов и слотов

public:
    // Конструктор (передаем parent для управления памятью в Qt)
    explicit SmpbCore(QObject *parent = nullptr);

public slots:
    // Объявляем функцию как СЛОТ, чтобы её могли вызвать внешне, например из таймера
    void startTimers();

private:
    QTimer consoleTimer;
    ConsoleParser consoleParser;
    CommandsLibrary commandsLibrary;
};