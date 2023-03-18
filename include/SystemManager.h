#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "Logger.h"
#include "BufferPool.h"
#include "CommandProcessor.h"
#include "GISRecord.h"
#include <World.h>
#include <NameIndex.h>
#include <CoordinateIndex.h>

#include <DataStructures.h>

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class SystemManager {

    public:

        Logger* m_Logger;
        BufferPool* m_BufferPool;
        CommandProcessor* m_CommandProcessor;
        GISRecord* m_GISRecord;
        World* m_World;
        NameIndex* m_NameIndex;
        CoordinateIndex* m_CoordinateIndex;

        string databaseFileName;
        string scriptFilePath;
        string logFileName;

        SystemManager();
        static SystemManager& GetInstance();
        SystemManager(const SystemManager&) = delete;

        void Start(string database, string script, string log);

    private:
        static SystemManager s_SystemManager;
};

#endif