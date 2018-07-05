#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include"functions.h"
#define M (8)
#define SPACEFORINDEX (1000)
#define SPACEFORDATA (51)
#define MAXMAPSIZE (5000)


using namespace std;

struct DataNode {
	int position;//using positive.
	string value;
	DataNode(int p, string d) : position(p), value(d) {};
};


struct InNode{
	int lastPos, nowPos;
	vector<string> keys;
	vector<int> position;
	bool fill(vector<string> &vec,string val, int &index);
	InNode();
	InNode(fstream &indexFile, int pos);
	InNode(fstream &indexFile);
	~InNode();
	void changeInfo(fstream &indexFile, int pos);
	int findPos(string key);
	void clear(fstream &indexFile);
	bool isFull();
	bool isExtraFull();
	void updateIndex();
	void updateIndexFile(fstream &indexFile);
	bool isLeaf();
	bool isRoot();

};


class Database {
private:
	int root;
	fstream indexFile;
	fstream dataFile;
	void updateMax(int);
	void splitFull(int index, InNode &fullNode);
	void changeRoot(int index);
	InNode split(int index, InNode &oldNode);
	void clearMemory();
	int updateData(const string &);
	int maxPointer = 0;
	int maxDataPointer = 0;
public:
	Database();
	~Database();
	bool checkFile();
	string insert(const string &key, const string &value);
	string search(const string &key);
	string del(string &key);
	string modify(const string &key, const string &value);
	bool refresh();
	void save();




};



template <typename T> int findInVector(vector<T> &vec, T key);
template <typename T> void delInVector(vector<T> &vec, T key);
template <typename T> void insertInVector(vector<T> &vec, T key);
