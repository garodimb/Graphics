SRC		:= src
INCLUDE := include/
LDIR 	:= lib
ODIR	:= obj
LIBS	:= -lm -lGL -lGLU -lglut
INCFLAG	:= -I$(INCLUDE) -I$(LDIR)/
CXX		:= g++
CC		:= gcc
MKDIR_P := mkdir -p
OUT_DIR := obj
_OBJ 	:= model.o view.o controller.o cube.o quaternion.o trackball.o texture.o camera.o main.o plyfile.o scenenode.o matrix.o animation.o
OBJ 	:= $(patsubst %,$(ODIR)/%,$(_OBJ))

MKDIR	:= mkdir obj
vpath %.cpp src
vpath %.c lib

all: directories main

$(ODIR)/%.o:	%.cpp
		$(CXX) -c -o $@ $< $(CXXFLAGS) $(LIBS) $(INCFLAG)

$(ODIR)/%.o: 	%.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS) $(INCFLAG)

main:	$(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) $(INCFLAG)

directories: $(OUT_DIR)

${OUT_DIR}:
			${MKDIR_P} ${OUT_DIR}
clean:
	-$(RM) $(ODIR)/*.o main