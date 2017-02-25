#include "BattStats.h"

void BattTracker::start(int MIN, bool clean){
	auto t = time(nullptr);
	auto tm = *localtime(&t);
	if(isRunning()){ //already running
		cout<<"Already running"<<endl;
		exit(1);
	}

	ofstream flock("battstats.lock");
	if(!flock){
		cout<<"Error creating lock file"<<endl;
		exit(1);
	}
	flock<<batteryLevel()<<endl;

	fstream ff;

	if(clean){
		remove("battstats.log");
	}

	ff.open("battstats.log", ios::out|ios::app);
	if(!ff){
		cerr<<"Error opening log file"<<endl;
		unlock();
		exit(1);
	}

	ff.seekg(0, ios_base::end);
	ff<<put_time(&tm, "%d-%m-%Y %H:%M:%S")<<'\t'<<"Battstats starting"<<endl;
	bool status; //0 - charging, 1 - discharging
	bool lastStatus = 0;

	int h = 0;
	int min = -MIN;

	//condition will be modified by other istances
	while(isRunning()){
		status = isDischarging();
		if(status!=lastStatus){
			lastStatus = !lastStatus;
			t = time(nullptr);
			tm = *localtime(&t);
			ff<<endl<<put_time(&tm, "%d-%m-%Y %H:%M:%S")<<'\t'<<"Status: "<< (status ? "discharging" : "charging")<<endl;
			min = -MIN;
			h = 0;
			if(status){
				flock.close();
				remove("battstats.lock");
				flock.open("battstats.lock");
				flock<<batteryLevel()<<endl;
			}
		}

		min += MIN;
		if(min>59){
			h++;
			min -= 60;
		}

		ff<<setw(2)<<setfill('0')<<h<<':'<<setw(2)<<setfill('0')<<min<<"\t"<<batteryLevel()<<endl;
		ff.flush(); //salva i cambiamenti
		flock<<h*60+min<<endl;
		flock.flush();
		sleep(MIN*60);
	}

	t = time(nullptr);
	tm = *localtime(&t);
	ff<<endl<<put_time(&tm, "%d-%m-%Y %H:%M:%S")<<'\t'<<"Battstats exiting"<<endl<<endl;
	ff.close();
	flock.close();
	unlock();
}

void BattTracker::unlock(){
	remove("battstats.lock");
}

bool BattTracker::isRunning(){
	ifstream flock("battstats.lock");
	return flock.good();
}

//This function display status when app is run without args
void BattTracker::status(){
	cout<<"Status: \t\t\t"<< (isRunning() ? "Running" : "Not Running") << endl;
	int startingPercentage;
	int min;
	if(isRunning()){
		ifstream flock("battstats.lock");
		flock>>startingPercentage;
		cout<<"Starting battery percentage: \t"<<startingPercentage<<endl;
		while(flock){
			flock>>min;
		}
		cout<<"Time elapsed: \t\t\t"<<setw(2)<<setfill('0')<<min/60<<':'<<setw(2)<<setfill('0')<<min%60<<endl;
	}

	cout<<"Current battery percentage: \t"<<batteryLevel()<<endl;

	if(isRunning() && isDischarging()){
		int currentPercentage = batteryLevel();
		cout<<"Estimated time remaining: \t";
		if(startingPercentage == currentPercentage || min == 0){
			cout<<"Insufficient data"<<endl;
		} else {
			int eta = ((float)min/(startingPercentage - currentPercentage))*currentPercentage;
			cout<<eta/60<<" hour(s) and "<<eta%60<<" minute(s)"<<endl;
		}
	}
}

void BattTracker::kill(){
	//kill battstats
	//exception for "battstats stop" is made excluding parent pid from kill
	if(system("kill $(pgrep battstats | grep -v $PPID ) 2>/dev/null")){
		cout<<"Error stopping or not running"<<endl;
	}
}

int BattTracker::batteryLevel(){
	ifstream fcapacity("/sys/class/power_supply/BAT1/capacity");
	int percentage;
	fcapacity>>percentage;
	return percentage;
}

bool BattTracker::isDischarging(){
	//(system("if [ `cat /sys/class/power_supply/BAT1/status` == 'Discharging' ]; then exit 1; fi; exit 0"))
	ifstream fstatus("/sys/class/power_supply/BAT1/status");
	char c;
	fstatus>>c;
	return (c=='D' ? true : false); //D for "Discharging" (reads only first char)
}