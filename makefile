LIBS = -framework Carbon -framework OpenGL -framework SDL2 -framework GLUT
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
