#ifndef HERO_H
#define HERO_H

#include <QtSql>
#include <QSqlDatabase>
#include <string>
#include <iostream>

class Hero {
public:
    Hero();
    ~Hero();

    void clearConsole();

    void levelSystem();
    void levelUp(int level);
    void printHero();

private:
    QSqlQuery mQuery, selectQuery, insertQuery, updateQuery;
};

#endif // HERO_H
