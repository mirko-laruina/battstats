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

class BattTracker {
	bool isRunning(); //1 - running, 0 - stopped
	int batteryLevel();
	bool isDischarging();

	public:
		void start(int MIN, bool clean); //first condition - clean start, second condition
		void unlock();
		void status(); //manages status screen
		void kill(); //kills first thread, inthand manages exit

};

#endif