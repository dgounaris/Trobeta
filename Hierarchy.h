#include "HierarchyLibraries.h"

using namespace std;

#ifndef HIERARCHY_H
#define HIERARCHY_H


class Hierarchy {
	public:
		Hierarchy(string);		//creates hierarchy using the file with the string name
		~Hierarchy();
		string getActiveNodePath();	//returns the node path of active node
		string getChildNodePath(int);	//returns the node path of specific child
		void Save(string);		//saves hierarchy to the file name specified
		bool toVisibleChild(int); //moves activenode to the child with specified index, IGNORES HIDDEN NODES
		void toChild(int);		//moves activenode to the child with specified index
		void toHome();			//moves activenode to homenode
		void Rename();			//handles renaming a child of activenode
		void New();			//handles creating a new child of activenode
		void Copy();			//handles copying an existing child of activenode
		void Delete();			//handles deleting an existing child of activenode
		void switchVisibility(int);	//handles switching visibility an existing child of activenode
		void switchVoiding(int);		//handles switching voiding of an existing child of activenode
		void switchPricesView();	//switches the prices view
		string getPricesView();		//prints the view selection of prices
		double getChildPrice(int);	//returns the price of selected child
		void printPath();		//prints path to activenode
		void printChildren();		//prints visible children of activenode
		void printChildrenAll();	//prints hidden and visible children of activenode
		bool isValidName(string);	//checks if the string given can be used as a name
		void settleBet(int);	        //settles the bet, with the winning selection being the one specified. if the bet is already winning then it resets the settling
		bool hasBettableChildren();	//checks if the active node has children on which bets can be placed
		bool hasVoidableChildren();	//checks if the active node has children that can be voided
		bool hasSettlableChildren();	//checks if the active node has children that can be settled
		bool isChildWinning(int);	//checks if the specified child is winning
		bool isChildLosing(int);	//checks if the specified child is losing
		bool isChildVoided(int);	//checks if the specified child is voided
		bool isChildBettable(int);	//checks if the specified child can be betted upon

	private:
		class Node* activenode;
		class Node* homenode;
		bool isDecimalView;
};

class Node {
	friend class Hierarchy;

	protected:
		Node(string, Node*);
		Node(Node&);
		~Node();				//no need for explicit destructors in each of the subclasses, since the only dynamically created variables/objects in all node subclasses are the data in mychildren vector
		string getId();				//returns the id of the node AS A STRING
		int childrenSize();			//returns the size of mychildren vector
		Node* getChild(int);			//returns the element with specified index of the mychildren vector
		virtual Node* toChild(int);		//returns the element with specified index of the mychildren vector. this should be used for moving through indexes instead of getChild.
		Node* getParent();			//returns the parent object. is NULL for categories
		string getName();			//returns the name of the node //BUGGY AS FUCK//
		void setName(string);			//sets name to the string specified
		virtual bool AddChild(string);
		virtual bool AddChild(Node*);
		virtual bool AddChild(string, double);
		virtual bool AddChild(string, string, string);	//AddChild MUST be overrided by inherited classes
		virtual void printPath(bool);		//prints the path
		virtual string printSelf(bool);		//prints whatever is neseccary for this node
		void DeleteChild(int);			//erases the child with given index
		void CopyChild(int);			//copies the child with given index
		void switchChildVisibility(int);	//switches visibility of specified child
		void switchVisibility();		//switches visibility of current node
		virtual void switchChildVoiding(int);		//switches voiding of specified child
		virtual void switchVoiding();			//switches voiding of current node
		virtual bool getVisibility();			//returns the visibility status of current node
		virtual bool getVoidStatus();			//returns the voiding status of current node
		virtual double getPrice();		//returns -1, is overrided in selection to return actual price
		virtual void setWinning(bool);	//sets the winning status of the node. Default behavior ignores the input, as there is no winning state
		virtual void setLosing(bool);	//sets the losing status of the node. Default behavior ignores the input, as there is no losing state
		virtual bool getWinning();	//returns true if the node can have winning status AND is set as winning, false otherwise
		virtual bool getLosing();	//returns true if the node can have losing status AND is set as losing, false otherwise
		virtual bool isBettable();	//checks if the node can be betted on
		virtual bool isVoidable();	//checks if the node can be voided
		virtual bool isSettlable();	//checks if the node can be settled

		string name;
		Node* parent;
		vector<Node*> mychildren;
		bool isVisible;
		int id;

	private:
};

class Category: public Node {
	friend class Hierarchy;

	public:
		Category(string, Node*);
		Category(Category&);

	protected:
		virtual bool AddChild(string);
		virtual bool AddChild(Node*);

	private:
};

class SubCategory: public Node {
	friend class Hierarchy;

	public:
		SubCategory(string, Node*);
		SubCategory(SubCategory&);

	protected:
		virtual bool AddChild(string);
		virtual bool AddChild(string, string, string);
		virtual bool AddChild(Node*);

	private:
};

class Event: public Node {
	friend class Hierarchy;

	public:
		Event(string, Node*);
		Event(string, Node*, string, string);
		Event(Event&);

	protected:
		virtual bool AddChild(string);
		virtual bool AddChild(Node*);
		virtual string printSelf(bool);

	private:
		string printTime();
		string time, day;
		bool hasTime;
};

class Market: public Node {
	friend class Hierarchy;

	public:
		Market(string, Node*);
		Market(Market&);
	protected:
		virtual bool AddChild(string, double);
		virtual bool AddChild(string);
		virtual bool AddChild(Node*);
		virtual Node* toChild(int);

	private:
};

class Selection: public Node {
	friend class Hierarchy;

	public:
		Selection(string, Node*, double);
		Selection(Selection&);

	protected:
		virtual string printSelf(bool);
		virtual bool AddChild(Node*);
		virtual double getPrice();
		virtual void switchVoiding();
		virtual bool getVoidStatus();
		virtual void setWinning(bool);
		virtual void setLosing(bool);
		virtual bool getWinning();
		virtual bool getLosing();
		virtual bool isBettable();
		virtual bool isVoidable();
		virtual bool isSettlable();

	private:
		string printPrices(bool);
		double price;
		bool isVoided;
		bool isWinning;
		bool isLosing;
};

#endif
