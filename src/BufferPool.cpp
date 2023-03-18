#include "BufferPool.h"

#include "SystemManager.h"

using namespace std;

BufferPool::BufferPool()
{
};

void BufferPool::Initialize(string databaseStr)
{
    databasePath = databaseStr;

    // Delete previous contents
    databaseFile.open(databasePath, ios::out | ios::trunc);
    databaseFile.close();
}

string BufferPool::CheckBufferPool(int offset)
{
    // Already exists
    if (bufferPool.find(offset) != bufferPool.end())
    {
        bufferQueue.remove(offset);
        bufferQueue.push_front(offset);

        return bufferPool[offset];
    }

    // Check if the queue can hold more
    // If the queue is full then need to perform lru
    if (bufferPool.size() >= poolCapacity)
    {
        bufferPool.erase(bufferQueue.back());
        bufferQueue.pop_back();
    }

    // Add record to pool
    string record = Read(offset);
    bufferQueue.push_front(offset);
    bufferPool[offset] = record;

    return record;
}

int BufferPool::Write(string item)
{
    fstream is = fstream(databasePath);

    int offset = 0;

    if (is)
    {
        is.seekg(0, ios::end);
        offset = is.tellg();
        is.close();

        databaseFile.open(databasePath, ios::out | ios::app | ios::binary);

        databaseFile << item;
        databaseFile << endl;

        databaseFile.close();
    }

    return offset;
}

string BufferPool::Read(int offset)
{
    ifstream is = ifstream(databasePath);

    string record;
    if (is)
    {
        is.clear();
        is.seekg(offset);
        getline(is, record);

        is.close();
    }

    return record;
}

void BufferPool::Print()
{
    SystemManager& systemmanager = SystemManager::GetInstance();
    ostringstream os;

    os << "MRU" << endl;
    for (auto i : bufferQueue)
    {
        os << bufferPool[i] << endl;
    }
    os << "LRU";

    systemmanager.m_Logger->LogString(os.str());
}