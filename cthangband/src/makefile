# File: Makefile

# This is not a very "nice" Makefile, but it usually works.

#
# Note that you may have to make modifications below according
# to your machine, available libraries, compilation options,
# and your "visual module" of choice.  This Makefile is intended
# for use with Unix machines running X11, Curses, Ncurses, or Vt100,
# or possibly for "Atari" or "Amiga" computers with "Curses" ports,
# see below for more information.
#
# Note that "main-mac.c", the visual module for the Macintosh,
# must be compiled in a special way, see elsewhere.
#
# Note that "main-win.c", the visual module for Windows,
# must be compiled in a special way, see elsewhere.
#
# Note that "main-ibm.c", "main-emx.c", and "main-286.c",
# the visual modules for various types of IBM-PC computers,
# must be compiled with special Makefiles, see elsewhere.
#
# Note that "main-lsl.c", the visual module for ???,
# must be compiled with "Makefile.lsl", see elsewhere.
#
# Note that "main-acn.c", the visual module for Risc Acorn,
# must be compiled with "Makefile.acn", see elsewhere.
#
# Note that "Makefile.w31" and "Makefile.wat" are slight variations
# on "Makefile.win" and "Makefile.ibm", respectively, which allow
# the use of the "main-win.c" and "main-ibm.c" files, respectively,
# with special compilers.
#
# If you are able to construct "main-xxx.c" and/or "Makefile.xxx"
# files for a currently unsupported system, please send them to me
# (benh@voicenet.com) for inclusion in future versions.
#
# This Makefile comes with "default" dependancies that may be obsolete.
#


#
# The "source" and "object" files.
#

SRCS = \
  z-util.c z-virt.c z-form.c z-rand.c z-term.c \
  variable.c tables.c util.c cave.c \
  object1.c object2.c monster1.c monster2.c \
  xtra1.c xtra2.c spells1.c spells2.c \
  melee1.c melee2.c save.c files.c \
  cmd1.c cmd2.c cmd3.c cmd4.c cmd5.c cmd6.c \
  store.c birth.c load.c \
  wizard1.c wizard2.c \
  generate.c dungeon.c init1.c init2.c \
  quest.c \
  main-gcu.c main-x11.c main.c

OBJS = \
  z-util.o z-virt.o z-form.o z-rand.o z-term.o \
  variable.o tables.o util.o cave.o \
  object1.o object2.o monster1.o monster2.o \
  xtra1.o xtra2.o spells1.o spells2.o \
  melee1.o melee2.o save.o files.o \
  cmd1.o cmd2.o cmd3.o cmd4.o cmd5.o cmd6.o \
  store.o birth.o load.o \
  wizard1.o wizard2.o \
  generate.o dungeon.o init1.o init2.o \
  quest.o \
  main-gcu.o main-x11.o main.o



##
## Following are some "system" definitions
##
## No changes are needed to compile a version that will run on both
## X11 and Curses, in debugging mode, with maximal warnings, on many
## normal Unix machines of the Sun OS variety (non-solaris).
##
## To use an "alternative" definition, simply "modify" (or "replace")
## the definition below with one that you like.  For example, you can
## change the compiler to "cc", or remove the "debugging" options, or
## remove the X11 or Curses support, etc, as desired.
##
## See also "config.h" and "h-config.h" for important information.
##
## Some "examples" are given below, they can be used by simply
## removing the FIRST column of "#" signs from the "block" of lines
## you wish to use, and commenting out "standard" block below.
##
## This is not intended to be a "good" Makefile, just a "simple" one.
##


#
# This is my compiler of choice, it seems to work most everywhere
#
CC = gcc


##
## Graphics under X11 in linux.  (Run with the -g option)
## Note: Get the 16x16.bmp file, and put in game_directory/lib/xtra/graf
##       to get 256 colours.
##
CFLAGS = -W -Wall -O3 -fno-strength-reduce -pipe -g \
         -D"USE_X11" -D"USE_GCU" -D"USE_TRANSPARENCY"
LIBS = -lX11 -lcurses -ltermcap -L/usr/X11R6/lib $(PYLIB)




#
# Hack -- "install" as the base target
#

install: Cthangband
	mv Cthangband ..


#
# Build the "Angband" program
#

Cthangband: $(OBJS)
	$(CC) $(CFLAGS) -o Cthangband $(OBJS) $(LDFLAGS) $(LIBS)


#
# Clean up old junk
#

clean:
	\rm *.bak *.o


#
# Generate dependancies automatically
#

depend:
	makedepend -D__MAKEDEPEND__ $(SRCS)


#
# Hack -- some file dependencies
#

HDRS = \
  h-basic.h \
  h-define.h h-type.h h-system.h h-config.h

INCS = \
  angband.h \
  config.h defines.h types.h externs.h \
  z-term.h z-rand.h z-util.h z-virt.h z-form.h $(HDRS)


birth.o: birth.c $(INCS)
cave.o: cave.c $(INCS)
cmd1.o: cmd1.c $(INCS)
cmd2.o: cmd2.c $(INCS)
cmd3.o: cmd3.c $(INCS)
cmd4.o: cmd4.c $(INCS)
cmd5.o: cmd5.c $(INCS)
cmd6.o: cmd6.c $(INCS)
dungeon.o: dungeon.c $(INCS)
files.o: files.c $(INCS)
generate.o: generate.c $(INCS)
init1.o: init1.c $(INCS)
init2.o: init2.c $(INCS)
load.o: load.c $(INCS) loadsave.h
main-gcu.o: main-gcu.c $(INCS)
main-x11.o: main-x11.c $(INCS)
main.o: main.c $(INCS)
melee1.o: melee1.c $(INCS)
melee2.o: melee2.c $(INCS)
monster1.o: monster1.c $(INCS)
monster2.o: monster2.c $(INCS)
object1.o: object1.c $(INCS)
object2.o: object2.c $(INCS)
quest.o: quest.c $(INCS)
save.o: save.c $(INCS) loadsave.h
spells1.o: spells1.c $(INCS)
spells2.o: spells2.c $(INCS)
store.o: store.c $(INCS)
tables.o: tables.c $(INCS)
util.o: util.c $(INCS)
variable.o: variable.c $(INCS)
wizard1.o: wizard1.c $(INCS)
wizard2.o: wizard2.c $(INCS)
xtra1.o: xtra1.c $(INCS)
xtra2.o: xtra2.c $(INCS)
z-form.o: z-form.c $(HDRS) z-form.h z-util.h z-virt.h
z-rand.o: z-rand.c $(HDRS) z-rand.h
z-term.o: z-term.c $(HDRS) z-term.h z-virt.h
z-util.o: z-util.c $(HDRS) z-util.h
z-virt.o: z-virt.c $(HDRS) z-virt.h z-util.h

