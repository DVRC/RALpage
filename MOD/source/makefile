OBJECTS=array.o boolean.o config.o control.o dictionary.o file.o\
	integer.o main.o math.o misc.o name.o operator.o\
	poly.o property.o real.o save.o stack.o string.o unix.o
LIBS=libww.a -lsuntool -lsunwindow -lpixrect -g
XLIB=/usr/lib/libX11.a
GRAPHICS=cache.o colour.o device.o fill.o font.o gsave.o image.o mat.o matrix.o\
	pat.o path.o state.o stroke.o

SOURCES=array.c boolean.c config.c control.c dictionary.c file.c\
	integer.c main.c math.c misc.c name.c operator.c\
	poly.c property.c real.c save.c stack.c string.c unix.c\
	cache.c colour.c device.c fill.c font.c gsave.c image.c mat.c matrix.c\
	pat.c path.c state.c stroke.c
LINT=lint
LFLAGS= -lm


# For SUN with 68881
#CFLAGS=-O -f68881

# For others
CFLAGS=-O

default: sunPS

xps: $(OBJECTS) $(GRAPHICS) X11.o canon.a $(XLIB)
	rm -f xps
	$(CC) -o xps $(CFLAGS) $(OBJECTS) $(GRAPHICS) X11.o canon.a -lm $(XLIB)

PS:	$(OBJECTS) $(GRAPHICS) hard.o canon.a
	$(CC) $(CFLAGS)  $(OBJECTS) $(GRAPHICS) hard.o canon.a -lm `libs` -o PS

sunPS:	$(OBJECTS) $(GRAPHICS) pixrect.o canon.a
	$(CC) $(CFLAGS)  $(OBJECTS) $(GRAPHICS) pixrect.o canon.a -lm -lpixrect -o sunPS

CPS:	$(OBJECTS) $(GRAPHICS) colour-ww.o trapezoid.o canon.o
	$(CC) $(CFLAGS)  $(OBJECTS) $(GRAPHICS) colour-ww.o canon.o trapezoid.o -lm `libs` -o CPS

postscript:	$(OBJECTS) $(GRAPHICS) adapter.o protocol.o
	$(CC) $(CFLAGS) $(OBJECTS) $(GRAPHICS) adapter.o protocol.o -lm -o postscript

XPS:	$(OBJECTS) $(GRAPHICS) X.o
	$(CC) $(CFLAGS)  $(OBJECTS) $(GRAPHICS) X.o -lm -lX -o XPS

lint:
	$(LINT) $(LFLAGS) $(SOURCES) $(LINTFILTER)

canon.a:	canon.o screen.o trapezoid.o paint.o
	ar ruv canon.a canon.o screen.o trapezoid.o paint.o
	ranlib canon.a

viewer:	protocol.o viewer.o hard.o canon.a
	$(CC) protocol.o viewer.o hard.o canon.a `libs` -o viewer

all:	PS postscript viewer

ww:	ww.o wwlib installww

sun:	ww wwsun

orion:	orion.o installorion orionlib

X.o:	
	$(CC) -c X.c

wwlib:
	if [ -f libww.a ]; \
	then \
		echo 'echo libww.a' >lww; \
	else \
		echo 'echo -lww' >lww; \
	fi; \
	chmod +x lww
	echo "echo `lww`" >libs; chmod +x libs

wwsun:	
	echo "echo `lww` -lsuntool -lsunwindow -lpixrect" >libs; chmod +x libs

orionlib:
	echo 'echo -lG' >libs; chmod +x libs

installww:
	cp ww.o hard.o

installorion:
	cp orion.o hard.o
