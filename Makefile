IDIR = include
ODIR = obj
SDIR = src

CC = gcc
CFLAGS = -g -std=c11 -I$(IDIR)
#CFLAGS = -g -Wall -Wextra -std=c11 -I$(IDIR)

PROG = scheduling

_DEPS = utilities.h list.h olist.h bstree.h instance.h schedule.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ= utilities.o list.o olist.o bstree.o instance.o schedule.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: run all remake clean delete

all : $(PROG)

$(PROG) : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(ODIR)/%.o : $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

run : all
	./$(PROG)

clean :
	rm -f $(ODIR)/*.o

delete : clean
	rm $(PROG)