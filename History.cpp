#include "History.h"

using namespace std;

deque<string> HistoryAdapter::history;
string HistoryAdapter::myfilename = "";

void HistoryAdapter::setFile(string filename) {
    myfilename = filename;
    history.clear();	//clears history when the file is changed
    string mystr;
    ifstream hfile;
    hfile.open(filename.c_str(), ios::in);
    while (getline(hfile, mystr)) {
        std::size_t myRpos;
        myRpos = mystr.find("\r");
        if (myRpos != string::npos) {
            mystr = mystr.substr(0, myRpos); //offers windows compatibility, removing the \r at the end of the line
        }
        history.push_back(mystr);
    }
    hfile.close();
}

void HistoryAdapter::addOperation(string mystr) {
    history.push_front(mystr);
    syncHistoryFile();
}

void HistoryAdapter::printRecentHistory(int items) {
    int i=0;

    cout << "========== System History ==========" << endl;
    if (items <= history.size()) {
    	for (;i<items;i++) {
        	cout << history[i] << endl;
    	}
        cout << "====================================" << endl << endl;
    	return;
    }
    else {
        for (;i<history.size();i++) {
                cout << history[i] << endl;
    	}
        cout << "====================================" << endl << endl;
    	return;
    }
}

void HistoryAdapter::syncHistoryFile() {
    ofstream hfile;
    hfile.open(myfilename.c_str(), ios::out | ios::trunc);
    int i;
    for (i=0;i<history.size();i++) {
        hfile << history[i] << endl;
    }
    hfile.close();
}
