#include "../include/database.h"

Database::Database() {
    Utils::clearConsole();

    std::string username;
    std::string password;
    std::string dbname;

    std::cout << "Please log in to the Realm of Ubuntu" << std::endl;
    
    bool loggedIn = false;
    while (!loggedIn) {
        std::cout << "Database name: ";
        std::cin >> dbname;
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;

        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName(QString::fromStdString(dbname));
        db.setUserName(QString::fromStdString(username));
        db.setPassword(QString::fromStdString(password));

        if (db.open()) {
            loggedIn = true;
            Utils::clearConsole(); 
            std::cout << "Login successful" << std::endl;
        } else {
            Utils::clearConsole();
            std::cout << "Wrong username/password. Please try again or type 'quit' to exit." << std::endl;
        }
    }
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();        
}

Database::~Database() {
    db.close();
}


