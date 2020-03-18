# Specify compiler
CC=gcc
# Specify linker
LINK=gcc
# Specify application name
TARGET=networktablet

# Build all target
.PHONY : all
all : networktablet_win

# Link object files + dependent libraries into binary #lstdc++: lib_standard;#lws2_32: lib_winsock32_2;
networktablet_win : networktablet_win.o
	$(LINK) -o $(TARGET) networktablet_win.o -lstdc++ -lws2_32

# Compile source files into object files
networktablet_win.o: networktablet_win.c
	$(CC) -c networktablet_win.c -o networktablet_win.o

# Clean target/built objects
.PHONY : clean
clean:
	rm -f networktablet_win.o $(TARGET) 