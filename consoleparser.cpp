#include "consoleparser.h"
#include "commands.h"

ConsoleParser::ConsoleParser(QObject *parent) : QObject(parent) {
    // Здесь можно инициализировать что-то при создании объекта
}

void ConsoleParser::inputFunction() {
    QString inputString = QString("\033[35m[SMPD:") + virtPath + QString("]$ \033[0m");
    std::string commandStdString;

    // Пишем инпут строку
    std::cout << inputString.toStdString() << std::flush;

    // Читаем всю строку, включая пробелы, пока пользователь не нажмет Enter
    std::getline(std::cin, commandStdString);
    QString commandString = QString::fromStdString(commandStdString).trimmed();

    if (commandString.isEmpty()){
        return; // Просто выходим, ведь парсить нечего
    }

    // Парсим строку
    QStringList commandStringList = commandString.split(QRegularExpression("\\s+"));
    QString command = commandStringList[0].toLower();
    QString args;
    QStringList pams;
    for (int i = 1; i < commandStringList.size(); i++){
        QString element = commandStringList[i];
        if (element[0] == '-'){
            args +=  element.sliced(1);
        }
        else{
            pams.append(element);
        }
    }

    // Исполняем команду из распарсенных данных
    commandRun(command, args, pams);
}
