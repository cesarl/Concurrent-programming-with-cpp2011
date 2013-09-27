// ConsoleApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <cstdlib>


void fnc1()
{
	std::cout << "Func1 print" << std::endl;
}

void simpleThreadAndWait()
{
	std::thread t(fnc1); // start fnc1 in thread t
	t.join(); // wait for t to finish
}

void simpleThreadDontWait()
{
	std::thread t(fnc1);
	t.detach();	// dont wait -> deamon process. Nothing is \
	printed out because main thread has finished before t has time to execute fnct1
}

bool isJoinable(const std::thread &t)
{
	return t.joinable();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//simpleThreadAndWait();
	
	//simpleThreadDontWait();

	//You cannot detach and join the same thread, if you detach and then join it, program'll crash
	// You can test if a thread can be joined like that
	// if (t.joinable())
	//     t.join();
	
	return EXIT_SUCCESS;
}

