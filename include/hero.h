#ifndef HERO_H
#define HERO_H

#include "utils.h"

class Hero {
public:
    Hero();
    ~Hero();

    bool newHero();
    bool loadHero();
    void deleteHero();

    void heroInfo();
    void levelSystem();

private:
    QSqlQuery mQuery, selectQuery, insertQuery, updateQuery;
};

#endif // HERO_H


