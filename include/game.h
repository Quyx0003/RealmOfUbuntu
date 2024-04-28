#ifndef GAME_H
#define GAME_H

#include "utils.h"

struct Character {
    QString name;
    QString className;
    int level;
    int experience;
    int hitpoints;
    int damage;
    int evadeChance;
};

class Game {
public:
    void explore();
    
private:
    QSqlQuery mQuery;

    bool battle();
    void attack(Character& attacker, Character& target, std::mt19937& g);
};

#endif // GAME_H


