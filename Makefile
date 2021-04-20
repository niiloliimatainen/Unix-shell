wish:	wish.o built-ins.o
	gcc -static -g wish.o built-ins.o -o wish -Wall -Werror 
wish.o:	wish.c	wish.h
	gcc -static -g wish.c -c -Wall -Werror 
built-ins.o:	built-ins.c	wish.h
	gcc -static -g built-ins.c -c -Wall -Werror 
