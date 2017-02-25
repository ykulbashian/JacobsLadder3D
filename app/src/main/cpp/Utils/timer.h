
#ifndef TIMER_H
#define TIMER_H

class Timer
{


public:

	enum TimerType
	{
		AD_HOC = 0,
		LONG_TERM = 1,
	};

	static void startTimer(char timerNumber = AD_HOC);
	static long long stopTimer(char timerNumber = AD_HOC);

    // store timer variables
	static unsigned long long *SavedTimes;

    static void printTimes();

private:
	static long long currentTimestamp0;
	static long long currentTimestamp1;

};

#endif
