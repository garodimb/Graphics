all: main

model.o: model.cpp model.h rply/rply.h
	g++ $(CFLAGS) -c model.cpp
	
quaternion.o: quaternion.cpp quaternion.h
	g++ $(CFLAGS) -c quaternion.cpp

rply.o:	rply/rply.c rply/rply.h
	gcc $(CFLAGS) -c rply/rply.c
	
main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp
	
main: main.o quaternion.o rply.o model.o
	g++ $(CFLAGS) -o main main.o model.o quaternion.o rply.o -lGL -lGLU -lglut -lm

clean:
	rm *.o main
