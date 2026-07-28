#include "config.h"

#ifdef DEBUG_CONSOLE_ENABLE
#include "debugconsole.h"

DebugConsole::DebugConsole(QObject *parent) : BaseConsole(parent) {
    // Задаем параметры
    setName("DebugConsole");
}
void DebugConsole::cmdHelp (cmdData &data) {
    std::cout << ANSI_GREEN.toStdString() << "Commands: " << std::endl;
    std::cout << "  \"h\" or \"help\" - show the command list" << std::endl;
    std::cout << "  \"prnt\" or \"print\" - print the object.\n    Use \"print path_element1.path_element2.path_element3\".\n    Use argument -c for use descriptionFunctionsAndParametersCLASSIC instead off descriptionFunctionsAndParameters.\n    Examples: \"prnt\", \"print Consoles[0]\", \"print\", \"print -c\"" << std::endl;
    //std::cout << "  \"tree\" - tree of objects" << std::endl;
    std::cout << ANSI_RST.toStdString() << std::flush;
}
void DebugConsole::cmdPrint (cmdData &data) {
    if (data.pams.size() <= 1) data.pams.append("");
    QString path = data.pams[1];
    QStringList raw_path_list = path.split(".", Qt::SkipEmptyParts);
    QList<PathElement> path_list;

    for (int i = 0; i < raw_path_list.size(); i++){
        PathElement pathElement = PathElement("", INT_MAX);
        QString pos_str;
        int val_mode = false;
        for (QChar chr : raw_path_list[i]) {
            if (chr == '['){
                val_mode = true;
            }
            else if (chr == ']'){
                val_mode = false;
                pathElement.pos = pos_str.toInt();
                pos_str.clear();
            }
            else if (val_mode){
                pos_str += chr;
            }
            else {
                pathElement.string += chr;
            }
        }
        path_list.append(pathElement);
    }

    BaseClass* obj = smpbCore;

    for (int i = 0; i < path_list.size(); i++){
        obj = obj->getObj(path_list[i].string, path_list[i].pos);
        if (!obj){
            QString path_to_i = "";
            for (int j = 0; j <= i; j++) {
                path_to_i += raw_path_list[j];
                if (j < i) path_to_i += ".";
            }
            std::cout << (ANSI_RED + "IndexError " + ANSI_GREEN + "(in DebugConsole::cmdPrint)" + ANSI_RST + ": Object not found by path " + path_to_i + ";").toStdString() << std::endl;
            return;
        }
    }

    if (data.args.size() > 0 && data.args.contains("c")) std::cout << obj->descriptionFunctionsAndParametersCLASSIC().toStdString() << std::endl;
    else std::cout << obj->descriptionFunctionsAndParameters().toStdString() << std::endl;
}
void DebugConsole::runCmd (cmdData &data) {
    if (data.live){
        if (data.command == "help" or data.command == "h") {
            cmdHelp(data);
        }
        else if (data.command == "print" || data.command == "prnt") {
            cmdPrint(data);
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