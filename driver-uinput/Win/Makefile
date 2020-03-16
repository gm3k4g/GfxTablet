# Specify compiler
CC=gcc
# Specify linker
LINK=gcc
# Specify application name
TARGET=networktablet

# Build all target
.PHONY : all
all : networktablet_win

# Link object files + dependent libraries into binary
networktablet_win : networktablet_win.o
	$(LINK) -o $(TARGET) networktablet_win.o -lstdc++ #?

# Compile source files into object files
networktablet_win.o: networktablet_win.c
	$(CC) -c networktablet_win.c -o networktablet_win.o

# Clean target/built objects
.PHONY : clean
clean:
	rm -f networktablet_win.o $(TARGET) 