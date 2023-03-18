#include "World.h"

#include "SystemManager.h"
#include <cstdlib>

using namespace std;

World::World()
{
}

void World::SetWorldBoundaries(vector<string> arguments)
{
    SystemManager& systemmanager = SystemManager::GetInstance();

    m_NorthWorldBoundary = FromDMS(arguments[3]);
    m_SouthWorldBoundary = FromDMS(arguments[2]);
    m_WestWorldBoundary = FromDMS(arguments[0]);
    m_EastWorldBoundary = FromDMS(arguments[1]);

    string log = "World boundaries are set to: " + to_string(m_NorthWorldBoundary) + "      " + to_string(m_WestWorldBoundary) + "  " + to_string(m_EastWorldBoundary) + "      " + to_string(m_SouthWorldBoundary);
    systemmanager.m_Logger->LogString(log);

    // Generate Quadtree boundaries
    float width = abs(m_WestWorldBoundary) + abs(m_EastWorldBoundary);
    float height = abs(m_NorthWorldBoundary) + abs(m_SouthWorldBoundary);
    systemmanager.m_CoordinateIndex->CreatePRQuadtree();
}

// https://github.com/jachappell/decimal-degrees-and-degrees-minutes-and-seconds/blob/master/dms.h

float World::FromDMS(string boundary)
{
    int length = boundary.length();

    string str = boundary;

    float ans = 0.0;

    // Longitude
    if (length == 8)
    {
        float degrees = stof(str.substr(0, 3));
        float min = stof(str.substr(3, 2));
        float sec = stof(str.substr(5, 2));

        ans = degrees + ((min + (sec / 60.0)) / 60.0);
        
        if (str[7] == 'W')
            ans = ans * -1;
    }
    // Latitude
    else
    {
        float degrees = stof(str.substr(0, 2));
        float min = stof(str.substr(2, 2));
        float sec = stof(str.substr(4, 2));

        ans = degrees + ((min + (sec / 60.0)) / 60.0);

        if (str[6] == 'S')
            ans = ans * -1;
    }

    return ans;
}

bool World::withinWorldBounds(float latitude, float longitude)
{

    bool withinLongBounds = false;
    bool withinLatBounds = false;

    if (m_EastWorldBoundary - m_WestWorldBoundary > 180) {
        // Longitude boundaries
        if (longitude >= m_EastWorldBoundary && longitude <= m_WestWorldBoundary) {
            withinLongBounds = true;
        }
    }
    else {
        // Longitude boundaries
        if (longitude <= m_EastWorldBoundary && longitude >= m_WestWorldBoundary) {
            withinLongBounds = true;
        }
    }
    // Latitude boundaries
    if (latitude <= m_NorthWorldBoundary && latitude >= m_SouthWorldBoundary) {
        withinLatBounds = true;
    }
    return (withinLatBounds && withinLongBounds);
}