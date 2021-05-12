IDIR = include
ODIR = obj
SDIR = src
JDIR = java

CC = gcc
CFLAGS = -Wall -fPIC -I$(IDIR)
LIB = $(JDIR)/libmylibrary.so

#JINCLUDES = -I/usr/lib/jvm/java-8-openjdk-amd64/ -I/usr/lib/jvm/java-8-openjdk-amd64/include/ -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux/
JINCLUDES = -I/usr/lib/jvm/java-11-openjdk-amd64/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/linux/

_DEPS = application_MyClass.h utilities.h list.h olist.h bstree.h instance.h schedule.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = utilities.o list.o olist.o bstree.o instance.o schedule.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(OBJ) $(ODIR)/application_MyClass.o
	$(CC) -shared -o $(LIB) $^

$(ODIR)/application_MyClass.o : $(SDIR)/application_MyClass.c $(DEPS)
	$(CC) $(CFLAGS) $(JINCLUDES) -c -o $@ $<

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(LIB)
