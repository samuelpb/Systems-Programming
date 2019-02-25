/* FILE WITH THE MAHJOORITY OF THE PA0 CODE
	SORTS THE CSV
	*/


/*will take in the ouput directory of the file, and a linked list of data
	also will take in the column heading to sort on*/




#include "sorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void print_info (int parent, char * fileName) {
	pthread_mutex_lock(&out_lock);

	int a = parent;
	a++;
	if (a == 5) {
	}

	printf("Initial PID: %d\n", getpid());
	printf("TIDS of all spawned threads: \n");
	printf("Total number of spawned threads from %s: 0\n\n", fileName);
	pthread_mutex_unlock(&out_lock);
}

void * add_file_to_list (void* voidPointer) {



	struct file_Args * info = (struct file_Args *)voidPointer;
	char * fileName = info->path;
	int * key_column = info->key_column;
	int * parent = info->parent;



	FILE * fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Unable to open file: %s\n", fileName);
		return NULL;
	}

	if (testCSV(fileName)!=0) {
		fclose(fp);
		print_info(*parent, fileName);
		return NULL;
	}


	char a = 'a';

	

	int i =0;
	int num_columns = 0;
	int column_counter =0;
	int inQuotes = 0;
	int key_length = 0;
	int one_line = 1;
	int location = 0;
	int num_data_lines=0;

	int column_locations[28];
	int present[28];

	for (i=0; i<28; i++) {
		column_locations[i]=42;
		present[i]=0;
	}
	i=0;

	char * key = (char *)malloc(sizeof(char)* 1024);
	if (!key) {
		fprintf(stderr, "%s: Error allocating memory\n", fileName);
		fclose(fp);
		return NULL;
	}


	/*reads the header line and maps the values to the order they should be in*/
	while (i==0) {

		if (fscanf(fp, "%c", &a)!=1) {
			if (one_line == 1) {
				fprintf(stderr, "%s: Error: file does not contain lines of data", fileName);
				print_info(*parent, fileName);
				fclose(fp);
				return NULL;
			}
		}else {
			if (a=='\"') {
				if (inQuotes==1) {
					inQuotes=0;
					key[key_length]='\"';
					key_length++;
				}else {
					inQuotes=1;
					key[key_length]='\"';
					key_length++;
				}
			}else if (a==',') {
				if (inQuotes == 1) {
					continue;
				}
				num_columns ++;
				key[key_length]='\0';
				location = column_num(key);
				column_locations[column_counter]= location;
				column_counter++;
				present[location]=1;
				/*in column_locations, the column of the file is mapped to where it should be stored*/
				key_length=0;
			}else if (a=='\n') {
				/*end of header line, break this loop after saving last value*/

				key[key_length]='\0';

				location = column_num(key);
				if (location<0) {
					fprintf(stderr, "%s: Invalid column name %s\n", fileName, key);
					print_info(*parent, fileName);
					fclose(fp);
					return NULL;
				}
				column_locations[column_counter]= location;
				present[location]=1;
				i=1;
				break;
			}else {
				if ((int)a==13) {
					continue;
				}
				key[key_length]=a;
				key_length++;
			}
		}


	}
	int line_length=0;
	/*now goes line by line and adds the data to the link list*/
	i=0;
	num_columns = column_counter;

	
	struct dataNode * start = (struct dataNode *)malloc(sizeof(struct dataNode));
	start->next = NULL;


	struct dataNode * ptr = start;
	struct dataNode  * prev = start;

	while (i==0) {
		line_length = 0;
		key_length = 0;
		inQuotes =0;
		column_counter = 0;
		struct dataNode * node = (struct dataNode *)malloc(sizeof(struct dataNode));
		if (!node) {
			fprintf(stderr, "%s: Error allocating memory\n", fileName);
			fclose(fp);
			return NULL;
		}
		node->next =NULL;
		ptr->next = node;
		prev = ptr;
		ptr = node;
		node->data = (char **)malloc(sizeof(char *)* 28);
		if (!node->data) {
			fprintf(stderr, "%s: Error allocating memory\n", fileName);
			fclose(fp);
			return NULL;
		}

		while (i==0) {

			if (fscanf(fp, "%c", &a)!=1) {
				if (num_data_lines==0) {
					fprintf(stderr, "%s: Error no data lines in file\n", fileName);
					fclose(fp);
					return NULL;
				}else {
					i=2;
					if (line_length==0) {
						i=3;
						ptr = prev;
						ptr->next=NULL;
						break;
					}
					key[key_length] = '\0';
					node->data[column_locations[column_counter]] = (char *)malloc(sizeof(char)*(key_length+1));
					if (!node->data[column_locations[column_counter]]){
						fprintf(stderr, "%s: Error no data lines in file\n", fileName);
						fclose(fp);
						return NULL;
					}
					strcpy(node->data[column_locations[column_counter]], key);
					key_length = 0;

				}
			} else if (a==',') {
				line_length++;
				if (inQuotes==1) {
					continue;
				}
				num_data_lines=1;
				key[key_length]='\0';
				node->data[column_locations[column_counter]] = (char *)malloc(sizeof(char)*(key_length+1));
				if (!node->data[column_locations[column_counter]]){
					fprintf(stderr, "%s: Error no data lines in file\n", fileName);
					print_info(*parent, fileName);
					fclose(fp);
					return NULL;
				}
				strcpy(node->data[column_locations[column_counter]], key);
				key_length = 0;
				column_counter++;
			} else if(a=='\"') {
				line_length++;
				num_data_lines=1;
				key[key_length]=a;
				key_length++;
				if (inQuotes==0) {
					inQuotes =1;
				}else {
					inQuotes = 0;
				}
			} else if (a=='\n') {
				line_length++;
				num_data_lines=1;
				i=1;
				key[key_length] = '\0';
				node->data[column_locations[column_counter]] = (char *)malloc(sizeof(char)*(key_length+1));
				if (!node->data[column_locations[column_counter]]){
					fprintf(stderr, "%s: Error no data lines in file\n", fileName);
					print_info(*parent, fileName);
					fclose(fp);
					return NULL;
				}
				strcpy(node->data[column_locations[column_counter]], key);
				key_length = 0;
			}else {
				line_length++;
				if ((int)a==13) {
					continue;
				}

				num_data_lines=1;
				key[key_length]=a;
				key_length++;
			}
		}

		if (i==3) {
			break;
		}
		
		
		int n;

		



		for (n=0; n<28; n++) {
			if (present[n]==0) {
				node->data[n] = (char *)malloc(sizeof(char)*12);
				if (node->data[n]==NULL) {
					fprintf(stderr, "%s: Error allocating memory\n", fileName);
					fclose(fp);
					return NULL;
				}
				strcpy(node->data[n], "spb139coded\0");
			
			}
		}

		/*this line*/
		node->key = (char *)malloc(sizeof(char)* ((int)strlen(node->data[*key_column])+1));
		
		if (!node->key) {
			fprintf(stderr, "%s: Error allocating memory\n", fileName);
			fclose(fp);
			return NULL;
		}

		strcpy(node->key, node->data[*key_column]);
		node->key = removeWhite(node->key, strlen(node->key)+1);
		



		if (i==2) {
			break;
		}else{
			i=0;
		}


	}
	pthread_mutex_lock(&list_lock);

	list_end_pointer->next = start->next;
	list_end_pointer = ptr;


	pthread_mutex_unlock(&list_lock);

	print_info(*parent, fileName);

	struct t_id_node * status = (struct t_id_node *)malloc(sizeof(struct t_id_node));
	status = NULL;

	pthread_exit((void *)status);
}