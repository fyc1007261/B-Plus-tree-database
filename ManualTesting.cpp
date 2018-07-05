#include "BPlusTree.h"
#include "functions.h"
#include<time.h>
#include<iomanip>
using namespace std;

int main() {
	Database Btree;
	cout << "Database by CaiYifan, Class 3, SE, SJTU." << endl;
	bool check = Btree.checkFile();
	if (!check) {
		//the category is wrong.
		cin.get();
		return 0;
	}
	while (true) {
		cout << "----------------------------------------------------------------------" << endl;
		cout << "Please input a number to choose what to do." << endl;
		cout << "1. create a new database. Be careful, it may cover the old one." << endl;
		cout << "2. Insert data." << endl;
		cout << "3. delete data." << endl;
		cout << "4. Fetch data." << endl;
		cout << "5. Modify data." << endl;
		cout << "6. Save." << endl;
		cout << "7. Quit." << endl;
		string s;
		cin >> s;
		if (s == "1") {
			Btree.refresh();
			cout << "Success." << endl;
			continue;
		}
		else if (s == "2") {
			cout << "Key: ";
			string index;
			cin >> index;
			cout << endl << "Value: ";
			string value;
			cin >> value;
			cout << endl << Btree.insert(index, value) << endl;
		}
		else if (s == "3") {
			cout << "Key: ";
			string index;
			cin >> index;
			cout << endl << Btree.del(index) << endl;
		}
		else if (s == "4") {
			Btree.save();
			cout << "Key: ";
			string index;
			cin >> index;
			cout << endl << Btree.search(index) << endl;
		}
		else if (s == "5") {
			cout << "Key: ";
			string index;
			cin >> index;
			cout << endl << "Value: ";
			string value;
			cin >> value;
			cout << endl << Btree.modify(index, value) << endl;
		}
		else if (s == "6") {
			Btree.save();
		}
		else if (s == "7") {
			break;
		}
		else cout << "Invalid Input!" << endl;

	}
	return 0;
}