#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <list>

using namespace std;

class World {

    public:

        float m_NorthWorldBoundary;
        float m_SouthWorldBoundary;
        float m_WestWorldBoundary;
        float m_EastWorldBoundary;

        World();

        void SetWorldBoundaries(vector<string> arguments);
        float FromDMS(string boundary);

        bool withinWorldBounds(float latitude, float longitude);
    private:
        ;
};

#endif