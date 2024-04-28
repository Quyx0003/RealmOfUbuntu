#ifndef DUNGEONS_H
#define DUNGEONS_H

#include "utils.h"

class Dungeon {
public:
    Dungeon();
    ~Dungeon();

    void loadDungeonFromFile();
    void loadDungeonEnemyFromFile();
    void loadDungeonBossFromFile();

    void loadDungeon();
    void printDungeon();

private:
    QSqlQuery mQuery;
};

#endif // DUNGEONS_H


