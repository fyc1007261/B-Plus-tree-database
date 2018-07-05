#pragma once
#include<vector>
#include<string>
#include<sstream>
using namespace std;

vector<string> split(string &str, char pattern);
vector<int> split(char, string&);
string cut_str(string s, int left, int right);
double str_to_num(string s);

