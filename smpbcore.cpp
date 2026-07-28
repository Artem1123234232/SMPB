#include "config.h"

#include "smpbcore.h"

#ifdef WORK_CONSOLE_ENABLE
#include "workconsole.h"
#endif
#ifdef DEBUG_CONSOLE_ENABLE
#include "debugconsole.h"
#endif

SmpbCore::SmpbCore(QObject *parent) : BaseClass(parent) {
    // Задаем параметры
    setName("SmpbCore");
    temporaryObj.makeSimple();

    // Подключаем библиотеку команд и ядро SMPB к парсеру консоли
    consoleReader = new ConsoleReader(this);//std::make_unique<ConsoleReader>();

    // Подключаем все консоли включенные в настройках
    #ifdef WORK_CONSOLE_ENABLE
    Consoles.append(new WorkConsole(this));
    #endif
    #ifdef DEBUG_CONSOLE_ENABLE
    Consoles.append(new DebugConsole(this));
    #endif

    for (int i = 0; i < Consoles.size(); i++){
        if (Consoles[i]) Consoles[i]->connectSmpbCore(*this);
    }

    if (!consoleReader) std::cout << "\033[31m[SmpbCore]$ \033[0mERROR: consoleReader is nullptr. Doesn't use this components" << std::endl;

    if (consoleReader) {
        QObject::connect(&*consoleReader, &ConsoleReader::keyReceived, this, &SmpbCore::keyProcessing);
    }

    keyProcessing("");
}
void SmpbCore::keyProcessing(QString key){
    if (key == '$'){
        selectedConsole = nullptr;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << ANSI_CYAN.toStdString();
        std::cout << "Console Manager. Press number for select console. Avaibles:" << std::endl;
        for (int i = 0; i < Consoles.size(); i++){
            if (Consoles[i]) std::cout << std::to_string(i) << ": " << Consoles[i]->getName().toStdString() << " - " << Consoles[i]->getDescription().toStdString() << std::endl;
        }
        std::cout << std::endl;
        consoleSelectMode = true;
    }
    else if (consoleSelectMode){
        bool ok = false;
        int value = key.toInt(&ok);
        if (ok && value < Consoles.size() && Consoles[value]) {
            std::cout << "Selected: " << Consoles[value]->getName().toStdString() << std::endl;
            selectedConsole = Consoles[value];
        }
        else {
            std::cout << "No console by key \"" << key.toStdString() << "\"" << std::endl;
        }
        std::cout << ANSI_RST.toStdString();
        std::cout << std::endl;
        consoleSelectMode = false;
        keyProcessing("");
    }
    else{
        if (selectedConsole) {
            selectedConsole->keyProcessing(key);
        }
        else {
            std::cout << "\rNo console selected. Print \"$\" for select console" << std::flush;
        }
    }
}
#ifdef DEBUG_FUNC_ENABLE
BaseClass* SmpbCore::getObj(QString name, int pos){
    if (name == "consoleReader") return consoleReader;
    if (name == "Consoles") {
        if (pos == INT_MAX){
            QString ConsolesStr = "QList<" + ANSI_RED + "BaseConsole" + ANSI_RST+ "*> Consoles = {";
            for (int i = 0; i < Consoles.size(); i++){
                if (Consoles[i]) {
                    ConsolesStr += ANSI_BLUE + "new " + ANSI_RED + Consoles[i]->getName() + ANSI_RST + "(" + ANSI_BLUE + "this" + ANSI_RST + ")";
                }
                else {
                    ConsolesStr += ANSI_BLUE + "nullptr" + ANSI_RST;
                }
                if (i < Consoles.size()-1){
                    ConsolesStr += ", ";
                }
            }
            ConsolesStr += "};";
            temporaryObj.setName(ConsolesStr);
            return &temporaryObj;
        }
        else{
            if (pos >= 0 && pos < Consoles.size()) return Consoles[pos];
            else std::cout << (ANSI_RED + "IndexError" + ANSI_GREEN + " (in SmpbCore::getObj)" + ANSI_RST + ": list index out of range (Consoles.size() = " + QString::number(Consoles.size()) + ", pos = " + QString::number(pos) + ");").toStdString() << std::endl;
        }
    }
    if (name == "selectedConsole") return selectedConsole;
    return nullptr;
}
QString SmpbCore::descriptionFunctionsAndParameters (){
    QString consoleReaderStr = ANSI_BLUE + "nullptr" + ANSI_RST;
    if (consoleReader) consoleReaderStr = ANSI_BLUE + "new" + ANSI_RED + consoleReader->getName() + ANSI_RST + "(" + ANSI_BLUE + "this" + ANSI_RST + ")";

    QString consoleSelectModeStr = ANSI_BLUE + "false" + ANSI_RST;
    if (consoleSelectMode) consoleSelectModeStr = ANSI_BLUE + "true" + ANSI_RST;

    QString selectedConsoleStr = ANSI_BLUE + "nullptr" + ANSI_RST;
    if (selectedConsole) selectedConsoleStr = ANSI_BLUE + "new " + ANSI_RED + selectedConsole->getName() + ANSI_RST + "(" + ANSI_BLUE + "this" + ANSI_RST + ")";

    QString ConsolesStr = "{";
    for (int i = 0; i < Consoles.size(); i++){
        if (Consoles[i]) {
            ConsolesStr += ANSI_BLUE + "new " + ANSI_RED + Consoles[i]->getName() + ANSI_RST + "(" + ANSI_BLUE + "this" + ANSI_RST + ")";
        }
        else {
            ConsolesStr += ANSI_BLUE + "nullptr" + ANSI_RST;
        }
        if (i < Consoles.size()-1){
            ConsolesStr += ", ";
        }
    }
    ConsolesStr += "}";
    //if (selectedConsole) selectedConsoleStr = ANSI_BLUE + "new " + ANSI_RED + selectedConsole->getName() + ANSI_RST + "(" + ANSI_BLUE + "this" + ANSI_RST + ")";


    QString s = "";
    s += ANSI_BLUE + "class " + ANSI_RED + getName() + ANSI_RST + " : " + ANSI_BLUE + "public" + ANSI_RST + " QObject {\n";
    s += "\tQ_OBJECT\n";


    s += ANSI_BLUE + "public" + ANSI_RST + ":\n";
    s += "\t" + ANSI_BLUE + "explicit " + ANSI_RED + getName() + ANSI_RST + "(QObject *parent = " + ANSI_BLUE + "nullptr" + ANSI_RST + ");\n";


    s += ANSI_BLUE + "private" + ANSI_RST + ":\n";
    s += "\t" + ANSI_RED + consoleReader->getName() + ANSI_RST + "* consoleReader = " + consoleReaderStr + ";\n";
    s += "\tQList<" + ANSI_RED + "BaseConsole" + ANSI_RST+ "*> Consoles = " + ConsolesStr + ";\n";
    s += "\t" + ANSI_RED + "BaseConsole" + ANSI_RST + "* selectedConsole = " + selectedConsoleStr + ";\n";
    s += "\t" + ANSI_PURPLE + "bool" + ANSI_RST + " consoleSelectMode = " + consoleSelectModeStr + ";\n";


    s += ANSI_BLUE + "public slots" + ANSI_RST + ":\n";
    s += "\t" + ANSI_PURPLE + "void" + ANSI_RST + " keyProcessing(" + ANSI_GREEN + "QString" + ANSI_RST + " key);\n";
    s += "\t" + ANSI_BLUE + "virtual" + ANSI_RED + " " + "BaseClass" + ANSI_RST + " getObj(" + ANSI_GREEN + "QString" + ANSI_RST + " name);\n";
    s += "\t" + ANSI_GREEN + "QString" + ANSI_RST + " descriptionFunctionsAndParameters ();\n";
    return(s);
}
#endif