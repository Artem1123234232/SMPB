#include "config.h"

#ifdef DEBUG_CONSOLE_ENABLE
#pragma once

#include "baseconsole.h"
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>
#include <iostream>
#include <string>

class DebugConsole : public BaseConsole {
    Q_OBJECT // Обязательный макрос для работы сигналов и слотов

public:
    // Конструктор (передаем parent для управления памятью в Qt)
    explicit DebugConsole(QObject *parent = nullptr);

public slots:
    // Объявляем функцию как СЛОТ, чтобы её могли вызвать внешне, например из таймера
    QString getName() const {
        return "DebugConsole";
    }
    virtual QString getDescription() const {
        return "console for debugging";
    }

protected:
    QString getInputString() const { // Строка инпут, строка приветствие или как ее еще можно назвать?
        return "[DebugConsole]$ ";
    }
    QString getInputStringColor() const {
        return "33m";
    }
};
#endif