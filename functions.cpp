#include "functions.h"


vector<string> split(string &str, char pattern) {

	str += pattern;
	vector<string> result;
	int size = str.size();
	int p = 0;
	for (int i = 0; i < size; i++) {
		if (str[i] == pattern) {
			string mes;
			for (int j = p; j < i; j++)
				mes += str[j];
			p = i + 1;
			result.push_back(mes);
		}
	}
	return result;
}

vector<int> split(char pattern, string &str) {
	str += pattern;
	vector<int> result;
	int size = str.size();
	int p = 0;
	for (int i = 0; i < size; i++) {
		if (str[i] == pattern) {
			string mes;
			for (int j = p; j < i; j++)
				mes += str[j];
			p = i + 1;
			result.push_back(str_to_num(mes));
		}
	}
	return result;
}

string cut_str(string s, int left, int right)
{
	// to return a string in the middle, starting at left and ending at right. 
	//left and right included.
	string result = "";
	for (int i = left; i <= right; i++) result += s[i];
	return result;
}

double str_to_num(string s) {
	istringstream iss(s);
	double  x;
	if (iss >> x) return x;
	return -1;
}

