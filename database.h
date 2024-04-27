#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlDatabase>
#include <iostream>
#include <QString>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Database {
public:
    Database();
    ~Database(); 

    void clearConsole();

    bool newHero();
    bool loadHero();
    void deleteHero();

    void loadEnemiesFromFile();
    void loadQuestsFromFile();

    void printEnemies();
    void printQuests();
    void printHero();

private: 
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QSqlQuery mQuery;
};

#endif // DATABASE_H
