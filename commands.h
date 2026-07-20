#include <QString>
#include <QStringList>

void commandHelp (QString &args, QStringList &pams){
    std::cout << "\033[32mCommands: " << std::endl;
    std::cout << "  \"h\" or \"help\" - show the command list" << std::endl;
    std::cout << "\033[0m" << std::flush;
}
void commandTest (QString &args, QStringList &pams){
    std::cout << "Мама самая лучшая" << std::endl;
}
void commandRun (QString command, QString &args, QStringList &pams){
    if (command == "help" or command == "h") {
        commandHelp(args, pams);
        return;
    }
    if (command == "test" or command == "tst") {
        commandTest(args, pams);
        return;
    }
    std::cout << "\033[31mNot \"" << command.toStdString() << "\" command found. Use \"help\" for show the command list\033[0m" << std::endl;
}