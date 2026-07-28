#include "baseclass.h"

BaseClass::BaseClass(QObject *parent) : QObject(parent) {
    // Здесь можно инициализировать что-то при создании объекта
}
QString BaseClass::descriptionFunctionsAndParametersCLASSIC (){
    if (simple) return getName();

#ifdef DEBUG_FUNC_ENABLE
    // Получаем текущее имя объекта
    QString currentName = getName();

    QString dynamicNameStr = ANSI_YELLOW + "\"" + dynamicName + "\"" + ANSI_RST;
    QString simpleStr = simple ? ANSI_GREEN + "true" + ANSI_RST : ANSI_RED + "false" + ANSI_RST;

    QString s = "";
    s += ANSI_BLUE + "class " + ANSI_RED + currentName + ANSI_RST + " : " + ANSI_BLUE + "public" + ANSI_RST + " QObject {\n";
    s += "\tQ_OBJECT " + ANSI_GREEN + "// Обязательный макрос для работы сигналов и слотов" + ANSI_RST + "\n\n";

    s += ANSI_BLUE + "public" + ANSI_RST + ":\n";
    s += "\t" + ANSI_GREEN + "// Конструктор (передаем parent для управления памятью в Qt)" + ANSI_RST + "\n";
    s += "\t" + ANSI_BLUE + "explicit " + ANSI_RED + currentName + ANSI_RST + "(QObject *parent = " + ANSI_BLUE + "nullptr" + ANSI_RST + ");\n\n";

    s += ANSI_BLUE + "private" + ANSI_RST + ":\n";
    s += "\t" + ANSI_GREEN + "QString" + ANSI_RST + " dynamicName = " + dynamicNameStr + "; " + ANSI_GREEN + "// Для возможности изменения имени динамически" + ANSI_RST + "\n";
    s += "\t" + ANSI_GREEN + "bool" + ANSI_RST + " simple = " + simpleStr + "; " + ANSI_GREEN + "// Если объект простой, то если функция descriptionFunctionsAndParameters не замещена, из нее вернется его имя" + ANSI_RST + "\n\n";

    s += ANSI_BLUE + "public slots" + ANSI_RST + ":\n";
    s += "\t" + ANSI_GREEN + "// Объявляем функцию как СЛОТ, чтобы её могли вызвать внешне, например из таймера" + ANSI_RST + "\n";
    s += "\t" + ANSI_GREEN + "QString" + ANSI_RST + " getName() {\n";
    s += "\t\t" + ANSI_BLUE + "return" + ANSI_RST + " dynamicName;\n";
    s += "\t}\n";
    s += "\t" + ANSI_BLUE + "void" + ANSI_RST + " setName(" + ANSI_GREEN + "QString" + ANSI_RST + " name) {\n";
    s += "\t\tdynamicName = name;\n";
    s += "\t}\n";
    s += "\t" + ANSI_BLUE + "void" + ANSI_RST + " makeSimple() {\n";
    s += "\t\tsimple = " + ANSI_BLUE + "true" + ANSI_RST + ";\n";
    s += "\t}\n";
    s += "\t" + ANSI_BLUE + "virtual" + ANSI_RED + " " + currentName + ANSI_RST + "* getObj(" + ANSI_GREEN + "QString" + ANSI_RST + " name, " + ANSI_BLUE + "int" + ANSI_RST + " pos);\n";
    s += "\t" + ANSI_GREEN + "QString" + ANSI_RST + " descriptionFunctionsAndParametersCLASSIC ();\n";
    s += "\t" + ANSI_BLUE + "virtual" + ANSI_GREEN + " QString" + ANSI_RST + " descriptionFunctionsAndParameters ();\n";

    return s;
#else
    return QString("Debug functions is not enabled, sorry");
#endif
}
QString BaseClass::descriptionFunctionsAndParameters (){
    return descriptionFunctionsAndParametersCLASSIC();
}
BaseClass* BaseClass::getObj(QString name, int pos){
    return nullptr;
}