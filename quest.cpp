#include "quest.h"

Quest::Quest() {
    mQuery.exec("CREATE TABLE currentEnemy ("
                "name CHAR(125),"
                "class CHAR(125),"
                "hitpoints INT,"
                "damage INT,"
                "race CHAR(125),"
                "XPreward INT)");

    mQuery.exec("CREATE TABLE currentQuests ("
                "id INT PRIMARY KEY,"
                "name CHAR(125),"
                "description TEXT,"
                "reward INT,"
                "goldReward INT,"
                "enemyRace CHAR(125))");
}

Quest::~Quest() {
    mQuery.exec("DROP TABLE currentEnemy");
    mQuery.exec("DROP TABLE currentQuests");
}

void Quest::clearConsole() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void Quest::loadQuest() {
    if (!mQuery.exec("DELETE FROM currentQuests")) {
        std::cerr << "Failed to clear currentQuests database." << std::endl;
        return;
    }

    int totalQuests = 0;
    mQuery.exec("SELECT COUNT(*) FROM questDatabase");
    if (mQuery.next()) {
        totalQuests = mQuery.value(0).toInt();
    }

    if (totalQuests == 0) {
        std::cerr << "No quests found in the database." << std::endl;
        return;
    }

    std::vector<int> indices(totalQuests);
    for (int i = 0; i < totalQuests; ++i) {
        indices[i] = i + 1;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    int questCount = 0;
    for (int i = 0; i < 3; ++i) {
        int index = indices[i];
        mQuery.prepare("SELECT name, description, reward, goldReward, enemyRace FROM questDatabase WHERE id = :id");
        mQuery.bindValue(":id", index);
        if (mQuery.exec() && mQuery.next()) {
            QString name = mQuery.value(0).toString();
            QString description = mQuery.value(1).toString();
            int reward = mQuery.value(2).toInt();
            int goldReward = mQuery.value(3).toInt();
            QString enemyRace = mQuery.value(4).toString();

            mQuery.prepare("INSERT INTO currentQuests (id, name, description, reward, goldReward, enemyRace) "
                           "VALUES (:id, :name, :description, :reward, :goldReward, :enemyRace)");
            mQuery.bindValue(":id", i + 1); 
            mQuery.bindValue(":name", name);
            mQuery.bindValue(":description", description);
            mQuery.bindValue(":reward", reward);
            mQuery.bindValue(":goldReward", goldReward);
            mQuery.bindValue(":enemyRace", enemyRace);
            if (mQuery.exec()) {
                questCount++;
            } else {
                std::cerr << "Failed to load quest into currentQuest database." << std::endl;
            }
        } else {
            std::cerr << "Failed to load quest with ID: " << index << std::endl;
        }
    }

    
}

void Quest::printQuests() {
    clearConsole();
    
    std::cout << "========================================" << std::endl;
    std::cout << "            Available Quests             " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    if (!mQuery.exec("SELECT * FROM currentQuests")) {
        std::cerr << "Failed to retrieve quest data: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    while (mQuery.next()) {
        int id = mQuery.value(0).toInt();
        QString name = mQuery.value(1).toString();
        QString description = mQuery.value(2).toString();
        int reward = mQuery.value(3).toInt();
        int goldReward = mQuery.value(4).toInt();

        std::cout << "ID: " << id << std::endl;
        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Description: ";
        
        printWrapped(description.toStdString(), 60);
        
        std::cout << "Reward: " << reward << std::endl;
        std::cout << "Gold Reward: " << goldReward << std::endl;
        std::cout << std::endl;
    }
}

void Quest::printWrapped(const std::string &text, int lineLength) {
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

bool Quest::battle() {
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
    
    Character enemy;
    mQuery.exec("SELECT * FROM currentEnemy");
    if (!mQuery.next()) {
        std::cerr << "Failed to retrieve enemy data." << std::endl;
        return false;
    }
    enemy.name = mQuery.value(0).toString();
    enemy.className = mQuery.value(1).toString();
    enemy.level = hero.level;
    enemy.hitpoints = mQuery.value(2).toInt();
    enemy.damage = mQuery.value(3).toInt();

    if (hero.className == "Warrior") {
        hero.evadeChance = 50;
    } else if (hero.className == "Scout") {
        hero.evadeChance = 25;
    } else {
        hero.evadeChance = 0;
    }

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

    while (hero.hitpoints > 0 && enemy.hitpoints > 0) {
        if (heroFirst) {
            attack(hero, enemy, g);

            if (enemy.hitpoints <= 0) {
                return true;
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
                return true;
            }
        }
    }
    return false; 
}

void Quest::attack(Character& attacker, Character& target, std::mt19937& g) {
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

void Quest::chooseQuest() {
    if (!mQuery.exec("DELETE FROM currentEnemy")) {
        std::cerr << "Failed to clear currentEnemy database." << std::endl;
        return;
    }

    clearConsole();
    printQuests();

    std::cout << "Choose a quest: ";
    int choice;
    std::cin >> choice;

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

    mQuery.prepare("SELECT * FROM enemiesDatabase WHERE race = :race");
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
    } else if (enemyRace == "Undead" || enemyRace == "Dragon" || enemyRace == "Beast") {
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

    clearConsole();
    std::cout << "You have chosen the quest: " << name.toStdString() << std::endl;
    std::cout << "You will face " << enemyName.toStdString() << ", the " << enemyClass.toStdString() << "." << std::endl;
    std::cout << "Press Enter to begin the battle...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    clearConsole();
    std::cout << "========================================" << std::endl;
    std::cout << "           The Battle Begins!           " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

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
}


