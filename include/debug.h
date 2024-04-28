#ifndef DEBUG_H
#define DEBUG_H

#include "utils.h"

class Debug {
public:
    void printHeroDatabase();
    void printEnemyDatabase();
    void printQuestsDatabase();

    void GiveExperience(int experience);

private: 
    QSqlQuery mQuery;
};

#endif // DEBUG_H


