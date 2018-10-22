/* FILE WITH THE MAHJOORITY OF THE PA0 CODE
	SORTS THE CSV
	*/


/*FILE THAT TAKES IN A CSV OF DATA, AND THEN SORTS THE DATA ON A GIVEN COLUMN

	Takes in 3 arguments:
		1)filepath
		2)outputDir path
		3)column header

	Stores all data in linked lists, and then uses mergeSort to ensure the list is in order by the given
		column name.
	Prints out all the data, in order by that column

*/


#include "simple.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>






void sort (char * fileName, char* outputDir, char * header) {

	

	FILE * fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Unable to open file: %s\n", fileName);
	}

	int i =0, length =0, keySpot=0, inQuotes=0, isNum=0;
	char a = 'a';
	/*char * line = (char *)malloc(sizeof (char) * 1024);*/
	char * key = (char * )malloc (sizeof(char) * 100);
	char * firstline = (char *)malloc(sizeof(char)*1024);
	int firstlength =0;
	if (!firstline) return;
	if (!key) return;
	int counter =0, column =0;
	struct dataNode * start = (struct dataNode *)malloc(sizeof(struct dataNode));
	if (!start) return;
	struct dataNode * ptr = start;
	struct dataNode * prev = start;
	char * line = (char *)malloc (sizeof(char)*1024);
	if (!line) return;
	/*int rowCount =0;*/
	

	
	/*reads through first line and finds which column we are sorting by*/
	while (i==0) {
		if (fscanf(fp, "%c", &a)==1) {
		key[length] = a;
		firstline[firstlength] = a;
		firstlength++;
		length++;
		if (a == ','){
			counter ++;

			key[length-1] = '\0';
			length =0;
			if (strcmp(header, key)==0){
				i=1;
				while (a!='\n') {
					if (fscanf(fp, "%c", &a)==1){
						firstline[firstlength] = a;
						firstlength++;
					}
					firstline[firstlength]='\0';
					/*just read to end of header line*/
				}
			
				
				break;
			}
		}

	
		/*if there was no match to the inputted title of a column to be sorted on*/
		if (a == '\n') {
			firstline[firstlength]='\0';
			counter++;
			key[length-1]='\0';
			int x;
			for (x=0; header[x]!='\0'; x++) {
				i=1;
				if (header[x]!=key[x]) {
					i=0;
					break;
				}
			}
			if (i==0) {
				fprintf(stderr, "Error. Not a valid column title for the file.\n");
				return;
			}
		}
	}else {
			fprintf(stderr, "Not a valid CSV file, only one line/empty");
			return;
		}
	}

	

	
	column = counter;

	/*if a field with a numerical value*/
	if (strcmp(header, "num_critic_for_reviews")==0 ||  strcmp(header, "duration")==0 ||
		strcmp(header, "director_facebook_likes")==0 ||  strcmp(header, "actor_3_facebook_likes")==0 ||
		strcmp(header, "actor_1_facebook_likes")==0 ||  strcmp(header, "gross")==0 ||
		strcmp(header, "num_voted_users")==0 ||  strcmp(header, "cast_total_facebook_likes")==0 ||
		strcmp(header, "facenumber_in_poster")==0 ||  strcmp(header, "num_user_for_reviews")==0 ||
		strcmp(header, "budget")==0 ||  strcmp(header, "title_year")==0 ||
		strcmp(header, "actor_2_facebook_likes")==0 ||  strcmp(header, "imdb_score")==0 ||
		strcmp(header, "aspect_ratio")==0 ||  strcmp(header, "movie_facebook_likes")==0) {

		isNum = 1;
	}

	i=0;
	a = 'a';

	/*reads through each line and puts the key into a node in the linked list, as well as all the data*/
	/*new line loop*/
	while (i==0) {

		struct dataNode * node = (struct dataNode *)malloc(sizeof(struct dataNode));
		prev = ptr;
		ptr->next = node;
		node->next = NULL;
		ptr = node;
		length = 0;
		counter = 1;
		keySpot = 0;
		inQuotes = 0;

		/*reads through for commas*/
		while (a!='\n') {

			/*if EOF is reached, end line and key(if that is the targetted column)*/
			if (fscanf(fp, "%c", &a)!=1) {
				i=1;
				line[length] = '\0';
					strcpy(node->data, line);
				if (column == counter) {
					if (keySpot>0) {
						key[keySpot-1] = '\0';
						strcpy(node->key, removeWhite(key, keySpot));
					}
				}
				break;
			}else {
				line[length] = a;
				length++;
			}

			if (inQuotes==1 && a=='\"') {
				inQuotes = 0;
			} else if (a=='\"') {
				inQuotes = 1;
			}
			
			/*if the right column*/
			if (counter == column) {
				key[keySpot]=a;
				keySpot++;
				if (inQuotes==0) {
					if (a==',' || a=='\n') {
						key[keySpot-1] = '\0';
						strcpy(node->key, removeWhite(key, keySpot));
					}
				}
			}

			/*keep track of what counter to be on*/
			if (a==',' && inQuotes==0) counter ++;



			
		}

		line[length]='\0';
		strcpy(node->data, line);
		a='a';


	}


	prev->next = NULL;

	start = mergeSort(start->next, isNum);
	
	fclose(fp);

	char * outFile = createOutputName(outputDir, fileName, header);

	FILE * fp2 = fopen(outFile, "w");
	if (!fp2) {
		fprintf(stderr, "Unable to open write file in: %s\n", outputDir);
		return;
	}
	/*print out the key values for a test*/
	ptr = start;

	
	fprintf(fp2, "%s", firstline);

	while (ptr!=NULL) {
		
		fprintf(fp2, "%s", ptr->data);
		/*printf("%s\n", ptr->data);*/
		ptr=ptr->next;
	}

	fclose(fp2);

	ptr = start; 
	while (ptr!=NULL){
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}

	free(line);
	free(key);
	free(firstline);



	return;
}

