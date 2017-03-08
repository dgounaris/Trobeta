#include "GeneralFunctions.h"
#include "History.h"

using namespace std;

string bets_get_bet_id(string& line){
   return line.substr(0, line.find("|"));
}

string bets_get_user_id(string& line){
   int pos1, pos2;

   pos1 = find_vertical_bar(1, line);
   pos2 = line.find("|", pos1+1);
   return line.substr(pos1+1, pos2-pos1-1);
}

string bets_get_node_id(string& line){
   int pos1, pos2;

   pos1 = find_vertical_bar(2, line);
   pos2 = line.find("|", pos1+1);
   return line.substr(pos1+1, pos2-pos1-1);
}

double bets_get_stake(string& line){
   int pos1, pos2;

   pos1 = find_vertical_bar(3, line);
   pos2 = line.find("|", pos1+1);
   return strtod(line.substr(pos1+1, pos2-pos1-1).c_str(), NULL);
}

string bets_get_stake_str(string& line){
   int pos1, pos2;

   pos1 = find_vertical_bar(3, line);
   pos2 = line.find("|", pos1+1);
   return line.substr(pos1+1, pos2-pos1-1);
}

string bets_get_result(string& line){
   int pos1, pos2;

   pos1 = find_vertical_bar(4, line);
   pos2 = line.length();
   return line.substr(pos1+1, pos2-pos1-1);
}

string double_to_string(double val){
   ostringstream strs;
   strs << val;
   return strs.str();
}

int find_vertical_bar(int num, string& line){
   int i, pos = -1;

   for (i = 0; i < num; i++){
      pos = line.find("|", pos+1);
   }
   return pos;
}

string get_user_id(string& line){
   return line.substr(0, line.find("|"));
}

string get_username(string& line){
   int pos1, pos2;

   pos1 = find_vertical_bar(1, line);
   pos2 = line.find("|", pos1+1);
   return line.substr(pos1+1, pos2-pos1-1);
}

string get_fullname(string& line){
   int pos1, pos2;

   pos1 = find_vertical_bar(2, line);
   pos2 = line.find("|", pos1+1);
   return line.substr(pos1+1, pos2-pos1-1);
}

string get_password(string& line){
   int pos1, pos2;

   pos1 = find_vertical_bar(3, line);
   pos2 = line.find("|", pos1+1);
   return line.substr(pos1+1, pos2-pos1-1);
}

string get_type(string& line){
   int pos1, pos2;

   pos1 = find_vertical_bar(4, line);
   pos2 = line.find("|", pos1+1);
   return line.substr(pos1+1, pos2-pos1-1);
}

string get_status(string& line){
   int pos1, pos2;

   pos1 = find_vertical_bar(5, line);
   pos2 = line.find("|", pos1+1);
   return line.substr(pos1+1, pos2-pos1-1);
}

double get_balance(string& line){
   int pos1, pos2;
   string balance_str;

   pos1 = find_vertical_bar(6, line);
   pos2 = line.find("|", pos1+1);
   balance_str = line.substr(pos1+1, pos2-pos1-1);
   return strtod(balance_str.c_str(), NULL);
}

vector<double> get_freebets(string& line){
   size_t pos1, pos2, pos3, pos4 = 0;
   string freebets_str;
   string one_freebet;
   vector<double> freebets_vect;

   pos1 = find_vertical_bar(7, line);
   pos2 = line.length();
   freebets_str = line.substr(pos1+1, pos2-pos1-1);

   if (freebets_str == ""){
      return freebets_vect;
   }
   else{
      pos3 = freebets_str.find(",", 0);
      freebets_vect.push_back(strtod(freebets_str.substr(0, pos3).c_str(), NULL));
      while(pos4 != string::npos){
         pos4 = freebets_str.find(",", pos3+1);
         if (pos4 == string::npos){
            freebets_vect.push_back(strtod(freebets_str.substr(pos3+1, (freebets_str.length())-pos3-1).c_str(), NULL));
         }
         else {
            freebets_vect.push_back(strtod(freebets_str.substr(pos3+1, pos4-pos3-1).c_str(), NULL));
         }
         pos3 = pos4;
      }
   }
}

string get_freebets_str(string& line){
   size_t pos1, pos2;

   pos1 = find_vertical_bar(7, line);
   pos2 = line.length();
   return line.substr(pos1+1, pos2-pos1-1);
}

string get_balance_str(string& line){
   int pos1, pos2;
   string balance_str;

   pos1 = find_vertical_bar(6, line);
   pos2 = line.find("|", pos1+1);
   balance_str = line.substr(pos1+1, pos2-pos1-1);
   return balance_str;
}

string increase_string_num(string& original_string){
   int original_int = strtol(original_string.c_str(), NULL, 10);

   return double_to_string(double(original_int+1));
}

bool Registration(UserFile& users, User* &U){
   string username, password, fullname, username_to_check, password_to_check;
   string line;
   bool exists = false;

   vector<double> fb;

   cout << "User Registration" << endl;
   cout << "Please insert your desired username, password and your full name." << endl;
   cout << "Username: ";
   getline(cin, username);
   cout << "Password: ";
   getline(cin, password);
   cout << "Fullname: ";
   getline(cin, fullname);

   if (username == "guest"){
      cout << endl << "Registration Failed (username cannot be \"guest\")" << endl << endl;
      U = NULL;
      return false;
   }
   else if (username == "" || password == "" || fullname == ""){
      cout << endl << "Registration Failed (one or more empty fields)" << endl << endl;
      U = NULL;
      return false;
   }

   while(users.get_line(line) && !exists){
      username_to_check = get_username(line);
      if (username == username_to_check){
         exists = true;
      }
   }

   if (exists){
      cout << endl << "Registration Failed (username already exists)" << endl << endl;
      U = NULL;
      return false;
   }
   else {
      double balance = 0;
      cout << endl << "Registration Successful" << endl << endl;
      string last_line = users.get_last_line();
      string last_user_id = get_user_id(last_line);
      string next_user_id = increase_string_num(last_user_id);
      U = new Punter(next_user_id, username, password, fullname, balance, fb);
      users.add_punter(U);
      HistoryAdapter::addOperation("Punter with id " + U->get_user_id() + " registered and logged in");
      return true;
   }
}

bool Login(UserFile& users, User* &U){
   string user_id, username, password, fullname, username_to_check, password_to_check, status;
   string line;
   double balance;
   vector<double> freebets;

   /* Login "Welcome" Message */
   cout << "Welcome to TroBeta!" << endl;
   cout << "Please insert your username and password to log in. If you do not have " << endl;
   cout << "an account, insert newline(enter) or guest:guest to log in as a guest." << endl << endl;
   cout << "Username: ";
   getline(cin, username);
   cout << "Password: ";
   getline(cin, password);

   /* Info Check (guest) */
   if ((username == "guest" && password == "guest") || (username == "" && password == "")){
      /* Login Succesful (guest) */
      cout << endl << "Login Successful (Guest Mode)" << endl << endl;
      U = new Guest();
      return true;
   }

   /* Info Check From File */
   users.reset_pos();
   while(users.get_line(line)){
      username_to_check = get_username(line);
      password_to_check = get_password(line);

      if (username == username_to_check && password == password_to_check){
            user_id = get_user_id(line);
            fullname = get_fullname(line);
            if (get_type(line) == "1") {
               status = get_status(line);
               if (status == "A"){
                  /* Login Succesful (not a guest) */
                  cout << endl << "Login Successful" << endl << endl;
                  balance = get_balance(line);
                  freebets = get_freebets(line);
                  U = new Punter(user_id, username, password, fullname, balance, freebets);
                  HistoryAdapter::addOperation("Punter with id " + U->get_user_id() + " logged in");
               }
               else {
                  /* Login Failed (User Blocked) */
                  cout << endl << "Login Failed. Your Account Has Been Locked (Reason: " << status.substr(2, status.length()-2) << ")" << endl << endl;
                  U = NULL;
                  return false;
               }
            }
            else if (get_type(line) == "2") {
               cout << endl << "Login Successful" << endl << endl;
               U = new Trader(user_id, username, password, fullname);
            }
            else if (get_type(line) == "3") {
               cout << endl << "Login Successful" << endl << endl;
               U = new Director(user_id, username, password, fullname);
            }
            return true;
      }
   }

   /* Login Failed */
   cout << endl << "Login Failed. Please Try Again" << endl << endl;
   U = NULL;
   return false;
}

void NodeNavigationScreen(Hierarchy& hierarchy, User* U){
   cout << "Location: ";
   hierarchy.printPath();
   cout << "Performance type: " << hierarchy.getPricesView() << endl << endl;
   cout << "Choose:" << endl;
   U->print_operations(hierarchy);
   cout << endl;
   U->print_nodes(hierarchy);
}
