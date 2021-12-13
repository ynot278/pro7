#ifndef OSS_H
#define OSS_H

#define TIMER 100
#define N_LICENSE 20

typedef struct sysClock{
	unsigned int seconds;
	unsigned int nanoseconds;
} sysClock;

static sysClock *sysClockptr;
int shmClockid;

typedef struct iterations{
	unsigned int repeatFactor;
	unsigned int sleepTime;
	pid_t pid;
} iterations;

static iterations *iterationsptr;
int iterationID;

static void removeSHM(){
	if(shmdt(sysClockptr) == -1){
		perror("error: shmdt sysClockptr");
	}

	if(shmctl(shmClockid, IPC_RMID, NULL) == -1){
		perror("error: shmctl shmClockid");
	}

	if(shmdt(iterationsptr) == -1){
		perror("error: shmdt iterationsptr");
	}

	if(shmctl(iterationID, IPC_RMID, NULL) == -1){
		perror("error: shmctl iterationID");
	}
}

#endif