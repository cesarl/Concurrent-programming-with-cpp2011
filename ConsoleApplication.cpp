// ConsoleApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>


void fnc1()
{
std::cout << "Func1 print" << std::endl;	
}

int _tmain(int argc, _TCHAR* argv[])
{
	fnc1();
	return 0;
}

