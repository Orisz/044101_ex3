#This is the Makefile for the ADT project
CC = gcc
CFLAGS = -g -Wall
CCLINK = $(CC)
LIBS =
OBJS = cluster.o list.o main.o person.o point.o
RM = rm -f
# Creating the executable (ADT)
ADT: $(OBJS)
	$(CCLINK) -o ADT $(OBJS) $(LIBS)

# Creating object files using default rules
cluster.o: cluster.c defs.h list.h point.h cluster.h
list.o: list.c defs.h list.h
main.o: main.c list.h defs.h point.h cluster.h person.h
person.o: person.c defs.h list.h person.h
point.o: point.c defs.h list.h point.h
# Cleaning old files before new make
clean:
	$(RM) ADT *.o *.bak *~ "#"* core
