/*#include "commands.h"
#include "smpbcore.h"

CommandsLibrary::CommandsLibrary(QObject *parent) : QObject(parent) {
    // Здесь можно инициализировать что-то при создании объекта
}

void CommandsLibrary::cmdHelp (SmpbCore &smpbCore, QString &args, QStringList &pams){
    std::cout << "\033[32mCommands: " << std::endl;
    std::cout << "  \"h\" or \"help\" - show the command list" << std::endl;
    std::cout << "\033[0m" << std::flush;
}
void CommandsLibrary::cmdTest (SmpbCore &smpbCore, QString &args, QStringList &pams){
    std::cout << "Мама самая лучшая" << std::endl;
}

void CommandsLibrary::runCmd(SmpbCore &smpbCore, QString &command, QString &args, QStringList &pams){
    if (command == "help" or command == "h") {
        cmdHelp(smpbCore, args, pams);
        return;
    }
    if (command == "test" or command == "tst") {
        cmdTest(smpbCore, args, pams);
        return;
    }
    std::cout << "\033[31mNot \"" << command.toStdString() << "\" command found. Use \"help\" for show the command list\033[0m" << std::endl;
}*/