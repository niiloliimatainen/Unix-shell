wish:	wish.o built-ins.o
	gcc wish.o built-ins.o -o wish -Wall -Werror
wish.o:	wish.c	wish.h
	gcc wish.c -c -Wall -Werror
built-ins.o:	built-ins.c	wish.h
	gcc built-ins.c -c -Wall -Werror
