#ifndef BATTSTATS
#define BATTSTATS

#include <cstring>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <csignal>
#include <unistd.h>
using namespace std;

//measuration interval
const int MIN = 5;

class BattTracker {
	int h;
	int min;
	bool isRunning(); //1 - running, 0 - stopped
	int batteryLevel();
	bool isCharging();

	public:
		void start(bool clean = 0);
		void unlock();
		void status(); //manages status screen
		void kill(); //kills first thread, inthand manages exit

};

#endif