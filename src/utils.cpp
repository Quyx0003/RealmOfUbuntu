#include "../include/utils.h"

void Utils::clearConsole() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void Utils::printMainMenu() {
    std::cout << "=============================" << std::endl;
    std::cout << "       Realm of Ubuntu       " << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "1. New Character" << std::endl;
    std::cout << "2. Load Character" << std::endl;
    std::cout << "3. Delete Character" << std::endl;
    std::cout << "4. Quit" << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "Enter your choice: ";
}

void Utils::printGameMenu() {
    std::cout << "=============================" << std::endl;
    std::cout << "       Explore the World      " << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "1. Print Hero Stats" << std::endl;
    std::cout << "2. Talk to Bartender" << std::endl;
    std::cout << "3. Enter Dungeon" << std::endl;
    std::cout << "4. Quit" << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "Enter your choice: ";
}

void Utils::printWrapped(const std::string &text, int lineLength) {
    int pos = 0;
    while (pos < text.length()) {
        int nextSpace = text.find(' ', pos + lineLength);
        if (nextSpace != std::string::npos && nextSpace < pos + 2 * lineLength) {
            std::cout << text.substr(pos, nextSpace - pos) << std::endl;
            pos = nextSpace + 1;
        } else {
            int spacePos = text.find_last_of(' ', pos + lineLength);
            if (spacePos != std::string::npos && spacePos > pos) {
                std::cout << text.substr(pos, spacePos - pos) << std::endl;
                pos = spacePos + 1;
            } else {
                std::cout << text.substr(pos) << std::endl;
                pos = text.length();
            }
        }
    }
}


