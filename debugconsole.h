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

struct PathElement {
    QString string;
    int pos = 0;

    // Конструктор по умолчанию
    PathElement() = default;

    // Конструктор инициализации
    PathElement(const QString &str, int position = 0)
        : string(str), pos(position) {}

    QString toQString(){
        return ANSI_RED + "PathElement" + ANSI_RST + "(" + string + ", " + QString::number(pos) + ")";
    }
};


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
    void cmdHelp(cmdData &data) override;
    void cmdPrint(cmdData &data);
    void runCmd(cmdData &data) override;

protected:
    QString getInputString() const { // Строка инпут, строка приветствие или как ее еще можно назвать?
        return "[DebugConsole]$ ";
    }
    QString getInputStringColor() const {
        return ANSI_YELLOW;
    }
};
#endif