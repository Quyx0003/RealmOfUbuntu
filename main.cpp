#include "include/database.h"
#include "include/hero.h"
#include "include/quest.h"
#include "include/utils.h"
#include "include/debug.h"
#include "include/enemy.h"
#include "include/game.h"
#include "include/dungeon.h"

void exploreWorld(Quest& quest, Hero& hero, Game& game, Dungeon& dungeon) {
    int choice;
    do {
        Utils::clearConsole();
        Utils::printGameMenu();
        std::cin >> choice;
        switch(choice) {
            case 1:
                hero.heroInfo();
                break;
            case 2:
                quest.loadQuest();
                quest.printQuests();
                game.chooseQuest();
                hero.levelSystem();
                break;
            case 3:
                dungeon.loadDungeon();
                dungeon.printDungeon();
                game.enterDungeon();
                hero.levelSystem();            
                break;

            case 4:
                std::cout << "Exiting the game..." << std::endl;
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while(choice != 4);
}

int main() {
    Database db;
    Hero hero;
    Quest quest;
    Debug debug;
    Enemy enemy;
    Game game;
    Dungeon dungeon;

    enemy.loadEnemyFromFile();
    quest.loadQuestsFromFile();
    dungeon.loadDungeonFromFile();
    dungeon.loadDungeonBossFromFile();
    dungeon.loadDungeonEnemyFromFile();

    int choice;
    do {
        Utils::clearConsole();
        Utils::printMainMenu();
        std::cin >> choice;
        switch(choice) {
            case 1:
                Utils::clearConsole();
                if (hero.newHero()) {
                    exploreWorld(quest, hero, game, dungeon);
                }
                break;
            case 2:
                Utils::clearConsole();
                if (hero.loadHero()) {
                    exploreWorld(quest, hero, game, dungeon);
                }
                break;
            case 3:
                Utils::clearConsole();
                hero.deleteHero();
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
