all: curves

bezier.o: bezier.cpp bezier.h
	g++ $(CFLAGS) -c bezier.cpp -lglut -lGL -lGLU -lm

lagrange.o:	lagrange.cpp lagrange.h
	g++ $(CFLAGS) -c lagrange.cpp -lglut -lGL -lGLU -lm

main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp -lglut -lGL -lGLU -lm

curves: main.o bezier.o lagrange.o
	g++ $(CFLAGS) -o curves main.o bezier.o lagrange.o -lGL -lGLU -lglut -lm
	
clean:
	rm *.o curves
