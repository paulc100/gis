#ifndef BUFFERPOOL_H
#define BUFFERPOOL_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <map>
#include <unordered_set>

using namespace std;

class BufferPool {

    const int poolCapacity = 15;

    public:
        string databasePath;
        fstream databaseFile;

        list<int> bufferQueue;
        map<int, string> bufferPool;

        BufferPool();

        void Initialize(string databaseStr);

        string CheckBufferPool(int offset);

        int Write(string item);
        string Read(int offset);

        void Print();
    private:
        ;
};

#endif