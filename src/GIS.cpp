#include "SystemManager.h"

#include <string>

using namespace std;

int main(int argc, char * argv[])
{
    // Exe Arguments
    string databaseFileName = argv[1];
    string scriptFilePath = argv[2];
    string logFileName = argv[3];

    SystemManager& systemmanager = SystemManager::GetInstance();

    // Begin Application
    systemmanager.Start(databaseFileName, scriptFilePath, logFileName);
};