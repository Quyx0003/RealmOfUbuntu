#ifndef ENEMIES_H
#define ENEMIES_H

#include "utils.h"

class Enemy {
public:
    Enemy();
    ~Enemy();

    void loadEnemyFromFile();

private:
    QSqlQuery mQuery;
};

#endif // ENEMIES_H


