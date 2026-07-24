/*#include "cpc.h"
#include "smpbcore.h"       // Обязательно! Чтобы компилятор знал структуру SmpbCore

#include <QMetaObject>
#include <QMetaProperty>    // Обязательно! Исправляет ошибку с QMetaProperty
#include <QDebug>
#include <climits>

CPC::CPC(QObject *parent) : QObject(parent) {
    // Здесь можно инициализировать что-то при создании объекта
}

void CPC::cmdProcessing(cmdData &data){
    if (data.command == "help" || data.command == "h"){
        std::cout << "\033[32mCommands: " << std::endl;
        std::cout << "  \"h\" or \"help\" - show the command list" << std::endl;
        std::cout << "  \"exit\" - exit from cpc mode" << std::endl;
        std::cout << "\033[0m" << std::flush;
    }
    else if (data.command == "exit"){
        cpcMode = false;
    }
    else if (data.command == "se"){
        if (!smpbCore) return;

        const QMetaObject *meta = smpbCore->metaObject();
        qDebug() << "Class name:" << meta->className();

        // Цикл по всем зарегистрированным свойствам
        for (int i = 0; i < meta->propertyCount(); ++i) {
            QMetaProperty prop = meta->property(i);
            QString name = prop.name();
            QVariant value = prop.read(smpbCore); // Читаем значение в QVariant

            qDebug() << "  " << name << "=" << value;
        }
    }
    else {
        std::cout << "\033[31mNot \"" << data.command.toStdString() << "\" command found. Use \"help\" for show the command list\033[0m" << std::endl;
    }
}

void CPC::keyProcessing(QString &key){
    bool enter = false;

    if (key.size() > 0){
        if (key == '\n' || key == '\r') {
            enter = true;
        }
        else if (key == "\u007F" || key == '\b') { // Backspace символ
            cpcBuffer.chop(1);
        }
        else if (key.size() > 0 && key[0] != '\033'  && key[0] != '\u001B') {
            if (cpcMode) {
                cpcBuffer += key;
            }
            else {
                if (cursorY >= 0 && cursorY < cmdHistory.size()) {
                    cmdHistory[cursorY].insert(cursorX, key);
                    cursorX += key.size();
                }
            }
        }
    }

    std::cout << "\r\033[2K\033[33m" << cpcInputString.toStdString() << "\033[0m" << cpcBuffer.toStdString() << std::flush;

    //qDebug() << QString("ERR");
    //qDebug() << key;
    //qDebug() << cursorX;
    //qDebug() << inputString.size();

    if (enter) {
        std::cout << std::endl;
        cmdData data = parseCommand(cpcBuffer);
        cmdProcessing(data);
        cpcBuffer.clear();
        keyProcessing("");
    }
}*/