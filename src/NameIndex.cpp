#include "NameIndex.h"

#include "SystemManager.h"

using namespace std;

NameIndex::NameIndex()
{
    m_CharacterCount = 0;
    m_LongestProbeSequence = 0;
    m_TableSize = 0;
}

void NameIndex::CreateTable(int size)
{
    // initialize the hash table with the given size
    hashTable.resize(size);
    m_TableSize = size;
    m_NumElements = 0;
}

int NameIndex::Hash(string nameState)
{
    // hash function implementation goes here
    // for simplicity, we will use the modulo operator
    // to reduce the key to a valid index in the hash table

    unsigned int hash = 0;
    unsigned int x = 0;
    unsigned int i = 0;
    unsigned int len = nameState.length();

    for (i = 0; i < len; i++)
    {
        hash = (hash << 4) + (nameState[i]);
        if ((x = hash & 0xF0000000) != 0)
        {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }

    return hash % hashTable.size();
}

void NameIndex::Insert(HashItem& key) 
{
    // check if the hash table is 70% capacity
    if (m_NumElements >= hashTable.size() * 0.7 )
    {
        Rehash();
        //cout << "Resized to " << hashTable.size() << endl;
    }

    // find the index for the key
    int index = Hash(key.nameState);

    // check if the index is already occupied
    // if it is, use quadratic probing to find the next empty index
    int i = 0;
    while (hashTable[index].isActive)
    {
        i++;
        index = (index + i * i) % hashTable.size();
    }

    // insert the key into the hash table at the empty index
    hashTable[index] = key;
    hashTable[index].isActive = true;

    // Stats
    m_NumElements++;
    m_CharacterCount += key.nameState.length();
    if (i > m_LongestProbeSequence)
        m_LongestProbeSequence = i;
}

void NameIndex::Remove(HashItem& key)
{
    // find the index for the key
    int index = Hash(key.nameState);

    // check if the key is at the index
    // if it's not, use quadratic probing to find the key
    int i = 0;
    while (hashTable[index].isActive)
    {
        i++;
        index = (index + i * i) % hashTable.size();
    }

    hashTable[index].isActive = false;
    m_NumElements--;
}

int NameIndex::Search(string nameState)
{
    // find the index for the key
    int index = Hash(nameState);

    // check if the key is at the index
    // if it's not, use quadratic probing to find the key
    int i = 0;
    while (hashTable[index].nameState != nameState)
    {
        i++;
        index = (index + i * i) % hashTable.size();

        // return false if the key is not found
        if (!hashTable[index].isActive || i > hashTable.size()) return false;
    }

    // return true if the key is found
    return hashTable[index].offset;
}

void NameIndex::Rehash()
{
    vector<HashItem> oldTable = hashTable;

    // Create new double-sized, empty table
    hashTable.resize(hashTable.size() * 2);
    m_TableSize = hashTable.size();

    for (auto& entry : hashTable)
        entry.isActive = false;

    // Copy table over
    m_NumElements = 0;
    for (auto& entry : oldTable)
        if (entry.isActive)
            Insert(entry);
}

void NameIndex::Print()
{
    SystemManager& systemmanager = SystemManager::GetInstance();

    ostringstream pr;

    pr << endl;
    pr << "Contents of hash table:" << endl;
    for (auto& content : hashTable)
    {
        if (content.isActive)
            pr << Hash(content.nameState) << ": [" << content.nameState << ", " << content.offset << "]" << endl;
    }

    systemmanager.m_Logger->LogString("Current table size is " + to_string(m_TableSize));
    systemmanager.m_Logger->LogString("Number of elements in table is " + to_string(m_NumElements));
    
    systemmanager.m_Logger->LogString(pr.str());
}