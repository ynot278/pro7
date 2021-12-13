#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/mman.h>


#include "runsim.h"

static int setUpSHM(){
	shmClockid = shmget(8837, 0, 0);
	if (shmClockid == -1){
		perror("userProcess: shmget shmClockid");
		return -1;
	}

	sysClockptr = shmat(shmClockid, NULL, 0);
	if(sysClockptr == (void *) -1){
		perror("oss error: shmat sysClockptr");
		return -1;
	}

	iterationID = shmget(2789, 0, 0);
	if (iterationID == -1){
		perror("userProcess: shmget iterationID");
		return -1;
	}

	iterationsptr = shmat(iterationID, NULL, 0);
	if(iterationsptr == (void *) -1){
		perror("oss error: shmat iterationsptr");
		return -1;
	}

	return 0;
}

int main(const int argc, char *const arv[]){
	time_t t;
  srand((unsigned) time(&t));

	setUpSHM();

	stdout = freopen("logfile.txt", "a,", stdout);
	if(stdout == NULL){
		perror("freopen failed:");
		return -1;
	}

	int i;
	for(i = 1; i <= iterationsptr->repeatFactor; i++){
		sleep(iterationsptr->sleepTime);
		sysClockptr->seconds += iterationsptr->sleepTime;
		printf("%d::%d seconds::nanoseconds\tPID: %d\tIteration %d of %d\n", sysClockptr->seconds, sysClockptr->nanoseconds, iterationsptr->pid, i, iterationsptr->repeatFactor);
	}

	removeSHM();
}