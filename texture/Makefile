all: main

model.o: model.cpp model.h rply/ply.h
	g++ $(CFLAGS) -c model.cpp

view.o: view.cpp view.h 
	g++ $(CFLAGS) -c view.cpp

controller.o: controller.cpp controller.h
	g++ $(CFLAGS) -c controller.cpp
	
cube.o: cube.cpp cube.h
	g++ $(CFLAGS) -c cube.cpp
	
quaternion.o: quaternion.cpp quaternion.h
	g++ $(CFLAGS) -c quaternion.cpp

trackball.o: trackball.cpp trackball.h
	g++ $(CFLAGS) -c trackball.cpp

plyfile.o:	rply/plyfile.c rply/ply.h
	gcc $(CFLAGS) -c rply/plyfile.c

main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp
	
main: main.o quaternion.o plyfile.o model.o view.o controller.o cube.o trackball.o
	g++ $(CFLAGS) -o main main.o model.o view.o controller.o cube.o quaternion.o plyfile.o trackball.o -lGL -lGLU -lglut -lm

clean:
	rm *.o main
