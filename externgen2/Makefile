# Project:   externgen

# Toolflags:
CC = gcc
link = gcc
THROWBACK = -Throwback
CCflags = -c -g -O0 -W -Wall

OBJS = externfunc.o externvar.o \
       interface.o

INCS = stdio.h stdlib.h string.h externfunc.h
LIBS = -lc

# LIBS = C:o.stubs

externgen:	$(OBJS) $(LIBS)
		$(link) $(linkflags) -o externgen.out $(OBJS) $(LIBS)

# Static dependencies:   
.SUFFIXES: .c .o
.c.o:
	$(CC) $(CCflags) -o $@ $<


# Dynamic dependencies:
o.externfunc:	externfunc.c	$(INCS)
o.externvar:	externvar.c	$(INCS)
o.interface:	interface.c	$(INCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o angband $(OBJS) $(LIBS)
