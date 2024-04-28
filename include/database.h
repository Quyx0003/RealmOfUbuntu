#ifndef DATABASE_H
#define DATABASE_H

#include "utils.h"

class Database {
public:
    Database();
    ~Database(); 

private: 
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QSqlQuery mQuery;
};

#endif // DATABASE_H


