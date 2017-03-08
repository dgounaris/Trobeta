#include "Users.h"
#include "GeneralFunctions.h"
#include "History.h"

using namespace std;


///////////////////////////////////////////////////////////////////// USERFILE //////////////////////////////////////////////////////////////////////

/* Constructor */
UserFile::UserFile(const char* filename): users(filename) {
   if (!users.is_open()){
      cout << "Error (File " << filename << " failed to open)" << endl;
   }
}

UserFile::~UserFile(){
   if (users.is_open()){
      users.close();
   }
}

istream& UserFile::get_line(string& line){
   return getline(users, line);
}

string UserFile::get_last_line(){
   string curline, lastline;
   this->reset_pos();
   while(getline(users, curline)){
      lastline = curline;
   }
   return lastline;
}

void UserFile::reset_pos(){
   users.clear();
   users.seekg(0);
}

void UserFile::set_pos_to_end(){
   users.clear();
   users.seekg(0, users.end);
}

void UserFile::add_punter(User* U){
   this->set_pos_to_end();
   users << U->get_user_id() << "|" << U->get_username() << "|" << U->get_fullname() << "|" << U->get_password() << "|" << "1" << "|" << "A" << "|" << U->get_balance() << "|" << "\n";
}

string UserFile::get_user_line(string& user_id){
   string line;

   this->open_UserFile("users.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (user_id == get_user_id(line)){
         this->close_UserFile();
         return line;
      }
   }
   this->close_UserFile();
}

string UserFile::get_username_line(string& username){
   string line;

   if (username_exists(username)){
   	this->open_UserFile("users.csv");
   	this->reset_pos();
   	while(this->get_line(line)){
     		if (username == get_username(line)){
         		this->close_UserFile();
         		return line;
                }
      	}
   }
   else {
   	return "not found";
   }
}

void UserFile::print_users(){
   string line, user_id, username, fullname, password, type, status, balance, freebets;


   this->open_UserFile("users.csv");
   this->reset_pos();
   cout << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;
   cout << "|User_id   |Username        |Fullname                 |Password        |Type  |Status                      |Balance   |Freebets          |" << endl;
   cout << "|----------|----------------|-------------------------|----------------|------|----------------------------|----------|------------------|" << endl;
   while(this->get_line(line)){
      user_id = get_user_id(line);
      username = get_username(line);
      fullname = get_fullname(line);
      password = get_password(line);
      type = get_type(line);
      status = get_status(line);
      balance = get_balance_str(line);
      freebets = get_freebets_str(line);
      cout << "|" << setw(10) << left << user_id << "|" << setw(16) << username << "|" << setw(25) << fullname << "|" << setw(16) << password << "|" << setw(6) << type << "|";
      if (type == "1" && get_status(line) != "A"){
         cout << setw(28) << left << status.substr(2, status.length()-2);
      }
      else {
         cout << "                            ";
      }
      cout << "|" << setw(10) << balance << "|" << setw(18) << freebets << "|" << endl;
   }
   cout << "------------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;
   this->close_UserFile();
}

bool UserFile::username_exists(string& new_username){
   string line;

   this->open_UserFile("users.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (new_username == get_username(line)){
         this->close_UserFile();
         return true;
      }
   }
   this->close_UserFile();
   return false;
}

void UserFile::lock_unlock_user(){
   string username, username_line, reason, user_id;

   cout << "Insert the username of the user you want to lock/unlock: ";
   getline(cin, username);
   cout << endl;

   username_line = this->get_username_line(username);
   if (username_line == "not found"){
	cout << "This username doesn't exist" << endl << endl;
   }
   else {
	if (get_type(username_line) == "1"){
                user_id = get_user_id(username_line);
		if (get_status(username_line) == "A"){
                        cout << "Insert the reason for locking this user: ";
   			getline(cin, reason);
   			cout << endl;
                        if (reason == ""){
				reason = "N/A";
                	}
			this->change_status_lock(user_id, reason);
                        cout << "User locked successfully" << endl << endl;
		}
                else {
			this->change_status_activate(user_id);
                        cout << "User activated successfully" << endl << endl;
                }
	}
        else {
		cout << "Cannot lock director or trader" << endl << endl;
	}
   }
}

void UserFile::change_username(string& user_id, string& old_username, string& new_username){
   unsigned int i;
   string line;
   vector<string> newfilelines;

   this->open_UserFile("users.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (user_id == get_user_id(line)){
         newfilelines.push_back(line.replace(find_vertical_bar(1, line)+1, old_username.length(), new_username));
      }
      else {
         newfilelines.push_back(line);
      }
   }

   this->close_UserFile();
   remove("users.csv");
   users.open ("users.csv", ios::in | ios::out | ios::trunc);
   for (i = 0; i < newfilelines.size(); i++){
      users << newfilelines[i] << "\n";
   }
   this->close_UserFile();
}

void UserFile::change_password(string& user_id, string& old_password, string& new_password){
   unsigned int i;
   string line;
   vector<string> newfilelines;

   this->open_UserFile("users.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (user_id == get_user_id(line)){
         newfilelines.push_back(line.replace(find_vertical_bar(3, line)+1, old_password.length(), new_password));
      }
      else {
         newfilelines.push_back(line);
      }
   }

   this->close_UserFile();
   remove("users.csv");
   users.open ("users.csv", ios::in | ios::out | ios::trunc);
   for (i = 0; i < newfilelines.size(); i++){
      users << newfilelines[i] << "\n";
   }
   this->close_UserFile();
}

void UserFile::change_balance(string& user_id, string& new_balance){
   unsigned int i;
   string line;
   vector<string> newfilelines;

   this->open_UserFile("users.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (user_id == get_user_id(line)){
         newfilelines.push_back(line.replace(find_vertical_bar(6, line)+1, get_balance_str(line).length(), new_balance));
      }
      else {
         newfilelines.push_back(line);
      }
   }

   this->close_UserFile();
   remove("users.csv");
   users.open ("users.csv", ios::in | ios::out | ios::trunc);
   for (i = 0; i < newfilelines.size(); i++){
      users << newfilelines[i] << "\n";
   }
   this->close_UserFile();
}

void UserFile::change_status_lock(string& user_id, string& reason){
   unsigned int i;
   string line;
   vector<string> newfilelines;

   this->open_UserFile("users.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (user_id == get_user_id(line)){
         newfilelines.push_back(line.replace(find_vertical_bar(5, line)+1, 1, ("L," + reason)));
      }
      else {
         newfilelines.push_back(line);
      }
   }

   this->close_UserFile();
   remove("users.csv");
   users.open ("users.csv", ios::in | ios::out | ios::trunc);
   for (i = 0; i < newfilelines.size(); i++){
      users << newfilelines[i] << "\n";
   }
   this->close_UserFile();
}

void UserFile::change_status_activate(string& user_id){
   unsigned int i;
   string line;
   vector<string> newfilelines;

   this->open_UserFile("users.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (user_id == get_user_id(line)){
         newfilelines.push_back(line.replace(find_vertical_bar(5, line)+1, get_status(line).length(), "A"));
      }
      else {
         newfilelines.push_back(line);
      }
   }

   this->close_UserFile();
   remove("users.csv");
   users.open ("users.csv", ios::in | ios::out | ios::trunc);
   for (i = 0; i < newfilelines.size(); i++){
      users << newfilelines[i] << "\n";
   }
   this->close_UserFile();
}

void UserFile::change_freebets(string& username, string& amount){
   unsigned int i;
   string line;
   vector<string> newfilelines;

   this->open_UserFile("users.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (username == get_username(line)){
         if (get_freebets_str(line) == ""){
	 	newfilelines.push_back(line + amount);
	 }
         else {
         	newfilelines.push_back(line + "," + amount);
	 }
      }
      else {
         newfilelines.push_back(line);
      }
   }

   this->close_UserFile();
   remove("users.csv");
   users.open ("users.csv", ios::in | ios::out | ios::trunc);
   for (i = 0; i < newfilelines.size(); i++){
      users << newfilelines[i] << "\n";
   }
   this->close_UserFile();
}

void UserFile::open_UserFile(const char* filename){
   users.open(filename, ios::in | ios::out);
   if (!users.is_open()){
      cout << "Error (File " << filename << " failed to open)" << endl;
   }
}

void UserFile::close_UserFile(){
   users.close();
}

///////////////////////////////////////////////////////////////////// WALLET //////////////////////////////////////////////////////////////////////

Wallet::Wallet(double ba, vector<double> fb): balance(ba), freebets(fb) {}

double Wallet::get_balance() {return balance;}
vector<double> Wallet::get_freebets() {return freebets;}

void Wallet::print_freebets() {
	unsigned int i;

	if (freebets.size() == 0){
		cout << "-empty-";
	}
	else {
		for(i = 0; i < freebets.size(); i++) {
     		 cout << char('a'+i) << ". " << freebets[i] << "  ";
   		}
   	}
}

void Wallet::increase_balance(double amount){
   balance += amount;
}

void Wallet::decrease_balance(double amount){
   balance -= amount;
}

///////////////////////////////////////////////////////////////////// USER //////////////////////////////////////////////////////////////////////

/* Constructor */
User::User(): user_id("0"), username("guest"), password("guest"), fullname("") {}
User::User(string ui, string un, string pw, string fn): user_id(ui), username(un), password(pw), fullname(fn) {}

/* Accessors */
string User::get_user_id() {return user_id;}
string User::get_username() {return username;}
string User::get_fullname() {return fullname;}
string User::get_password() {return password;}
double User::get_balance() {return -1;}

/* Mutators */
void User::set_user_id(string& ui) {this->user_id = ui;}
void User::set_username(string& un) {this->username = un;}
void User::set_fullname(string& fn) {this->fullname = fn;}
void User::set_password(string& pw) {this->password = pw;}

/* Printing */
void User::print_operations(Hierarchy& hierarchy) {}
void User::print_nodes(Hierarchy& hierarchy){}
void User::operation(string& op, Hierarchy& hierarchy, UserFile& users, BetFile& bets) {}

///////////////////////////////////////////////////////////////////// GUEST //////////////////////////////////////////////////////////////////////

/* Constructors */
Guest::Guest(): User(){}
Guest::Guest(string ui, string un, string pw, string fn): User(ui, un, pw, fn){}

/* Printing */
void Guest::print_operations(Hierarchy& hierarchy) {
	cout << "The ID of the node you want to visit" << endl;
	cout << "H (Home)        to return to the home screen" << endl;
	cout << "T (Toggle)      to change the price view" << endl;
	cout << "X (Exit)        to exit from the system" << endl;
}

void Guest::print_nodes(Hierarchy& hierarchy){
	hierarchy.printChildren();
}

/* Execute Operations */
void Guest::operation(string& op, Hierarchy& hierarchy, UserFile& users, BetFile& bets) {
	char* p;
	int converted = strtol(op.c_str(), &p, 10);

	if (*p == 0) { // Integer value in input
		hierarchy.toVisibleChild(converted);
	}
	else if (op == "H") {
		hierarchy.toHome();
	}
	else if (op == "T") {
		hierarchy.switchPricesView();
	}
	else if (op == "X"){
		// Do nothing. Main will finish
	}
	else {
		cout << "--- Invalid Operation ---" << endl << endl;
	}
}

///////////////////////////////////////////////////////////////////// PUNTER //////////////////////////////////////////////////////////////////////

/* Constructor */
Punter::Punter(string ui, string un, string pw, string fn, double b, vector<double> f): Guest(ui, un, pw, fn), wallet(b, f){}

double Punter::get_balance() {return wallet.get_balance();}
vector<double> Punter::get_freebets() {return wallet.get_freebets();}

/* Printing */
void Punter::print_operations(Hierarchy& hierarchy) {
	Guest::print_operations(hierarchy);
	cout << "A (Account)     to manage your account's information" << endl;
	if(hierarchy.hasBettableChildren()){
		cout << "P (Place)       to place a bet" << endl;
	}
}

void Punter::print_nodes(Hierarchy& hierarchy){
	hierarchy.printChildren();
}

/* Execute Operations */
void Punter::operation(string& op, Hierarchy& hierarchy, UserFile& users, BetFile& bets) {
	char* p;
	double amount;
	string new_balance, amount_str, user_id;
	int converted = strtol(op.c_str(), &p, 10);

	if (*p == 0) { // Integer value in input
		if (hierarchy.toVisibleChild(converted)){
			cout << "1" << endl;
			HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " moved to node with index " + hierarchy.getActiveNodePath());
		}
	}
	else if (op == "H") {
               	  HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " moved to Home");
                  hierarchy.toHome();
	}
 	else if (op == "T") {
             	HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " toggled the prices view to " + hierarchy.getPricesView());
              	hierarchy.switchPricesView();
	}
	else if (op == "X"){
		HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " exited the application");
   		// Do nothing. Main will finish and return 0
	}
	else if (op == "A"){
		HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " moved to his account view");
		this->edit_profil(users, bets);
	}
	else if (hierarchy.hasBettableChildren() && op == "P"){
		string selection_str;
		cout << "Your Balance: " << wallet.get_balance() << endl;
		cout << "Insert your selection or \"cancel\" to cancel the bet: ";
		getline(cin, selection_str);
		if (selection_str == "cancel"){
			cout << endl << "Bet Cancelled" << endl << endl;
		}
		else {
			cout << endl << "Your Freebets: ";
			wallet.print_freebets();
			cout << endl;
			cout << "Insert Amount Or Select A Freebet By typing The Corresponding Letter: ";
			getline(cin, amount_str);
 			amount = strtod(amount_str.c_str(), &p);
			if (*p == 0){	// Amount Selection
				if (wallet.get_balance() == 0){
					cout << endl << "Empty Wallet. Please Refill Your Wallet From Your Account's Menu" << endl << endl;
				}
				else {
					while(amount > wallet.get_balance() && wallet.get_balance() > 0){
						cout << endl << "Insufficient Balance. Please Insert A Smaller Amount: ";
						getline(cin, amount_str);
						amount = strtod(amount_str.c_str(), NULL);
					}
					new_balance = double_to_string(wallet.get_balance()-amount);
					user_id = User::get_user_id();
					users.change_balance(user_id, new_balance);
					wallet.decrease_balance(amount);
					int selection = strtol(selection_str.c_str(), NULL, 10);
					string node_id = hierarchy.getChildNodePath(selection);
					HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " placed a bet on node " + node_id + " with amount " + amount_str);
					bets.add_bet(user_id, node_id, amount_str);
					cout << endl << "Bet Has Been Placed Successfully" << endl;
 					cout << "Amount Betted: " << amount_str << endl << endl;
                            		// Bet placement here
				}
			}
                    	else {		// Freebet Selection

                   	}
		}
	}
	else {
		cout << "--- Invalid Operation ---" << endl << endl;
	}
}

/* Profil Management */
void Punter::profil_operation(string &op, UserFile& users, BetFile& bets) {
	string user_id, old_password, new_username, new_password1, new_password2, amount_str;
	double old_balance;

	if (op == "R") {
		cout << "Insert your new username: ";
		getline(cin, new_username);
              	cout << endl;

               	if (new_username == "guest"){
            		cout << "Username cannot be \"guest\". Please choose another username" << endl << endl;
            		HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " attempted to change his username to \"guest\"");
             	}
           	else if(users.username_exists(new_username)){
              		HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " attempted to change his username to an already existing one");
               		cout << "This username is already being used. Please choose another username" << endl << endl;
              	}
          	else {
                     string old_username = User::get_username();
                     HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " changed his username from " + old_username + " to " + new_username);
                     user_id = User::get_user_id();
                     users.change_username(user_id, old_username, new_username);
                     User::set_username(new_username);
                     cout << "Username Changed Successfully" << endl << endl;
           	}
	}
	else if (op == "P") {
              	cout << "Insert your new password: ";
           	getline(cin, new_password1);
           	cout << "Insert your new password again: ";
            	getline(cin, new_password2);
             	cout << endl;

            	if ((new_password1 == new_password2) && (new_password1 != "")){
               		old_password = User::get_password();
             		user_id = User::get_user_id();
             		users.change_password(user_id, old_password, new_password1);
                	User::set_password(new_password1);
                   	cout << "Password Changed Successfully" << endl << endl;
                    	HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " changed his password");
              	}
             	else if(new_password1 == "" || new_password2 == ""){
                     	cout << "Password Change Failed (one or more empty fields)" << endl << endl;
                     	HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " attempted to change his password but failed");
             	}
             	else {
                     	cout << "Password Change Failed (passwords do not match)" << endl << endl;
                    	HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " attempted to change his password but failed");
           	}
	} 
	else if (op == "B"){
             	string user_id = this->get_user_id();
              	bets.print_bet_history(user_id);
             	HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " viewed his bet history");
	}
	else if(op == "D"){
            	cout << "Insert the amount you want to add to your balance: ";
            	getline(cin, amount_str);
           	cout << endl;

          	HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " increased his balance by " + amount_str);
          	user_id = User::get_user_id();
           	old_balance = wallet.get_balance();
           	double amount = strtod(amount_str.c_str(), NULL);
           	amount_str = double_to_string(amount+old_balance);
           	users.change_balance(user_id, amount_str);
            	wallet.increase_balance(amount);
            	cout << "Balance Increased Successfully" << endl << endl;
	}
	else if (op == "X"){
              	HistoryAdapter::addOperation("Punter with id " + this->get_user_id() + " exited from the account view");
	}
	else {
           	cout << "--- Invalid Operation ---" << endl << endl;
	}
}

void Punter::edit_profil(UserFile& users, BetFile& bets) {
	string op;

	while(op != "X"){
		cout << "=============================== PROFILE ==============================" << endl;
               	cout << "Punter: " << get_username() << endl;
              	cout << "Balance: " << wallet.get_balance() << endl;
              	cout << "Freebets: ";
             	wallet.print_freebets();
               	cout << endl << endl;

            	cout << "Choose:" << endl;
            	cout << "R (Rename)    to change username" << endl;
              	cout << "P (Password)  to change password" << endl;
              	cout << "B (Bets)      to show bet history" << endl;
            	cout << "D (Deposit)   to increase the balance" << endl;
            	cout << "X (Exit)      to exit the profil settings" << endl;

           	cout << endl;
            	cout << "Your Choice: ";
       		getline(cin, op);
           	cout << endl;

               	this->profil_operation(op, users, bets);                  // Execute the given operation
	}
}

///////////////////////////////////////////////////////////////////// TRADER //////////////////////////////////////////////////////////////////////

/* Constructor */
Trader::Trader(string ui, string un, string pw, string fn): Guest(ui, un, pw, fn){}

/* Printing */
void Trader::print_operations(Hierarchy& hierarchy) {
	Guest::print_operations(hierarchy);
     	cout << "B (Bets)        to show the last 20 bets" << endl;
    	cout << "F (Freebets)    to give coupon to a user" << endl;
     	if(hierarchy.hasVoidableChildren()){
        	cout << "V (Void)        to cancel a selection" << endl;
      	}
     	if(hierarchy.hasSettlableChildren()){
            	cout << "S (Settle)      to settle the market" << endl;
       	}
}

void Trader::print_nodes(Hierarchy& hierarchy){
      	hierarchy.printChildren();
}

/* Execute Operations */
void Trader::operation(string& op, Hierarchy& hierarchy, UserFile& users, BetFile& bets) {
     	char* p;
	int converted = strtol(op.c_str(), &p, 10);

	if (*p == 0) { // Integer value in input
     		hierarchy.toVisibleChild(converted);
     	}
      	else if (op == "H") {
             	hierarchy.toHome();
      	}
     	else if (op == "T") {
           	hierarchy.switchPricesView();
       	}
       	else if (op == "X"){
             	// Do nothing. Main will finish and return 0
     	}
      	else if (op == "B"){
            	bets.print_bet_history();
  	}
      	else if (op == "F"){
                string username, amount;
                cout << "Insert a punter's username to add a freebet: ";
                getline(cin, username);
                cout << endl;
                cout << "Insert the amount of the freebet: ";
                getline(cin, amount);
                cout << endl;
              	users.change_freebets(username, amount);
       	}
       	else if (hierarchy.hasVoidableChildren() && op == "V"){
            	int index;
           	string index_str;
	   	hierarchy.printChildren();
	   	cout << "Insert the index of the node you want to void/unvoid: ";
	     	getline(cin, index_str);
             	index = atoi(index_str.c_str());
              	string node_id = hierarchy.getChildNodePath(index);
             	if (hierarchy.isChildVoided(index)){
                	bets.change_bet_unvoiding(node_id, users);
             	}
            	else{
                   	bets.change_bet_voiding(node_id, users);
             	}
              	hierarchy.switchVoiding(index);
               	hierarchy.Save("hierarchy.dat");
      	}
   	else if (hierarchy.hasSettlableChildren() && op == "S"){
            	int index;
             	string index_str;
	    	hierarchy.printChildren();
	     	cout << "Insert the index of the node you want to void/unvoid: ";
	      	getline(cin, index_str);
             	index = atoi(index_str.c_str());
           	string node_id = hierarchy.getChildNodePath(index);
          	if (hierarchy.isChildWinning(index)){
                	bets.change_bet_unwinning(node_id, users, hierarchy.getChildPrice(index));
               	}
            	else{
              		bets.change_bet_winning(node_id, users, hierarchy.getChildPrice(index));
             	}
               	hierarchy.settleBet(index);
           	hierarchy.Save("hierarchy.dat");
    	}
     	else {
            	cout << "--- Invalid Operation ---" << endl << endl;
       	}
}

///////////////////////////////////////////////////////////////////// DIRECTOR //////////////////////////////////////////////////////////////////////

/* Constructor */
Director::Director(string ui, string un, string pw, string fn): Trader(ui, un, pw, fn) {}

/* Printing */
void Director::print_operations(Hierarchy& hierarchy) {
	Guest::print_operations(hierarchy);
    	cout << "B (Bets)        to show the last 20 bets" << endl;
   	cout << "F (Freebets)    to give coupon to a user" << endl;
  	cout << "S (Save)        to save current state and synchronise the system files" << endl;
    	cout << endl << "Management:" << endl;
   	cout << "L (Logs)        to show the last 25 actions on the system" << endl;
     	cout << "U (Users)       to manage users" << endl;
    	cout << "V (Visibility)  to change the node visibility" << endl;
      	cout << "R (Rename)      to rename a node" << endl;
       	cout << "N (New)         to create a new node" << endl;
     	cout << "C (Copy)        to copy a node (as hidden)" << endl;
    	cout << "D (Delete)      to delete a node" << endl;
}

void Director::print_nodes(Hierarchy& hierarchy){
      	hierarchy.printChildrenAll();
}

/* Execute Operations */
void Director::operation(string& op, Hierarchy& hierarchy, UserFile& users, BetFile& bets) {
    	char* p;
	int converted = strtol(op.c_str(), &p, 10);

	if (*p == 0) { // Integer value in input
        	hierarchy.toChild(converted);
       	}
     	else if (op == "H") {
        	hierarchy.toHome();
      	}
     	else if (op == "T") {
            	hierarchy.switchPricesView();
      	}
       	else if (op == "X"){
          	// Do nothing. Main will finish and return 0
      	}
    	else if (op == "B"){
          	bets.print_bet_history();
      	}
    	else if (op == "F"){
         	string username, amount;
                cout << "Insert a punter's username to add a freebet: ";
                getline(cin, username);
                cout << endl;
                cout << "Insert the amount of the freebet: ";
                getline(cin, amount);
                cout << endl;
              	users.change_freebets(username, amount);
   	}
     	else if (op == "S") {
  	 	hierarchy.Save("hierarchy.dat");
	}
      	else if (op == "L"){
        	HistoryAdapter::printRecentHistory(25);
     	}
    	else if (op == "U"){
           	this->edit_users(users);
      	}
     	else if (op == "V"){
             	int index;
                string index_str;
	    	hierarchy.printChildrenAll();
		cout << "Insert the index of the node you want to make visible/hidden: ";
                getline(cin, index_str);
	   	index = strtol(index_str.c_str(), NULL, 10);
        	hierarchy.switchVisibility(index);
 	}
    	else if (op == "R") {
        	hierarchy.printChildrenAll();
          	hierarchy.Rename();
     	}
    	else if (op == "N") {
            	hierarchy.printChildrenAll();
             	hierarchy.New();
      	}
    	else if (op == "C") {
    		hierarchy.printChildrenAll();
     		hierarchy.Copy();
    	}
  	else if (op == "D") {
     		hierarchy.printChildrenAll();
       		hierarchy.Delete();
       	}
   	else {
             	cout << "--- Invalid Operation ---" << endl << endl;
       	}
}

/* Users Management */
void Director::users_operation(string &op, UserFile& users) {
     	string user_id, old_password, new_username, new_password1, new_password2, amount_str;
     	double old_balance;

    	if (op == "V") {
   		users.print_users();
    	}
   	else if (op == "S") {

    	}
     	else if (op == "L"){
		users.lock_unlock_user();
	}
    	else if (op == "X"){
   		// Do nothing. The director will return to the hierarchy
        }
      	else {
       		cout << "--- Invalid Operation ---" << endl << endl;
      	}
}

void Director::edit_users(UserFile& users) {
     	string op;

   	while(op != "X"){
      		cout << "=============================== USERS ===============================" << endl;
       		cout << "Choose:" << endl;
           	cout << "V (View)      to view the users" << endl;
              	cout << "S (Search)    to search for a user" << endl;
          	cout << "L (Lock)      to change the status of a user" << endl;
           	cout << "X (Exit)      to exit from the users menu" << endl << endl;

           	cout << "Your Choice: ";
          	getline(cin, op);
          	cout << endl;

           	this->users_operation(op, users);                  // Execute the given operation
    	}
}
