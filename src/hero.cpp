#include "../include/hero.h"

Hero::Hero() {
    mQuery.exec("CREATE TABLE heroDatabase ("
                "id INT PRIMARY KEY AUTO_INCREMENT,"
                "name CHAR(125),"
                "class CHAR(125),"
                "level INT,"
                "experience INT,"
                "hitpoints INT,"
                "damage INT,"
                "goldAmount INT)");
    
    mQuery.exec("CREATE TABLE currentHero ("
                "name CHAR(125)," 
                "class CHAR(125),"
                "level INT,"
                "experience INT,"
                "hitpoints INT,"
                "damage INT,"
                "goldAmount INT)");
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
    int goldAmount = selectQuery.value(6).toInt();

    updateQuery.prepare("UPDATE heroDatabase SET level = :level, experience = :experience, hitpoints = :hitpoints, damage = :damage, goldAmount = :goldAmount WHERE name = :name");
    updateQuery.bindValue(":name", name);
    updateQuery.bindValue(":level", level);
    updateQuery.bindValue(":experience", experience);
    updateQuery.bindValue(":hitpoints", hitpoints);
    updateQuery.bindValue(":damage", damage);
    updateQuery.bindValue(":goldAmount", goldAmount);

    if (!updateQuery.exec()) {
        std::cerr << "Failed to update hero data: " << updateQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    mQuery.exec("DROP TABLE currentHero");
}

bool Hero::newHero() {
    std::string name;
    int choice;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter hero name: ";
    std::getline(std::cin, name);
    std::cout << std::endl;
    
    std::cout << "Choose your hero class:" << std::endl;
    std::cout << "1. Warrior" << std::endl;
    std::cout << "2. Scout" << std::endl;
    std::cout << "3. Mage" << std::endl;
    std::cout << std::endl;

    std::cout << "Enter your choice (1-3): ";
    std::cin >> choice;

    while (choice < 1 || choice > 3) {
        std::cout << "Invalid choice. Please enter a number between 1 and 3: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> choice;
    }

    std::string heroClass;
    switch (choice) {
        case 1: 
            heroClass = "Warrior";
            break;
        case 2: 
            heroClass = "Scout";
            break;
        case 3: 
            heroClass = "Mage";
            break;
    }

    mQuery.prepare("INSERT INTO heroDatabase (name, class, level, experience, hitpoints, damage, goldAmount) "
                   "VALUES (:name, :class, :level, :experience, :hitpoints, :damage, :goldAmount)");
    mQuery.bindValue(":name", QString::fromStdString(name));
    mQuery.bindValue(":class", QString::fromStdString(heroClass));
    mQuery.bindValue(":level", 1);
    mQuery.bindValue(":experience", 0);
    mQuery.bindValue(":hitpoints", 25);
    mQuery.bindValue(":damage", 8);
    mQuery.bindValue(":goldAmount", 0);

    if (!mQuery.exec()) {
        std::cout << "Failed to add hero to heroDatabase: " << mQuery.lastError().text().toStdString() << std::endl;
        return false;
    }

    mQuery.exec("DELETE FROM currentHero");
    mQuery.prepare("INSERT INTO currentHero (name, class, level, experience, hitpoints, damage, goldAmount) "
                   "VALUES (:name, :class, :level, :experience, :hitpoints, :damage, :goldAmount)");
    mQuery.bindValue(":name", QString::fromStdString(name));
    mQuery.bindValue(":class", QString::fromStdString(heroClass));
    mQuery.bindValue(":level", 1);
    mQuery.bindValue(":experience", 0);
    mQuery.bindValue(":hitpoints", 25);
    mQuery.bindValue(":damage", 8);
    mQuery.bindValue(":goldAmount", 0);

    if (!mQuery.exec()) {
        std::cout << "Failed to add hero to currentHero: " << mQuery.lastError().text().toStdString() << std::endl;
        return false;
    }

    Utils::clearConsole();
    std::cout << "Welcome, " << name << "! Your journey begins now in the Realm of Ubuntu.\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return true;
}

bool Hero::loadHero() {
    mQuery.prepare("SELECT * FROM heroDatabase");

    if (!mQuery.exec()) {
        std::cerr << "Failed to retrieve hero data: " << mQuery.lastError().text().toStdString() << std::endl;
        std::cout << "Press Enter to return to the main menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return false;
    }

    if (!mQuery.next()) {
        std::cerr << "No heroes found in the database." << std::endl;
        std::cout << "Press Enter to return to the main menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return false;
    }

    std::cout << "Available Heroes:\n";
    do {
        QString name = mQuery.value(1).toString();
        QString heroClass = mQuery.value(2).toString();
        int level = mQuery.value(3).toInt();
        int experience = mQuery.value(4).toInt();
        int hitpoints = mQuery.value(5).toInt();
        int damage = mQuery.value(6).toInt();
        int goldAmount = mQuery.value(7).toInt();

        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Class: " << heroClass.toStdString() << std::endl;
        std::cout << "Level: " << level << std::endl;
        std::cout << "Experience: " << experience << std::endl;
        std::cout << "Hitpoints: " << hitpoints << std::endl;
        std::cout << "Damage: " << damage << std::endl;
        std::cout << "Gold: " << goldAmount << std::endl;
        std::cout << std::endl;

    } while (mQuery.next());

    std::string heroName;

    std::cout << "Enter the name of the hero you wish to load: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, heroName);

    mQuery.prepare("DELETE FROM currentHero");
    if (!mQuery.exec()) {
        std::cerr << "Failed to delete previous hero data: " << mQuery.lastError().text().toStdString() << std::endl;
        return false;
    }

    mQuery.prepare("SELECT * FROM heroDatabase WHERE name = :name");
    mQuery.bindValue(":name", QString::fromStdString(heroName));

    if (!mQuery.exec()) {
        std::cerr << "Failed to retrieve hero data: " << mQuery.lastError().text().toStdString() << std::endl;
        return false;
    }

    if (!mQuery.next()) {
        Utils::clearConsole();
        std::cerr << "No hero found with the name " << heroName << std::endl;
        std::cout << "Press Enter to return to the main menu...";
        std::cin.get();
        return false;
    }

    QString name = mQuery.value(1).toString();
    QString heroClass = mQuery.value(2).toString();
    int level = mQuery.value(3).toInt();
    int experience = mQuery.value(4).toInt();
    int hitpoints = mQuery.value(5).toInt();
    int damage = mQuery.value(6).toInt();
    int goldAmount = mQuery.value(7).toInt();

    mQuery.prepare("INSERT INTO currentHero (name, class, level, experience, hitpoints, damage, goldAmount) "
                   "VALUES (:name, :class, :level, :experience, :hitpoints, :damage, :goldAmount)");
    mQuery.bindValue(":name", name);
    mQuery.bindValue(":class", heroClass);
    mQuery.bindValue(":level", level);
    mQuery.bindValue(":experience", experience);
    mQuery.bindValue(":hitpoints", hitpoints);
    mQuery.bindValue(":damage", damage);
    mQuery.bindValue(":goldAmount", goldAmount);

    if (!mQuery.exec()) {
        std::cerr << "Failed to insert hero data: " << mQuery.lastError().text().toStdString() << std::endl;
        return false;
    }

    Utils::clearConsole();
    std::cout << "Hero loaded successfully." << std::endl;
    std::cout << "Welcome, " << name.toStdString() << "! Your journey begins now in the Realm of Ubuntu.\n";
    std::cout << "Press Enter to continue...";
    std::cin.get();
    return true;
}

void Hero::deleteHero() {
    std::cout << "Hero Information" << std::endl;
    if (!mQuery.exec("SELECT * FROM heroDatabase")) {
        std::cerr << "Failed to retrieve hero data: " << mQuery.lastError().text().toStdString() << std::endl;
        std::cout << "Press Enter to return to the main menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    if (!mQuery.next()) {
        std::cerr << "No hero found in the database." << std::endl;
        std::cout << "Press Enter to return to the main menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    bool heroFound = false;
    do {
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
        heroFound = true;
    } while (mQuery.next());

    if (!heroFound) {
        std::cerr << "No hero found in the database." << std::endl;
        std::cout << "Press Enter to return to the main menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    int heroId;

    std::cout << "Enter hero ID to delete: ";
    if (!(std::cin >> heroId)) {
        Utils::clearConsole();
        std::cerr << "Invalid input for hero id." << std::endl;
        std::cout << "Press Enter to return to the main menu...";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::cin.get();
        return;
    }

    mQuery.prepare("SELECT * FROM heroDatabase WHERE id = :id");
    mQuery.bindValue(":id", heroId);
    if (!mQuery.exec() || !mQuery.next()) {
        Utils::clearConsole();
        std::cerr << "No hero found with ID " << heroId << std::endl;
        std::cout << "Press Enter to return to the main menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    mQuery.prepare("DELETE FROM heroDatabase WHERE id = :id");
    mQuery.bindValue(":id", heroId);
    if (!mQuery.exec()) {
        std::cout << "Failed to delete hero from database: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }
    Utils::clearConsole();
    std::cout << "Hero deleted successfully from database" << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Hero::heroInfo() {
    selectQuery.prepare("SELECT * FROM currentHero");
    if (!selectQuery.exec()) {
        std::cerr << "Failed to retrieve hero data: " << selectQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    if (!selectQuery.next()) {
        std::cerr << "No hero found in the database." << std::endl;
        return;
    }

    Utils::clearConsole();
    
    std::cout << "========================================" << std::endl;
    std::cout << "              Hero Information           " << std::endl;
    std::cout << "========================================" << std::endl;

    QString name = selectQuery.value(0).toString();
    QString heroClass = selectQuery.value(1).toString();
    int level = selectQuery.value(2).toInt();
    int experience = selectQuery.value(3).toInt();
    int hitpoints = selectQuery.value(4).toInt();
    int damage = selectQuery.value(5).toInt();
    int goldAmount = selectQuery.value(6).toInt();

    std::cout << "Name: " << name.toStdString() << std::endl;
    std::cout << "Class: " << heroClass.toStdString() << std::endl;
    std::cout << "Level: " << level << std::endl;
    std::cout << "Experience: " << experience << std::endl;
    std::cout << "Hitpoints: " << hitpoints << std::endl;
    std::cout << "Damage: " << damage << std::endl;
    std::cout << "Gold: " << goldAmount << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "Press Enter to return to the main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Hero::levelSystem() {
    Utils::clearConsole();

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
    if (experience >= level * 1000){
        while (experience >= level * 1000) {
            mQuery.prepare("UPDATE currentHero SET experience = experience - :experience, level = level + 1");
            mQuery.bindValue(":experience", level * 1000);
            if (!mQuery.exec()) {
                std::cerr << "Failed to update hero stats: " << mQuery.lastError().text().toStdString() << std::endl;
                return;
            }
            experience -= level * 1000;
            level++;
        
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
            std::cout << "Congratulations! Your hero has leveled up to level " << level << "." << std::endl;
        }
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}


