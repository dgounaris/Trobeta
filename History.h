#include "HistoryLibraries.h"

using namespace std;

#ifndef HISTORY_H
#define HISTORY_H

class HistoryAdapter {
    public:
        static void setFile(string);			//sets file and reads the data from that file, clearing any previous data
        static void addOperation(string);  		//adds the string describing the operation to the stack
        static void printRecentHistory(int);		//prints the most recent operations stored in history, integer specifies how many operations to be printed (default 25)
    private:
        static void syncHistoryFile(); 			//synchronizes the history file with the application
        static deque<string> history;  			//enabling compatibility for every possible action regarding history operations
        static string myfilename;
};

#endif