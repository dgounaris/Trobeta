#include "UserLibraries.h"

using namespace std;

#ifndef BETS_H
#define BETS_H

class Hierarchy;
class BetFile;
class UserFile;
class Wallet;
class User;
class Guest;
class Punter;
class Trader;
class Director;

class BetFile {
    public:
            /* Constructor */
            BetFile(const char*);
            BetFile();
            ~BetFile();

            istream& get_line(string&);
            string get_last_line();

            void reset_pos();
            void set_pos_to_end();

            void print_bet_history(string&);
            void print_bet_history();

            void add_bet(string&, string&, string&);
            void change_bet_voiding(string&, UserFile&);
            void change_bet_unvoiding(string&, UserFile&);
            void change_bet_winning(string&, UserFile&, double);
            void change_bet_unwinning(string&, UserFile&, double);

            void open_BetFile(const char*);
            void close_BetFile();

    private:
            fstream bets;
};

#endif
