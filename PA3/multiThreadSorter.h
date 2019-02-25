

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include "sorter.c"


struct sublists {
    struct  t_id_node * list;
    struct sublists * next;
};


void * navDir (void* voidPointer) {

	struct navDirArgs * info = (struct navDirArgs*) voidPointer;

	char * dirPath = info->dirPath ;
	int key_column =  *info->key_column;
	char * out_File = info->out_File;
    /*int parent = *(info->parent);*/

    pthread_mutex_lock(&check_lock);
    int * check = info->check;

    


  

    if (check[0]==0) {
        finished = 1;
        check[0] = 7;
    }else {
        check[0]=1;
    }

   /* int z = check[0];*/

    pthread_mutex_unlock(&check_lock);
    
    int counter = 0;


	struct t_id_node * start = info->list;

    struct t_id_node * ptr = start;
   	struct t_id_node * prev = start;
	start->next = NULL;
	
	/*printf("%s\n", directory);*/
	DIR *dir;
    struct dirent *dp;
    dir = opendir(dirPath);
    if (!dir) {
    	fprintf(stderr, "Unable to open directory %s\n", dirPath);
    	return NULL;
    }
   	

    struct sublists * subs = (struct sublists *)malloc(sizeof(struct sublists));
    if (!subs) {
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }


    struct sublists * subsptr = subs;
    subs->next = NULL;










    while ((dp=readdir(dir)) != NULL) {
    	
    	if (strcmp(dp->d_name, ".")==0 || strcmp(dp->d_name, "..")==0 || (strcmp(dp->d_name, ".DS_Store")==0)) {
    		continue;

    	};

    	

    	/*if a directory, fork a process, and recursively navigate*/
        if (dp->d_type == 4) {
        		
        		finished ++;

                  int * type = (int *)malloc(sizeof(int));
                    type[0] = 1;

        		/*printf("%d, ", getpid());
        		fflush(stdout);*/
        		char * path = (char *)malloc(sizeof(char)*1024);
        		if (!path) {
        			fprintf(stderr, "Error allocating memory\n");
        			return NULL;
        		}
        		
        		strcpy(path, dirPath);
        		strcat(path, "/");
        		strcat(path, dp->d_name);


                struct t_id_node * list = (struct t_id_node *)malloc(sizeof(struct t_id_node));
                if (!list) {
                    fprintf(stderr, "Error: could not allocate data\n");
                    return NULL;
                }

                struct sublists * x = (struct sublists *)malloc(sizeof(struct sublists));
                x->list = list;
                x->next = NULL;
                subsptr->next = x;
                subsptr = x;

        		struct navDirArgs * a = (struct navDirArgs *) malloc (sizeof(struct navDirArgs));
        		a->dirPath = path;
        		a->key_column = &key_column;
        		a->out_File = out_File;
                a->check = check;
                a->list = list;


        		struct t_id_node * node = (struct t_id_node *)malloc(sizeof(struct t_id_node));
        		ptr->next = node;
        		node->next = NULL;
                node->status= type;
        		ptr = node;
                pthread_mutex_lock(&counter_lock);
        		t_counter ++;
                int * id = (int *)malloc(sizeof(int));
                id[0] = t_counter;
                node->id = id;
                a->parent = id;
                pthread_mutex_unlock(&counter_lock);

        		int thread = pthread_create(&node->thread, NULL, navDir, a);
        		if (thread !=0) {
        			fprintf(stderr, "Error allocating thread\n");
        			return NULL;
        		};



       		 /*if a csv file, fork a process and sort*/
        }else {
        		char * path = (char *)malloc(sizeof(char)*1024);
        		if (!path) {
        			fprintf(stderr, "Error allocating memory\n");
        			return NULL;
        		}
        		strcpy(path, dirPath);
        		strcat(path, "/");
        		strcat(path, dp->d_name);

        		
        		if (strcmp(dp->d_name, out_File)==0) {
        			free(path);
        			continue;
        		}

        		
        		finished ++;

                int * type = (int *)malloc(sizeof(int));
                type[0] = 0;
        			
        		/*fprintf(fp, "%d %s\n, ", getpid(), path);*/
        	
        		/*printf("%d, ", getpid());
        		fflush(stdout);*/
        		/*ptr->next = add_file_to_list(path, key_column);*/

        		struct file_Args * a = (struct file_Args *)malloc(sizeof(struct file_Args));
        		if (!a) {
        			fprintf(stderr, "Error allocating memory\n");
        			return NULL;
        		}

        		a->path = path;
        		a->key_column = &key_column;

        		struct t_id_node * node = (struct t_id_node *)malloc(sizeof(struct t_id_node));
        		ptr->next = node;
                node->status= type;
        		node->next = NULL;
        		ptr = node;
                pthread_mutex_lock(&counter_lock);
                t_counter ++;
                int * id = (int *)malloc(sizeof(int));
                id[0] = t_counter;
                node->id = id;
                a->parent = id;
                pthread_mutex_unlock(&counter_lock);

        		int thread = pthread_create(&node->thread, NULL, add_file_to_list, a);
        		if (thread !=0) {
        			fprintf(stderr, "Error allocating thread\n");
        			return NULL;
        		};
        		
        }
        
 
    }

    /*wait for the processes to finish*/
    ptr = start->next;

    void * status = (void *)malloc(sizeof(struct t_id_node));
 
   


    while (ptr!=NULL) {

      
        counter ++;
    	

        prev = ptr->next;


    	pthread_join(ptr->thread, status);

        if (ptr->status[0]==1) {
        
        }else {
            ptr->next = prev;
        }

    

    	ptr->next = prev;

        if (ptr->next != NULL) {  
            ptr = ptr->next;
        }else {
            break;
        }
         
    
    
    }
  
    
    printf("Initial PID: %d\n", getpid());
    finished = 0;

    pthread_mutex_lock(&out_lock);
    printf("TIDs of all spawned threads: ");
    subsptr = subs->next;
    if (subsptr!=NULL) {
        struct t_id_node * deep = subsptr->list->next;
        ptr->next = deep;
        while (subsptr!=NULL) {
         while (deep!=NULL) {
            counter++;
             
             if (deep->next!=NULL) {
                 deep = deep->next;
             }else {
                ptr = deep;
                break;
             }
         }
         subsptr= subsptr->next;
         if (subsptr!=NULL) {

             deep = subsptr->list->next;
             ptr->next = deep;
             ptr = deep;
          }
        }   
    }

    ptr = start->next;

    if (counter!=0) {
     
     while (ptr!=NULL) {

        printf("%d, ", *(ptr->id));
        ptr = ptr->next;


     }

    

        printf("\b\b \b\n");
        printf("Total number of threads spawned from %s: %d\n\n", dirPath, counter);
       

    }else{
        printf("No files in %s, so 0 threads spawned in subdirectory.\n", dirPath);
    }
    
     pthread_mutex_unlock(&out_lock);

    ptr = start;

   

    closedir(dir);
  
    
   
   
    pthread_exit(start->next);


}