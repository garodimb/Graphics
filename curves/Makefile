all: bezier_curve

bezier.o: bezier.cpp
	g++ -c bezier.cpp -lglut -lGL -lGLU -lm


main.o: main.cpp
	g++ -c main.cpp -lglut -lGL -lGLU -lm

bezier_curve: main.o bezier.o
	g++ -o bezier_curve main.o bezier.o -lGL -lGLU -lglut -lm
	
clean:
	rm *.o bezier_curve
