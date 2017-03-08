#include "UserLibraries.h"
#include <vector>
#include "Users.h"

using namespace std;

string bets_get_bet_id(string&);
string bets_get_user_id(string&);
string bets_get_node_id(string&);
double bets_get_stake(string&);
string bets_get_stake_str(string&);
string bets_get_result(string&);
string double_to_string(double);
int find_vertical_bar(int, string&);
string bets_get_bet_id(string&);
string get_user_id(string&);
string get_username(string&);
string get_fullname(string&);
string get_password(string&);
string get_type(string&);
string get_status(string&);
double get_balance(string&);
string get_balance_str(string&);
vector<double> get_freebets(string&);
string get_freebets_str(string&);
string increase_string_num(string&);
bool Registration(UserFile&, User* &);
bool Login(UserFile&, User* &);
void NodeNavigationScreen(Hierarchy&, User*);
