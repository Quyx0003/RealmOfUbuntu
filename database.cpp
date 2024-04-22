#include "database.h"

Database::Database() {
    db.setHostName("localhost");
    db.setDatabaseName("RealmOfUbuntu");
    db.setUserName("root");
    db.setPassword("Fyu75wfr");
    
    if (db.open()) {
        std::cout << "Connected to database" << std::endl;
    } else {
        std::cout << "Failed to connect to database: " << db.lastError().text().toStdString() << std::endl;
    }

    mQuery.exec("CREATE TABLE heroDatabase ("
                "id INT PRIMARY KEY AUTO_INCREMENT,"
                "name CHAR(125)," 
                "class CHAR(125),"
                "level INT," 
                "experience INT," 
                "hitpoints INT,"
                "damage INT)");

    mQuery.exec("CREATE TABLE enemiesDatabase ("
                "id INT PRIMARY KEY AUTO_INCREMENT,"
                "name CHAR(125),"
                "class CHAR(125),"
                "hitpoints INT,"
                "damage INT,"
                "race CHAR(125))");

    mQuery.exec("CREATE TABLE questDatabase ("
                "id INT PRIMARY KEY AUTO_INCREMENT,"
                "name CHAR(125),"
                "description TEXT,"
                "reward INT,"
                "enemyRace CHAR(125))");
        
}

Database::~Database() {
    mQuery.exec("DROP TABLE enemiesDatabase");
    mQuery.exec("DROP TABLE questDatabase");
    db.close();
}

void Database::clearConsole() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void Database::newHero() {
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

    mQuery.prepare("INSERT INTO heroDatabase (name, class, level, hitpoints, damage)" 
                   "VALUES (:name, :class, :level, :hitpoints, :damage)");
    mQuery.bindValue(":name", QString::fromStdString(name));
    mQuery.bindValue(":class", QString::fromStdString(heroClass));
    mQuery.bindValue(":level", 1);
    mQuery.bindValue(":hitpoints", 10);
    mQuery.bindValue(":damage", 2);

    if (!mQuery.exec()) {
        std::cout << "Failed to add hero to heroDatabase: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    mQuery.prepare("INSERT INTO currentHero (name, class, level, hitpoints, damage)" 
                   "VALUES (:name, :class, :level, :hitpoints, :damage)");
    mQuery.bindValue(":name", QString::fromStdString(name));
    mQuery.bindValue(":class", QString::fromStdString(heroClass));
    mQuery.bindValue(":level", 1);
    mQuery.bindValue(":hitpoints", 10);
    mQuery.bindValue(":damage", 2);

    if (!mQuery.exec()) {
        std::cout << "Failed to add hero to currentHero: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    clearConsole();
    std::cout << "Welcome, " << name << "! Your journey begins now in the Realm of Ubuntu.\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

bool Database::loadHero() {
    mQuery.prepare("SELECT * FROM heroDatabase");

    if (!mQuery.exec()) {
        std::cerr << "Failed to retrieve hero data: " << mQuery.lastError().text().toStdString() << std::endl;
        return false;
    }

    if (!mQuery.next()) {
        std::cerr << "No heroes found in the database." << std::endl;
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

        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Class: " << heroClass.toStdString() << std::endl;
        std::cout << "Level: " << level << std::endl;
        std::cout << "Experience: " << experience << std::endl;
        std::cout << "Hitpoints: " << hitpoints << std::endl;
        std::cout << "Damage: " << damage << std::endl;
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
        clearConsole();
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

    mQuery.prepare("INSERT INTO currentHero (name, class, level, experience, hitpoints, damage) "
                        "VALUES (:name, :class, :level, :experience, :hitpoints, :damage)");
    mQuery.bindValue(":name", name);
    mQuery.bindValue(":class", heroClass);
    mQuery.bindValue(":level", level);
    mQuery.bindValue(":experience", experience);
    mQuery.bindValue(":hitpoints", hitpoints);
    mQuery.bindValue(":damage", damage);

    if (!mQuery.exec()) {
        std::cerr << "Failed to insert hero data: " << mQuery.lastError().text().toStdString() << std::endl;
        return false;
    }

    clearConsole();
    std::cout << "Hero loaded successfully." << std::endl;
    std::cout << "Welcome, " << name.toStdString() << "! Your journey begins now in the Realm of Ubuntu.\n";
    std::cout << "Press Enter to continue...";
    std::cin.get();
    return true;
}

void Database::deleteHero() {
    std::cout << "Hero Information" << std::endl;
    if (!mQuery.exec("SELECT * FROM heroDatabase")) {
        std::cerr << "Failed to retrieve hero data: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    if (!mQuery.next()) {
        std::cerr << "No hero found in the database." << std::endl;
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

        std::cout << "ID: " << id << std::endl;
        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Class: " << heroClass.toStdString() << std::endl;
        std::cout << "Level: " << level << std::endl;
        std::cout << "Experience: " << experience << std::endl;
        std::cout << "Hitpoints: " << hitpoints << std::endl;
        std::cout << "Damage: " << damage << std::endl;
        std::cout << std::endl;
        heroFound = true;
    } while (mQuery.next());

    if (!heroFound) {
        std::cerr << "No hero found in the database." << std::endl;
        return;
    }

    int heroId;

    std::cout << "Enter hero ID to delete: ";
    if (!(std::cin >> heroId)) {
        clearConsole();
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
        clearConsole();
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
    clearConsole();
    std::cout << "Hero deleted successfully from database" << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Database::loadEnemiesFromFile() {
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

    for (int i = 0; i < enemyData.size(); i += 5) {
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

        mQuery.prepare("INSERT INTO enemiesDatabase (name, class, hitpoints, damage, race) VALUES (:name, :class, :hitpoints, :damage, :race)");
        mQuery.bindValue(":name", QString::fromStdString(name));
        mQuery.bindValue(":class", QString::fromStdString(enemyclass));
        mQuery.bindValue(":hitpoints", hitpoints);
        mQuery.bindValue(":damage", damage);
        mQuery.bindValue(":race", QString::fromStdString(race));

        if (!mQuery.exec()) {
            std::cerr << "Failed to insert enemy into database: " << mQuery.lastError().text().toStdString() << std::endl;
        }
    }

    infile.close();
}

void Database::loadQuestsFromFile() {
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

    for (int i = 0; i < questData.size(); i += 4) {
        std::string name = questData[i];
        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t") + 1);      
        std::string description = questData[i + 1];
        description.erase(0, description.find_first_not_of(" \t"));
        description.erase(description.find_last_not_of(" \t") + 1);
        int reward = std::stoi(questData[i + 2]);
        std::string enemyRace = questData[i + 3];
        enemyRace.erase(0, enemyRace.find_first_not_of(" \t"));
        enemyRace.erase(enemyRace.find_last_not_of(" \t") + 1);

        mQuery.prepare("INSERT INTO questDatabase (name, description, reward, enemyRace) VALUES (:name, :description, :reward, :enemyRace)");
        mQuery.bindValue(":name", QString::fromStdString(name));
        mQuery.bindValue(":description", QString::fromStdString(description));
        mQuery.bindValue(":reward", reward);
        mQuery.bindValue(":enemyRace", QString::fromStdString(enemyRace));

        if (!mQuery.exec()) {
            std::cerr << "Failed to insert quest into database: " << mQuery.lastError().text().toStdString() << std::endl;
        }
    }

    infile.close();
}

void Database::printEnemies() {
    std::cout << "Enemy Information" << std::endl;
    if (!mQuery.exec("SELECT * FROM enemiesDatabase")) {
        std::cerr << "Failed to retrieve enemy data: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    while (mQuery.next()) {
        QString name = mQuery.value(1).toString();
        QString enemyclass = mQuery.value(2).toString();
        int hitpoints = mQuery.value(3).toInt();
        int damage = mQuery.value(4).toInt();
        QString race = mQuery.value(5).toString();

        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Class: " << enemyclass.toStdString() << std::endl;
        std::cout << "Hitpoints: " << hitpoints << std::endl;
        std::cout << "Damage: " << damage << std::endl;
        std::cout << "race: " << race.toStdString() << std::endl;
        std::cout << std::endl;
    }
}

void Database::printQuests() {
    std::cout << "Quest Information" << std::endl;
    if (!mQuery.exec("SELECT * FROM questDatabase")) {
        std::cerr << "Failed to retrieve quest data: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    while (mQuery.next()) {
        QString name = mQuery.value(1).toString();
        QString description = mQuery.value(2).toString();
        int reward = mQuery.value(3).toInt();
        QString enemyRace = mQuery.value(4).toString();

        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Description:";
        
        int lineLength = 60;
        int pos = 0;
        while (pos < description.length()) {
            int nextSpace = description.indexOf(' ', pos + lineLength);
            if (nextSpace != -1 && nextSpace < pos + 2 * lineLength) {
                std::cout << description.mid(pos, nextSpace - pos).toStdString() << std::endl;
                pos = nextSpace + 1;
            } else {
                int spacePos = description.lastIndexOf(' ', pos + lineLength);
                if (spacePos != -1 && spacePos > pos) {
                    std::cout << description.mid(pos, spacePos - pos).toStdString() << std::endl;
                    pos = spacePos + 1;
                } else {
                    std::cout << description.mid(pos).toStdString() << std::endl;
                    pos = description.length();
                }
            }
        }
        
        std::cout << "Reward: " << reward << std::endl;
        std::cout << "Enemy Race: " << enemyRace.toStdString() << std::endl;
        std::cout << std::endl;
    }
}

void Database::printHero() {
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

        std::cout << "ID: " << id << std::endl;
        std::cout << "Name: " << name.toStdString() << std::endl;
        std::cout << "Class: " << heroClass.toStdString() << std::endl;
        std::cout << "Level: " << level << std::endl;
        std::cout << "Experience: " << experience << std::endl;
        std::cout << "Hitpoints: " << hitpoints << std::endl;
        std::cout << "Damage: " << damage << std::endl;
        std::cout << std::endl;
    }
}