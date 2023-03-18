#include "Logger.h"

using namespace std;

Logger::Logger()
{
    
};

void Logger::Initialize(string databaseFileName, string scriptFilePath, string logFileName)
{
    logPath = logFileName;

    logFile.open(logPath, ios::out);

    logFile << "Course Project for COMP 8042" << endl;
    logFile << "Student Name: Paul Cavallo, Student Id: A01061713" << endl;
    logFile << "Begin of GIS Program log: " << endl;

    logFile << "dbFile: " << databaseFileName << endl;
    logFile << "logFile: " << logFileName << endl;
    logFile << "scriptFile: " << scriptFilePath << endl;

    time_t start_time = chrono::system_clock::to_time_t(chrono::system_clock::now());

    logFile << "Start time: " << ctime(&start_time) << endl;
}

void Logger::LogCommand(string commandStr, int commandNumber)
{
    if (commandNumber > 0)
        logFile << "Command " << commandNumber << ": " << commandStr << endl << endl;;
}

void Logger::LogString(string str)
{
    if(str.length() >= 1)
        logFile << str << endl;
}

void Logger::Finalize()
{
    time_t end_time = chrono::system_clock::to_time_t(chrono::system_clock::now());

    logFile << "End time: " << ctime(&end_time) << endl;
    logFile.close();
}