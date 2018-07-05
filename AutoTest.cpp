/*#define DATASIZE (1000000)
#include "BPlusTree.h"
#include "functions.h"
#include<time.h>
#include<iomanip>
using namespace std;

int main() {

	Database Btree;
	Btree.refresh();
	clock_t start, finish,finish2,deleted,modified,searched;
	double totaltime;
	start = clock();

	for (int i = 1; i <= DATASIZE; i++) {
		string s = to_string(i);
		Btree.insert(s, s);
		if (i % 10000 == 0) cout << i << endl;
	}
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout.precision(6);
	Btree.save();
	finish2 = clock();
	double savetime = (double)(finish2 - start) / CLOCKS_PER_SEC;
	cout << "Insert consumed:" << savetime << "s." << endl << endl;
	cin.get();
	for (int i = 1; i <=  DATASIZE; i++) {
		string s = to_string(i);
		Btree.modify(s, "lalala");
		if (i % 10000 == 0) cout << Btree.modify(s,"lalala")<<i << endl;
	}
	Btree.save();
	modified = clock();
	cout << "Modified consumed:" << (double)(modified - finish2) / CLOCKS_PER_SEC << "s." << endl << endl;
	for (int i = 1; i <= DATASIZE; i++) {
		string s = to_string(i);
		Btree.search(s);
	}
	searched = clock();
	cout << "Search consumed:" << (double)(searched - modified) / CLOCKS_PER_SEC << "s." << endl << endl;
	
	for (int i = 1; i <= DATASIZE; i++) {
		string s = to_string(i);
		Btree.del(s);
	}
	Btree.save();
	deleted = clock();
	cout << "Del consumed:" << (double)(deleted -searched) / CLOCKS_PER_SEC << "s." << endl << endl;
	
	cin.get();
	return 0;
}*/