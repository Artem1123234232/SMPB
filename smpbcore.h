#pragma once

#include "baseclass.h"
//#include "baseconsole.h"
#include "consoleapi.h"

#include <QTimer>
#include <QObject>
#include <QString>
#include <QStringList>
#include <iostream>

class BaseConsole;

class SmpbCore : public BaseClass {
    Q_OBJECT // Обязательный макрос для работы сигналов и слотов

public:
    // Конструктор (передаем parent для управления памятью в Qt)
    explicit SmpbCore(QObject *parent = nullptr);

private:
    ConsoleReader* consoleReader = nullptr; // Для обработки ввода текста в консоли
    QList<BaseConsole*> Consoles; // Список консолей
    BaseConsole* selectedConsole = nullptr; // Выбранная консоль
    bool consoleSelectMode = false;
    BaseClass temporaryObj;

public slots:
    // Объявляем функцию как СЛОТ, чтобы её могли вызвать внешне, например из таймера
    void keyProcessing(QString key);
    #ifdef DEBUG_FUNC_ENABLE
    BaseClass* getObj(QString name, int pos);
    QString descriptionFunctionsAndParameters ();
    #endif
};