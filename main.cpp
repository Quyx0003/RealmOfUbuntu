#include <iostream>
#include "database.h"
#include "hero.h"
#include "quest.h"

void clearConsole() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void printMainMenu() {
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

void printGameMenu() {
    std::cout << "=============================" << std::endl;
    std::cout << "       Explore the World      " << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "1. Print Hero Stats" << std::endl;
    std::cout << "2. Explore" << std::endl;
    std::cout << "3. Quit" << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "Enter your choice: ";
}

void exploreWorld(Quest& quest, Hero& hero) {
    int choice;
    do {
        clearConsole();
        printGameMenu();
        std::cin >> choice;
        switch(choice) {
            case 1:
                hero.printHero();
                break;
            case 2:
                quest.loadQuest();
                quest.chooseQuest();
                hero.levelSystem();
                break;
            case 3:
                std::cout << "Exiting the game..." << std::endl;
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while(choice != 3);
}

int main() {
    Database db;
    Hero hero;
    Quest quest;

    db.loadEnemiesFromFile();
    db.loadQuestsFromFile();
    
    int choice;
    do {
        clearConsole();
        printMainMenu();
        std::cin >> choice;
        switch(choice) {
            case 1:
                clearConsole();
                db.newHero();
                exploreWorld(quest, hero);
                break;
            case 2:
                clearConsole();
                if (db.loadHero()) {
                    exploreWorld(quest, hero);
                }
                break;
            case 3:
                clearConsole();
                db.deleteHero();
                break;
            case 4:
                std::cout << "Exiting the game..." << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while(choice != 4);

    return 0;
}
