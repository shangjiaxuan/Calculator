﻿//////////////////////////////////////////////////////////////////////
//For debugging certain functions directly by compiling into exe

#include "Header.h"
#include "SwapEnc.h"
#include "Calculator.h"

using namespace std;

#ifndef _WINDLL
int main() {
	SwapEnc();
	system("pause");
	return 0;
}

#endif