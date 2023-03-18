#ifndef NAMEINDEX_H
#define NAMEINDEX_H

using namespace std;

#include "DataStructures.h"

#include <string>
#include <iostream>
#include <vector>

class NameIndex {

    public:
        int m_LongestProbeSequence;
        int m_CharacterCount;

        // current number of elements in the hash table
        int m_NumElements;
        int m_CurrentImportNum;
        int m_TableSize;

        NameIndex();
        void CreateTable(int size);
        int Hash(string nameState);
        void Insert(HashItem& key);
        void Remove(HashItem& key);
        int Search(string nameState);

        void Rehash();

        void Print();
    private:
        // hash table
        std::vector<HashItem> hashTable;
};

#endif