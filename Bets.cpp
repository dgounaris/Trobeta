#include "Bets.h"
#include "UserLibraries.h"
#include "GeneralFunctions.h"
#include "History.h"

using namespace std;

/* Constructor */
BetFile::BetFile(const char* filename): bets(filename) {
   if (!bets.is_open()){
      cout << "Error (File " << filename << " failed to open)" << endl;
   }
} 

BetFile::BetFile() {}

BetFile::~BetFile(){
   if (bets.is_open()){
      bets.close();
   }
}
            
istream& BetFile::get_line(string& line){
   return getline(bets, line);
}

string BetFile::get_last_line() {
   string curline, lastline;

   this->open_BetFile("bets.csv");
   this->reset_pos();
   while(getline(bets, curline)){
      lastline = curline;
   }
   this->close_BetFile();
   return lastline;       
}
            
void BetFile::reset_pos(){
   bets.clear();
   bets.seekg(0);
}

void BetFile::set_pos_to_end(){
   bets.clear();
   bets.seekg(0, bets.end);
}

void BetFile::print_bet_history(string& user_id){
   string line;

   this->open_BetFile("bets.csv");
   this->reset_pos();
   cout << "============ Bet History ===========" << endl;
   while(this->get_line(line)){
      if (bets_get_user_id(line) == user_id){
           cout << "You placed a bet on node " << bets_get_node_id(line) << " with amount " << bets_get_stake(line) << ". This bet ";
           if (bets_get_result(line) == "-"){
              cout << "is unresolved." << endl;
           }
           else if (bets_get_result(line) == "W"){
              cout << "has been won." << endl;
           }
           else if (bets_get_result(line) == "L"){
              cout << "has been lost." << endl;
           }
           else if (bets_get_result(line) == "V"){
              cout << "has been cancelled." << endl; 
           }
      }
   }
   this->close_BetFile();
   cout << "====================================" << endl << endl;
}

void BetFile::print_bet_history(){
	int i;
	string line;
	vector<string> bethistory;

	this->open_BetFile("bets.csv");
 	this->reset_pos();
	cout << "----------------------------------------------------" << endl;
        cout << "|Bet_id  |User_id   |Node_id     |Stake     |Result|" << endl;
        cout << "|--------|----------|------------|----------|------|" << endl;
   	while(this->get_line(line)){
        	bethistory.push_back(line);
   	}
   	if (bethistory.size() < 20){
		for (i = 0; i < bethistory.size(); i++){
			cout << "|" << setw(8) << left << bets_get_bet_id(bethistory[i]) << "|" << setw(10) << bets_get_user_id(bethistory[i]) << "|" << setw(12) << bets_get_node_id(bethistory[i]) << "|" << setw(10) << bets_get_stake(bethistory[i]) << "|" << setw(6) << bets_get_result(bethistory[i]) << "|" << endl;
		}
  	}
        else {
		for (i = bethistory.size()-20; i < bethistory.size(); i++){
			cout << "|" << setw(8) << left << bets_get_bet_id(bethistory[i]) << "|" << setw(10) << bets_get_user_id(bethistory[i]) << "|" << setw(12) << bets_get_node_id(bethistory[i]) << "|" << setw(10) << bets_get_stake(bethistory[i]) << "|" << setw(6) << bets_get_result(bethistory[i]) << "|" << endl;
		}
	}
        cout << "----------------------------------------------------" << endl << endl;
  	this->close_BetFile();
}

void BetFile::add_bet(string& user_id, string& node_id, string& stake){
   string lastline, last_bet_id, next_bet_id;

   lastline = this->get_last_line();
   last_bet_id = bets_get_bet_id(lastline);
   next_bet_id = increase_string_num(last_bet_id);

   this->open_BetFile("bets.csv");
   this->set_pos_to_end();
   bets << next_bet_id << "|" << user_id << "|" << node_id << "|" << stake << "|" << "-" << "\n";
   this->close_BetFile();
}

void BetFile::change_bet_voiding(string& node_id, UserFile& users){
   unsigned int i;
   string line;
   vector<string> newfilelines;

   this->open_BetFile("bets.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (node_id == bets_get_node_id(line)){
         newfilelines.push_back(line.replace(find_vertical_bar(4, line)+1, 1, "V"));
         string user_id = bets_get_user_id(line);
         string user_line = users.get_user_line(user_id);
         double balance = get_balance(user_line);
         double stake = bets_get_stake(line);
         string new_balance = double_to_string(balance+stake);
         users.change_balance(user_id, new_balance);
         HistoryAdapter::addOperation("Punter with id " + bets_get_user_id(line) + " had the amount of " + bets_get_stake_str(line) + " returned to his balance due to the voiding of the node " + node_id);
      }
      else {
         newfilelines.push_back(line);
      }
   }

   this->close_BetFile();
   remove("bets.csv");
   bets.open ("bets.csv", ios::in | ios::out | ios::trunc);
   for (i = 0; i < newfilelines.size(); i++){
      bets << newfilelines[i] << "\n";
   }
   this->close_BetFile();
}

void BetFile::change_bet_unvoiding(string& node_id, UserFile& users){
   unsigned int i;
   string line;
   vector<string> newfilelines;

   this->open_BetFile("bets.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (node_id == bets_get_node_id(line)){
         newfilelines.push_back(line.replace(find_vertical_bar(4, line)+1, 1, "-"));
         string user_id = bets_get_user_id(line);
         string user_line = users.get_user_line(user_id);
         double balance = get_balance(user_line);
         double stake = bets_get_stake(line);
         string new_balance = double_to_string(balance-stake);
         users.change_balance(user_id, new_balance);
         HistoryAdapter::addOperation("Punter with id " + bets_get_user_id(line) + " had the amount of " + bets_get_stake_str(line) + " drawn from his balance due to the unvoiding of the node " + node_id);
      }
      else {
         newfilelines.push_back(line);
      }
   }

   this->close_BetFile();
   remove("bets.csv");
   bets.open ("bets.csv", ios::in | ios::out | ios::trunc);
   for (i = 0; i < newfilelines.size(); i++){
      bets << newfilelines[i] << "\n";
   }
   this->close_BetFile();
}

void BetFile::change_bet_winning(string& node_id, UserFile& users, double performance){
   unsigned int i;
   string line;
   string parent_node_id = node_id.substr(0, node_id.find_last_of(".")+1);
   vector<string> newfilelines;

   this->open_BetFile("bets.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (parent_node_id == (bets_get_node_id(line)).substr(0, (bets_get_node_id(line)).find_last_of(".")+1)){
         if (bets_get_node_id(line) == node_id){
            newfilelines.push_back(line.replace(find_vertical_bar(4, line)+1, 1, "W"));
            double full_amount = performance*bets_get_stake(line);
            double user_amount = full_amount*0.95;
            double system_profit = full_amount*0.05;
            string user_id = bets_get_user_id(line);
            string user_line = users.get_user_line(user_id);
            double balance = get_balance(user_line);
            string new_balance = double_to_string(balance+user_amount);
            users.change_balance(user_id, new_balance);
            HistoryAdapter::addOperation("Punter with id " + bets_get_user_id(line) + " won the amount of " + double_to_string(user_amount) + " from his bet on node " + node_id + ". System profit: " + double_to_string(system_profit));
         }
         else {
            newfilelines.push_back(line.replace(find_vertical_bar(4, line)+1, 1, "L"));
            HistoryAdapter::addOperation("Punter with id " + bets_get_user_id(line) + " lost the amount of " + bets_get_stake_str(line) + " from his bet on node " + bets_get_node_id(line) + ". System profit: " + bets_get_stake_str(line));
         }
      }
      else {
         newfilelines.push_back(line);
      }
   }

   this->close_BetFile();
   remove("bets.csv");
   bets.open ("bets.csv", ios::in | ios::out | ios::trunc);
   for (i = 0; i < newfilelines.size(); i++){
      bets << newfilelines[i] << "\n";
   }
   this->close_BetFile();
}

void BetFile::change_bet_unwinning(string& node_id, UserFile& users, double performance){
   unsigned int i;
   string line;
   string parent_node_id = node_id.substr(0, node_id.find_last_of(".")+1);
   vector<string> newfilelines;

   this->open_BetFile("bets.csv");
   this->reset_pos();
   while(this->get_line(line)){
      if (parent_node_id == (bets_get_node_id(line)).substr(0, (bets_get_node_id(line)).find_last_of(".")+1)){
         newfilelines.push_back(line.replace(find_vertical_bar(4, line)+1, 1, "-"));
         if (bets_get_node_id(line) == node_id){
            double full_amount = performance*bets_get_stake(line);
            double user_amount = full_amount*0.95;
            double system_profit = full_amount*0.05;
            string user_id = bets_get_user_id(line);
            string user_line = users.get_user_line(user_id);
            double balance = get_balance(user_line);
            string new_balance = double_to_string(balance-user_amount);
            users.change_balance(user_id, new_balance);
         }
      }
      else {
         newfilelines.push_back(line);
      }
   }

   this->close_BetFile();
   remove("bets.csv");
   bets.open ("bets.csv", ios::in | ios::out | ios::trunc);
   for (i = 0; i < newfilelines.size(); i++){
      bets << newfilelines[i] << "\n";
   }
   this->close_BetFile();
}

void BetFile::open_BetFile(const char* filename){
   bets.open(filename, ios::in | ios::out);
   if (!bets.is_open()){
      cout << "Error (File " << filename << " failed to open)" << endl;
   }
}            

void BetFile::close_BetFile(){
   bets.close();
}