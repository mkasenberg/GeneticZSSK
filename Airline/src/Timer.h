#pragma once
#include <iostream>
#include <time.h>

#define WORK_ON_LINUX 1
#ifdef WORK_ON_LINUX
typedef int64_t __int64;
#define BILLION  1000000000L;
//Remember about -lrt (real time lib) if using clock_gettime()
#else
#include <windows.h>
#endif

using namespace std;

class Timer
{
	static struct timespec start, stop;
	static double PCFreq;
	static __int64 CounterStart;
public:
	Timer();

	//Startuje od nowa zegar
	static void startTimer();

	//Pobiera czas od uruchomienia zegara w milisekundach
	static double millis();
};
