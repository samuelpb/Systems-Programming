


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include "multiThreadSorter.h"






int main (int argc, char ** argv) {

	t_counter = 0;
	finished = 0;

	if (pthread_mutex_init(&list_lock, NULL)!=0 || pthread_mutex_init(&out_lock, NULL)!=0
	 || pthread_mutex_init(&counter_lock, NULL)!=0 || pthread_mutex_init(&check_lock, NULL)!=0 ){
		fprintf(stderr, "Error initializing mutex\n");
		return 1;
	}

	char* inputDir= (char *)malloc(sizeof(char)*1024);
	char * outputDir= (char *)malloc(sizeof(char)*1024);
	if (!inputDir || !outputDir) {

		fprintf(stderr, "Error allocating memory\n");
		return 1;
	}
	int samePlace = -1;
	int flag = 2;
	int err = 2;

	char error[1024] = "Input Format: ./sorter -c movie_title -d thisdir -o thatdir. -c is necessary. -d is optional, but if there then there must be an input directory path name. -o is optional, but if there then there must be an output direcotry\n";


/*SECTION TO CHECK FOR CORRECT PARAMETERS INPUTTED IN PROGRAM CALL*/

	if (argc!= 3 && argc!=5 && argc!=7) {

		write(err, error, sizeof(error));
		printf("%s\n", error);
		return 1;
	}

	if (argc==3 && strcmp(argv[1], "-c")!=0) {
		write(err, error, sizeof(error));
		printf("%s\n", error);
		return 1;
	}

	if (argc == 3) {

		inputDir[0] = '.';
		inputDir[1] = '\0';
		outputDir[0] = '.';
		outputDir[1] = '\0';

		samePlace = 0;
	}



	if (argc == 5) {
		if (strcmp(argv[3],"-d")==0 && strcmp(argv[1],"-c")==0 ) {
			strcpy(outputDir, argv[4]);
			strcpy(inputDir, argv[4]);
			flag = 2;
			samePlace = 0;
		}else if (strcmp(argv[1],"-d")==0 && strcmp(argv[3],"-c")==0) {
			strcpy(outputDir, argv[2]);
			strcpy(inputDir, argv[2]);
			flag = 4;
			samePlace = 0;
		}else if (strcmp(argv[3],"-o")==0 && strcmp(argv[1],"-c")==0) {
			inputDir[0] = '.';
			inputDir[1] = '\0';
			strcpy(outputDir, argv[4]);
			flag = 2;
		}else if (strcmp(argv[1],"-o")==0 && strcmp(argv[3],"-c")==0) {
			inputDir[0] = '.';
			inputDir[1] = '\0';
			strcpy(outputDir, argv[2]);
			/*if (argv[2][0]=='.'){
				getcwd(outputDir, sizeof(outputDir));
				strcat(outputDir, argv[2]);
			}else{
				strcpy(outputDir, argv[2]);
			}*/
			flag = 4;
		}else {
			write(err, error, sizeof(error));
			printf("%s\n", error);
			return 1;
		}


	}

	if (samePlace ==0) {};

	if (argc == 7) {
		if (strcmp(argv[3],"-d")==0) {
			strcpy(inputDir, argv[4]);
		}else if (strcmp(argv[1],"-d")==0) {
			strcpy(inputDir, argv[2]);
		}else if (strcmp(argv[5],"-d")==0) {
			strcpy(inputDir, argv[6]);
		}else{
			write(err, error, sizeof(error));
			printf("%s\n", error);
			return 1;
		};
		if (strcmp(argv[3],"-o")==0) {
			strcpy(outputDir, argv[4]);
		}else if (strcmp(argv[1],"-o")==0) {
			strcpy(outputDir, argv[2]);
		}else if (strcmp(argv[5],"-o")==0) {
			strcpy(outputDir, argv[6]);
		}else{
			write(err, error, sizeof(error));
			printf("%s\n", error);
			return 1;
		}
		if (strcmp(argv[3],"-c")==0) {
			flag = 4;
		}else if (strcmp(argv[1],"-c")==0) {
			flag = 2;
		}else if (strcmp(argv[5],"-c")==0) {
			flag = 6;
		}else{
			write(err, error, sizeof(error));
			printf("%s\n", error);
			return 1;
		}
	}

	if (outputDir[0]=='.') {
			char temp [1024];
			getcwd(temp, sizeof(temp));
			strcat(temp, "/");
			strcat(temp, outputDir);

			strcpy(outputDir, temp);

	}

	DIR *dir;
    dir = opendir(inputDir);
    if (!dir) {
    	char y[100] = "Unable to reach input directory\n";
    	write(err, y, sizeof(y));
    	printf("Error: Unable to reach input Directory: %s\n", outputDir);
    	return 1;
    }

    closedir(dir);

    dir = opendir(outputDir);
    if (!dir) {
    	char y[100] = "Unable to reach output directory\n";
    	write(err, y, sizeof(y));
    	printf("Error: Unable to reach output Directory: %s\n", outputDir);
    	return 1;
    }

    closedir(dir);

    char * key = malloc(sizeof(char)*100);
    strcpy(key, argv[flag]);


	char  validNames[1024] = {",color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes,"};
	char * needle = strstr(validNames, argv[flag]);
	
	if (needle==NULL) {
		fprintf(stderr, "Error. %s is not a valid column name ot be sorted on\n", key);
		printf("Error. %s is not a valid column name ot be sorted on\n", key);
		return 1;
	}else {
		if (needle[-1]==',') {
			if (needle[strlen(argv[flag])]==',') {

			}else {
				fprintf(stderr, "Error. %s is not a valid column name ot be sorted on\n", key);
				printf("Error. %s is not a valid column name ot be sorted on\n", key);
				return 1;
			}
		}
	}

	int  * key_column = (int *)malloc(sizeof(int));
	key_column[0] = column_num(key);
	

	
	int  isNum =0;

	if (strcmp(key, "num_critic_for_reviews")==0 ||  strcmp(key, "duration")==0 ||
		strcmp(key, "director_facebook_likes")==0 ||  strcmp(key, "actor_3_facebook_likes")==0 ||
		strcmp(key, "actor_1_facebook_likes")==0 ||  strcmp(key, "gross")==0 ||
		strcmp(key, "num_voted_users")==0 ||  strcmp(key, "cast_total_facebook_likes")==0 ||
		strcmp(key, "facenumber_in_poster")==0 ||  strcmp(key, "num_user_for_reviews")==0 ||
		strcmp(key, "budget")==0 ||  strcmp(key, "title_year")==0 ||
		strcmp(key, "actor_2_facebook_likes")==0 ||  strcmp(key, "imdb_score")==0 ||
		strcmp(key, "aspect_ratio")==0 ||  strcmp(key, "movie_facebook_likes")==0) {

		isNum = 1;
	}
	
	

	/*sort("movie_metadata.csv", "../SortedCSV/sorted_movie_metadata-sorted-movie_facebook_likes.csv", "movie_facebook_likes");*/
	
	
	int p;
	fflush(stdout);
	char *out_name= (char *)malloc(sizeof(char)* 1024);
	char * out_File = (char *)malloc(sizeof(char)* 1024);
	strcpy(out_File, outputDir);
	strcat(out_File, "/");
	strcpy(out_name, "AllFiles-sorted-");
	strcat(out_name, key);
	strcat(out_name, ".csv");
	strcat(out_File, out_name);
	FILE * f = fopen(out_File, "a");
	struct dataNode * start = (struct dataNode*)malloc(sizeof(struct dataNode));
	list_end_pointer = start;
	start->next=NULL;



	int * check = (int *)malloc(sizeof(int));
	check[0] = 0;
	int * parent = (int *)malloc(sizeof(int));
	parent[0]= 0;

	struct navDirArgs * a = (struct navDirArgs *) malloc (sizeof(struct navDirArgs));
   	a->dirPath = inputDir;
    a->key_column = key_column;
    a->out_File = out_File;
    a->check = check;
    a->parent = parent;


    struct t_id_node * node = (struct t_id_node *)malloc(sizeof(struct t_id_node));
    int * id = (int *)malloc(sizeof(int));
    id[0]=0;
    node->id = id;

    a->list = node;
    

    int thread = pthread_create(&node->thread, NULL, navDir, a);
    if (thread !=0) {
      	fprintf(stderr, "Error allocating thread\n");
        return 1;
   };

   	void * status = (void *)malloc(sizeof(struct t_id_node));


  	pthread_join(node->thread, status);

  	
  	

  


	start = mergeSort(start->next, isNum);





	struct dataNode * ptr = start;
	struct dataNode * prev = ptr;

	

	fprintf(f, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
	

	
	while (ptr!=NULL) {
		for (p=0; p<28; p++){
				if (strcmp(ptr->data[p], "spb139coded\0")==0){
				}else{
					fprintf(f, "%s", ptr->data[p]);
				}
			if (p<27) {
				fprintf(f, ",");
			}else {
				fprintf(f, "\n");
			}
		}
		ptr = ptr->next;
	}

	fclose(f);
	
	

	ptr = start->next;
	prev = ptr;

	int i;
	while (ptr!=NULL) {
		for (i=0; i<28; i++) {
			free(ptr->data[i]);
		}
		free(ptr->data);
		if (strcmp(ptr->key, "\0")!=0) {
			free(ptr->key);
		}
		
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}


	
	
	free(key);
  	free(inputDir);
  	free(outputDir);
   
   

	return 0;
}