/*file that is the server for the bank*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <netinet/in.h>
#include <errno.h>
#include "server.h"




/*function that prints everything every 15 seconds*/

void * printAll () {

	while (1) {

		struct account * p = start;

		sem_wait(&global_lock);
		if (p==NULL) {
			printf("No accounts in bank.\n");
		}else {
			printf("Columns: Account Name    Balance    Status\n\n");
		}
		while (p!=NULL) {
			printf("%s:\t$%0.2lf", p->name, p->balance);
			if (p->status == INSERVICE) {
				printf("\tINSERVICE\n");
			}else{
				printf("\n");
			}
			p = p->next;
		}

		sem_post(&global_lock);


		sleep(15);


	}

	return NULL;
}




/*funtion that listens to accept new clients*/


void * listen_for_clients(void * a) {

	int port = *(int *)a;

	struct sockaddr_in server;

	char message[1024];
	memset(message, '\0', 1024);

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int FD = socket (PF_INET, SOCK_STREAM, 0);
	if (FD==-1) {
		printf("Error creating socket.\n");
		exit(1);
	}

	if (bind(FD, (struct sockaddr *)&server, (sizeof(server)))==-1) {
		printf("Error. Could not bind socket. %s\n", strerror(errno));
		exit(1);
	}

	while (1) {
		if (listen(FD, 100)==-1) {
			printf("Error listening.\n");
			return NULL;
		}
		int FD_client = accept(FD, NULL, NULL);
		
		strcpy(message, "Server successfully connected to client");
		printf("%s #%d\n", message, FD_client);
		write(FD_client, message, strlen(message));

		/*get_command*/
		thread_ptr->next = (struct thread_Node *)malloc(sizeof(struct thread_Node));
		thread_ptr = thread_ptr->next;
		int thread = pthread_create(&thread_ptr->thread, NULL, get_Commands, &FD_client);
		if (thread!=0) {
			printf("Could not create a thread for a client. Will stop listening\n");
			continue;
		}


	}



}


void exitThreads () {

	if (thread_start == NULL) {
		return;
	}

	thread_ptr = thread_start;
	struct thread_Node * thread_prev = thread_ptr;

	while (thread_ptr !=NULL) {

		pthread_cancel(thread_ptr->thread);
		thread_prev = thread_ptr;
		thread_ptr = thread_ptr->next;
		free(thread_prev);

	}
	return;


}

void freeAccounts() {

	struct account * prev = start;
	ptr = start;



	while (ptr!=NULL) {

		free(ptr->name);
		prev = ptr;
		ptr = ptr->next;
		free(prev);

	}

	return;


}

void freeFDs () {

	struct FD_node * FDprev = startFD;
	FDptr = startFD;

	char quit [20];
	memset(quit, '\0', 20);
	strcpy(quit, "Quitting");

	while (FDptr!=NULL) {

		send(FDptr->FD, quit, strlen(quit), 0);
		close(FDptr->FD);
		FDprev = FDptr;
		FDptr = FDptr->next;
		free(FDprev);

	}
	return;
}
 
void  quitSignal (int sig) {


	freeFDs();
	
	freeAccounts();
	
	exitThreads();

	printf("Server shutting down. Goodbye\n");


	exit(0);
}

int main ( int argc, char ** argv) {

	signal(SIGINT, quitSignal);

	startFD = NULL;

	FDptr = startFD;

	start = NULL;
	ptr = start;

	thread_start = NULL;
	thread_ptr = thread_start;



	if (argc!=2) {

		printf("Invalid number of arguments. Must call ./BankingServer XXXX where (XXXX) is port number.\n");
		return 1;

	}

	if (sem_init(&global_lock, 0, 1)!=0) {
		printf("Error initializing a semaphore.\n");
		return 1;
	}

	int port = atoi(argv[1]);
	if (port < 8192) {
		printf("Argument must be a valid port number, 8192 or above.\n");
		return 1;
	}



	/*need two threads. One to listen, and one to print

			thread to listen for clients
			thread to print the status every 15 seconds
	*/
	int * x = &port;

	thread_start = (struct thread_Node *)malloc(sizeof(struct thread_Node));
	thread_start->next = NULL;
	thread_ptr = thread_start;
	int a = pthread_create(&thread_start->thread, NULL, printAll, NULL);
	if (a!=0) {
		printf("Could not Create a thread to print data\n");
		return 0;
	}
	thread_start->next = (struct thread_Node *)malloc(sizeof(struct thread_Node));
	thread_ptr = thread_start->next;
	thread_ptr->next = NULL;
	a = pthread_create(&thread_ptr->thread, NULL, listen_for_clients, (void *) x);
	if (a!=0) {
		printf("Could not Create a thread to listen for clients\n");
		return 0;
	}
	/*printAll();
		listen_for_clients
	*/
	while(1) {};


	return 0;
}


