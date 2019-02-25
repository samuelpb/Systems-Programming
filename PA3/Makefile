all: multiThreadSorter

multiThreadSorter: multiThreadSorter.c
	gcc -lpthread -Wall -Werror -fsanitize=address multiThreadSorter.c -o multiThreadSorter

clean:
	rm -rf multiThreadSorter