
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
/*global variables and struct definitions*/

struct dataNode {

	char ** data;
	char * key;
	struct dataNode * next;
};



struct file_Args {

	char * path;
	int * key_column;
	int * parent;
};

struct t_id_node {

	pthread_t thread;
	struct t_id_node * next;
	int * status;
	int * id;
};

struct navDirArgs {

	char * dirPath;
	int * key_column;
	char * out_File;
	int * check;
	struct t_id_node * list;
	int * parent;
	
};

int t_counter;
int finished;
pthread_mutex_t list_lock;
pthread_mutex_t out_lock;
pthread_mutex_t counter_lock;
pthread_mutex_t check_lock;


struct dataNode * list_end_pointer;