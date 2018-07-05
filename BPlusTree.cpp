
#include "BPlusTree.h"

using namespace std;


set<int> emptyIndex = {};
map<int,InNode> NodeMap = {};
vector<DataNode> DataList = {};

InNode::InNode(fstream &indexFile, int pos)
{
	nowPos = pos;
	if (NodeMap.find(pos) != NodeMap.end()) {
		keys = NodeMap[pos].keys;
		lastPos = NodeMap[pos].lastPos;
		position = NodeMap[pos].position;
		nowPos = NodeMap[pos].nowPos;
	}
	else {
		changeInfo(indexFile, pos);
		NodeMap[pos].nowPos = nowPos;
		NodeMap[pos].keys = keys; 
		NodeMap[pos].lastPos = lastPos;
		NodeMap[pos].position = position;
	}
}

InNode::InNode(fstream & indexFile)
{
	nowPos = 0;
	keys = {};
	position = {};
}

InNode::~InNode()
{
	return;
}

InNode::InNode() {
	nowPos = 0;
	keys = {};
	position = {};
}


bool InNode::fill(vector<string> &vec,string val , int &index) {
	if (vec[0] == "") vec.pop_back();
	if (vec.size() == 0) {
		vec.push_back(val); 
		index = 0;
		return true;
	}
	if (val > vec[vec.size() - 1]) {
		vec.push_back(val);
		index = vec.size() - 1;
		return true;
	}
	else if (val < vec[0]) {
		vec.insert(vec.begin(), val);
		index = 0;
		return true;
	}

	else {
		for (int i = 0; i < vec.size() - 1; i++) {
			if (val > vec[i] && val < vec[i + 1]) {
				vector<string>::iterator it = vec.begin();
				for (int j = 1; j <= i + 1; j++)
					it++;
				vec.insert(it, val);
				index = i + 1;
				return true;
			}
			if (val == vec[i + 1])
				return false;
		}
		insertInVector(vec, val);
	}
	return false;
}
void InNode::changeInfo(fstream &indexFile,int pos)
{
	indexFile.seekg(pos,indexFile.beg);
	string s_keys, s_pos;
	getline(indexFile, s_keys);
	getline(indexFile, s_pos);
	string s_last;
	indexFile >> s_last;
	lastPos = str_to_num(s_last);
	keys.clear();
	keys = split(s_keys, ' ');
	position = split(' ', s_pos);
	
}

int InNode::findPos(string key)
{
	//return 0 if not found.
	//return a negative number if a leaf node is found. (stored as negative in the file)
	bool leaf = false;
	int pos;
	int result;
	bool equ = false;//considering not found.
	if (keys.size() == 0) return 0;//not found.
	if (key < keys[0]) result = position[0];
	else if (key >= keys[keys.size() - 1]) { 
		if (key == keys[keys.size() - 1]) equ = true;
		result = position[position.size() - 1];
	}
	else
	{
		for (int i = 0; i < keys.size() - 1; i++) {
			if (key >= keys[i] && key < keys[i + 1]) {
				result = position[i + 1];
				if (key == keys[i])
					equ = true;
				break;
			}

		}
	}
	if (result < 0) {
		if (equ) return result;
		else return 0;
	}
	return result;
}

void InNode::clear(fstream & indexFile)
{
	indexFile.seekp(nowPos);
	string s = "";
	for (int i = 1; i <= SPACEFORINDEX; i++)
		s += " ";
	indexFile << s;
}

bool InNode::isFull()
{
	if (keys.size() == M) return true;
	return false;
}

bool InNode::isExtraFull()
{
	return (keys.size() > M);
}

void InNode::updateIndex()
{
	NodeMap[nowPos].nowPos = nowPos;
	NodeMap[nowPos].keys = keys;
	NodeMap[nowPos].lastPos = lastPos;
	NodeMap[nowPos].position = position;
}

void InNode::updateIndexFile(fstream & indexFile)
{
	string s="";
	indexFile.seekp(nowPos-1);
	s += "\n";
	if (keys.size() == 0) s += "\n";
	else {
		for (int i = 0; i < keys.size() - 1; i++) {
			s = s + keys[i] + " ";
		}
		s = s + keys[keys.size() - 1] + "\n";
	}
	for (int i = 0; i < position.size()-1; i++) {
		s = s + to_string(position[i]) + " ";
	}
	s = s + to_string(position[position.size() - 1]) + "\n" + to_string(lastPos);
	int size = s.size();
	for (int i = 1; i <= SPACEFORINDEX - size; i++) {
		s += " ";
	}
	s += "\n";
	indexFile << s;
}

bool InNode::isLeaf()
{
	for (int i = 0; i < position.size(); i++) {
		if (position[i] < 0) return true;
	}
	return false;
}

bool InNode::isRoot()
{
	return (lastPos == 0);
}

void Database::updateMax(int i = 0/*by default in the file*/)
{
	if (i == 0) {// in the file
		int now = indexFile.tellp();
		if (now >= maxPointer)
			maxPointer = now;
	}
	else if (i+1>maxPointer)
		maxPointer = i + 1;//in the RAM
}

void Database::splitFull(int index, InNode & fullNode)
{
	//create a new node.
	if (fullNode.isRoot()) {
		InNode newNode = split(index, fullNode);
		//newNode.updateIndexFile(indexFile);
		newNode.updateIndex();
		updateMax(newNode.nowPos);
		//create a new node.
		InNode newRoot(indexFile);
		newRoot.keys = { newNode.keys[0] };
		newRoot.position = { fullNode.nowPos,newNode.nowPos };
		newRoot.lastPos = 0;
		if (!newNode.isLeaf()) {
			newNode.position.erase(newNode.position.begin());
			newNode.keys.erase(newNode.keys.begin());
		}
		if (emptyIndex.size() != 0) {
			newRoot.nowPos = *(emptyIndex.begin());
			emptyIndex.erase(newRoot.nowPos);
		}
		else {
			int endpos = maxPointer;
			newRoot.nowPos = ((int)((endpos + 1) / SPACEFORINDEX) + 1) * SPACEFORINDEX;
		}
		fullNode.lastPos = newRoot.nowPos;
		newNode.lastPos = newRoot.nowPos;
		changeRoot(newRoot.nowPos);
		//newRoot.updateIndexFile(indexFile);
		newRoot.updateIndex();
		updateMax(newRoot.nowPos);
		//fullNode.updateIndexFile(indexFile);
 		fullNode.updateIndex();
		updateMax(fullNode.nowPos);
		//newNode.updateIndexFile(indexFile);
		newNode.updateIndex();
		updateMax(newNode.nowPos);
	}
	else {
		InNode newNode = split(index, fullNode);
		//add to the parent.
		InNode parent(indexFile, fullNode.lastPos);
		int id;
		parent.fill(parent.keys, (newNode.keys[0]), id);//change the 1st line of inside node.
		if (!newNode.isLeaf()) {
			newNode.position.erase(newNode.position.begin());
			newNode.keys.erase(newNode.keys.begin());
		}
		int i = 0;
		for (vector<int>::iterator it = parent.position.begin(); ; it++) {
			//change the 2nd line of inside node.
			if (id == i - 1) { parent.position.insert(it, newNode.nowPos); break; }
			i++;
		}
		fullNode.lastPos = parent.nowPos;
		newNode.lastPos = parent.nowPos;
		//parent.updateIndexFile(indexFile);
		parent.updateIndex();
		updateMax(parent.nowPos);
		//newNode.updateIndexFile(indexFile);
		newNode.updateIndex();
		updateMax(newNode.nowPos);
		//fullNode.updateIndexFile(indexFile);
		fullNode.updateIndex();
		updateMax(fullNode.nowPos);
		if (parent.isExtraFull())
			splitFull(id, parent);
	}
}

Database::Database()
{
	root = 0;
	indexFile.open("data/index.dat", ios_base::in | ios_base::out | ios_base::binary);
	indexFile.seekg(0);
	//find the entrance node of the tree.
	string s_root;
	indexFile >> s_root;
	root = str_to_num(s_root);
	indexFile.seekp(0,indexFile.end);
	dataFile.open("data/data.dat", ios_base::in | ios_base::out | ios_base::binary);
	dataFile.seekg(0);
}

Database::~Database()
{	
	save();
	indexFile.close();
	dataFile.close();
}

bool Database::checkFile()
{
	fstream indexFile, dataFile;
	indexFile.open("data/index.dat", ios::in | ios::out | ios::binary);
	dataFile.open("data/data.dat", ios::in | ios::out | ios::binary);
	bool result = (indexFile.is_open() && dataFile.is_open());
	indexFile.close();
	dataFile.close();
	if (!result) {
		indexFile.open("data/index.dat",  ios::out | ios::binary);
		dataFile.open("data/data.dat", ios::out | ios::binary);
		result = (indexFile.is_open() && dataFile.is_open());
		indexFile.close();
		dataFile.close();
	}
	if (!result) {
		cout << "Loading failed. Please check whether the 'data' category is in the right position." << endl;
	}
	return result;
}

void Database::changeRoot(int index)
{
	root = index;
	indexFile.seekp(0);
	string s = "";
	for (int i = 1; i <= SPACEFORINDEX-1; i++)
		s += " ";
	indexFile << s<<endl;
	indexFile.seekp(0);
	indexFile << index;
}

string Database::insert(const string &key, const string &value)
{
	//index less than 8.
	if (key.size() > 8) return "Sorry. Index should be no more than 8 digits.";
	if (key.find(" ")!=key.npos) return "Sorry. <Space> should not be included in the key.";
	if (value.size() >= SPACEFORDATA) return "Sorry. Value should be less than 50 digits.";
	int now = root;
	int last = 0;
	//if leaf node is found, the postion will be negative.
	while (now > 0) {
		InNode node(indexFile, now);
		last = now;
		now = node.findPos(key);
	}
	if (now != 0) return "Sorry. Index already exists!";

	//give each piece of data a 50 byte space.
	int newPosition = updateData(value);

	InNode leaf(indexFile, last);
	if (!leaf.isFull()) {
		int index;
		leaf.fill(leaf.keys, key, index);//change the 1st line of inside node.
		int i = 0;
		for (vector<int>::iterator it = leaf.position.begin(); ; it++) {
			//change the 2nd line of inside node.
			if (index == i-1) { leaf.position.insert(it, newPosition); break; }
			i++;
		}
		//leaf.updateIndexFile(indexFile);
		leaf.updateIndex();
		updateMax(leaf.nowPos);
	}
	else {
		//need to split the node.
		int index;
		leaf.fill(leaf.keys, key, index);//change the 1st line of inside node.
		int i = 0;
		for (vector<int>::iterator it = leaf.position.begin(); ; it++) {
			//change the 2nd line of inside node.
			if (index == i - 1) { leaf.position.insert(it, newPosition); break; }
			i++;
		}
		splitFull(index, leaf);
	}

	clearMemory();
	return "Success.";
}

string Database::search(const string &key)
{
	//save();
	int now = root;
	//if leaf node is found, the postion will be negative.
	while (now > 0) {
		InNode node(indexFile,now);
		now = node.findPos(key);
	}
	if (now == 0) return "Not Found.";
	string data;
	dataFile.seekg(-now);
	getline(dataFile, data);
	return "Found: "+ data;
}

string Database::del(string & key)
{
	int now = root;
	int last = 0;
	//if leaf node is found, the postion will be negative.
	while (now > 0) {
		InNode node(indexFile, now);
		last = now;
		now = node.findPos(key);
	}
	if (now == 0) return "Sorry. Index not found!";

	//delete the keys in the leaf.
	now = last;
	InNode leaf(indexFile, now);
	int dataPos = leaf.findPos(key);
	for (vector<int>::iterator it = leaf.position.begin(); it != leaf.position.end(); it++) {
		if ((*it) == dataPos)
		{
			leaf.position.erase(it);
			break;
		}
	}
	delInVector(leaf.keys, key);
	leaf.updateIndex();
	clearMemory();
	return "Success.";
}

string Database::modify(const string & key, const string & value)
{
	//index less than 8.
	if (key.size() > 8) return "Sorry. Index should be no more than 8 digits.";
	if (value.size() >= SPACEFORDATA) return "Sorry. Value should be less than 50 digits.";
	int now = root;
	int last = 0;
	//if leaf node is found, the postion will be negative.
	while (now > 0) {
		InNode node(indexFile, now);
		last = now;
		now = node.findPos(key);
	}
	if (now == 0) return "Sorry. Index not found!";
	int newPosition = updateData(value);
	int oldPosition = now;
	InNode leaf(indexFile, last);
	for (int i = 0; i < leaf.position.size(); i++) {
		if (leaf.position[i] == oldPosition)
			leaf.position[i] = newPosition;
	}
	leaf.updateIndex();
	clearMemory();
	return "Success.";
}

bool Database::refresh()
{
	fstream fout("data/index.dat",ios::out|ios::binary);
	//The entrance.
	fout << SPACEFORINDEX << endl;
	string s = "";
	for (int i = 1; i <= SPACEFORINDEX-1; i++)
		s += " ";
	fout << s;
	fout.seekp(SPACEFORINDEX);
	fout << endl;//empty node.
	fout << "0" << endl;//goto nowhere.
	fout << "0" << endl;//from nowhere.
	fout.close();
	fstream foutt("data/data.dat", ios::out | ios::binary);
	foutt.close();
	root = SPACEFORINDEX;
	return false;
}

InNode Database::split(int index, InNode & oldNode)
{

	int mid = (int)((M + 1) / 2);
	InNode newNode(indexFile);
	if (emptyIndex.size() != 0) {
		newNode.nowPos = *(emptyIndex.begin());
		emptyIndex.erase(newNode.nowPos);
	}
	else {
		int endpos = maxPointer;
		newNode.nowPos = ((int)(maxPointer / SPACEFORINDEX) + 1) * SPACEFORINDEX;
	}
	for (int i = oldNode.keys.size() - 1; i >= mid; i--) {
		//id from mid to -1
		newNode.keys.insert(newNode.keys.begin(), oldNode.keys[i]);
		newNode.position.insert(newNode.position.begin(), oldNode.position[i + 1]);
		oldNode.keys.pop_back();
		oldNode.position.pop_back();
		if (newNode.position[0] > 0) {
			InNode nextNode(indexFile, newNode.position[0]);
			nextNode.lastPos = newNode.nowPos;
			//nextNode.updateIndexFile(indexFile);
			nextNode.updateIndex();
		}
	}
	newNode.position.insert(newNode.position.begin(), 0);
	
	newNode.lastPos = oldNode.lastPos;
	return newNode;

	
}

void Database::clearMemory()
{	
	//if map is larger than 5000, to save memory, clear it.
	if (NodeMap.size() > MAXMAPSIZE) {
		map<int, InNode>::iterator it;
		int i = 0;
		for (it = NodeMap.begin(); it != NodeMap.end(); it++) {
			i++;
			it->second.updateIndexFile(indexFile);
		}
		NodeMap.clear();
	}

}

void Database::save()
{
	map<int, InNode>::iterator it;
	cout << "Saving. Please wait." << endl;
	int i = 0;
	for (it = NodeMap.begin(); it != NodeMap.end(); it++) {
		it->second.updateIndexFile(indexFile);
	}
	cout << "Index Saved." << endl;

	int lastSavedPosition = SPACEFORDATA;
	for(vector<DataNode>::iterator it = DataList.begin(); it != DataList.end(); it++){
		if (it->position - lastSavedPosition != SPACEFORDATA)
			dataFile.seekp(it->position);
		lastSavedPosition = it->position;
		dataFile << it->value << endl;
		string s = "";
		for (int i = 1; i <= SPACEFORDATA -1 -1 - it->value.size(); i++)//two \n
			s += " ";
		dataFile << s << endl;
	}
	NodeMap.clear();
	DataList.clear();
	indexFile.close();
	dataFile.close();
	indexFile.open("data/index.dat", ios_base::in | ios_base::out | ios_base::binary);
	dataFile.open("data/data.dat", ios_base::in | ios_base::out | ios_base::binary);
	cout << "Data Saved." << endl;
}

int Database::updateData(const string & data)
{
	//return the index.

	int newPosition = ((int)(maxDataPointer / SPACEFORDATA) + 1) * SPACEFORDATA;
	maxDataPointer += SPACEFORDATA;
	DataNode newNode(newPosition, data);
	DataList.push_back(newNode);
	newPosition = -newPosition;//leaf node is negative.
	return newPosition;
}



template <typename T>
int findInVector(vector<T> &vec, T key)
{
	if (vec.size() == 0) return 0;
	int mid = int(vec.size() / 2);
	int left = 0;
	int right = vec.size() - 1;
	while (1) {
		if (key > vec[right]) return right + 1;
		if (key < vec[left]) return 0;
		if (key == vec[mid]) return mid;
		if (key == vec[left]) return left;
		if (key > vec[mid]) {
			left = mid;
			mid = (int)((left + right) / 2);
		}
		else {
			right = mid;
			mid = (int)((left + right) / 2);
		}
		if (right - left <= 1) return right;
	}
}

template <typename T>
void insertInVector(vector<T> &vec, T key) {
	int index = findInVector(vec, key);
	int i = 0;
	for (vector<T>::iterator it = vec.begin(); it != vec.end(); it++) {
		if (index == i) { vec.insert(it, key); break; }
		i++;
	}
}

template <typename T>
void delInVector(vector<T> &vec, T key) {

	int index = findInVector(vec, key);
	int i = 0;
	for (vector<T>::iterator it = vec.begin(); it != vec.end(); it++) {
		if (index == i) { vec.erase(it); break; }
		i++;
	}
}