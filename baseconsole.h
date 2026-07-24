#include "config.h"

#ifdef BASE_CONSOLE_ENABLE
#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>
#include <iostream>
#include <string>

class SmpbCore;

struct cmdData{
    QString command;
    QString args;
    QStringList pams;
    bool live = false;
};

class BaseConsole : public QObject {
    Q_OBJECT // Обязательный макрос для работы сигналов и слотов

public:
    // Конструктор (передаем parent для управления памятью в Qt)
    explicit BaseConsole(QObject *parent = nullptr);

private:
    SmpbCore *smpbCore;
    QStringList cmdHistory = {""}; // Хранит историю команд
    int cursorX = 0;
    int cursorY = 0;
    cmdData parseCommand(QString &commandString);

public slots:
    // Объявляем функцию как СЛОТ, чтобы её могли вызвать внешне, например из таймера
    void connectSmpbCore(SmpbCore &t_smpbCore);
    void keyProcessing(QString key);
    void cmdHelp(cmdData &data);
    void cmdTest(cmdData &data);
    void runCmd(cmdData &data);
    virtual QString getName() const {
        return "BaseConsole";
    }
    virtual QString getDescription() const {
        return "base class console";
    }

protected:
    virtual QString getInputString() const { // Строка инпут, строка приветствие или как ее еще можно назвать?
        return "[BaseConsole]$ ";
    }
    virtual QString getInputStringColor() const {
        return "34m";
    }
};
#endif