#ifndef UTILS_H
#define UTILS_H

#include <QtSql>
#include <QSqlDatabase>
#include <iostream>
#include <QString>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>

class Utils {
public:
    static void clearConsole();
    static void printMainMenu();
    static void printGameMenu();
    static void printWrapped(const std::string &text, int lineLength);
};

#endif // UTILS_H


