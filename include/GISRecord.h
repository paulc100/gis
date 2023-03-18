#ifndef GISRECORD_H
#define GISRECORD_H

#include "DataStructures.h"

#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>

using namespace std;

class GISRecord {

    static unordered_map<string, string> classToTypeMap;

    public:
        string recordPath;
        fstream recordFile;

        GISRecord();
        HashItem GISToHash(vector<string> items, int charPos);

        void Import(string fileStr);
    private:
        ;
};

#endif