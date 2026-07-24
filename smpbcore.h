#pragma once

#include <QTimer>
#include <QObject>
#include <QString>
#include <QStringList>
#include <iostream>

class ConsoleReader;
class BaseConsole;

class SmpbCore : public QObject {
    Q_OBJECT // Обязательный макрос для работы сигналов и слотов

    // 1. Размечаем поля для мета-системы Qt
    //Q_PROPERTY(ConsoleReader* m_consoleReader MEMBER consoleReader)
    //Q_PROPERTY(WorkConsole* m_workConsole MEMBER workConsole)
    //Q_PROPERTY(CommandsLibrary* m_commandsLibrary MEMBER commandsLibrary)

public:
    // Конструктор (передаем parent для управления памятью в Qt)
    explicit SmpbCore(QObject *parent = nullptr);

private:
    ConsoleReader* consoleReader = nullptr; // Для обработки ввода текста в консоли
    QList<BaseConsole*> Consoles; // Список консолей
    BaseConsole* selectedConsole = nullptr; // Выбранная консоль
    bool consoleSelectMode = false;

public slots:
    // Объявляем функцию как СЛОТ, чтобы её могли вызвать внешне, например из таймера
    void keyProcessing(QString key);
};