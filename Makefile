prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin

#OBJS specifies which files to compile as part of the project
OBJS = state_monitor.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w `pkg-config --cflags gtk+-3.0`

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = `pkg-config --libs gtk+-3.0`

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = limo

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

install:
	install -d $(bindir)
	install -m 0755 $(OBJ_NAME)  $(bindir)

uninstall:
	rm $(bindir)/$(OBJ_NAME)

clean:
	rm -rf $(OBJ_NAME)
