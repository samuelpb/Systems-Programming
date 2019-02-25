

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <pthread.h>

char in_Message [1024];
char out_Message [1024];
int server = 0;
pthread_t output_thread;
pthread_t input_thread;



void * output (void * FD) {

	int success = 0;
	while(1) {
		memset(in_Message, '\0', 1023);

		success= recv(server, in_Message, (sizeof(in_Message)), 0);

		if (success<=0) {
			printf("Could not recieve message from server. Connection Terminated\n");
			close(*(int*)FD);
			exit(0);
		}

		if (strcmp(in_Message, "Quitting")==0) {
			printf("Client will close.\n");
			close(*(int*)FD);
			exit(0);
		}

		printf("%s\n", in_Message);


	}


	return NULL;
}



void * input (void * FD) {

	printf("Possible commands:  create (account name)\n");
	printf("serve (account name)       deposit (amount)\n");
	printf("withdraw (amount)      query\n");
	printf("end    quit\n");

	

	while (1) {

		printf("Please enter a command:\n");

		char temp[1024];
		char temp2[1024];
		char extra [30];
		memset(temp2, '\0', sizeof(temp2));
		memset(temp, '\0', sizeof(temp));
		memset(out_Message, '\0', sizeof(temp));

		fgets(out_Message, 1024, stdin);

		int d = sscanf(out_Message, "%s %s %s", temp, temp2, extra);

		if (d>2) {
			printf("Error. Not a valid command because too many arguments.");
			continue;
		}
		if (d==2) {
			strcat(temp, " ");
			strcat(temp, temp2);
		}

		strcpy(out_Message, temp);
		
		
		if (send(server, out_Message, strlen(out_Message), 0)==-1) {
			printf("Could not send command to server. Shutting down.\n");
			close(server);
			exit(1);
		}else {
			printf("Command sent to server\n");
		}

		sleep(2);
	}


	return NULL;


}







