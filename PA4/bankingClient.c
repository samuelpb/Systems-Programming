

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
#include "bankingClient.h"


void quitSignal (int  sig) {


	memset(out_Message, '\0', 1023);
	strcpy(out_Message, "quit");
	if (send(server, out_Message, strlen(out_Message), 0)==-1) {
		printf("Could not close client on the server end while terminating session\n");
		close(server);
		pthread_exit((void *)input_thread);
		pthread_exit((void *)output_thread);
		exit(1);
	}
	close(server);
	pthread_exit((void *)input_thread);
	pthread_exit((void *)output_thread);
	exit(0);


}

int main (int argc, char ** argv) {

	signal(SIGINT, quitSignal);

	if (argc!=3) {
		printf("Error. Incorrect inputs.\n");
		return 1;
	}

	memset(in_Message, '\0', 1023);
	memset(out_Message, '\0', 1023);


	struct hostent * connection = gethostbyname(argv[1]);
	if (connection==NULL) {
		printf("Error. Could not find server.\n");
		return 1;
	}
	char * IP =  inet_ntoa(*((struct in_addr*) connection->h_addr_list[0]));

	int port_num = atoi(argv[2]);
	if (port_num<8192 || port_num> 65535) {
		printf("Error. Port number must be between 8192 and 65535\n");
		return 1;
	}

	/*create a socket to connect to the server*/

	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server < 0) {
		printf("Could not create a socket to connect to the server\n");
		return 1;
	}


	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port_num);
	serverAddress.sin_addr.s_addr = inet_addr(IP);

	while (connect(server, (struct sockaddr *)&serverAddress, (sizeof(serverAddress)))<0) {
		printf("Trying to connect to server. Will keep trying every 3 seconds. If no connection occurs, close and restart\n");
		sleep(3);
	}

	int size = 1023;
	int a = read(server, in_Message, size);
	if (a<0) {
		printf("Error. Connected to server but could not recieve conection messaage from server. Quitting\n");
		close(server);
		return 1;
	}

	printf("%s\n", in_Message);



	 int thread = pthread_create(&output_thread, NULL, output, (void *)&server);
	 if (thread<0) {
	 	printf("Could not allocate a thread for outputs. Shutting down.\n");
	 	exit(0);
	 }

	 thread = pthread_create(&input_thread, NULL, input, (void *)&server);
	 if (thread<0) {
	 	printf("Could not allocate a thread for outputs. Shutting down.\n");
	 	exit(0);
	 }

	 while(1){
	 	
	 };

	return 0;
}