/*#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <iostream>

// SMPB Core
class SmpbCore;

class CommandsLibrary : public QObject {
    Q_OBJECT // Обязательный макрос для работы сигналов и слотов

public:
    // Конструктор (передаем parent для управления памятью в Qt)
    explicit CommandsLibrary(QObject *parent = nullptr);

private:
    void cmdHelp(SmpbCore &smpbCore, QString &args, QStringList &pams);
    void cmdTest(SmpbCore &smpbCore, QString &args, QStringList &pams);

public slots:
    // Объявляем функцию как СЛОТ, чтобы её могли вызвать внешне, например из таймера
    void runCmd(SmpbCore &smpbCore, QString &command, QString &args, QStringList &pams);
};*/