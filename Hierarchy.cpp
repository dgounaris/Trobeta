#include "Hierarchy.h"
#include "HierarchyLibraries.h"

using namespace std;


Hierarchy::Hierarchy(string filename): isDecimalView(true) {
	this->homenode = new Node("Home", NULL);
	this->activenode = this->homenode;
	string mystr;
	int token;
	bool isHidden;
	bool isVoided;
	bool isWinning;
	bool isLosing;
	Node* tempptr = this->activenode;
	ifstream hfile;
	hfile.open(filename.c_str(), ios::in);
	string temp;
	string myname;
	while (getline(hfile, mystr)) {
		std::size_t myRpos;
		myRpos = mystr.find("\r");
		if (myRpos != string::npos) {
			mystr = mystr.substr(0, myRpos);
		}
		isHidden = false;
		isVoided = false;
		isWinning = false;
		isLosing = false;
		//create a substring with only the node path and set start to home
		temp = mystr.substr(0, mystr.find(" "));
		std::size_t closebracket;
		while ((closebracket = temp.find("}")) != string::npos) {
			string propcheck = temp.substr(0, closebracket);
			temp = temp.substr(closebracket+1);
			if (propcheck.at(closebracket-1) == 'H') {
				isHidden = true;
			}
			else if (propcheck.at(closebracket-1) == 'V') {
				isVoided = true;
			}
			else if (propcheck.at(closebracket-1) == 'W') {
				isWinning = true;
			}
			else if (propcheck.at(closebracket-1) == 'L') {
				isLosing = true;
			}
		}
		myname = mystr.substr(mystr.find(" ")+1, mystr.size()-mystr.find(" ")-1); //capturing the rest of the string ignoring the newline
		this->activenode = this->homenode;
		//add the node after taking the correct path
		istringstream countnumbers(temp);
		int numcount = 0;
		while (countnumbers >> token) {
			numcount++;
			if (this->activenode->childrenSize() >= token) { //reads existing path, doesnt create yet
				this->activenode = this->activenode->getChild(token);
			}
			else { //needs to create
				switch (numcount) {
					case 1:
					case 2:
						this->activenode->AddChild(myname);
						break;
					case 3:
						{
							std::size_t found = myname.find(" - ");
							if (found==string::npos) { //didnt find what we seeked for, aka no time/day
								this->activenode->AddChild(myname);
								break;
							}
							string timeday = myname.substr(myname.find(" - ") + 3);
							myname = myname.substr(0, found);
							string myday = timeday.substr(0, timeday.find(" "));
							string mytime = timeday.substr(timeday.find(" ") + 1);
							this->activenode->AddChild(myname, mytime, myday);
						}
						break;
					case 4:
						this->activenode->AddChild(myname);
						break;
					case 5:
						string valuestr = myname.substr(myname.find("#")+1); //isolates value
						double myvalue = 1 + (double)(atoi(valuestr.substr(0, valuestr.find("/")).c_str()))/(double)(atoi(valuestr.substr(valuestr.find("/")+1).c_str()));
						this->activenode->AddChild(myname.substr(0, myname.find("#")), myvalue);
						break;
				}
				//activenode is still set to the parent of the new child
				if (isHidden) {
					this->activenode->switchChildVisibility(this->activenode->childrenSize());
				}
				if (isVoided) {
					this->activenode->switchChildVoiding(this->activenode->childrenSize());
				}
				this->activenode->getChild(this->activenode->childrenSize())->setWinning(isWinning);
				this->activenode->getChild(this->activenode->childrenSize())->setLosing(isLosing);
			}
			char mydot;
			countnumbers >> mydot;
		}
	}
	hfile.close();
	this->activenode = this->homenode;
	this->isDecimalView = true;
}

Hierarchy::~Hierarchy() {
	delete this->homenode;
}

void Hierarchy::Save(string filename) {
	Node* temp = this->homenode;
	ofstream hfile;
	hfile.open(filename.c_str(), ios::out | ios::trunc);
	int i, j, k, m, n;
	//Yes, I tried using nested loops to redirect the pointer. No, it wasn't very successful
	for (i=1;i<=temp->childrenSize();i++) { // each category
		if (!temp->getChild(i)->getVisibility()) hfile << "{H}";
		if (temp->getChild(i)->getVoidStatus()) hfile << "{V}";
		if (temp->getChild(i)->getWinning()) hfile << "{W}";
		if (temp->getChild(i)->getLosing()) hfile << "{L}";
		hfile << i << ". " << temp->getChild(i)->printSelf(false) << "\n";
		for (j=1;j<=temp->getChild(i)->childrenSize();j++) { // each subcategory
			if (!temp->getChild(i)->getChild(j)->getVisibility()) hfile << "{H}";
			if (temp->getChild(i)->getChild(j)->getVoidStatus()) hfile << "{V}";
			if (temp->getChild(i)->getChild(j)->getWinning()) hfile << "{W}";
			if (temp->getChild(i)->getChild(j)->getLosing()) hfile << "{L}";
			hfile << i << "." << j << " " << temp->getChild(i)->getChild(j)->printSelf(false) << "\n";
			for (k=1;k<=temp->getChild(i)->getChild(j)->childrenSize();k++) { //each event
				if (!temp->getChild(i)->getChild(j)->getChild(k)->getVisibility()) hfile << "{H}";
				if (temp->getChild(i)->getChild(j)->getChild(k)->getVoidStatus()) hfile << "{V}";
				if (temp->getChild(i)->getChild(j)->getChild(k)->getWinning()) hfile << "{W}";
				if (temp->getChild(i)->getChild(j)->getChild(k)->getLosing()) hfile << "{L}";
				hfile << i << "." << j << "." << k << " " << temp->getChild(i)->getChild(j)->getChild(k)->printSelf(false) << "\n";
				for (m=1;m<=temp->getChild(i)->getChild(j)->getChild(k)->childrenSize();m++) { // each market
					if (!temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->getVisibility()) hfile << "{H}";
					if (temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->getVoidStatus()) hfile << "{V}";
					if (temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->getWinning()) hfile << "{W}";
					if (temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->getLosing()) hfile << "{L}";
					hfile << i << "." << j << "." << k << "." << m << " " << temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->printSelf(false) << "\n";
					for (n=1;n<=temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->childrenSize();n++) {
						if (!temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->getChild(n)->getVisibility()) hfile << "{H}";
						if (temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->getChild(n)->getVoidStatus()) hfile << "{V}";
						if (temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->getChild(n)->getWinning()) hfile << "{W}";
						if (temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->getChild(n)->getLosing()) hfile << "{L}";
						hfile << i << "." << j << "." << k << "." << m << "." << n << " " << temp->getChild(i)->getChild(j)->getChild(k)->getChild(m)->getChild(n)->printSelf(false) << "\n";
					}
				}
			}
		}
	}
	hfile.close();
	cout << "Hierarchy was saved successfully" << endl;
}

string Hierarchy::getActiveNodePath() {
	Node* temp = this->activenode;
	string mPath = "";
	while (temp != this->homenode) {
		mPath.insert(0, "." + temp->getId());
		temp = temp->getParent();
	}
	if (mPath.size() >= 1)
		mPath = mPath.substr(1); //removing the dot in front
	return mPath;
}

string Hierarchy::getChildNodePath(int index) {
	Node* temp = this->activenode->getChild(index);
	string mPath = "";
	while (temp != this->homenode) {
		mPath.insert(0, "." + temp->getId());
		temp = temp->getParent();
	}
	mPath = mPath.substr(1); //removing the dot in front
	return mPath;
}

bool Hierarchy::toVisibleChild(int index) {
	if (this->activenode->childrenSize() < index || index < 1) {
		cout << "--- Invalid Index ---" << endl << endl;
		return false;
	}
	if (!this->activenode->getChild(index)->getVisibility()) {
		cout << "--- Invalid Index ---" << endl << endl;
		return false;
	}
	Node* temp = this->activenode->toChild(index);
	if (temp != NULL)
		this->activenode = temp;
                return true;
        return false;
}

void Hierarchy::toChild(int index) {
	if (this->activenode->childrenSize() < index || index < 1) {
		cout << "--- Invalid Index ---" << endl << endl;
		return;
	}
	Node* temp = this->activenode->toChild(index);
	if (temp != NULL)
		this->activenode = temp;
}

void Hierarchy::toHome() {
	this->activenode = this->homenode;
}

void Hierarchy::Rename() {
	int index;
	string myline;
	char* p;
	cout << "Insert the index of the node you want to rename: ";
	getline(cin, myline); //eating up endl too
	if (!this->isValidName(myline)) {
		cout << "Operation canceled" << endl;
		return;
	}
	index = strtol(myline.c_str(), &p, 10);
	if (*p != 0 || this->activenode->childrenSize() < index || index < 1) {
		cout << "--- Invalid Index ---" << endl << endl;
		return;
	}
	cout << "Insert a name: ";
	getline(cin, myline);
	if (this->isValidName(myline)) {
		this->activenode->getChild(index)->setName(myline);
	}
	else {
		cout << "Operation canceled" << endl;
		return;
	}
}

void Hierarchy::New() {
	string newname;
	cout << "Insert a name: ";
	getline(cin, newname);
	if (this->isValidName(newname)) {
		this->activenode->AddChild(newname);
	}
	else {
		cout << "Operation canceled" << endl;
		return;
	}
}

void Hierarchy::Delete() {
	int index;
	string myline;
	cout << "Insert the index of the node you want to delete: ";
	getline(cin, myline); //eating up endl too
	if (!this->isValidName(myline)) {
		cout << "Operation canceled" << endl;
		return;
	}
	char* p;
	index = strtol(myline.c_str(), &p, 10);
	if (*p != 0 || this->activenode->childrenSize() < index || index < 1) {
		cout << "--- Invalid Index ---" << endl << endl;
		return;
	}
	while (true) {
		cout << "Are you sure you want to delete this node? (Y/N)" << endl;
		getline(cin, myline);
		if (myline.compare("y") == 0 || myline.compare("Y") == 0) {
			this->activenode->DeleteChild(index);
			return;
		}
		else if (myline.compare("n") == 0 || myline.compare("N") == 0 || !this->isValidName(myline)) {
			cout << "Operation canceled" << endl;
			return;
		}
		else {
			cout << "Illegal command" << endl;
		}
	}
}

void Hierarchy::Copy() {
	int index;
	string myline;
	cout << "Insert the index of the node you want to copy: ";
	getline(cin, myline); //eating up endl too
	if (!this->isValidName(myline)) {
		cout << "Operation canceled" << endl;
		return;
	}
	char* p;
	index = strtol(myline.c_str(), &p, 10);
	if (*p != 0 || this->activenode->childrenSize() < index || index < 1) {
		cout << "--- Invalid Index ---" << endl << endl;
		return;
	}
	this->activenode->AddChild(activenode->getChild(index));
}

void Hierarchy::switchVisibility(int index) {
	if (((this->activenode->childrenSize()) < index) || (index < 1)) {
		cout << "--- Invalid Index ---" << endl << endl;
		return;
	}
	this->activenode->switchChildVisibility(index);
}

void Hierarchy::switchVoiding(int index) {
	if (this->activenode->childrenSize() < index || index < 1) {
		cout << "--- Invalid Index ---" << endl << endl;
		return;
	}
	if (!this->activenode->getChild(index)->isVoidable()) {
		cout << "--- Error: This node cannot be voided ---" << endl << endl;
	}
	this->activenode->switchChildVoiding(index);
}

void Hierarchy::switchPricesView() {
	this->isDecimalView = !this->isDecimalView;
}

string Hierarchy::getPricesView() {
	if (this->isDecimalView) {
		return "Decimal";
	}
	else {
		return "Fractional";
	}
}

double Hierarchy::getChildPrice(int index) {
	return this->activenode->getChild(index)->getPrice();
}

void Hierarchy::printPath() {
	this->activenode->printPath(this->isDecimalView);
	cout << endl;
}

void Hierarchy::printChildren() {
	int i=1;
	for (;i<=this->activenode->childrenSize();i++) {
		if (this->activenode->getChild(i)->getVisibility()) {
			cout << i << ". " << this->activenode->getChild(i)->printSelf(this->isDecimalView) << endl;
		}
	}
}

void Hierarchy::printChildrenAll() {
	int i=1;
	for (;i<=this->activenode->childrenSize();i++) {
		string childprint = this->activenode->getChild(i)->printSelf(this->isDecimalView);
		cout << i << ". " << childprint;
		if (!this->activenode->getChild(i)->getVisibility()) {
			cout << "[HIDDEN]";
		}
		cout << endl;
	}
}

bool Hierarchy::isValidName(string mystring) {
	int i=0;
	for (;i<mystring.size();i++) {
		if ((int) (mystring[i]) >= 'A' && (int) (mystring[i]) <= 'Z') {
			mystring[i] = std::tolower(mystring[i]);
		}
	}
	if (mystring.compare("cancel") == 0 ||
		mystring.compare("abort") == 0 ||
		mystring.compare("c") == 0 ||
		mystring.compare("a") == 0) {
			return false;
		}
	return true;
}

void Hierarchy::settleBet(int index) {
	int i=1;
	if (!this->activenode->getChild(index)->isSettlable()) {
		cout << "--- Error: This node cannot be settled as winning ---" << endl << endl;
	}
	if (this->activenode->getChild(index)->getWinning()) {
		for (;i<=this->activenode->childrenSize();i++) {
			this->activenode->getChild(i)->setLosing(false);
			this->activenode->getChild(i)->setWinning(false);
		}
		return;
	}
	for (;i<=this->activenode->childrenSize();i++) {
		this->activenode->getChild(i)->setLosing(true);
		this->activenode->getChild(i)->setWinning(false);
	}
	this->activenode->getChild(index)->setLosing(false);
	this->activenode->getChild(index)->setWinning(true);
}

bool Hierarchy::hasBettableChildren() {
	int i=1;
	for (;i<=this->activenode->childrenSize();i++) {
		if (this->activenode->getChild(i)->isBettable()) {
			return true;
		}
	}
	return false;
}

bool Hierarchy::hasVoidableChildren() {
	int i=1;
	for (;i<=this->activenode->childrenSize();i++) {
		if (this->activenode->getChild(i)->isVoidable()) {
			return true;
		}
	}
	return false;
}

bool Hierarchy::hasSettlableChildren() {
	int i=1;
	for (;i<=this->activenode->childrenSize();i++) {
		if (this->activenode->getChild(i)->isSettlable()) {
			return true;
		}
	}
	return false;
}

bool Hierarchy::isChildWinning(int index) {
	return this->activenode->getChild(index)->getWinning();
}

bool Hierarchy::isChildLosing(int index) {
	return this->activenode->getChild(index)->getLosing();
}

bool Hierarchy::isChildVoided(int index) {
	return this->activenode->getChild(index)->getVoidStatus();
}

bool Hierarchy::isChildBettable(int index) {
	if (this->activenode->getChild(index)->isBettable()) {
		return true;
	}
	cout << "--- Error: This node cannot be betted on ---" << endl << endl;
	return false;
}

Node::Node(string newname, Node* newparent): name(newname), parent(newparent), isVisible(true) {
	if (this->parent != NULL)
		this->id = this->parent->childrenSize() + 1;
}

Node::Node(Node& n): name(n.name + " - Copy"), parent(n.parent), isVisible(false) {
	if (this->parent != NULL)
		this->id = this->parent->childrenSize() + 1;
}

Node::~Node() {
	int i=1;
	for (;i<=this->childrenSize();i++) {
		delete this->getChild(i);
	}
}

string Node::getId() {
	ostringstream oss;
	oss << this->id;
	return oss.str();
}

int Node::childrenSize() {
	return this->mychildren.size();
}

Node* Node::getChild(int index) {
	return this->mychildren[index-1];
}

Node* Node::toChild(int index) {
	return this->mychildren[index-1];
}

Node* Node::getParent() {
	return this->parent;
}

string Node::getName() {
	string toreturn = this->name;
	return toreturn;
}

void Node::setName(string newname) {
	this->name = newname;
}

bool Node::AddChild(string s) {
	this->mychildren.push_back(new Category(s, this));
	return true;
}

bool Node::AddChild(Node* n) {
	Category* mycopy = new Category(*(Category*)n);
	this->mychildren.push_back(mycopy);
	return true;
}

bool Node::AddChild(string s, double d) {
	return false;
}

bool Node::AddChild(string s, string ss, string sss) {
	return false;
}

void Node::printPath(bool pview) {
	if (this->parent != NULL) {
		this->parent->printPath(pview);
	}
	cout << this->printSelf(pview) << "/";
}

string Node::printSelf(bool pview) {
	return this->getName();
}

void Node::DeleteChild(int index) {
	delete this->getChild(index);
	this->mychildren.erase(mychildren.begin() + index - 1);
}

void Node::switchChildVisibility(int index) {
	this->mychildren[index-1]->switchVisibility();
}

void Node::switchVisibility() {
	this->isVisible = !this->isVisible;
}

void Node::switchChildVoiding(int index) {
	this->mychildren[index-1]->switchVoiding();
}

void Node::switchVoiding() {
	return;
}

bool Node::getVisibility() {
	return this->isVisible;
}

bool Node::getVoidStatus() {
	return false;
}

double Node::getPrice() {
	return -1.0;
}

void Node::setWinning(bool status) {
	return;
}

void Node::setLosing(bool status) {
	return;
}

bool Node::getWinning() {
	return false;
}

bool Node::getLosing() {
	return false;
}

bool Node::isBettable() {
	return false;
}

bool Node::isVoidable() {
	return false;
}

bool Node::isSettlable() {
	return false;
}

Category::Category(string newname, Node* newparent): Node(newname, newparent) {

}

Category::Category(Category& c): Node(c) {
	int i=0;
	for (;i<c.mychildren.size();i++) {
		this->mychildren.push_back(new SubCategory(*(SubCategory*)(c.mychildren[i])));
	}
}

bool Category::AddChild(string newname) {
	this->mychildren.push_back(new SubCategory(newname, this));
	return true;
}

bool Category::AddChild(Node* n) {
	SubCategory* mycopy = new SubCategory(*(SubCategory*)n);
	this->mychildren.push_back(mycopy);
	return true;
}

SubCategory::SubCategory(string newname, Node* newparent): Node(newname, newparent) {

}

SubCategory::SubCategory(SubCategory& c): Node(c) {
	int i=0;
	for (;i<c.mychildren.size();i++) {
		this->mychildren.push_back(new Event(*(Event*)(c.mychildren[i])));
	}
}

bool SubCategory::AddChild(string newname) {
	this->mychildren.push_back(new Event(newname, this));
	return true;
}

bool SubCategory::AddChild(string newname, string time, string day) {
	this->mychildren.push_back(new Event(newname, this, time, day));
	return true;
}

bool SubCategory::AddChild(Node* n) {
	Event* mycopy = new Event(*(Event*)n);
	this->mychildren.push_back(mycopy);
	return true;
}

Event::Event(string newname, Node* newparent): Node(newname, newparent), hasTime(false) {}

Event::Event(string newname, Node* newparent, string time, string day): Node(newname, newparent), time(time), day(day), hasTime(true) {}

Event::Event(Event& c): Node(c) {
	this->time = c.time;
	this->day = c.day;
	int i=0;
	for (;i<c.mychildren.size();i++) {
		this->mychildren.push_back(new Market(*(Market*)(c.mychildren[i])));
	}
}

bool Event::AddChild(string newname) {
	this->mychildren.push_back(new Market(newname, this));
	return true;
}

bool Event::AddChild(Node* n) {
	Market* mycopy = new Market(*(Market*)n);
	this->mychildren.push_back(mycopy);
	return true;
}

string Event::printSelf(bool pview) {
	string str = this->getName() + this->printTime();
	return str;
}

string Event::printTime() {
	string str = "";
	if (hasTime) {
		str += " - ";
		str += day;
		str += " ";
		str += time;
	}
	return str;
}

Market::Market(string newname, Node* newparent): Node(newname, newparent) {}

Market::Market(Market& c): Node(c) {
	int i=0;
	for (;i<c.mychildren.size();i++) {
		this->mychildren.push_back(new Selection(*(Selection*)(c.mychildren[i])));
	}
}

bool Market::AddChild(string newname, double newprice) {
	this->mychildren.push_back(new Selection(newname, this, newprice));
	return true;
}

bool Market::AddChild(Node* n) {
	Selection* mycopy = new Selection(*(Selection*)n);
	this->mychildren.push_back(mycopy);
	return true;
}

bool Market::AddChild(string s) {
	this->mychildren.push_back(new Selection(s, this, 0));
	return true;
}

Node* Market::toChild(int index) {
	return this; //can't see below selections
}

Selection::Selection(string newname, Node* newparent, double newprice):
Node(newname, newparent), price(newprice), isVoided(false), isWinning(false), isLosing(false) {}

Selection::Selection(Selection& c): Node(c), price(c.price), isVoided(c.isVoided), isWinning(c.isWinning), isLosing(c.isLosing) {}

bool Selection::AddChild(Node* n) { //selection has no children
	return false;
}

void Selection::switchVoiding() {
	this->isVoided = !this->isVoided;
}

bool Selection::getVoidStatus() {
	return this->isVoided;
}

string Selection::printSelf(bool pview) {
	string str = this->getName() + "#" + this->printPrices(pview);
	if (this->isWinning) {
		str += " {W}";
	}
	if (this->isLosing) {
		str += " {L}";
	}
	if (this->isVoided) {
		str += " {V}";
	}
	return str;
}

string Selection::printPrices(bool pview) {
	ostringstream strs;
	if (pview) {
		strs << this->price;
		return strs.str();
	}
	else {
		strs << (int)((this->price - 1)*100) << "/" << 100;
		return strs.str();
	}
}

double Selection::getPrice() {
	return this->price;
}

void Selection::setWinning(bool status) {
	this->isWinning=status;
}

void Selection::setLosing(bool status) {
	this->isLosing=status;
}

bool Selection::getWinning() {
	return this->isWinning;
}

bool Selection::getLosing() {
	return this->isLosing;
}

bool Selection::isBettable() {
	if (this->getWinning() || this->getLosing() || this->getVoidStatus()) {
		return false;
	}
	return true;
}

bool Selection::isVoidable() {
	if (this->getWinning() || this->getLosing()) {
		return false;
	}
	return true;
}

bool Selection::isSettlable() {
	if (this->getVoidStatus()) {
		return false;
	}
	return true;
}
