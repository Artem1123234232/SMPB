#pragma once

#include <QObject>
#include <QString>

#include "config.h"

#ifndef BASECLASS_H
#define BASECLASS_H

class BaseClass : public QObject {
    Q_OBJECT // Обязательный макрос для работы сигналов и слотов

public:
    // Конструктор (передаем parent для управления памятью в Qt)
    explicit BaseClass(QObject *parent = nullptr);

private:
    QString dynamicName = "BaseClass"; // Для возможности изменения имени динамически
    bool simple = false; // Если объект простой, то если функция descriptionFunctionsAndParameters не замещена, из нее вернется его имя

public slots:
    // Объявляем функцию как СЛОТ, чтобы её могли вызвать внешне, например из таймера
    QString getName() {
        return dynamicName;
    }
    void setName(QString name) {
        dynamicName = name;
    }
    void makeSimple(){
        simple = true;
    }
    virtual BaseClass* getObj(QString name, int pos);
    QString descriptionFunctionsAndParametersCLASSIC ();
    virtual QString descriptionFunctionsAndParameters ();
};

#endif // BASECLASS_H
