#include "UserLibraries.h"
#include "Hierarchy.h"
#include "Bets.h"

using namespace std;

#ifndef USERS_H
#define USERS_H

class Hierarchy;
class BetFile;
class UserFile;
class Wallet;
class User;
class Guest;
class Punter;
class Trader;
class Director;


class UserFile {
    public:
            /* Constructor */
            UserFile(const char*);
            ~UserFile();

            istream& get_line(string&);
            string get_last_line();
            string get_user_line(string&);
            string get_username_line(string&);

            void reset_pos();
            void set_pos_to_end();

            void print_users();

            void add_punter(User*);
            bool username_exists(string&);
            void lock_unlock_user();
            void change_username(string&, string&, string&);
            void change_password(string&, string&, string&);
            void change_balance(string&, string&);
            void change_status_lock(string&, string&);
	    void change_status_activate(string&);
            void change_freebets(string&, string&);

            void open_UserFile(const char*);
            void close_UserFile();

    private:
            fstream users;
};

class Wallet {
    public:
            Wallet(double, vector<double>);
            double get_balance();
            vector<double> get_freebets();
            void print_freebets();
            void increase_balance(double);
            void decrease_balance(double);

    private:
            double balance;
            vector<double> freebets;
};

class User {
	public:
            /* Constructor */
            User();
            User(string, string, string, string);

            /* Accessors */
            string get_user_id();
            string get_username();
            string get_fullname();
            string get_password();
            virtual double get_balance();

            /* Mutators */
            void set_user_id(string&);
            void set_username(string&);
            void set_fullname(string&);
            void set_password(string&);

            /* Printing */
            virtual void print_operations(Hierarchy&);
            virtual void print_nodes(Hierarchy&);
            virtual void operation(string&, Hierarchy&, UserFile&, BetFile&);

	private:
            string user_id;
            string username;
            string password;
            string fullname;
};

class Guest: public User {
	public:
            /* Constructors */
            Guest();
            Guest(string, string, string, string);

            /* Printing */
            virtual void print_operations(Hierarchy&);

            void print_nodes(Hierarchy&);

            /* Execute Operations */
            virtual void operation(string&, Hierarchy&, UserFile&, BetFile&);
};

class Punter: public Guest{
	public:
            /* Constructor */
            Punter(string, string, string, string, double, vector<double>);

            double get_balance();
            vector<double> get_freebets();

            /* Printing */
            virtual void print_operations(Hierarchy&);

            void print_nodes(Hierarchy&);

            /* Execute Operations */
            virtual void operation(string&, Hierarchy&, UserFile&, BetFile&);

            /* Profil Management */
            void profil_operation(string&, UserFile&, BetFile&);
            void edit_profil(UserFile&, BetFile&);

        private:
            string status;
            Wallet wallet;
};

class Trader: public Guest{
	public:
            /* Constructor */
            Trader(string, string, string, string);

            /* Printing */
            virtual void print_operations(Hierarchy&);
            void print_nodes(Hierarchy&);

            /* Execute Operations */
            virtual void operation(string&, Hierarchy&, UserFile&, BetFile&);
};

class Director: public Trader{
 	public:
            /* Constructor */
            Director(string, string, string, string);

            /* Printing */
            virtual void print_operations(Hierarchy&);
            void print_nodes(Hierarchy&);

            /* Execute Operations */
            virtual void operation(string&, Hierarchy&, UserFile&, BetFile&);

            /* Users Management */
            void users_operation(string&, UserFile&);
            void edit_users(UserFile&);
};

#endif
