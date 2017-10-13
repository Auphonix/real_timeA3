# Detect OS
UNAME:=$(shell uname)

# Change libs based on OS
ifeq ($(UNAME), Darwin) # Mac
LIBS = -framework Carbon -framework OpenGL -framework SDL2 -framework GLUT
# Fedora
else
LIBS = -lGL -lGLU -lm -lglut -lGLEW `sdl2-config --cflags` `sdl2-config --libs`
endif

LIBPATH = -F /Library/Frameworks
INCPATH = -I include
GCC = gcc
CFLAGS = -Wno-deprecated -g

SRC=$(wildcard src/*.c)

a3: $(SRC)
	@echo "Building a3"
	$(GCC) -o $@ $^ $(CFLAGS) $(INCPATH) $(LIBPATH) $(LIBS)

clean:
	$(RM) a3











	ifeq ($(UNAME), Darwin) # MAC MAKE
	CPP=clang
	CFLAGS=-g -framework Carbon -framework OpenGL -framework GLUT -Wno-deprecated
	else
	CPP=g++
	CFLAGS=-g -lGL -lGLU -lglut -lGLEW `sdl2-config --cflags` `sdl2-config --libs`
	endif
	a2make: sine_wave.cpp shaders.c
		$(CPP) -o a2 sine_wave.cpp shaders.c $(CFLAGS)
