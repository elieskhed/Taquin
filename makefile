Taquin : bloc.o main.o
	gcc -o Taquin bloc.o main.o `sdl-config --cflags --libs`

bloc.o : bloc.c bloc.h
	gcc -c bloc.c
 
main.o : main.c
	gcc -c main.c
clean :
	rm -f *.o

