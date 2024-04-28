#include "../include/debug.h"

void Debug::printHeroDatabase() {
    std::cout << "Hero Information" << std::endl;
    if (!mQuery.exec("SELECT * FROM heroDatabase")) {
        std::cerr << "Failed to retrieve hero data: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    if (!mQuery.next()) {
        std::cerr << "No hero found in the database." << std::endl;
        return;
    }

    while (mQuery.next()) {
        int id = mQuery.value(0).toInt();
        QString name = mQuery.value(1).toString();
        QString heroClass = mQuery.value(2).toString();
        int level = mQuery.value(3).toInt();
        int experience = mQuery.value(4).toInt();
        int hitpoints = mQuery.value(5).toInt();
        int damage = mQuery.value(6).toInt();
        int goldAmount = mQuery.value(7).toInt();

        std::cout << "ID: " << id << std::endl;
        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Class: " << heroClass.toStdString() << std::endl;
        std::cout << "Level: " << level << std::endl;
        std::cout << "Experience: " << experience << std::endl;
        std::cout << "Hitpoints: " << hitpoints << std::endl;
        std::cout << "Damage: " << damage << std::endl;
        std::cout << "Gold: " << goldAmount << std::endl;
        std::cout << std::endl;
    }
}

void Debug::printEnemyDatabase() {
    std::cout << "Enemy Information" << std::endl;
    if (!mQuery.exec("SELECT * FROM enemyDatabase")) {
        std::cerr << "Failed to retrieve enemy data: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    while (mQuery.next()) {
        QString name = mQuery.value(1).toString();
        QString enemyclass = mQuery.value(2).toString();
        int hitpoints = mQuery.value(3).toInt();
        int damage = mQuery.value(4).toInt();
        QString race = mQuery.value(5).toString();
        int XPreward = mQuery.value(6).toInt();

        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Class: " << enemyclass.toStdString() << std::endl;
        std::cout << "Hitpoints: " << hitpoints << std::endl;
        std::cout << "Damage: " << damage << std::endl;
        std::cout << "race: " << race.toStdString() << std::endl;
        std::cout << "XPreward: " << XPreward << std::endl;
        std::cout << std::endl;
    }
}

void Debug::printQuestsDatabase() {
    std::cout << "Quest Information" << std::endl;
    if (!mQuery.exec("SELECT * FROM questDatabase")) {
        std::cerr << "Failed to retrieve quest data: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    while (mQuery.next()) {
        QString name = mQuery.value(1).toString();
        QString description = mQuery.value(2).toString();
        int reward = mQuery.value(3).toInt();
        int goldReward = mQuery.value(4).toInt();
        QString enemyRace = mQuery.value(5).toString();

        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Description: ";
        
        Utils::printWrapped(description.toStdString(), 60);
        
        std::cout << "Reward: " << reward << std::endl;
        std::cout << "Gold Reward: " << goldReward << std::endl;
        std::cout << "Enemy Race: " << enemyRace.toStdString() << std::endl;
        std::cout << std::endl;
    }
}

void Debug::GiveExperience(int experience) {
    mQuery.prepare("UPDATE currentHero SET experience = experience + :experience");
    mQuery.bindValue(":experience", experience);
    if (!mQuery.exec()) {
        std::cerr << "Failed to update hero experience: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }
}


