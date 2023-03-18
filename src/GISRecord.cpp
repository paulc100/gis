#include "GISRecord.h"

#include "DataStructures.h"
#include "SystemManager.h"

using namespace std;

GISRecord::GISRecord()
{
}

void GISRecord::Import(string recordFilePath)
{
    SystemManager& systemmanager = SystemManager::GetInstance();

    recordFile.open(recordFilePath, ios::in);

    int importedCount = 0;
    int fileLine = 0;
    string lineString;
    string fullRecordString;

    while (getline(recordFile, lineString))
    {
        fileLine++;

        if (fileLine >= 2)
        {
            fullRecordString = lineString;
            vector<string> items;
            size_t pos = 0;
            string delimiter = "|";

            // Separate arguments
            while ((pos = lineString.find(delimiter)) != string::npos) {
                items.push_back(lineString.substr(0, pos));
                lineString.erase(0, pos + delimiter.length());
            }

            // Not valid if no coordinates
            if (items[7] == "Unknown" || items[8] == "Unknown")
                continue;

            float latitude = systemmanager.m_World->FromDMS(items[7]);
            float longitude = systemmanager.m_World->FromDMS(items[8]);

            // Validate
            if (systemmanager.m_World->withinWorldBounds(latitude, longitude))
            {
                // Write to DB and get offset
                int offset = 0;
                while (offset == 0)
                {
                    offset = systemmanager.m_BufferPool->Write(fullRecordString);
                }

                HashItem item = GISToHash(items, offset);
                systemmanager.m_NameIndex->Insert(item);

                Point location;
                location = {
                    latitude,
                    longitude,
                    {offset}
                };

                systemmanager.m_CoordinateIndex->Insert(location);
                importedCount++;
            }
        }
    }

    recordFile.close();

    if (importedCount <= 0)
    {
        //cout << "Nothing imported - likely out of bounds" << endl;
        systemmanager.m_Logger->LogString("Nothing imported - likely out of bounds");
        return;
    }

    string FeaturesCount = "Imported Features by name : " + to_string(importedCount);
    string longestProbe = "Longest probe sequence : " + to_string(systemmanager.m_NameIndex->m_LongestProbeSequence);
    string LocationsCount = "Imported Locations : " + to_string(importedCount);
    string averageNameLength = "Average name length : " + to_string(systemmanager.m_NameIndex->m_CharacterCount/systemmanager.m_NameIndex->m_NumElements);
    
    systemmanager.m_Logger->LogString(FeaturesCount);
    systemmanager.m_Logger->LogString(longestProbe);
    systemmanager.m_Logger->LogString(LocationsCount);
    systemmanager.m_Logger->LogString(averageNameLength);
}

HashItem GISRecord::GISToHash(vector<string> items, int charPos)
{
    HashItem item;
    item.nameState = items[1] + ":" + items[3];
    item.offset = charPos;
    item.isActive = false;

    return item;
}