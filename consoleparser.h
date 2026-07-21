#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>
#include <iostream>
#include <string>

class SmpbCore;
class CommandsLibrary;

class ConsoleParser : public QObject {
    Q_OBJECT // Обязательный макрос для работы сигналов и слотов

public:
    // Конструктор (передаем parent для управления памятью в Qt)
    explicit ConsoleParser(QObject *parent = nullptr);

private:
    CommandsLibrary *commandsLibrary;
    SmpbCore *smpbCore;

public slots:
    // Объявляем функцию как СЛОТ, чтобы её могли вызвать внешне, например из таймера
    void connectCommandsLib(CommandsLibrary &t_commandsLibrary);
    void connectSmpbCore(SmpbCore &t_smpbCore);
    void inputFunction();
};