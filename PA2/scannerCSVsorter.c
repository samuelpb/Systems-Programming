 /*./sorter -c movie_title -d thisdir -o thatdir*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "scannerCSVsorter.h"


int main (int argc, char ** argv) {

	char* inputDir= (char *)malloc(sizeof(char)*1024);
	char * outputDir= (char *)malloc(sizeof(char)*1024);
	if (!inputDir || !outputDir) {
		fprintf(stderr, "Error allocating memory\n");
		return 1;
	}
	int samePlace = -1;

/*SECTION TO CHECK FOR CORRECT PARAMETERS INPUTTED IN PROGRAM CALL*/

	if (argc!= 3 && argc!=5 && argc!=7) {

		fprintf(stderr, "Input Format: ./sorter -c movie_title -d thisdir -o thatdir. -c is necessary. -d is optional, but if there then there must be an input directory path name. -o is optional, but if there then there must be an output direcotry\n");
		return 1;
	}

	if (strcmp(argv[1], "-c")!=0) {
		fprintf(stderr, "Must have -c flag as second arg, and must be sorting on column");
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
		if (strcmp(argv[3],"-d")==0) {
			strcpy(inputDir, argv[4]);
			strcpy(outputDir, argv[4]);
			samePlace = 0;
		}else if (strcmp(argv[3],"-o")==0) {
			inputDir[0] = '.';
			inputDir[1] = '\0';
			strcpy(outputDir, argv[4]);
		}else {
			fprintf(stderr, "Input Format: ./sorter -c movie_title -d thisdir -o thatdir. -c is necessary. -d is optional, but if there then there must be an input directory path name. -o is optional, but if there then there must be an output direcotry\n");
			return 1;
		}
	}

	if (argc == 7) {
		if (strcmp(argv[3],"-d")==0) {
			strcpy(inputDir, argv[4]);
		}else {
			fprintf(stderr, "Input Format: ./sorter -c movie_title -d thisdir -o thatdir. -c is necessary. -d is optional, but if there \nthen there must be an input directory path name. -o is optional, but if there then there must be an output direcotry\n");
			return 1;
		};
		if (strcmp(argv[5],"-o")==0) {
			strcpy(outputDir, argv[6]);
		}else {
			fprintf(stderr, "Input Format: ./sorter -c movie_title -d thisdir -o thatdir. -c is necessary. -d is optional, but if there\nthen there must be an input directory path name. -o is optional, but if there then there must be an output direcotry\n");
			return 1;
		}
	}




	/*sort("movie_metadata.csv", "../SortedCSV/sorted_movie_metadata-sorted-movie_facebook_likes.csv", "movie_facebook_likes");*/
	int masterPID = getpid();
	
	
	fflush(stdout);
	
	char a = 'a';
	char info[1024];
	strcpy(info, inputDir);
	strcat(info, "/");
	strcat(info, "info.txt");
	FILE * f = fopen(info, "a");
	fclose(f);


	navDir(inputDir, argv[2], outputDir, samePlace, info);
	
	wait(0);
	int pids[255];
	int k;
	int duplicate=0;


	f=fopen(info, "r");
	if (getpid()==masterPID) {

		int count=0; int id=0;
		printf("Initial PID: %d\n", masterPID);
  		printf("PIDS of all child processes: ");
  		

  		while (fscanf(f, "%c", &a)==1) {
  			if (a==',') {
  				for (k=0; k<count; k++) {
  					if (id == pids[k]) {
  						duplicate =1;
  					}
  				}
  				if (duplicate!=1) {

  					pids[count] = id;
  					count ++;
  				}
  				duplicate = 0;
  				id = 0;
  			}else if (a!= ' ') {
  				id = id*10;
  				id += (int)a - 48;
  			}
  		}
  		for (k=0; k<count; k++) {
  			printf("%d", pids[k]);
  			if (k<count-1) {
  				printf(", ");
  			}
  		}
  		printf("\n");
  		printf("Total number of processes: %d\n", count+1);

  	}else {
  		exit(0);
  	}

    fclose(f);
   	remove(info);

	return 0;
}