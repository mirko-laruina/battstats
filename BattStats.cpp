#include "BattStats.h"
#include <limits.h> //just fot PATH_MAX
#include <libgen.h>

BattTracker bt;

void inthand(int signum){
	bt.unlock();
}

void displayHelp();

int main(int argc, char** argv){

	signal(SIGINT, inthand);
	signal(SIGTERM, inthand);
	signal(SIGKILL, inthand);

	//Fix directory to executable path (otherwise app run from another directory has different behaviour)
	{
		char* path = new char[PATH_MAX];
		//get path with executable name
		if(readlink("/proc/self/exe", path, PATH_MAX) == -1){
			cout<<"readlink error"<<endl;
			exit(1);
		}
		//extract path only
		path = dirname(path);
		//change dir to path
		if( chdir(path) == -1){
			cout<<"chdir error"<<endl;
			exit(1);
		}
	}

	if(argc>1 && !strcmp(argv[1], "start")){

		//launching child to detach from console
		int pid = fork();
		if(pid < 0){
			cout << "Error launching app" << endl;
		} else if (pid != 0){
			//Parent process has launched child (deamon)
			//Parent is no more needed
			exit(0);
		}
		setsid();
		//From now on the child has power

		//setting measurement interval
		int interval = atoi(argv[argc-1]); //get last argument
		if(interval == 0){ //not set or set to 0
			interval = 5;  //set to 5 minutes
		}

		//second condition sets boolean for clean
		bt.start(interval, (argc>2 && !strcmp(argv[2], "clean"))); 
	} else if(argc>1 && !strcmp(argv[1], "stop")){
		bt.kill();
	} else if(argc>1 && !strcmp(argv[1], "help")){
		displayHelp();
	} else {
		bt.status();
	}
	
	return 0;
}

void displayHelp(){
	cout<<"Battstats is a simple utility to track your battery usage."<<endl
		<<endl<<'\t'<<"./battstats start : starts the 'daemon'"<<endl
		<<'\t'<<"./battstats start clean : starts and cleans the log file"<<endl
		<<'\t'<<"./battstats start 2 : starts with 2 minutes (or whetever it is set) measurement interval"<<endl
		<<'\t'<<"./battstats start clean 2 : same as before but it cleans log"<<endl
		<<'\t'<<"./battstats stop : obvious"<<endl
		<<'\t'<<"./battstats : prints information about battery percentage and time remaining"<<endl
		<<'\t'<<"./battstats help : prints this help screen"<<endl
		<<endl<<"battstats.log contains information about computer state (charging or discharging), time on battery and relative battery percentage."
		<<endl;
}