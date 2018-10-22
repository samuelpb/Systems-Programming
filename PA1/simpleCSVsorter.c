/*FILE THAT TAKES IN A CSV OF DATA, AND THEN SORTS THE DATA ON A GIVEN COLUMN
	Takes in 3 arguments:
		1) executable
		2) -c (means it is sorted by column)
		3) column name of the column to be sorted upon
	Stores all data in linked lists, and then uses mergeSort to ensure the list is in order by the given
		column name.
	Prints out all the data, in order by that column
*/


#include "simpleCSVsorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>






int main (int argc, char ** argv) {

	if (argc!=3) return 1;

	if (strcmp(argv[1], "-c") != 0) {
		fprintf(stderr, "Error. Invalid number of arguments\n");
		return 1;
	}

	FILE * fp;
	fp = stdin;

	int i =0, length =0, keySpot=0, inQuotes=0, isNum=0;
	char a = 'a';
	/*char * line = (char *)malloc(sizeof (char) * 1024);*/
	char * key = (char * )malloc (sizeof(char) * 100);
	if (!key) return 1;
	int counter =0, column =0;
	struct dataNode * start = (struct dataNode *)malloc(sizeof(struct dataNode));
	if (!start) return 1;
	struct dataNode * ptr = start;
	struct dataNode * prev = start;
	char * line = (char *)malloc (sizeof(char)*1024);
	if (!line) return 1;
	/*int rowCount =0;*/
	
	/*reads through first line and finds which column we are sorting by*/
	while (i==0) {
		fscanf(fp, "%c", &a);
		key[length] = a;
		length++;
		if (a == ','){
			counter ++;
			key[length-1] = '\0';
			length =0;
			if (strcmp(argv[2], key)==0){
				i=1;
				while (a!='\n') {
					fscanf(fp, "%c", &a);
					/*just read to end of header line*/
				}
				break;
			}
		}
		/*if there was no match to the inputted title of a column to be sorted on*/
		if (a == '\n') {
			counter++;
			key[length-1]='\0';
			int x;
			for (x=0; argv[2][x]!='\0'; x++) {
				i=1;
				if (argv[2][x]!=key[x]) {
					i=0;
					break;
				}
			}
			if (i==0) {
				fprintf(stderr, "Error. Not a valid column title.\n");
				return 1;
			}
		}
	}

	

	
	column = counter;

	/*if a field with a numerical value*/
	if ( counter == 3 || counter == 4 || counter == 5 || counter == 6 || counter == 8 || counter == 9 || counter == 13 || counter == 14
		|| counter == 16 || counter == 19 || counter == 23 || counter == 24 || counter == 25 || counter == 26 || counter == 27 || counter == 28) {

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
					key[keySpot-1] = '\0';
					strcpy(node->key, removeWhite(key, keySpot));
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
	
	/*print out the key values for a test*/
	ptr = start;
	while (ptr!=NULL) {
		
		printf("%s\n", ptr->data);
		/*printf("%s\n", ptr->data);*/
		ptr=ptr->next;
	}

	ptr = start; 
	while (ptr!=NULL){
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}

	free(line);
	free(key);



	return 0;
}
