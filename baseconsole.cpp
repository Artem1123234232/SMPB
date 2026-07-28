#include "config.h"

#ifdef BASE_CONSOLE_ENABLE
#include "baseconsole.h"

#include <QMetaObject>
#include <QMetaProperty>    // Обязательно! Исправляет ошибку с QMetaProperty
#include <QDebug>

BaseConsole::BaseConsole(QObject *parent) : BaseClass (parent) {
    // Задаем параметры
    setName("BaseConsole");
}

void BaseConsole::connectSmpbCore(SmpbCore &t_smpbCore){
    smpbCore = &t_smpbCore;
}

void BaseConsole::keyProcessing(QString key){
    bool enter = false;

    if (key.size() > 0){
        if (key == '\n' || key == '\r') {
            enter = true;
        }
        else if (key == "\u007F" || key == '\b') { // Backspace символ
            if (cursorY >= 0 && cursorY < cmdHistory.size() && cursorX > 0) {
                cursorX -= 1;
                cmdHistory[cursorY].remove(cursorX, 1);
            }
        }
        else if (key == "\u001B[A"){
            cursorY -= 1;
            cursorX = INT_MAX;
        }
        else if (key == "\u001B[B"){
            cursorY += 1;
            cursorX = INT_MAX;
        }
        else if (key == "\u001B[C"){
            cursorX += 1;
        }
        else if (key == "\u001B[D"){
            cursorX -= 1;

        }
        else if (key.size() > 0 && key[0] != '\033'  && key[0] != '\u001B') {
            if (cursorY >= 0 && cursorY < cmdHistory.size()) {
                cmdHistory[cursorY].insert(cursorX, key);
                cursorX += key.size();
            }
        }
    }

    cursorY = std::min<int>(std::max<int>(cursorY,0),cmdHistory.size() - 1);
    if (cursorY >= 0 && cursorY < cmdHistory.size()) cursorX = std::min<int>(std::max<int>(cursorX,0),cmdHistory[cursorY].size());
    else cursorX = 0;
    QString cmdHistorySelected = ANSI_RED + "IndexError" + ANSI_GREEN + " (in " + getName() + "::keyProcessing)" + ANSI_RST + ": list index out of range (cmdHistory.size() = " + QString::number(cmdHistory.size()) + ", cursorY = " + QString::number(cursorY) + ");";
    if (cursorY >= 0 && cursorY < cmdHistory.size()) {
        cmdHistorySelected = cmdHistory[cursorY];
    }

    std::cout << "\r\033[2K" << getInputStringColor().toStdString() << getInputString().toStdString() << "\033[0m" << cmdHistorySelected.toStdString() << "\r\033[" << std::to_string(cursorX+getInputString().size()) << "C" << std::flush;

    if (enter) {
        std::cout << std::endl;
        if (cursorY >= 0 && cursorY < cmdHistory.size()) {
            cmdData data = parseCommand(cmdHistory[cursorY]);
            runCmd(data);
        }
        if (cmdHistory.size() <= 0 || cmdHistory[cmdHistory.size()-1].size() > 0) cmdHistory.append(QString(""));
        cursorY = cmdHistory.size()-1;
        cursorX = 0;
        keyProcessing("");
    }
}
cmdData BaseConsole::parseCommand(QString &commandString) {
    cmdData data;

    if (commandString.size() <= 0){
        return data; // Просто выходим, ведь парсить нечего
    }


    // Парсим строку
    std::vector<std::string> commandStdStringList;
    std::string block = "";
    bool backslash_mode = false;
    bool string_mode = false;
    for (char chr : commandString.toStdString()+' ') {
        if (backslash_mode){
            if (chr == 'n') block += '\n';
            else if (chr == 't') block += '\t';
            else if (chr == 'r') block += '\r';
            else block += chr;
        }
        else if (chr == '\"'){
            string_mode = !string_mode;
        }
        else if (chr != '\\' && (chr != ' ' || string_mode)){
            block += chr;
        }
        else if (chr != '\\'){
            if (!block.empty()) commandStdStringList.push_back(block);
            block = "";
        }

        backslash_mode = chr == '\\' && !backslash_mode;
    }

    if (commandStdStringList.size() <= 0){
        return data; // Просто выходим, ведь парсить нечего
    }

    QString command = QString::fromStdString(commandStdStringList[0]).toLower();
    QString args;
    QStringList pams;
    for (const std::string& element : commandStdStringList) {
        QString qElement = QString::fromStdString(element);
        //qDebug() << "Element: " << qElement;
        if (element[0] == '-'){
            args +=  qElement.sliced(1);
        }
        else{
            pams.append(qElement);
        }
    }

    // Исполняем команду из распарсенных данных
    data.command = command;
    data.args = args;
    data.pams = pams;
    data.live = true;
    return data;
}
void BaseConsole::cmdHelp (cmdData &data){
    std::cout << ANSI_GREEN.toStdString() << "Commands: " << std::endl;
    std::cout << "  \"h\" or \"help\" - show the command list" << std::endl;
    std::cout << ANSI_RST.toStdString() << std::flush;
}
void BaseConsole::cmdTest (cmdData &data){
    std::cout << "Моя мама самая лучшая в мире!" << std::endl;
}
void BaseConsole::runCmd(cmdData &data){
    if (data.live){
        if (data.command == "help" or data.command == "h") {
            cmdHelp(data);
        }
        else if (data.command == "test" or data.command == "tst") {
            cmdTest(data);
        }
        else{
            std::cout << "\033[31mNot \"" << data.command.toStdString() << "\" command found. Use \"help\" for show the command list\033[0m" << std::endl;
        }
    }
}
#endif