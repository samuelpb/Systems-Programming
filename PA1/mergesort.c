/*mergesort used in ASST0, sorts on the string in the node in an array list*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node {

	char key [30];
	struct node * next;
	int num;
};

struct node * merge(struct node * first, struct node * second) {

	struct node * start;

	if (first==NULL) return second;
	if (second==NULL) return first;


	if (strcmp(first->key, second->key)<=0) {
		start = first;
		first = first->next;
	}else {
		start = second;
		second = second->next;
	}

	

	struct node * ptr = start;

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





	return start;
}


struct node * mergeSort (struct node * start) {

	struct node * ptr = start;
	struct node * half = start;
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


	ptr = mergeSort(ptr);
	half = mergeSort(start);

	ptr = merge(half, ptr);

	

	return ptr;


};


/*this was used for my own tests*/
int main (int argc, char** argv) {

	char * data []= {"3.2", "3.15", "3.5", "21.6", "sam", "night", "code", "yes", "7", "but"};
	int i =0;
	struct node * start = (struct node *)malloc(sizeof(struct node));
	struct node * ptr = start;
	start->next = NULL;
	for (i=0; i<10; i++ ) {

		struct node * a = (struct node *)malloc(sizeof(struct node));
		ptr->next = a;
		strcpy(a->key, data[i]);
		ptr = a;
		a->next = NULL;
		a->num = i;
	}


	start->next = mergeSort(start->next);
	ptr = start->next;

	while ( ptr !=NULL) {
		printf("%s %d\n", ptr->key, ptr->num);
		ptr = ptr->next;
	}

	while ( ptr !=NULL) {
		printf("%s\n", ptr->key);
		ptr = ptr->next;
	}

	return 0;
}
