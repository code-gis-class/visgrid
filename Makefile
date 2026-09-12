PLATFORM = $(shell uname)

PROGRAMS = main

DEFAULT: $(PROGRAMS)

#### EXPERIMENTS MODE 
#### using -DNDEBUG turns asserts off. Use it only for timing. 
##CFLAGS = -m64 -Winline -DNDEBUG -O3 -Wall 

#DEBUG MODE 
CFLAGS = -m64 -Winline -O3 -Wall

CC = gcc -std=c99  

main:  main.o pixel_buffer.o grid.o 
	$(CC) -o $@ main.o pixel_buffer.o grid.o $(LIBS)


main.o: main.c grid.h pixel_buffer.h stb_image_write.h
	$(CC) $(CFLAGS) -o $@ -c main.c 

pixel_buffer.o: pixel_buffer.c pixel_buffer.h stb_image_write.h
	$(CC) $(CFLAGS) -o $@ -c pixel_buffer.c

grid.o: grid.h
	$(CC) $(CFLAGS) -o $@ -c grid.c

clean: 
	rm -f *.o $(PROGRAMS)
