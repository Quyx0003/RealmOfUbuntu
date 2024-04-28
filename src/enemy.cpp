#include "../include/enemy.h"

Enemy::Enemy() {
    mQuery.exec("CREATE TABLE enemyDatabase ("
                "id INT PRIMARY KEY AUTO_INCREMENT,"
                "name CHAR(125),"
                "class CHAR(125),"
                "hitpoints INT,"
                "damage INT,"
                "race CHAR(125),"
                "XPreward INT)");

    mQuery.exec("CREATE TABLE currentEnemy ("
                "name CHAR(125),"
                "class CHAR(125),"
                "hitpoints INT,"
                "damage INT,"
                "race CHAR(125),"
                "XPreward INT)");
}

Enemy::~Enemy() {
    mQuery.exec("DROP TABLE enemyDatabase");
    mQuery.exec("DROP TABLE currentEnemy");
}

void Enemy::loadEnemyFromFile() {
    std::ifstream infile("enemy_data.txt");
    if (!infile.is_open()) {
        std::cerr << "Failed to open enemy file." << std::endl;
        return;
    }
    
    std::vector<std::string> enemyData;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string garbage, restOfLine;
        if (iss >> garbage) {  
            std::getline(iss, restOfLine); 
            enemyData.push_back(restOfLine);
        }
    } 

    for (int i = 0; i < enemyData.size(); i += 6) {
        std::string name = enemyData[i];
        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t") + 1);
        std::string enemyclass = enemyData[i + 1];
        enemyclass.erase(0, enemyclass.find_first_not_of(" \t"));
        enemyclass.erase(enemyclass.find_last_not_of(" \t") + 1);
        int hitpoints = std::stoi(enemyData[i + 2]);
        int damage = std::stoi(enemyData[i + 3]);
        std::string race = enemyData[i + 4];
        race.erase(0, race.find_first_not_of(" \t"));
        race.erase(race.find_last_not_of(" \t") + 1);
        int XPreward = std::stoi(enemyData[i + 5]);

        mQuery.prepare("INSERT INTO enemyDatabase (name, class, hitpoints, damage, race, XPreward) VALUES (:name, :class, :hitpoints, :damage, :race, :XPreward)");
        mQuery.bindValue(":name", QString::fromStdString(name));
        mQuery.bindValue(":class", QString::fromStdString(enemyclass));
        mQuery.bindValue(":hitpoints", hitpoints);
        mQuery.bindValue(":damage", damage);
        mQuery.bindValue(":race", QString::fromStdString(race));
        mQuery.bindValue(":XPreward", XPreward);

        if (!mQuery.exec()) {
            std::cerr << "Failed to insert enemy into database: " << mQuery.lastError().text().toStdString() << std::endl;
        }
    }

    infile.close();
}


