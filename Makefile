# Using the gcc compiler
CC = gcc

# Compile-time flags
CFLAGS = -Wall -g

# Include external header files
INCLUDES = -I../nsh

# Define the source files
SRCS = shellMain.c commands.c customCommands.c

# Define the object files
OBJS = $(SRCS:.c=.o)

# Define the name of the executable file
MAIN = nsh

# Build the executable
default: $(MAIN)
		 @echo Compiled nsh project successfully

# Files required to maintain the executable
$(MAIN): $(OBJS)
		 $(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)

# Compiles all .c files into .o files 
.c.o:
		 $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up the old project
clean:
		 $(RM) *.o *~ $(MAIN)

