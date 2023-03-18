#include "CommandProcessor.h"
#include "SystemManager.h"
#include "DataStructures.h"

using namespace std;

CommandProcessor::CommandProcessor()
{
    commandNumber = -1;
};

vector<string> parseRecord(string lineString)
{
    vector<string> items;
    size_t pos = 0;
    string delimiter = "|";

    // Separate arguments
    while ((pos = lineString.find(delimiter)) != string::npos) {
        items.push_back(lineString.substr(0, pos));
        lineString.erase(0, pos + delimiter.length());
    }

    return items;
}

string formatRecordContent(vector<string> content)
{
    std::stringstream ss;

    ss << "Feature ID   : " << content[0] << endl;

    if (!content[1].empty())
        ss << "Feature Name : " << content[1] << endl;
    if (!content[2].empty())
        ss << "Feature Cat  : " << content[2] << endl;
    if (!content[3].empty())
        ss << "State        : " << content[3] << endl;
    if (!content[5].empty())
        ss << "County       : " << content[5] << endl;
    if (!content[7].empty())
        ss << "Longitude    : " << content[7] << endl;
    if (!content[8].empty())
        ss << "Latitude     : " << content[8] << endl;
    if (!content[16].empty())
        ss << "Elev in ft   : " << content[16] << endl;
    if (!content[17].empty())
        ss << "USGS Quad    : " << content[17] << endl;
    if (!content[18].empty())
        ss << "Date created : " << content[18] << endl;

    return ss.str();
}

void CommandProcessor::Initialize(string scriptStr)
{
    SystemManager& systemmanager = SystemManager::GetInstance();

    scriptPath = scriptStr;

    scriptFile.open(scriptPath, ios::in);

    string lineString;
    vector<string> command;

    while (getline(scriptFile, lineString))
    {
        command.clear();

        // Ignore comments
        if (lineString[0] == ";"[0])
        {
            systemmanager.m_Logger->LogString(lineString);
            continue;
        }
        else
        {
            commandNumber++;

            systemmanager.m_Logger->LogCommand(lineString, commandNumber);

            size_t pos = 0;
            string delimiter = "\t";

            // Separate arguments
            while ((pos = lineString.find(delimiter)) != string::npos) {
                command.push_back(lineString.substr(0, pos));
                lineString.erase(0, pos + delimiter.length());
            }
            command.push_back(lineString);
            
            // Handle commands
            if (command.front() == "world")
            {
                command.erase(command.begin());
                // Remaining command elements are arguments
                
                systemmanager.m_World->SetWorldBoundaries(command);
                continue;
            }
            else if (command.front() == "import")
            {
                systemmanager.m_GISRecord->Import(command[1]);
            }
            else if (command.front() == "debug")
            {
                if (command[1] == "quad")
                    systemmanager.m_CoordinateIndex->Print();
                else if (command[1] == "hash")
                    systemmanager.m_NameIndex->Print();
                else if (command[1] == "pool")
                    systemmanager.m_BufferPool->Print();
                else if (command[1] == "world")
                    ;
            }
            else if (command.front() == "quit")
            {
                break;
            }
            else if (command.front() == "what_is_at")
            {
                command.erase(command.begin());
                // Remaining command elements are arguments

                float lat = systemmanager.m_World->FromDMS(command[0]);
                float lon = systemmanager.m_World->FromDMS(command[1]);

                // Check bounds
                if (!systemmanager.m_World->withinWorldBounds(lat, lon))
                {
                    systemmanager.m_Logger->LogString("Out of bounds");
                    systemmanager.m_Logger->LogString("\n------------------------------------------------------------------------------------------");
                    continue;
                }

                vector<int> recordsList = systemmanager.m_CoordinateIndex->search(lat, lon);
                
                if (!recordsList.empty())
                {
                    for (auto& i : recordsList)
                    {
                        string record = systemmanager.m_BufferPool->CheckBufferPool(i);
                        systemmanager.m_Logger->LogString(record);
                    }
                }
                else
                    systemmanager.m_Logger->LogString("Nothing Found");
            }
            else if (command.front() == "what_is")
            {
                int offset = systemmanager.m_NameIndex->Search(command[1] + ":" + command[2]);
                if (offset != 0)
                {
                    string record = systemmanager.m_BufferPool->CheckBufferPool(offset);
                    systemmanager.m_Logger->LogString(record);
                }
                else
                {
                    systemmanager.m_Logger->LogString("Nothing Found");
                }
            }
            else if (command.front() == "what_is_in")
            {
                command.erase(command.begin());
                // Remaining command elements are arguments

                float lat;
                float lon;
                float height;
                float width;

                int count = 0;

                // Argument options
                if (command[0].find("-long") != string::npos)
                    // every important non-empty field, nicely formatted and labeled
                {
                    lat = systemmanager.m_World->FromDMS(command[1]);
                    lon = systemmanager.m_World->FromDMS(command[2]);

                    // Check bounds
                    if (!systemmanager.m_World->withinWorldBounds(lat, lon))
                    {
                        systemmanager.m_Logger->LogString("Out of bounds");
                        systemmanager.m_Logger->LogString("\n------------------------------------------------------------------------------------------");
                        continue;
                    }

                    height = stof(command[3]) / 3600;
                    width = stof(command[4]) / 3600;

                    vector<int> recordsList = systemmanager.m_CoordinateIndex->findArea(lat, lon, width, height);

                    if (!recordsList.empty())
                    {
                        for (auto& i : recordsList)
                        {
                            string record = systemmanager.m_BufferPool->CheckBufferPool(i);
                            
                            vector<string> recordContent = parseRecord(record);
                            string formatedRecord = formatRecordContent(recordContent);
                            systemmanager.m_Logger->LogString(formatedRecord);
                            count++;
                        }
                        string s = "\nNumber of items found: ";
                        systemmanager.m_Logger->LogString(s);
                        systemmanager.m_Logger->LogString(to_string(count));
                    }
                    else
                        systemmanager.m_Logger->LogString("Nothing Found");
                }
                else if (command[0].find("-filter") != string::npos)
                {
                    lat = systemmanager.m_World->FromDMS(command[2]);
                    lon = systemmanager.m_World->FromDMS(command[3]);

                    // Check bounds
                    if (!systemmanager.m_World->withinWorldBounds(lat, lon))
                    {
                        systemmanager.m_Logger->LogString("Out of bounds");
                        systemmanager.m_Logger->LogString("\n------------------------------------------------------------------------------------------");
                        continue;
                    }

                    height = stof(command[4]) / 3600;
                    width = stof(command[5]) / 3600;

                    vector<int> recordsList = systemmanager.m_CoordinateIndex->findArea(lat, lon, width, height);

                    if (!recordsList.empty())
                    {
                        for (auto& i : recordsList)
                        {
                            string record = systemmanager.m_BufferPool->CheckBufferPool(i);
                            
                            vector<string> recordContent = parseRecord(record);
                            string recordType = recordClassToTypeMap[recordContent[2]];
                            if (recordType == command[1])
                            {
                                systemmanager.m_Logger->LogString(record);
                                count++;
                            }
                        }
                        string s = "\nNumber of items found: ";
                        systemmanager.m_Logger->LogString(s);
                        systemmanager.m_Logger->LogString(to_string(count));
                    }
                    else
                        systemmanager.m_Logger->LogString("Nothing Found");
                }
                else
                {
                    lat = systemmanager.m_World->FromDMS(command[0]);
                    lon = systemmanager.m_World->FromDMS(command[1]);

                    // Check bounds
                    if (!systemmanager.m_World->withinWorldBounds(lat, lon))
                    {
                        systemmanager.m_Logger->LogString("Out of bounds");
                        systemmanager.m_Logger->LogString("\n------------------------------------------------------------------------------------------");
                        continue;
                    }

                    height = stof(command[2]) / 3600;
                    width = stof(command[3]) / 3600;

                    vector<int> recordsList = systemmanager.m_CoordinateIndex->findArea(lat, lon, width, height);

                    if (!recordsList.empty())
                    {
                        for (auto& i : recordsList)
                        {
                            string record = systemmanager.m_BufferPool->CheckBufferPool(i);

                            systemmanager.m_Logger->LogString(record);

                            count++;
                        }
                        string s = "\nNumber of items found: ";
                        systemmanager.m_Logger->LogString(s);
                        systemmanager.m_Logger->LogString(to_string(count));
                    }
                    else
                        systemmanager.m_Logger->LogString("Nothing Found");
                }
            }
        }
        systemmanager.m_Logger->LogString("\n------------------------------------------------------------------------------------------");
    }

    scriptFile.close();
};