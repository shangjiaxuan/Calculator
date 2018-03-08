#include "Header.h"

#include "../Calculator/Token.h"

#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

typedef void(*void_ist_ptr)(istream&);

int main() {
	HINSTANCE hInst = LoadLibrary("Calculator.dll");
	if (!hInst) {
		cout << "Error!" << endl;
	}
	const void_ist_ptr calculator = void_ist_ptr(GetProcAddress(hInst, "calculator"));
	Calc::Token_stream ts;
	try {
		calculator(cin);
		FreeLibrary(hInst);
		return 0;
	}
	catch (...) {
		cerr << "unknown error\n";
		FreeLibrary(hInst);
		return 2;
	}
}


/*
void test(istream& ist) {
	string s;
	ist >> s;
	cout << s << '\n';
}

int main() {
	test(cin);
	system("pause");
	return 0;
}
*/