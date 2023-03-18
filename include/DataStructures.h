#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <list>

using namespace std;

struct HashItem
{
    string nameState;
    int offset;
    bool isActive;
};

struct Point {
    float lat;
    float lon;
    vector<int> offsets;
};

#endif