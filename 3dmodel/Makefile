all: main

cube.o: cube.cpp cube.h
	g++ $(CFLAGS) -c cube.cpp -lglut -lGL -lGLU -lm

quaternion.o: quaternion.cpp quaternion.h
	g++ $(CFLAGS) -c quaternion.cpp -lglut -lGL -lGLU -lm
	
main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp -lglut -lGL -lGLU -lm

main: main.o cube.o quaternion.o
	g++ $(CFLAGS) -o main main.o cube.o quaternion.o -lGL -lGLU -lglut -lm
	
clean:
	rm *.o main
