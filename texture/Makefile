all: main

lib = lib/
model.o: model.cpp model.h lib/ply.h
	g++ $(CFLAGS) -c model.cpp -I. -I$(lib)

view.o: view.cpp view.h
	g++ $(CFLAGS) -c view.cpp -I. -I$(lib)

controller.o: controller.cpp controller.h
	g++ $(CFLAGS) -c controller.cpp -I. -I$(lib)

cube.o: cube.cpp cube.h
	g++ $(CFLAGS) -c cube.cpp -I. -I$(lib)

quaternion.o: quaternion.cpp quaternion.h
	g++ $(CFLAGS) -c quaternion.cpp -I. -I$(lib)

trackball.o: trackball.cpp trackball.h
	g++ $(CFLAGS) -c trackball.cpp -I. -I$(lib)

plyfile.o:	lib/plyfile.c lib/ply.h
	gcc $(CFLAGS) -c lib/plyfile.c -I$(lib)

main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp -I. -I$(lib)

main: main.o quaternion.o plyfile.o model.o view.o controller.o cube.o trackball.o
	g++ $(CFLAGS) -o main main.o model.o view.o controller.o cube.o quaternion.o plyfile.o trackball.o -lGL -lGLU -lglut -lm

clean:
	rm *.o main
