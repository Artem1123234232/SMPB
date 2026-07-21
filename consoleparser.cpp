#include "consoleparser.h"
#include "commands.h"

ConsoleParser::ConsoleParser(QObject *parent) : QObject(parent) {
    // Здесь можно инициализировать что-то при создании объекта
}

void ConsoleParser::inputFunction() {
    // Пишем инпут строку`
    std::cout << "\033[35m[SMPD]$ \033[0m" << std::flush;


    // Читаем всю строку, включая пробелы, пока пользователь не нажмет Enter
    std::string commandStdString;
    std::getline(std::cin, commandStdString);
    //QString commandString = QString::fromStdString(commandStdString).trimmed();

    if (commandStdString.empty()){
        return; // Просто выходим, ведь парсить нечего
    }


    // Парсим строку
    std::vector<std::string> commandStdStringList;
    std::string block = "";
    bool backslash_mode = false;
    bool string_mode = false;
    for (char chr : commandStdString+' ') {
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

        backslash_mode = chr == '\\';
    }

    QString command = QString::fromStdString(commandStdStringList[0]);
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
    commandRun(command, args, pams);
}
