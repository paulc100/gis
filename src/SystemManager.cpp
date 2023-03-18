#include "SystemManager.h"

using namespace std;

SystemManager::SystemManager()
{
    Logger* logger = new Logger;
    m_Logger = logger;

    BufferPool* bufferpool = new BufferPool;
    m_BufferPool = bufferpool;

    CommandProcessor* commandprocessor = new CommandProcessor;
    m_CommandProcessor = commandprocessor;

    GISRecord* gisrecord = new GISRecord;
    m_GISRecord = gisrecord;

    World* world = new World;
    m_World = world;

    NameIndex* nameindex = new NameIndex;
    m_NameIndex = nameindex;

    CoordinateIndex* coordinateindex = new CoordinateIndex;
    m_CoordinateIndex = coordinateindex;
};

SystemManager& SystemManager::GetInstance()
{
    static SystemManager instance;
    return instance;
}

void SystemManager::Start(string database, string script, string log)
{
    databaseFileName = database;
    scriptFilePath = script;
    logFileName = log;

    m_Logger->Initialize(databaseFileName, scriptFilePath, logFileName);

    m_NameIndex->CreateTable(1024);

    m_BufferPool->Initialize(databaseFileName);

    m_CommandProcessor->Initialize(scriptFilePath);

    m_Logger->Finalize();

    //cout << "100% complete" << endl;
}