#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>
#include <iostream>
#include <string>

class ConsoleParser : public QObject {
    Q_OBJECT // Обязательный макрос для работы сигналов и слотов

public:
    // Конструктор (передаем parent для управления памятью в Qt)
    explicit ConsoleParser(QObject *parent = nullptr);

    QString virtPath = "/";

public slots:
    // Объявляем функцию как СЛОТ, чтобы её мошги вызвать внешне, например из таймера
    void inputFunction();
};
