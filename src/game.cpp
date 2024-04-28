#include "../include/game.h"

bool Game::battle() {
    Character hero;
    mQuery.exec("SELECT * FROM currentHero");
    if (!mQuery.next()) {
        std::cerr << "Failed to retrieve hero data." << std::endl;
        return false;
    }
    hero.name = mQuery.value(0).toString();
    hero.className = mQuery.value(1).toString();
    hero.level = mQuery.value(2).toInt();
    hero.hitpoints = mQuery.value(4).toInt();
    hero.damage = mQuery.value(5).toInt();

    if (hero.className == "Warrior") {
        hero.evadeChance = 50;
    } else if (hero.className == "Scout") {
        hero.evadeChance = 25;
    } else {
        hero.evadeChance = 0;
    }

    Utils::clearConsole();
    std::cout << "========================================" << std::endl;
    std::cout << "           The Battle Begins!           " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    while (true) {
        Character enemy;
        mQuery.exec("SELECT * FROM currentEnemy");
        if (!mQuery.next()) {
            return true;
        }
        enemy.name = mQuery.value(0).toString();
        enemy.className = mQuery.value(1).toString();
        enemy.level = hero.level;
        enemy.hitpoints = mQuery.value(2).toInt();
        enemy.damage = mQuery.value(3).toInt();

        if (enemy.className == "Warrior") {
            enemy.evadeChance = 50;
        } else if (enemy.className == "Scout") {
            enemy.evadeChance = 25;
        } else {
            enemy.evadeChance = 0;
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> distribution(0, 1);
        bool heroFirst = distribution(g);

        std::cout << "You are now facing " << enemy.name.toStdString() << ", a " << enemy.className.toStdString() << "." << std::endl;
        std::cout << std::endl;

        while (hero.hitpoints > 0 && enemy.hitpoints > 0) {
            if (heroFirst) {
                attack(hero, enemy, g);

                if (enemy.hitpoints <= 0) {
                    mQuery.prepare("DELETE FROM currentEnemy WHERE name = :enemyName");
                    mQuery.bindValue(":enemyName", enemy.name);
                    mQuery.exec();
                    std::cout << "You defeated " << enemy.name.toStdString() << "!" << std::endl;
                    std::cout << std::endl;
                    break;
                }

                attack(enemy, hero, g);

                if (hero.hitpoints <= 0) {
                    return false;
                }
            } else {
                attack(enemy, hero, g);

                if (hero.hitpoints <= 0) {
                    return false;
                }

                attack(hero, enemy, g);

                if (enemy.hitpoints <= 0) {
                    mQuery.prepare("DELETE FROM currentEnemy WHERE name = :enemyName");
                    mQuery.bindValue(":enemyName", enemy.name);
                    mQuery.exec();
                    std::cout << "You defeated " << enemy.name.toStdString() << "!" << std::endl;
                    std::cout << std::endl;
                    break;
                }
            }
        }
    }
}

void Game::attack(Character& attacker, Character& target, std::mt19937& g) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1500)); 
    std::cout << attacker.name.toStdString() << " attacks " << target.name.toStdString() << "!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500)); 

    int damageVariation = 2 * attacker.level;
    int damage = std::max(attacker.damage + std::uniform_int_distribution<int>(-damageVariation, damageVariation)(g), 0);
    bool isCritical = (std::uniform_int_distribution<int>(1, 100)(g) <= 25);

    if (attacker.className != "Mage") {
        if (std::uniform_int_distribution<int>(1, 100)(g) > target.evadeChance) {

            if (isCritical) {
                damage *= 2; 
            }
            std::cout << attacker.name.toStdString() << " hits " << target.name.toStdString()
                    << " for " << damage << " damage!";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500)); 
            
            if (isCritical) {
                std::cout << " *** Critical Hit! ***";
            }
            std::cout << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1500)); 
            
            target.hitpoints -= damage;
            
            if (damage > 0)
                std::cout << "Remaining hitpoints of " << target.name.toStdString() << ": " << target.hitpoints << std::endl;
            
        } else {
            
            if (target.className == "Warrior") {
                std::cout << target.name.toStdString() << " blocks " << attacker.name.toStdString() << "'s attack!" << std::endl;
            } else {
                std::cout << target.name.toStdString() << " evades " << attacker.name.toStdString() << "'s attack!" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1500)); 
        }
    } else {
        if (isCritical) {
                damage *= 2; 
        }

        std::cout << attacker.name.toStdString() << " hits " << target.name.toStdString()
                << " for " << damage << " damage!";
        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); 
        
        if (isCritical) {
            std::cout << " *** Critical Hit! ***";
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); 
        
        target.hitpoints -= damage;
        
        if (damage > 0)
            std::cout << "Remaining hitpoints of " << target.name.toStdString() << ": " << target.hitpoints << std::endl;
    }
}

void Game::chooseQuest() {
    std::cout << "Enter your choice: ";

    int choice;
    std::cin >> choice;

    if (choice == 1 || choice == 2 || choice == 3) {
        mQuery.prepare("SELECT * FROM currentQuests WHERE id = :id");
        mQuery.bindValue(":id", choice);
        if (!mQuery.exec() || !mQuery.next()) {
            std::cerr << "Failed to retrieve quest data." << std::endl;
            return;
        }

        QString name = mQuery.value(1).toString();
        int questXPreward = mQuery.value(3).toInt();
        int goldReward = mQuery.value(4).toInt();
        QString enemyRace = mQuery.value(5).toString();

        mQuery.prepare("SELECT * FROM enemyDatabase WHERE race = :race");
        mQuery.bindValue(":race", enemyRace);
        if (!mQuery.exec()) {
            std::cerr << "Failed to retrieve enemy data." << std::endl;
            return;
        }

        QVector<QVector<QString>> enemies;
        while (mQuery.next()) {
            QVector<QString> enemy;
            enemy.push_back(mQuery.value(1).toString());
            enemy.push_back(mQuery.value(2).toString());
            enemy.push_back(QString::number(mQuery.value(3).toInt()));
            enemy.push_back(QString::number(mQuery.value(4).toInt()));
            enemy.push_back(mQuery.value(5).toString());
            enemy.push_back(QString::number(mQuery.value(6).toInt()));
            enemies.push_back(enemy);
        }

        if (enemies.empty()) {
            std::cerr << "No enemies found." << std::endl;
            return;
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(enemies.begin(), enemies.end(), g);

        QString enemyName = enemies[0][0];
        QString enemyClass = enemies[0][1];
        int baseHitpoints = enemies[0][2].toInt();
        int baseDamage = enemies[0][3].toInt();
        int enemyXPreward = enemies[0][5].toInt();

        int heroLevel = 0;
        mQuery.exec("SELECT level FROM currentHero");
        if (mQuery.next()) {
            heroLevel = mQuery.value(0).toInt();
        }
        int hitpoints = baseHitpoints;
        int damage = baseDamage;
        if (enemyRace == "Human" || enemyRace == "Elf" || enemyRace == "Dwarf") {
            hitpoints += (2 * heroLevel);
            damage += heroLevel;
        } else if (enemyRace == "Orc" || enemyRace == "Troll" || enemyRace == "Goblin") {
            hitpoints += (3 * heroLevel);
            damage += (2 * heroLevel);
        } else if (enemyRace == "Dragon" || enemyRace == "Beast") {
            hitpoints += (4 * heroLevel);
            damage += (3 * heroLevel);
        }
        enemyXPreward = enemyXPreward * (1 + heroLevel / 10);
        int XPreward = enemyXPreward + questXPreward;

        mQuery.prepare("INSERT INTO currentEnemy (name, class, hitpoints, damage, race, XPreward) "
                       "VALUES (:name, :class, :hitpoints, :damage, :race, :XPreward)");
        mQuery.bindValue(":name", enemyName);
        mQuery.bindValue(":class", enemyClass);
        mQuery.bindValue(":hitpoints", hitpoints);
        mQuery.bindValue(":damage", damage);
        mQuery.bindValue(":race", enemyRace);
        mQuery.bindValue(":XPreward", enemyXPreward);
        if (!mQuery.exec()) {
            std::cerr << "Failed to insert enemy data." << std::endl;
            std::cout << "Press Enter to return to the main menu...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return;
        }

        Utils::clearConsole();
        std::cout << "You have chosen the quest: " << name.toStdString() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); 
        std::cout << "An enemy has appeared!" << std::endl;
        std::cout << "Press Enter to begin the battle...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

        if (battle()) {
            std::cout << "Congratulations! You have defeated " << enemyName.toStdString() << "." << std::endl;
            std::cout << "You have earned " << XPreward << " experience points and " << goldReward << " gold." << std::endl;
            mQuery.prepare("UPDATE currentHero SET experience = experience + :reward");
            mQuery.bindValue(":reward", XPreward);
            mQuery.exec();
            mQuery.prepare("UPDATE currentHero SET goldAmount = goldAmount + :goldReward");
            mQuery.bindValue(":goldReward", goldReward);
            mQuery.exec();
        } else {
            std::cout << "You have been defeated by " << enemyName.toStdString() << "." << std::endl;
            std::cout << "You have failed the quest." << std::endl;
        }
        std::cout << "Press Enter to return to the main menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    } else if (choice == 4) {
        return;
    } else {
        std::cerr << "Invalid choice!" << std::endl;
        return;
    }
}

void Game::enterDungeon() {
    std::cout << "Enter your choice: ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        Utils::clearConsole();
        std::cout << "========================================" << std::endl;
        std::cout << "           Entering the Dungeon          " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;

        mQuery.exec("SELECT * FROM currentDungeon");
        if (!mQuery.next()) {
            std::cerr << "Failed to retrieve dungeon data." << std::endl;
            return;
        }

        int dungeonXPreward = mQuery.value(3).toInt();
        int goldReward = mQuery.value(4).toInt();
        std::string completionMessage = mQuery.value(6).toString().toStdString();

        mQuery.prepare("SELECT * FROM currentEnemy");
        if (!mQuery.exec()) {
            std::cerr << "Failed to retrieve dungeon enemy data." << std::endl;
            return;
        }

        if (!battle()) {
            std::cout << "You have been defeated." << std::endl;
            std::cout << "Press Enter to return to the main menu...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return;
        }    
        
        mQuery.prepare("SELECT * FROM currentDungeonBoss");
        if (!mQuery.exec() || !mQuery.next()) {
            std::cerr << "Failed to retrieve dungeon boss data." << std::endl;
            return;
        }

        int bossID = mQuery.value(0).toInt();
        std::string bossName = mQuery.value(1).toString().toStdString();
        std::string bossClass = mQuery.value(2).toString().toStdString();
        int bossHitpoints = mQuery.value(3).toInt();
        int bossDamage = mQuery.value(4).toInt();
        std::string bossRace = mQuery.value(5).toString().toStdString();

        mQuery.prepare("INSERT INTO currentEnemy (name, class, hitpoints, damage, race) "
                       "VALUES (:name, :class, :hitpoints, :damage, :race)");
        mQuery.bindValue(":name", QString::fromStdString(bossName));
        mQuery.bindValue(":class", QString::fromStdString(bossClass));
        mQuery.bindValue(":hitpoints", bossHitpoints);
        mQuery.bindValue(":damage", bossDamage);
        mQuery.bindValue(":race", QString::fromStdString(bossRace));
        if (!mQuery.exec()) {
            std::cerr << "Failed to insert dungeon boss data." << std::endl;
            return;
        }
        
        std::cout << "========================================" << std::endl;
        std::cout << "          BOSS ALERT!!!" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Prepare to face the dungeon boss!" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

        if (battle()) {
            std::cout << "Congratulations! You have defeated the dungeon boss." << std::endl;
            std::cout << "You have earned " << dungeonXPreward << " experience points and " << goldReward << " gold." << std::endl;
            std::cout << completionMessage << std::endl; 
            mQuery.prepare("UPDATE currentHero SET experience = experience + :reward");
            mQuery.bindValue(":reward", dungeonXPreward);
            mQuery.exec();
            mQuery.prepare("UPDATE currentHero SET goldAmount = goldAmount + :goldReward");
            mQuery.bindValue(":goldReward", goldReward);
            mQuery.exec();
            mQuery.prepare("UPDATE currentHero SET dungeonProgress = 1 + :dungeonProgress");
            mQuery.bindValue(":dungeonProgress", bossID);
            mQuery.exec();
        } else {
            std::cout << "You have been defeated by the dungeon boss." << std::endl;
            std::cout << "You have failed the dungeon." << std::endl;
        }
        
        std::cout << "Press Enter to return to the main menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    } else if (choice == 2) {
        return;
    } else {
        std::cerr << "Invalid choice!" << std::endl;
        return;
    }
}


