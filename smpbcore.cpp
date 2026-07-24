#include "config.h"

#include "smpbcore.h"

#include "consoleapi.h"

#ifdef WORK_CONSOLE_ENABLE
#include "workconsole.h"
#endif
#ifdef DEBUG_CONSOLE_ENABLE
#include "debugconsole.h"
#endif

SmpbCore::SmpbCore(QObject *parent) : QObject(parent) {
    // Подключаем библиотеку команд и ядро SMPB к парсеру консоли
    consoleReader = new ConsoleReader(this);

    // Подключаем все консоли включенные в настройках
    #ifdef WORK_CONSOLE_ENABLE
    Consoles.append(new WorkConsole(this));
    #endif
    #ifdef DEBUG_CONSOLE_ENABLE
    Consoles.append(new DebugConsole(this));
    #endif

    /*for (int i = 0; i < Consoles.size(); i++){
        if (Consoles[i]) Consoles[i]->connectSmpbCore(*this);
    }*/

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
        std::cout << "\033[36m";
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
        std::cout << "\033[0m";
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