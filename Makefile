wish:	wish.o built-ins.o
	gcc -g wish.o built-ins.o -o wish -Wall -Werror 
wish.o:	wish.c	wish.h
	gcc -g wish.c -c -Wall -Werror 
built-ins.o:	built-ins.c	wish.h
	gcc -g built-ins.c -c -Wall -Werror 
