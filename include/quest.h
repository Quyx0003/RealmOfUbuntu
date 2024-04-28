#ifndef QUEST_H
#define QUEST_H

#include "utils.h"

class Quest {
public:
    Quest();
    ~Quest();

    void loadQuestsFromFile();
    void loadQuest();
    void printQuests();

private:
    QSqlQuery mQuery;
};

#endif // QUEST_H


