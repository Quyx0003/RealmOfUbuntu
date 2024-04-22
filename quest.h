#ifndef QUEST_H
#define QUEST_H

#include <QtSql>
#include <QSqlDatabase>
#include <QString>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>
#include <random>

struct Character {
    QString name;
    QString className;
    int level;
    int experience;
    int hitpoints;
    int damage;
    int evadeChance;
    };

class Quest {
public:
    Quest();
    ~Quest();

    void clearConsole();

    void loadQuest();
    void printQuests();
    void printWrapped(const std::string &text, int lineLength);

    void chooseQuest();
    bool battle();


private:
    QSqlQuery mQuery;

    void attack(Character& attacker, Character& target, std::mt19937& g);
};

#endif // QUEST_H
