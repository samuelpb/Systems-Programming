/*HEADER FILE FOR scannerCSVsorter.c*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "simple.c"
#include <sys/types.h>
#include <sys/wait.h>




/*function to return if a file is a CSV file*/
int testCSV (char * filename, char* key) {

	int i=0;
	for (i=0; filename[i]!=0; i++) {};

	char sorted[100];
	strcpy(sorted, "-sorted-");
	strcat(sorted, key);
	strcat(sorted, ".csv");


	if (i<5) {
		return 1;
	}
	if (filename[i-4]!='.' && (filename[i-3]!='c' || filename[i-3]!='C') 
		&& (filename[i-3]!='s' || filename[i-3]!='S') 
		&& (filename[i-3]!='V' || filename[i-3]!='v') ){
			fprintf(stderr, "%s: Not a valid CSV file\n", filename);
			return 1;
	}

	for (i=0; filename[i]!='\0'; i++) {
		if (filename[i]=='-') {
			if (strcmp(sorted, &filename[i])==0) {
				fprintf(stderr, "%s: File already sorted\n", filename);
				return 1;
			}
		}
	}

	i=0;
	int columns =0;
	int head = 0;
	int curr = 0;
	int inQuotes = 0;
	char a;

	FILE * fp = fopen(filename, "r");

	while (i==0) {

		if (fscanf(fp, "%c", &a)==1) {
			if (a==',') {
				if (inQuotes==0) {


					if (head ==0) {
						columns ++;
					};
						curr++;
				}
			}
			if (a=='\n') {
				head = 1;
				if (curr!=columns) {
					i=1;
					break;
				}
				curr=0;
			}
			if (a=='"') {
				if (inQuotes==0) {
					inQuotes=1;
				}else {
					inQuotes=0;
				}
			}

		}else {
			if (curr!=columns) {
				if (curr!=0) {
					i=1;
				}
			}
			break;
		}

	}

	fclose(fp);
	if (i==1) {
		printf("%s: Not a valid CSV file. Different number of column headers and columns\n", filename);
		return 1;
	}

	return 0;


}



/*function to navigate through the directory, calls fork, sort*/

void navDir (char * directory, char * key, char * outputDir, int samePlace, char * info) {

	int counter = 0;
	
	/*printf("%s\n", directory);*/
	DIR *dir;
    struct dirent *dp;
    dir = opendir(directory);
   	
   	FILE * fp = fopen(info, "a");


    while ((dp=readdir(dir)) != NULL) {
    	
    	if (strcmp(dp->d_name, ".")==0 || strcmp(dp->d_name, "..")==0) {
    		continue;

    	};

    	if (strcmp(dp->d_name, "info.txt")==0) {
    		continue;
    	}

    	/*if a directory, fork a process, and recursively navigate*/
        if (dp->d_type == 4) {
        	
        	int p = fork();
        	counter++;
        	if (p==0) {
        		
        		

        		/*printf("%d, ", getpid());
        		fflush(stdout);*/
        		char path[1024];
        		path[0] = '\0';
        		strcpy(path, directory);
        		strcat(path, "/");
        		strcat(path, dp->d_name);

        		fprintf(fp, "%d, ", getpid());
        		/*fprintf(fp, "%d %s\n, ", getpid(), path);*/

        		if (samePlace==0){
        			char outPath[1024];
        			
        			strcpy(outPath, outputDir);
        			strcat(outPath, "/");
        			strcat(outPath, dp->d_name);

        			navDir(path, key, outPath, samePlace, info);
        			
        		
        		}else{
        		
        			navDir(path, key, outputDir, samePlace, info);
        			
        		}
        	
        		exit(0);
       		 }else {
       		 	wait(0);
       		 }
       		 /*if a csv file, fork a process and sort*/
        }else {
        		char path[1024];
        		strcpy(path, directory);
        		strcat(path, "/");
        		strcat(path, dp->d_name);


        	if (testCSV(path, key)==0) {
       			
        		counter ++;
        		int p = fork();
        		if (p==0) {
        			
        			/*fprintf(fp, "%d %s\n, ", getpid(), path);*/
        			fprintf(fp, "%d, ", getpid());
        			/*printf("%d, ", getpid());
        			fflush(stdout);*/
        			sort(path, outputDir, key);
        			exit(0);
        		}else{
        			wait(0);
        		}
        	}
        }
        
 
    }

    closedir(dir);
    fclose(fp);
    

    return;





}
