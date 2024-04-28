#include "../include/dungeon.h"

Dungeon::Dungeon() {
    mQuery.exec("CREATE TABLE dungeonDatabase ("  
                "id INT PRIMARY KEY AUTO_INCREMENT,"
                "name CHAR(125),"
                "description TEXT,"
                "reward INT,"
                "goldReward INT,"
                "bossName CHAR(125),"
                "completionMessage TEXT)");

    mQuery.exec("CREATE TABLE currentDungeon ("
                "id INT PRIMARY KEY,"
                "name CHAR(125),"
                "description TEXT,"
                "reward INT,"
                "goldReward INT,"
                "bossName CHAR(125),"
                "completionMessage TEXT)");

    mQuery.exec("CREATE TABLE dungeonBossDatabase ("
                "id INT PRIMARY KEY AUTO_INCREMENT,"
                "name CHAR(125),"
                "class CHAR(125),"
                "hitpoints INT,"
                "damage INT,"
                "enemyRace CHAR(125))");

    mQuery.exec("CREATE TABLE currentDungeonBoss ("
                "id INT PRIMARY KEY,"
                "name CHAR(125),"
                "class CHAR(125),"
                "hitpoints INT,"
                "damage INT,"
                "enemyRace CHAR(125))");

    mQuery.exec("CREATE TABLE dungeonEnemyDatabase ("
                "id INT PRIMARY KEY AUTO_INCREMENT,"
                "name CHAR(125),"
                "class CHAR(125),"
                "hitpoints INT,"
                "damage INT,"
                "enemyRace CHAR(125),"
                "serves CHAR(125))");
}

Dungeon::~Dungeon() {
    mQuery.exec("DROP TABLE dungeonDatabase");
    mQuery.exec("DROP TABLE currentDungeon");
    mQuery.exec("DROP TABLE dungeonBossDatabase");
    mQuery.exec("DROP TABLE currentDungeonBoss");
    mQuery.exec("DROP TABLE dungeonEnemyDatabase");
}

void Dungeon::loadDungeonFromFile() {
    std::ifstream infile("dungeon_data.txt");
    if (!infile.is_open()) {
        std::cerr << "Failed to open dungeon file." << std::endl;
        return;
    }
    
    std::vector<std::string> dungeonData;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string garbage, restOfLine;
        if (iss >> garbage) {  
            std::getline(iss, restOfLine); 
            dungeonData.push_back(restOfLine);
        }
    } 

    for (int i = 0; i < dungeonData.size(); i += 6) {
        std::string name = dungeonData[i];
        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t") + 1);      
        std::string description = dungeonData[i + 1];
        description.erase(0, description.find_first_not_of(" \t"));
        description.erase(description.find_last_not_of(" \t") + 1);
        int reward = std::stoi(dungeonData[i + 2]);
        int goldReward = std::stoi(dungeonData[i + 3]);
        std::string bossName = dungeonData[i + 4];
        bossName.erase(0, bossName.find_first_not_of(" \t"));
        bossName.erase(bossName.find_last_not_of(" \t") + 1);
        std::string completionMessage = dungeonData[i + 5];
        completionMessage.erase(0, completionMessage.find_first_not_of(" \t"));
        completionMessage.erase(completionMessage.find_last_not_of(" \t") + 1);

        mQuery.prepare("INSERT INTO dungeonDatabase (name, description, reward, goldReward, bossName, completionMessage) VALUES (:name, :description, :reward, :goldReward, :bossName, :completionMessage)");
        mQuery.bindValue(":name", QString::fromStdString(name));
        mQuery.bindValue(":description", QString::fromStdString(description));
        mQuery.bindValue(":reward", reward);
        mQuery.bindValue(":goldReward", goldReward);
        mQuery.bindValue(":bossName", QString::fromStdString(bossName));
        mQuery.bindValue(":completionMessage", QString::fromStdString(completionMessage));

        if (!mQuery.exec()) {
            std::cerr << "Failed to insert dungeon data." << std::endl;
        }
    }

    infile.close();
}

void Dungeon::loadDungeonBossFromFile() {
    std::ifstream infile("dungeon_boss_data.txt");
    if (!infile.is_open()) {
        std::cerr << "Failed to open dungeon boss file." << std::endl;
        return;
    }
    
    std::vector<std::string> dungeonBossData;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string garbage, restOfLine;
        if (iss >> garbage) {  
            std::getline(iss, restOfLine); 
            dungeonBossData.push_back(restOfLine);
        }
    } 

    for (int i = 0; i < dungeonBossData.size(); i += 5) {
        std::string name = dungeonBossData[i];
        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t") + 1);      
        std::string enemyclass = dungeonBossData[i + 1];
        enemyclass.erase(0, enemyclass.find_first_not_of(" \t"));
        enemyclass.erase(enemyclass.find_last_not_of(" \t") + 1);
        int hitpoints = std::stoi(dungeonBossData[i + 2]);
        int damage = std::stoi(dungeonBossData[i + 3]);
        std::string enemyRace = dungeonBossData[i + 4];
        enemyRace.erase(0, enemyRace.find_first_not_of(" \t"));
        enemyRace.erase(enemyRace.find_last_not_of(" \t") + 1);

        mQuery.prepare("INSERT INTO dungeonBossDatabase (name, class, hitpoints, damage, enemyRace) VALUES (:name, :class, :hitpoints, :damage, :enemyRace)");
        mQuery.bindValue(":name", QString::fromStdString(name));
        mQuery.bindValue(":class", QString::fromStdString(enemyclass));
        mQuery.bindValue(":hitpoints", hitpoints);
        mQuery.bindValue(":damage", damage);
        mQuery.bindValue(":enemyRace", QString::fromStdString(enemyRace));

        if (!mQuery.exec()) {
            std::cerr << "Failed to insert dungeon boss data." << std::endl;
        }
    }

    infile.close();
}

void Dungeon::loadDungeonEnemyFromFile() {
    std::ifstream infile("dungeon_enemy_data.txt");
    if (!infile.is_open()) {
        std::cerr << "Failed to open dungeon enemy file." << std::endl;
        return;
    }
    
    std::vector<std::string> dungeonEnemyData;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string garbage, restOfLine;
        if (iss >> garbage) {  
            std::getline(iss, restOfLine); 
            dungeonEnemyData.push_back(restOfLine);
        }
    } 

    for (int i = 0; i < dungeonEnemyData.size(); i += 6) {
        std::string name = dungeonEnemyData[i];
        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t") + 1);      
        std::string enemyclass = dungeonEnemyData[i + 1];
        enemyclass.erase(0, enemyclass.find_first_not_of(" \t"));
        enemyclass.erase(enemyclass.find_last_not_of(" \t") + 1);
        int hitpoints = std::stoi(dungeonEnemyData[i + 2]);
        int damage = std::stoi(dungeonEnemyData[i + 3]);
        std::string enemyRace = dungeonEnemyData[i + 4];
        enemyRace.erase(0, enemyRace.find_first_not_of(" \t"));
        enemyRace.erase(enemyRace.find_last_not_of(" \t") + 1);
        std::string serves = dungeonEnemyData[i + 5];
        serves.erase(0, serves.find_first_not_of(" \t"));
        serves.erase(serves.find_last_not_of(" \t") + 1);

        mQuery.prepare("INSERT INTO dungeonEnemyDatabase (name, class, hitpoints, damage, enemyRace, serves) VALUES (:name, :class, :hitpoints, :damage, :enemyRace, :serves)");
        mQuery.bindValue(":name", QString::fromStdString(name));
        mQuery.bindValue(":class", QString::fromStdString(enemyclass));
        mQuery.bindValue(":hitpoints", hitpoints);
        mQuery.bindValue(":damage", damage);
        mQuery.bindValue(":enemyRace", QString::fromStdString(enemyRace));
        mQuery.bindValue(":serves", QString::fromStdString(serves));

        if (!mQuery.exec()) {
            std::cerr << "Failed to insert dungeon enemy data." << std::endl;
        }
    }

    infile.close();
}

void Dungeon::loadDungeon() {
    mQuery.exec("DELETE FROM currentDungeonBoss");
    mQuery.exec("DELETE FROM currentDungeon");
    mQuery.exec("DELETE FROM currentEnemy");
        
    mQuery.exec("SELECT dungeonProgress FROM currentHero");
    if (!mQuery.next()) {
        std::cerr << "Failed to retrieve dungeon progress." << std::endl;
        return;
    }

    int dungeonProgress = mQuery.value(0).toInt();
    mQuery.prepare("SELECT * FROM dungeonDatabase WHERE id = :id");
    mQuery.bindValue(":id", dungeonProgress);
    if (!mQuery.exec() || !mQuery.next()) {
        std::cerr << "Failed to retrieve dungeon data." << std::endl;
        return;
    }

    QString name = mQuery.value(1).toString();
    QString description = mQuery.value(2).toString();
    int reward = mQuery.value(3).toInt();
    int goldReward = mQuery.value(4).toInt();
    QString bossName = mQuery.value(5).toString();
    QString completionMessage = mQuery.value(6).toString();

    mQuery.prepare("INSERT INTO currentDungeon (id, name, description, reward, goldReward, bossName, completionMessage) VALUES (:id, :name, :description, :reward, :goldReward, :bossName, :completionMessage)");
    mQuery.bindValue(":id", dungeonProgress);
    mQuery.bindValue(":name", name);
    mQuery.bindValue(":description", description);
    mQuery.bindValue(":reward", reward);
    mQuery.bindValue(":goldReward", goldReward);
    mQuery.bindValue(":bossName", bossName);
    mQuery.bindValue(":completionMessage", completionMessage);

    if (!mQuery.exec()) {
        std::cerr << "Failed to insert current dungeon data." << std::endl;
    }

    mQuery.prepare("SELECT * FROM dungeonBossDatabase WHERE name = :name");
    mQuery.bindValue(":name", bossName);
    if (!mQuery.exec() || !mQuery.next()) {
        std::cerr << "Failed to retrieve dungeon boss data." << std::endl;
        return;
    }

    QString bossClass = mQuery.value(2).toString();
    int bossHitpoints = mQuery.value(3).toInt();
    int bossDamage = mQuery.value(4).toInt();
    QString bossRace = mQuery.value(5).toString();

    mQuery.prepare("INSERT INTO currentDungeonBoss (id, name, class, hitpoints, damage, enemyRace) VALUES (:id, :name, :class, :hitpoints, :damage, :enemyRace)");
    mQuery.bindValue(":id", dungeonProgress);
    mQuery.bindValue(":name", bossName);
    mQuery.bindValue(":class", bossClass);
    mQuery.bindValue(":hitpoints", bossHitpoints);
    mQuery.bindValue(":damage", bossDamage);
    mQuery.bindValue(":enemyRace", bossRace);

    if (!mQuery.exec()) {
        std::cerr << "Failed to insert current dungeon boss data." << std::endl;
    }

    mQuery.prepare("SELECT * FROM dungeonEnemyDatabase WHERE serves = :serves");
    mQuery.bindValue(":serves", bossName);
    if (!mQuery.exec()) {
        std::cerr << "Failed to retrieve dungeon enemy data." << std::endl;
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
        enemies.push_back(enemy);
    }

    if (enemies.empty()) {
        std::cerr << "No enemies found." << std::endl;
        return;
    }

    for (int i = 0; i < enemies.size(); i++) {
        mQuery.prepare("INSERT INTO currentEnemy (name, class, hitpoints, damage, race) VALUES (:name, :class, :hitpoints, :damage, :race)");
        mQuery.bindValue(":name", enemies[i][0]);
        mQuery.bindValue(":class", enemies[i][1]);
        mQuery.bindValue(":hitpoints", enemies[i][2].toInt());
        mQuery.bindValue(":damage", enemies[i][3].toInt());
        mQuery.bindValue(":race", enemies[i][4]);

        if (!mQuery.exec()) {
            std::cerr << "Failed to insert current dungeon enemy data." << std::endl;
        }
    }
}

void Dungeon::printDungeon() {
    Utils::clearConsole();
    std::cout << "========================================" << std::endl;
    std::cout << "       Entering the Dungeon Menu        " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    if (!mQuery.exec("SELECT * FROM currentDungeon")) {
        std::cerr << "Failed to retrieve dungeon data: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    while (mQuery.next()) {
        QString name = mQuery.value(1).toString();
        QString description = mQuery.value(2).toString();
        QString bossName = mQuery.value(5).toString();

        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Description: ";
        
        Utils::printWrapped(description.toStdString(), 60);

        std::cout << std::endl;
    }

    std::cout << "Are you ready to enter the dungeon?" << std::endl;
    std::cout << "1. Enter" << std::endl;
    std::cout << "2. Go back" << std::endl;
}


