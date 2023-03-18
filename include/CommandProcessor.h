#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <unordered_map>

using namespace std;

class CommandProcessor {

    public:
        int commandNumber;
        string scriptPath;
        fstream scriptFile;

        CommandProcessor();

        void Initialize(string scriptStr);
    private:
        unordered_map<string, string> recordClassToTypeMap = {
            {"Airport", "structure"},
            {"Arroyo", "water"},
            {"Bay", "water"},
            {"Bend", "water"},
            {"Bridge", "structure"},
            {"Building", "structure"},
            {"Canal", "water"},
            {"Channel", "water"},
            {"Church", "structure"},
            {"Dam", "structure"},
            {"Falls", "water"},
            {"Glacier", "water"},
            {"Gut", "water"},
            {"Harbor", "water"},
            {"Hospital", "structure"},
            {"Lake", "water"},
            {"Levee", "structure"},
            {"Park", "structure"},
            {"Populated Place", "pop"},
            {"Post Office", "structure"},
            {"Rapids", "water"},
            {"Resevoir", "water"},
            {"School", "structure"},
            {"Sea", "water"},
            {"Spring", "water"},
            {"Stream", "water"},
            {"Swamp", "water"},
            {"Tower", "structure"},
            {"Tunnel", "structure"},
            {"Well", "water"},
        };
};

#endif