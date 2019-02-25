/*header file for the server*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <signal.h>

#define INSERVICE 1
#define INACTIVE 0

struct account {

	char * name;
	double balance;
	int status;
	struct account * next;
};

struct FD_node {
	int FD;
	struct FD_node * next;
	struct FD_node * prev;
};

struct thread_Node {

	pthread_t thread;
	struct thread_Node * next;

};

struct account * start;
struct account * ptr;

sem_t global_lock;

struct FD_node * startFD;
struct FD_node * FDptr;

struct thread_Node * thread_start;
struct thread_Node * thread_ptr;


/*mehtod to add an account to the bank*/
struct account * createAccount (char name[]) {

	struct account * temp = (struct account *)malloc(sizeof(struct account));

	temp->name = (char *)malloc(sizeof(char)*1024);
	if (!temp->name) {
		printf("Well shit. Cannot allocate memory for an account. Closing server.\n");
		exit(2);
	}

	strcpy(temp->name, name);
	temp->balance = 0;
	

	temp->status = INACTIVE;

	if (start==NULL) {
		sem_wait(&global_lock);
		start = temp;
		temp->next = NULL;
		ptr = start;
		sem_post(&global_lock);
		return temp;
	}

	sem_wait(&global_lock);
	ptr->next = temp;
	ptr = temp;
	temp->next = NULL;
	sem_post(&global_lock);

 	return temp;
}


/*method to search for an account in the bank*/
struct account * find(char name[]){

	struct account * p = start;
	while( p !=NULL){
		if(strcmp(name, p->name)==0){
			return p;
		}

		p = p->next;
	}
	return NULL;

}

/*function to change the balance of an account*/
double changeBalance(struct account * a, double amount){
	double newBalance;

	newBalance= a->balance + amount;
	if(newBalance<0){
		return -1;
	}
	else{
		a->balance = newBalance;
		return newBalance;
	}

}



/*function to process the command from the client*/

struct account * doCommand (struct account * local, char arg[]){

	/*split string into two, one for the command, and one for the data*/
	char command[1024];
	char data[1024];
	memset(data, '\0', strlen(arg));
	memset(command, '\0', strlen(arg));
	
	int count = sscanf(arg, "%s %s", command, data);
	memset(arg, '\0', strlen(arg));


	if (strcmp("create", command)==0) {
		if (strlen(data)>255) {
			strcpy(arg, "Error: Account name is too long");
			return local;
		}else if (count == 1) {
			strcpy(arg, "Error: Account name is all whitespace/no name entered.");
			return local;
		}else if (find(data)!=NULL) {
			strcpy(arg, "Error. Account with the same name already exists");
			return local;
		} else if (local !=NULL) {
			strcpy(arg, "Error. Cannot create another account while serving a different account");
			return local;
		}
		
		createAccount(data);
		strcpy(arg, "Server successfully created account ");
		strcat(arg, data);
		return local;
	}else if (strcmp("serve", command)==0) {
		if (local !=NULL) {
			strcpy(arg, "Error. Cannot create another account while serving a different account");
			return local;
		}
		struct account * p = find(data);
		sem_wait(&global_lock);
		if (p!=NULL) {
			if (p->status == INSERVICE) {
				strcpy(arg, "Error. Account is already in service elsewhere");
				sem_post(&global_lock);
				return local;
			}
		}else {
			strcpy(arg, "Error. Account does not exist");
			sem_post(&global_lock);
				return local;
		}

		local = p;
		p->status = INSERVICE;
		sem_post(&global_lock);

		strcpy(arg, "Client is now servicing account ");
		strcat(arg, data);

		return local;

	}else if(strcmp("deposit", command)==0 || strcmp("withdraw", command)==0) {
		if (local == NULL) {
			strcpy(arg, "Error. Have to be servicing an account");
			return local;
		}else if (data<=0) {
			strcpy(arg, "Error. Enter a valid positive amount to deposit or withdraw");
			return local;
		}
		float amount = atof(data);
		if (amount <= 0) {
			strcpy(arg, "Error. Enter a numerical positive amount to deposit or withdraw");
			return local;
		}
		if (command[0]=='w') {
			amount = amount * -1;
		}

		sem_wait(&global_lock);
		double balance = changeBalance(local, amount);
		if (balance == -1) {
			strcpy(arg, "Error. Cannot withdraw more than what is in account");
			sem_post(&global_lock);
			return local;
		}
		sprintf(arg, "Server changed account %s's balance to $%0.2lf", local->name, balance);
		sem_post(&global_lock);
		return local;
	}else if (strcmp("query", command)==0) {
		if (local == NULL){
			strcpy(arg, "Error. Must be serving an account to query it");
			return local;
		}
		sprintf(arg, "Account balance for %s: $%0.2lf", data, local->balance);
	}else if (strcmp("end", command)==0) {
		if (local == NULL) {
			strcpy(arg, "Error. Must be serving an account to end serving it");
			return local;
		}
		sem_wait(&global_lock);
		local->status = INACTIVE;
		strcpy(arg, "Ending account session\n");
		sem_post(&global_lock);

		return NULL;
	}else if(strcmp("quit", command)==0) {

		if (local !=NULL) {
			local->status = INACTIVE;
		}
		local = NULL;
		
		strcpy(arg, "Quitting");

		return local;

	}else {
		strcpy(arg, "Error. Not a valid command");
		return local;
	}

	return local;
}









/*function to deal with client commands*/

void * get_Commands (void * a) {

	int FD = *(int *)a;
	sem_wait(&global_lock);
	struct FD_node * curr = (struct FD_node *)malloc(sizeof( struct FD_node));
	if (!curr) {
		printf("Error allocating memory.\n");
		exit(0);
	}
	curr->FD = FD;



	if (FDptr == NULL) {
		startFD = curr;
		curr->prev = NULL;
		curr->next = NULL;
	}else {
		FDptr->next = curr;
		curr->prev = FDptr;
		curr->next = NULL;
		FDptr = curr;
	}

	sem_post(&global_lock);

	struct account * local = NULL;

	while (1) {


		char command[1024] = {'\0'};
		int status = recv(FD, command, 1024, 0);

		/*if (status == 0) {
			printf("Server connection ended with client %d\n", FD);
			break;
		}else*/

		if(status==-1) {
			printf("Error recieving message from client %d\n", FD);
			break;
		}
		command[status] = '\0';

		printf("Action recieved from client %d\n", FD);
		local = doCommand(local, command);
		if (send(FD, command, strlen(command), 0)==-1) {
			printf("Error. Could not send message to client\n");
			break;
		}

		if (strcmp(command, "Quitting")==0) {
			printf("Disconnecting from Client %d\n", FD);
			break;
		}

		

		/* POSSIBLY DO NOT NEED THIS SLEEP*/
	


	}

	sem_wait(&global_lock);
	if (curr->prev!=NULL) {
		curr->prev->next = curr->next;
	}else {
		startFD = curr->next;

	}

	if (curr->next != NULL) {
		curr->next->prev = curr->prev;
	}
	sem_post(&global_lock);

	free(curr);
	close(FD);

	return NULL;

}



















