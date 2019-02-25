/*HEADER FILE FOR THE sorter.C, THE CODE FROM PA0*/
	/*INCLUDES MERGESORT CODE*/

/*INCLUDES:

	Struct dataNode:
		Holds both the string of all the data for a row
		As well as the key, which is the string for the specific column to be sorted

	column_num :
		returns which column the data belongs in

	parseInt :
		takes a string and returns the numerical value of the string

	removeWhite :
		removes leading and trailing whitespace from a string

	testCSV:
		ensures the data file is valid and can be included in the output file

	merge:
		function to merge two linked lists in ascending order

	mergeSort:
		Uses merge to merge a linked list
*/

#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



/*function to remove leading and trailing whitespace*/
char * removeWhite (char * key, int length) {

	int i=0, start=0;
	char * result = (char *)malloc(sizeof(char)* length);

	/*finds index of fist non space*/
	for (i=0; key[i]!='\0'; i++) {
		if (key[i]!=' ') {
			start = i;
			break;
		}
	}

	/*ends the string after the last nonspaace*/
	for ( i = length-1; i>=0; i--) {
		if (key[i]!=' ' && key[i]!='\0'){
			key[i+1] = '\0';
			break;
		}
	}

	/*copies it into the result*/
	for (i=0; key[start+i]!='\0'; i++) {
		result[i] = key[i+start];
	}

	result[i] = '\0';

	return result;


}

/*takes in the column name and returns which one it is int he order*/
int column_num (char * key) {

	char * trimmed_key = (char * )malloc(sizeof(char)* strlen(key));
	if (!trimmed_key) {
		fprintf(stderr, "Error mallocing data\n");
		return -3;
	}
	trimmed_key = removeWhite(key, strlen(key)+1);

	int num=0;

	if (strcmp(trimmed_key, "color")==0) {
		num = 0;
	} else if (strcmp(trimmed_key, "director_name")==0) {
		num = 1;
	} else if (strcmp(trimmed_key, "num_critic_for_reviews")==0) {
		num = 2;
	} else if (strcmp(trimmed_key, "duration")==0) {
		num = 3;
	} else if (strcmp(trimmed_key, "director_facebook_likes")==0) {
		num = 4;
	} else if (strcmp(trimmed_key, "actor_3_facebook_likes")==0) {
		num = 5;
	} else if (strcmp(trimmed_key, "actor_2_name")==0) {
		num = 6;
	} else if (strcmp(trimmed_key, "actor_1_facebook_likes")==0) {
		num = 7;
	} else if (strcmp(trimmed_key, "gross")==0) {
		num = 8;
	} else if (strcmp(trimmed_key, "genres")==0) {
		num = 9;
	} else if (strcmp(trimmed_key, "actor_1_name")==0) {
		num = 10;
	} else if (strcmp(trimmed_key, "movie_title")==0) {
		num = 11;
	} else if (strcmp(trimmed_key, "num_voted_users")==0) {
		num = 12;
	} else if (strcmp(trimmed_key, "cast_total_facebook_likes")==0) {
		num = 13;
	} else if (strcmp(trimmed_key, "actor_3_name")==0) {
		num = 14;
	} else if (strcmp(trimmed_key, "facenumber_in_poster")==0) {
		num = 15;
	} else if (strcmp(trimmed_key, "plot_keywords")==0) {
		num = 16;
	} else if (strcmp(trimmed_key, "movie_imdb_link")==0) {
		num = 17;
	} else if (strcmp(trimmed_key, "num_user_for_reviews")==0) {
		num = 18;
	} else if (strcmp(trimmed_key, "language")==0) {
		num = 19;
	} else if (strcmp(trimmed_key, "country")==0) {
		num = 20;
	} else if (strcmp(trimmed_key, "content_rating")==0) {
		num = 21;
	} else if (strcmp(trimmed_key, "budget")==0) {
		num = 22;
	} else if (strcmp(trimmed_key, "title_year")==0) {
		num = 23;
	} else if (strcmp(trimmed_key, "actor_2_facebook_likes")==0) {
		num = 24;
	} else if (strcmp(trimmed_key, "imdb_score")==0) {
		num = 25;
	} else if (strcmp(trimmed_key, "aspect_ratio")==0) {
		num = 26;
	} else if (strcmp(trimmed_key, "movie_facebook_likes")==0) {
		num = 27;
	} else {
		num = -1;
	}


	free(trimmed_key);
	return num;

}


int alreadySorted (char * filename) {

	
	 int size =0;
	 size = (int)strlen(filename);
	 int i;
	 for (i = size-1; i>=0; i--) {
	 	if (filename[i]=='/') {
	 		break;
	 	}
	 }

	char * file = filename+i+1;

	char * isSorted = strstr(file, "-sorted-");
	if (isSorted!=NULL) {
		
	}else {
		/*not sorted*/
	
		return 0;
	}
	char * w;
	if (strlen(file)==strlen(isSorted)) {
		/*nothing before*/
		w = strstr(file+1, "-sorted-");
		if (w!=NULL) {
			isSorted=w;
		}else{
			return 0;
		}
	}

	if (strlen(isSorted)<=12)
 	{
 		/*nothing after -sorted- before csv*/
 		
 		return 0;
 	}

	size = strlen(file);

	return 1;


}

/*method to ensure the CSV file is a valid one that needs to be sorted
		0 if valid, 1 if not valid*/
int testCSV (char * filename) {

	int i=0;
	for (i=0; filename[i]!='\0'; i++) {};


	char  validNames[1024] = {",color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes,"};
	



	if (i<5) {
		fprintf(stderr, "%s: File is not a named csv file", filename);
		return 1;
	}
	if (filename[i-4]!='.' && (filename[i-3]!='c' || filename[i-3]!='C') 
		&& (filename[i-3]!='s' || filename[i-3]!='S') 
		&& (filename[i-3]!='V' || filename[i-3]!='v') ){
			fprintf(stderr, "%s: Not a valid CSV file extension\n", filename);
			return 1;
	}

	if (strstr(filename, "AllFiles-sorted-")!=NULL) {
		fprintf(stderr, "%s: CSV of All Files sorted. Will not include.\n", filename);
		return 1;
	}
	/*
	if (alreadySorted(filename)==1) {
			fprintf(stderr, "%s: File is already sorted\n", filename);
			return 1;
	}
	*/
	

	i=0;
	int index=0;
	int columns =0;
	int head = 0;
	int curr = 0;
	int inQuotes = 0;
	int lines = 0, enters = 0;
	char a;
	int c_counter = 0;
	char * test = (char *)malloc(sizeof(char)*1024);
	FILE * fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "%s: Error, unable to read file\n", filename);
		return 1;
	}



	while (i==0) {

		if (fscanf(fp, "%c", &a)==1) {

			c_counter = 1;
			test[index]=a;
			test[index+1] = '\0';
			
			index++;
			if (a==',') {
				if (lines==0) {
					test[index-1]='\0';
					/*int h;
					for (h=0; h<28; h++) {
						if (strcmp(test, validNames[h])==0) {
							index=-1;
							break;
						}
					}
					if (index!=-1) {
						
						fprintf(stderr, "%s: File has column of title %s with unknown data. Will not sort\n", filename, test);
						return 1;
					}*/
					strcpy(test, removeWhite(test, index));
					
					char * needle =	strstr(validNames, test);
					if(needle==NULL) {
							fprintf(stderr, "%s: File has column of title %s with unknown data. Will not sort\n", filename, test);
							return 1;
					}else {
						if (needle[-1]==',') {
							if (needle[strlen(test)]==',') {

							}else {
								fprintf(stderr, "%s: File has column of title %s with unknown data. Will not sort\n", filename, test);
								return 1;
							}
						}		
					}
				}
				index=0;
				if (inQuotes==0) {

					if (head ==0) {
						columns ++;
					};
						curr++;
				}
			}
			if (a=='\n') {
				if (lines==0) {
					int x=0;
					x=1;
					int h =0;
					test[index-1]='\0';
					strcpy(test, removeWhite(test, index));
					
					/*char * needle =	strstr(validNames, test);
					if(needle==NULL) {
							fprintf(stderr, "%s: File has column of title %s with unknown data. Will not sort\n", filename, test);
							return 1;
					}else {
						if (needle[-1]==',') {
							if (needle[strlen(test)]==',') {

							}else {
								fprintf(stderr, "%s: File has column of title %s with unknown data. Will not sort\n", filename, test);
								return 1;
							}
						}		
					}*/
					
					for (h=0; validNames[h]!='\0'; h++){
						if (validNames[h]==test[0]) {
							int g=0; 
							for (g=0; validNames[g+h]!='\0'; g++) {
								x=0;
								
								if (validNames[g+h+1]==',') {
									x=-1; break;
								}

								if (test[g+1]=='\0'){
									break;
								}
								if (validNames[g+h]==test[g]) {
									x=-1;
								}else{
									break;
								}
								
								
							}
							if (x==-1) {
								break;
							}
						}


					}

					if(x!= -1) {
						fprintf(stderr, "%s: File has column of title %s with unknown data. Will not sort\n", filename, test);
						return 1;
					}
				}
				index=0;
				enters++;
				head = 1;
				if (curr!=columns) {
					i=1;
					break;
				}
				curr=0;
			}else if (enters==lines+1 && a!=' ')  {
				lines++;
			}


			if (a=='"') {
				if (inQuotes==0) {
					inQuotes=1;
				}else {
					inQuotes=0;
				}
			}

		}else {
			if (c_counter == 0) {
				fprintf(stderr, "%s: Empty File", filename);
				return 1;
			}
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
		fprintf(stderr, "%s: Not a valid CSV file. Different number of column headers and columns\n", filename);
		return 1;
	}
	if (lines==0) {
		fprintf(stderr, "%s: Not a valid CSV file. No data\n", filename);
		return 1;
	}

	return 0;


}

/*commenting out unused functions for now*/

/*method to return the numerical value of a key if needed*/
float parseInt (char * key ) {
	
	if(strcmp(key, "spb139coded\0")==0) {
		return -1;
	}
	return strtof(key, NULL);
}

/*merge function to combine two linked lists*/
struct dataNode * merge(struct dataNode * first, struct dataNode * second, int isNum) {

	struct dataNode * start;

	if (first==NULL) return second;
	if (second==NULL) return first;

	if (isNum==1) {
		if (parseInt(first->key) <= parseInt(second->key)) {
			start = first;
			first = first->next;
		}else {
			start = second;
			second = second->next;
		}

	}else {
		if (strcmp(first->key, "spb139coded\0")==0) {
			first->key = "\0";
		}
		if (strcmp(second->key, "spb139coded\0")==0) {
			second->key = "\0";
		}
		if (strcmp(first->key, second->key)<=0) {
			start = first;
			first = first->next;
		}else {
			start = second;
			second = second->next;
		}
	}

	

	struct dataNode * ptr = start;

	while (first!=NULL || second!=NULL) {


		if (first==NULL) {
			ptr->next = second;
			second = second->next;
			ptr=ptr->next;
			continue;
		}

		if (second == NULL) {
			ptr->next = first;
			first = first->next;
			ptr = ptr->next;
			continue;
		}


		if (isNum == 1) {
			if (parseInt(first->key) <= parseInt(second->key)) {
				ptr->next = first;
				ptr=ptr->next;
				first = first->next;
			}else {
				ptr->next = second;
				ptr=ptr->next;
				second = second->next;
			}


		}else {
			if (strcmp(first->key, second->key) <=0) {
				ptr->next = first;
				ptr=ptr->next;
				first = first->next;
			}else {
				ptr->next = second;
				ptr=ptr->next;
				second = second->next;
			}
		}

	}





	return start;
}


/*mergeSort driver function*/
struct dataNode * mergeSort (struct dataNode * start, int isNum) {

	struct dataNode * ptr = start;
	struct dataNode * half = start;
	int counter = 0;

	if (start->next==NULL) {
		return ptr;
	}
	if (start==NULL) {
		return NULL;
	}

		

	while (ptr!=NULL) {

		ptr = ptr->next;
		counter++;
		if (counter%2==0 && ptr!=NULL) {
			half = half->next;
		}
		if (counter ==1 && ptr==NULL) {
			half = half->next;
		}
	}

	


	ptr = half->next;
	half->next = NULL;


	ptr = mergeSort(ptr, isNum);
	half = mergeSort(start, isNum);

	ptr = merge(half, ptr, isNum);

	

	return ptr;


};




