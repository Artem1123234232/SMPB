#include "config.h"

#ifdef WORK_CONSOLE_ENABLE
#pragma once

#include "baseconsole.h"
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>
#include <iostream>
#include <string>

class WorkConsole : public BaseConsole {
    Q_OBJECT // Обязательный макрос для работы сигналов и слотов

public:
    // Конструктор (передаем parent для управления памятью в Qt)
    explicit WorkConsole(QObject *parent = nullptr);

public slots:
    // Объявляем функцию как СЛОТ, чтобы её могли вызвать внешне, например из таймера
    QString getName() const {
        return "WorkConsole";
    }
    virtual QString getDescription() const {
        return "standart console";
    }

protected:
    QString getInputString() const { // Строка инпут, строка приветствие или как ее еще можно назвать?
        return "[WorkConsole]$ ";
    }
    QString getInputStringColor() const {
        return ANSI_PURPLE;
    }
};
#endif