# Project:   externgen

# Toolflags:
CC = cc
link = link
THROWBACK = -Throwback
CCflags = -c -depend !Depend -IC: $(THROWBACK)
C++flags = -c -depend !Depend -IC: $(THROWBACK)
linkflags = -aif -c++ -o $@
ObjAsmflags = $(THROWBACK) -NoCache -depend !Depend
CMHGflags = 
LibFileflags = -c -o $@
Squeezeflags = -o $@

OBJS = o.externfunc o.externvar \
       o.interface

LIBS = C:o.stubs

externgen:	$(OBJS) $(LIBS)
		$(link) $(linkflags) $(OBJS) $(LIBS)

# Static dependencies:   
.SUFFIXES: .c .o
.c.o:
        $(CC) $(CCflags) -o $@ $<


# Dynamic dependencies:
o.externfunc:	c.externfunc
o.externfunc:	C:h.stdio
o.externfunc:	C:h.stdlib
o.externfunc:	C:h.string
o.externfunc:	h.externfunc
o.externvar:	c.externvar
o.externvar:	C:h.stdio
o.externvar:	C:h.stdlib
o.externvar:	C:h.string
o.externvar:	h.externfunc
o.interface:	c.interface
o.interface:	C:h.stdio
o.interface:	C:h.stdlib
o.interface:	C:h.string
o.interface:	h.externfunc
