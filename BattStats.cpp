#include "BattStats.h"

BattTracker bt;

void inthand(int signum){
	bt.unlock();
}

int main(int argc, char** argv){

	signal(SIGINT, inthand);
	signal(SIGTERM, inthand);
	signal(SIGKILL, inthand);


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

		bt.start(argc>2 && !strcmp(argv[2], "clean")); //condition sets boolean for clean
	} else if(argc>1 && !strcmp(argv[1], "stop")){
		bt.kill();
	} else
		bt.status();
	
	return 0;
}