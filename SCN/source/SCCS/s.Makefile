h52562
s 00142/00000/00000
d D 1.1 89/01/15 23:50:35 mic 1 0
c date and time created 89/01/15 23:50:35 by mic
e
u
U
t
T
I 1
#
#	For SUN3s FPFLAGS=-f68881 OR -fswitch
#	For SUN4s FPFLAGS=-Dfloat=double
#

OBJECTS=array.o boolean.o config.o control.o dictionary.o file.o \
	integer.o main.o math.o misc.o name.o operator.o \
	poly.o property.o real.o save.o stack.o string.o unix.o

LIBS=libww.a -lsuntool -lsunwindow -lpixrect -g

GRAPHICS=cache.o colour.o device.o fill.o font.o gsave.o image.o mat.o matrix.o\
	pat.o path.o state.o stroke.o

DEFAULT_LIBRARY = /usr/local/lib/postscript

FPFLAGS =
CCFLAGS =	-O
CFLAGS =	$(CCFLAGS) $(FPFLAGS)
LDFLAGS =

all::
	@echo No default set
	@false

SUNBINS = x11PS sunPS

# -----------------------------------------------
sun2::
	make ${SUNBINS}

# -----------------------------------------------
sun4::
	make FPFLAGS=-Dfloat=double CCFLAGS="" ${SUNBINS}

# -----------------------------------------------
sun3::
	make FPFLAGS=-f68881 ${SUNBINS}

# -----------------------------------------------
# X11 monochrome PS
#
X11LIB = -lX11
# X11INCLUDES = -I/usr/src/X11/X11 -I/usr/src/X11

x11PS: $(OBJECTS) $(GRAPHICS) X11.o canon_x11.o canon.a 
	rm -f x11PS
	cc $(LDFLAGS) -o x11PS $(OBJECTS) $(GRAPHICS) X11.o canon_x11.o canon.a -lm $(X11LIB)

X11.o: X11.c
	cc -c $(CFLAGS) $(X11INCLUDES) X11.c

canon_x11.o:	canon.c
	cc -c $(CFLAGS) -DX11 canon.c
	mv canon.o canon_x11.o

# -----------------------------------------------
# X10 monochrome PS
#
X10LIB= -lX10
# X10INCLUDES = -I/usr/src/X11/X11 -I/usr/src/X11
x10PS: $(OBJECTS) $(GRAPHICS) X10.o canon.a 
	rm -f x10PS
	cc -o x10PS $(OBJECTS) $(GRAPHICS) X10.o canon.a -lm $(X10LIB)

X10.o: X10.c
	cc -c $(CFLAGS) $(X10INCLUDES) X10.c

# -----------------------------------------------
# Default PS
#
#PS:	$(OBJECTS) $(GRAPHICS) hard.o canon.a
#	cc $(CFLAGS)  $(OBJECTS) $(GRAPHICS) hard.o canon.a -lm `libs` -o PS

# -----------------------------------------------
# Sun pixrect PS
#
sun:	sunPS

sunPS:	$(OBJECTS) $(GRAPHICS) pixrect.o canon_sun.o canon.a
	cc $(LDFLAGS) $(OBJECTS) $(GRAPHICS) pixrect.o canon_sun.o canon.a -lm -lpixrect -o sunPS

canon_sun.o:	canon.c
	cc -c $(CFLAGS) canon.c
	mv canon.o canon_sun.o

# -----------------------------------------------
# Color WW PS
#
CPS:	$(OBJECTS) $(GRAPHICS) colour-ww.o trapezoid.o canon.o
	cc $(CFLAGS)  $(OBJECTS) $(GRAPHICS) colour-ww.o canon.o trapezoid.o -lm `libs` -o CPS

# -----------------------------------------------
# Generic PS which talks to a viewer
#
postscript:	$(OBJECTS) $(GRAPHICS) adapter.o protocol.o
	cc $(CFLAGS) $(OBJECTS) $(GRAPHICS) adapter.o protocol.o -lm -o postscript

# -----------------------------------------------

config.o:	config.c
	cc -c $(CFLAGS) -DDEFAULT_LIBRARY=\"$(DEFAULT_LIBRARY) config.c

canon.a:	screen.o trapezoid.o paint.o
	ar ruv canon.a screen.o trapezoid.o paint.o
	ranlib canon.a

# -----------------------------------------------
# Build seperate viewer for each output device type
#
viewer:	protocol.o viewer.o hard.o canon.a
	cc protocol.o viewer.o hard.o canon.a `libs` -o viewer

# -----------------------------------------------

orion:	orion.o installorion orionlib

orionlib:
	echo 'echo -lG' >libs; chmod +x libs

installorion:
	cp orion.o hard.o

# -----------------------------------------------

ww:	ww.o wwlib installww

wwlib:
	if [ -f libww.a ]; \
	then \
		echo 'echo libww.a' >lww; \
	else \
		echo 'echo -lww' >lww; \
	fi; \
	chmod +x lww
	echo "echo `lww`" >libs; chmod +x libs

installww:
	cp ww.o hard.o

clean::
	rm -f *.o *.a postscript *PS
E 1
