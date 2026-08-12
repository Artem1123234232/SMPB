#include "TuiMenu.h"
#include "ConsoleApi.h"

TuiMenu::TuiMenu() {
    std::clog << "Console size: " << "DYNAMIC" << "x" << "DYNAMIC" << ";" << std::endl;
    std::clog << "About render:"
              << "\n\tNot use textures"
              << "\n\tUse one color (lambert lighting) on triangle (not problem, one triangle has one normal che)"
              << "\n\tNot use depth buffer, use depth triagnles sort"
              << "\n\tRender dingus-maxwell-jessy cat"
              << "\n\tRender in console, use \\033[48;2;R;G;B for rgb" << std::endl;
    // Constructor
}

TuiMenu::~TuiMenu() {
    // Destructor
}

void TuiMenu::tick(long long tickCounter, int ticksPerSecond){
    // Tick function
    float time = (float) tickCounter / (float) ticksPerSecond;
    std::vector<std::string> stringsBuffer;
    std::string out;

    bool dirty = false;

    consoleSize = getConsoleSize();
    if (consoleSize != oldConsoleSize){
        std::clog << "Console size changed. New = {" << consoleSize.first << ", " << consoleSize.second << "};" << std::endl;
        if (!eggRotation) test3D.calc_buffer(0.0f, consoleSize.first, consoleSize.second-1);
        test3D.writeTo(&stringsBuffer);
        oldConsoleSize = consoleSize;
        dirty = true;
        out += "\033[2J\033[3J";
    };

    if (eggRotation) {
        test3D.calc_buffer(time, consoleSize.first, consoleSize.second-1);
        test3D.writeTo(&stringsBuffer);
    }
    stringsBuffer.emplace_back("consoleSize = {" + std::to_string(consoleSize.first) + ", " + std::to_string(consoleSize.second) + "};");

    out += "\033[H";
    size_t cursorMove = 0;
    for (size_t row = 0; row < stringsBuffer.size(); row++) {
        if (oldStringsBuffer.size() < stringsBuffer.size() || stringsBuffer[row] != oldStringsBuffer[row]){
            if (cursorMove > 0) out += "\033[" + std::to_string(cursorMove) + "B";
            cursorMove = 0;
            dirty = true;
            out += "\r" + stringsBuffer[row];
        }
        cursorMove++;
        //out += "\rRow: " + std::to_string(row);
    }

    if (dirty) {
        std::cout.write(out.data(), out.size());
        std::cout << std::flush;
        oldStringsBuffer = stringsBuffer;
    }
}