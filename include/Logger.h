#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <fstream>
#include <list>

using namespace std;

class Logger {

    public:
        string logPath;
        fstream logFile;

        Logger();;

        void Initialize(string databaseFileName, string scriptFilePath, string logFileName);

        void LogCommand(string commandStr, int commandNumber);
        void LogString(string str);

        void Finalize();
    private:
        ;
};

#endif