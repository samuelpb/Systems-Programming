

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct dataNode {

	char key [100];
	char data [1024];
	struct dataNode * next;
};

/*function to remove leading and trailing whitespace*/
char * removeWhite (char * key, int length) {

	int i=0, start=0;
	char * result = (char *)malloc(sizeof(char)* length);
	for (i=0; key[i]!='\0'; i++) {
		if (key[i]!=' ') {
			start = i;
			break;
		}
	}

	for ( i = length-1; i>=0; i--) {
		if (key[i]!=' ' && key[i]!='\0'){
			key[i+1] = '\0';
			break;
		}
	}

	for (i=0; key[start+i]!='\0'; i++) {
		result[i] = key[i+start];
	}

	result[i] = '\0';

	return result;


}

/*return a strings numerical value*/
float parseInt (char * key ) {
	
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
		if (counter ==1 & ptr==NULL) {
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
