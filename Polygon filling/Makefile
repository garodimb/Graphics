all: Polygon.o testPoly.bin

Polygon.o: Polygon.cpp
	g++ -c Polygon.cpp -lglut -lGL -lGLU

testPoly.bin: testPoly.cpp Polygon.o
	g++ -o testPoly.bin testPoly.cpp Polygon.o -lglut -lGL -lGLU
