#include "hero.h"

Hero::Hero() {
    mQuery.exec("CREATE TABLE currentHero ("
                "name CHAR(125)," 
                "class CHAR(125),"
                "level INT," 
                "experience INT," 
                "hitpoints INT,"
                "damage INT)");
}

Hero::~Hero() {   
    selectQuery.prepare("SELECT * FROM currentHero");
    if (!selectQuery.exec()) {
        std::cerr << "Failed to retrieve hero data: " << selectQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    if (!selectQuery.next()) {
        std::cerr << "No hero found in the database." << std::endl;
        return;
    }

    QString name = selectQuery.value(0).toString();
    int level = selectQuery.value(2).toInt();
    int experience = selectQuery.value(3).toInt();
    int hitpoints = selectQuery.value(4).toInt();
    int damage = selectQuery.value(5).toInt();

    updateQuery.prepare("UPDATE heroDatabase SET level = :level, experience = :experience, hitpoints = :hitpoints, damage = :damage WHERE name = :name");
    updateQuery.bindValue(":name", name);
    updateQuery.bindValue(":level", level);
    updateQuery.bindValue(":experience", experience);
    updateQuery.bindValue(":hitpoints", hitpoints);
    updateQuery.bindValue(":damage", damage);

    if (!updateQuery.exec()) {
        std::cerr << "Failed to update hero data: " << updateQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    mQuery.exec("DROP TABLE currentHero");
}

void Hero::clearConsole() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void Hero::levelSystem() {
    selectQuery.prepare("SELECT * FROM currentHero");
    if (!selectQuery.exec()) {
        std::cerr << "Failed to retrieve hero data: " << selectQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    if (!selectQuery.next()) {
        std::cerr << "No hero found in the database." << std::endl;
        return;
    }

    int level = selectQuery.value(2).toInt();
    int experience = selectQuery.value(3).toInt();

    while (experience >= level * 1000) {
        mQuery.prepare("UPDATE currentHero SET experience = experience - :experience, level = level + 1");
        mQuery.bindValue(":experience", level * 1000);
        if (!mQuery.exec()) {
            std::cerr << "Failed to update hero stats: " << mQuery.lastError().text().toStdString() << std::endl;
            return;
        }
        level++;
        levelUp(level);
    }
}

void Hero::levelUp(int level) {
    selectQuery.prepare("SELECT * FROM currentHero");
    if (!selectQuery.exec()) {
        std::cerr << "Failed to retrieve hero data: " << selectQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    if (!selectQuery.next()) {
        std::cerr << "No hero found in the database." << std::endl;
        return;
    }
    
    QString heroName = selectQuery.value(0).toString();
    QString heroClass = selectQuery.value(1).toString();
    int hitpoints = selectQuery.value(4).toInt();
    int damage = selectQuery.value(5).toInt();

    QString updateStatement;
    updateStatement = "UPDATE currentHero SET level = :level";

    int hitpointsIncrease = 0;
    int damageIncrease = 0;

    if (heroClass == "Warrior") {
        hitpointsIncrease = 10;
        damageIncrease = 4;
    } else if (heroClass == "Scout") {
        hitpointsIncrease = 8;
        damageIncrease = 6;
    } else if (heroClass == "Mage") {
        hitpointsIncrease = 4;
        damageIncrease = 10;
    }

    updateStatement += ", hitpoints = hitpoints + :hitpoints, damage = damage + :damage WHERE class = :class";
    updateQuery.prepare(updateStatement);
    updateQuery.bindValue(":level", level);
    updateQuery.bindValue(":hitpoints", hitpointsIncrease);
    updateQuery.bindValue(":damage", damageIncrease);
    updateQuery.bindValue(":class", heroClass);

    if (!updateQuery.exec()) {
        std::cerr << "Failed to update hero stats: " << updateQuery.lastError().text().toStdString() << std::endl;
        return;
    }
    std::cout << "Congratulations! Your hero, " << heroName.toStdString() << ", has leveled up to level " << level << "." << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}


void Hero::printHero() {
    selectQuery.prepare("SELECT * FROM currentHero");
    if (!selectQuery.exec()) {
        std::cerr << "Failed to retrieve hero data: " << selectQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    if (!selectQuery.next()) {
        std::cerr << "No hero found in the database." << std::endl;
        return;
    }

    clearConsole();
    
    std::cout << "========================================" << std::endl;
    std::cout << "              Hero Information           " << std::endl;
    std::cout << "========================================" << std::endl;

    QString name = selectQuery.value(0).toString();
    QString heroClass = selectQuery.value(1).toString();
    int level = selectQuery.value(2).toInt();
    int experience = selectQuery.value(3).toInt();
    int hitpoints = selectQuery.value(4).toInt();
    int damage = selectQuery.value(5).toInt();

    std::cout << "Name: " << name.toStdString() << std::endl;
    std::cout << "Class: " << heroClass.toStdString() << std::endl;
    std::cout << "Level: " << level << std::endl;
    std::cout << "Experience: " << experience << std::endl;
    std::cout << "Hitpoints: " << hitpoints << std::endl;
    std::cout << "Damage: " << damage << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "Press Enter to return to the main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

