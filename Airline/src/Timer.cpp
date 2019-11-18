#include "Timer.h"

double Timer::PCFreq = 0.0;

__int64 Timer::CounterStart = 0;

struct timespec Timer::start = {
		.tv_sec = 0,
		.tv_nsec = 0,
};

struct timespec Timer::stop = {
		.tv_sec = 0,
		.tv_nsec = 0,
};

Timer::Timer()
{
}

#ifdef WORK_ON_LINUX
void Timer::startTimer()
{
	if (clock_gettime( CLOCK_REALTIME, &start) == -1) {
		perror("clock gettime");
		exit( EXIT_FAILURE);
	}
}

double Timer::millis()
{
	if (clock_gettime( CLOCK_REALTIME, &stop) == -1) {
		perror("clock gettime");
		exit( EXIT_FAILURE);
	}

	return (stop.tv_sec - start.tv_sec) * 1000
			+ (stop.tv_nsec - start.tv_nsec) / 1000000;
}
#else
void Timer::startTimer()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
	{
		cout << "QueryPerformanceFrequency failed!\n";
		return;
	}

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

double Timer::millis()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}
#endif
