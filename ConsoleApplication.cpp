// ConsoleApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <cstdlib>
#include <cmath>
#include <string>
#include <mutex>
#include <fstream>


class Fctor
{
public:
	Fctor(){}
	~Fctor(){}
	void operator()(const std::string & msg)
	{
		std::cout << "Thread says: " << msg << std::endl;
	}
};

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
	t.detach();	/* dont wait -> deamon process. Nothing is
	printed out because main thread has finished before t has time to execute fnct1 */
}

bool isJoinable(const std::thread &t)
{
	return t.joinable();
}

void computeInMainBeforeJoiningOrAttach()
{
	/* explanation :
	a thread have to be joined or detached
	risk here is that t finish before t.join() or dettach() because main thread proceed
	to some "heavy" work before joining t.
	If that happen, then program will throw an exception, because he'll
	To avoid this problem, we put the main thread work, into a try catch block
	*/

	std::thread t(fnc1);

	try
	{
		float j = 0;
		for (unsigned int i = 0; i < 100; ++i)
		{
			j = std::pow((float)i, 5);
			j = std::sqrt(j);
			std::cout << "Main thread compute : " << j << std::endl;
		}
	} catch (...)
	{
		t.join();
		throw;
	}

	t.join();
}

void withArguments()
{
	//even if msg is passed by reference in method operator() of Functor
	// it is passed here by value
	// because parameters are always passed by value with treads
	// if you really want to pass parameteres as reference, you'll have tu use :
	// std::thread t((Fctor()), std::ref(myMessage));
	// or pointer
	// or std::move
	std::thread t((Fctor()), "Hello there, how're you");
	t.join();
}

void printCurrentThreadId()
{
	std::cout << std::this_thread::get_id() << std::endl;
}

void avoidingOversubscription()
{
	/*
	To much thread than your CPU can handle is bad for performance
	so you need to know how many thread you can create
	*/

	std::cout << std::thread::hardware_concurrency() << std::endl;
}

static std::mutex mu;

void sharedPrint(const std::string &msg, unsigned int id)
{
	mu.lock();

	/* if that line of code throw exeption,
	std::mutex mu will be locked forever
	that's why it's better to use solution you can find in
	sharedPrint2
	*/
	std::cout << msg << id << std::endl;
	mu.unlock();
}

void sharedPrint2(const std::string &msg, unsigned int id)
{
	std::lock_guard<std::mutex> guard(mu);
	std::cout << msg << id << std::endl;

/*
We still have a problem here,
std::cout is not entirely under protection of mutex mu
if face it can be used without locking mu
a better solution would be to do like downthere
mutex has to be bounded with ressource that he's protecting
*/
}

class LogFile
{
private:
	std::mutex mutex_;
	std::ofstream f_;
public:
	LogFile()
	{
		f_.open("LogFile.log");
	}
	~LogFile()
	{
		f_.close();
	}
	void sharedPrint(const std::string & msg, unsigned int id)
	{
		std::lock_guard<std::mutex>	guard(mutex_);
		f_ << msg << id << std::endl;
	}
	// be careful to never return avalue of a class of this type, like :
	std::ofstream & getFile()
	{
		return f_;
	} // it'll be accessible without usage of the mutex !!!

	//also never pass f_ as argument to user provided function
	// ex :
	void process(void fun(std::ofstream& s))
	{
		fun(f_);
	} // because function fun can ba whatever to f_ without using mutex
};

void threadLogToFile(LogFile &log)
{
	for (unsigned int i = 100; i > 0; --i)
		log.sharedPrint("I'm a thread and I print ", i);
}

void logToFile()
{
	LogFile log;
	std::thread t(threadLogToFile, std::ref(log));
	for (unsigned int i = 0; i < 100; ++i)
		log.sharedPrint("I'm a main and I print ", i);
	t.join();
}

void threadDec()
{
	for (unsigned int i = 100; i > 0; --i)
		sharedPrint("I'm a thread and I print ", i);
}

void mutexUsage()
{
	std::thread t(threadDec);

	t.detach();
	for (int i = 0; i < 100; i++)
	{
		sharedPrint("I'm main and I print ", i);
	}	
}

int _tmain(int argc, _TCHAR* argv[])
{
	//simpleThreadAndWait();
	
	//simpleThreadDontWait();

	//You cannot detach and join the same thread, if you detach and then join it, program'll crash
	// You can test if a thread can be joined like that
	// if (t.joinable())
	//     t.join();
	

	//	computeInMainBeforeJoiningOrAttach();
	
	//withArguments();

	//avoidingOversubscription();

	//mutexUsage();

logToFile();	
	return EXIT_SUCCESS;
}

