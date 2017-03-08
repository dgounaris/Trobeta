#include "Users.h"
#include "UserLibraries.h"
#include "History.h"
#include "GeneralFunctions.h"

using namespace std;


int main(int argc, char* argv[]){
    User* User1 = NULL;
    string op;
    bool login = false;
    UserFile myUsers("users.csv");
    BetFile myBets;
    Hierarchy myHierarchy("hierarchy.dat");
    HistoryAdapter::setFile("audit.log");


    if (argc == 2 && strcmp(argv[1], "-R") == 0){	// Registration Mode
       cout << "============================= REGISTRATION ===========================" << endl;
       login = Registration(myUsers, User1);
    }
    else if(argc == 1){                                 // Login Mode
       while(!login){
          cout << "=============================== LOGIN ================================" << endl;
          login = Login(myUsers, User1);
       }
    }
    else {						// Invalid Command Line Arguments
       cout << "================================ ERROR ===============================" << endl;
       cout << "--- Invalid Command Line Arguments ---" << endl << endl;
    }
    myUsers.close_UserFile();


    if (login){                                         // Check Login
       // If you are here, the login was successful
       while(op != "X"){
          cout << "======================================================================" << endl;
          NodeNavigationScreen(myHierarchy, User1);
          cout << endl << "Your Choice: ";
          getline(cin, op);
          cout << endl;
          User1->operation(op, myHierarchy, myUsers, myBets);   // Execute the given operation
       }
    }


    // if you are here, operation X(Exit) has been given
    cout << "Exiting the system..." << endl;
    delete User1;
    cout << "======================================================================" << endl;
    return 0;
}
