// 01 test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"
#include <iostream>
#include <sstream>

using namespace std;


int main() {
	FILE* file_buff;
	freopen_s(&file_buff,"1.txt", "wb", stdout);
	string str{ "test string" };
	istringstream iss{ str };
	char c;
	iss >> c;
	cout << c;
	iss.seekg(1);
	iss >> c;
	cout << c;
	int pos=iss.tellg();
	iss.seekg(pos);
	iss >> c;
	cout << c;
	cout << "Hello World!\n";
}
