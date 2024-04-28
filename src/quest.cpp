#include "../include/quest.h"

Quest::Quest() {
    mQuery.exec("CREATE TABLE questDatabase ("  
                "id INT PRIMARY KEY AUTO_INCREMENT,"
                "name CHAR(125),"
                "description TEXT,"
                "reward INT,"
                "goldReward INT,"
                "enemyRace CHAR(125))");

    mQuery.exec("CREATE TABLE currentQuests ("
                "id INT PRIMARY KEY,"
                "name CHAR(125),"
                "description TEXT,"
                "reward INT,"
                "goldReward INT,"
                "enemyRace CHAR(125))");
}

Quest::~Quest() {
    mQuery.exec("DROP TABLE questDatabase");
    mQuery.exec("DROP TABLE currentQuests");
}

void Quest::loadQuestsFromFile() {
    std::ifstream infile("quest_data.txt");
    if (!infile.is_open()) {
        std::cerr << "Failed to open quest file." << std::endl;
        return;
    }
    
    std::vector<std::string> questData;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string garbage, restOfLine;
        if (iss >> garbage) {  
            std::getline(iss, restOfLine); 
            questData.push_back(restOfLine);
        }
    } 

    for (int i = 0; i < questData.size(); i += 5) {
        std::string name = questData[i];
        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t") + 1);      
        std::string description = questData[i + 1];
        description.erase(0, description.find_first_not_of(" \t"));
        description.erase(description.find_last_not_of(" \t") + 1);
        int reward = std::stoi(questData[i + 2]);
        int goldReward = std::stoi(questData[i + 3]);
        std::string enemyRace = questData[i + 4];
        enemyRace.erase(0, enemyRace.find_first_not_of(" \t"));
        enemyRace.erase(enemyRace.find_last_not_of(" \t") + 1);

        mQuery.prepare("INSERT INTO questDatabase (name, description, reward, goldReward, enemyRace) VALUES (:name, :description, :reward, :goldReward, :enemyRace)");
        mQuery.bindValue(":name", QString::fromStdString(name));
        mQuery.bindValue(":description", QString::fromStdString(description));
        mQuery.bindValue(":reward", reward);
        mQuery.bindValue(":goldReward", goldReward);
        mQuery.bindValue(":enemyRace", QString::fromStdString(enemyRace));

        if (!mQuery.exec()) {
            std::cerr << "Failed to insert quest into database: " << mQuery.lastError().text().toStdString() << std::endl;
        }
    }

    infile.close();
}

void Quest::loadQuest() {
    mQuery.exec("DELETE FROM currentQuests");
    mQuery.exec("DELETE FROM currentEnemy");

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
    Utils::clearConsole();
    
    std::cout << "========================================" << std::endl;
    std::cout << "            Available Quests             " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "Select a quest:" << std::endl;

    if (!mQuery.exec("SELECT * FROM currentQuests")) {
        std::cerr << "Failed to retrieve quest data: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }
    QVector<QString> questNames;
    while (mQuery.next()) {
        int id = mQuery.value(0).toInt();
        QString name = mQuery.value(1).toString();
        questNames.push_back(name);
        QString description = mQuery.value(2).toString();
        int reward = mQuery.value(3).toInt();
        int goldReward = mQuery.value(4).toInt();

        std::cout << "ID: " << id << std::endl;
        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Description: ";
        
        Utils::printWrapped(description.toStdString(), 60);
        
        std::cout << "Reward: " << reward << std::endl;
        std::cout << "Gold Reward: " << goldReward << std::endl;
        std::cout << std::endl;
    }

    std::cout << "1. " << questNames[0].toStdString() << std::endl;
    std::cout << "2. " << questNames[1].toStdString() << std::endl;
    std::cout << "3. " << questNames[2].toStdString() << std::endl;
    std::cout << "4. Go back" << std::endl;
}


