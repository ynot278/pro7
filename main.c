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

int license;

static int setUpSHM(){
	shmClockid = shmget(8837, sizeof(sysClock), IPC_CREAT | IPC_EXCL | 0666);
	if(shmClockid == -1){
		perror("oss error: shmget shmClockid");
		return -1;
	}

	sysClockptr = shmat(shmClockid, NULL, 0);
	if(sysClockptr == (void *) -1){
		perror("oss error: shmat sysClockptr");
		return -1;
	}

	iterationID = shmget(2789, sizeof(iterations), IPC_CREAT | IPC_EXCL | 0666);
	if(iterationID == -1){
		perror("oss error: shmget iterationID");
		return -1;
	}

	iterationsptr = shmat(iterationID, NULL, 0);
	if(iterationsptr == (void *) -1){
		perror("oss error: shmat iterationsptr");
		return -1;
	}

	return 0;
}

void sig_handler(){
	removeSHM();
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
	int i;

	signal(SIGALRM,sig_handler);
	signal(SIGINT, sig_handler);
  alarm(TIMER);

	if (argc != 2){
		printf("Usage: ./runsim n (n = runs up to n application processes at a time).\n");
		exit(EXIT_FAILURE);
	}

	int opt;

	while ((opt = getopt(argc, argv, "h")) != -1){
		switch (opt){
			case 'h':
				printf("Usage: ./runsim n (n = runs up to n application processes at a time).\n");
				exit(EXIT_SUCCESS);
				break;
			default:
				printf("Usage: ./runsim n (n = runs up to n application processes at a time).\n");
				exit(EXIT_SUCCESS);
		}
	}


	char *p;
	errno = 0;
	long conv = strtol(argv[1], &p, 10);

	if (errno != 0 || *p != '\0' || conv > N_LICENSE || conv < 1){
		printf("Number of licenses should not be greater than 20.");
		exit(EXIT_FAILURE);
	} else
		license = conv;


	time_t t;
  srand((unsigned) time(&t));

  setUpSHM();

	while(i < license){

		iterationsptr->repeatFactor = (unsigned int) (rand() % 10 + 1);
		iterationsptr->sleepTime = (unsigned int) (rand() % 10 + 1);

		iterationsptr->pid = fork();
		printf("%d", iterationsptr->pid);
		if(iterationsptr->pid == 0){
			execl("testsim", "testsim", NULL);
		} else {
			printf("p");
		}
		i++;
	}
	

	return 0;
}