#
# makefile for goswell's postscript interpreter
#
DEPEND=/usr/athena/makedepend
CFLAGS = -O -DX11 -fno-function-cse -fcaller-saves -pg # -q loops -q novolatile
CC = gcc
#CC = /mit/gnu/vaxbin/gcc-1.27.1

SRCS =	array.c boolean.c cache.c colour.c config.c control.c device.c \
	dictionary.c file.c fill.c font.c gsave.c image.c integer.c \
	main.c mat.c math.c matrix.c misc.c name.c operator.c \
	pat.c path.c poly.c property.c real.c stack.c state.c \
	string.c stroke.c unix.c 

OBJS =	array.o boolean.o cache.o colour.o config.o control.o device.o \
	dictionary.o file.o fill.o font.o gsave.o image.o integer.o \
	main.o mat.o math.o matrix.o misc.o name.o operator.o \
	pat.o path.o poly.o property.o real.o stack.o state.o \
	string.o stroke.o unix.o

XLIBINCLUDES = -I/usr/include/X11

all:	xps

xps:	${OBJS} X11.o # canon.a
	rm -f xps
	${CC} ${CFLAGS} -o xps ${OBJS} X11.o -lX11 -lm
#	strip xps

lint:
	lint -DX11 ${SRCS} X11.c -lX11 -lm

#postps:	${OBJS} X.o
#	${CC} ${CFLAGS} -o postps ${OBJS} X.o /usr/lib/X10/libX.a -lm

canon.a:	canon.o screen.o trapezoid.o paint.o
	ar ruv canon.a canon.o screen.o trapezoid.o paint.o
	ranlib canon.a

X11.o:	X11.c
	${CC} -c ${CFLAGS} ${XLIBINCLUDES} X11.c

clean:
	/bin/rm -f xps postps *.o

depend.old:
	for i in ${SRCS} X11.c; do \
	    cc -M $$i | \
	    awk ' { if ($$1 != prev) \
		{ if (rec != "") print rec; rec = $$0; prev = $$1; } \
		else { if (length(rec $$2) > 78) { print rec; rec = $$0; } \
		else rec = rec " " $$2 } } \
		END { print rec } ' >> makedep; done
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm eddep makedep
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

depend:
	${DEPEND} -v -I. ${XLIBINCLUDES} -s'# DO NOT DELETE' ${SRCS} X11.c

# Dependencies!  Ha HA!
# DO NOT DELETE THIS LINE -- make depend uses it

array.o: main.h
# main.h includes:
#	stdio.h
#	math.h
#	assert.h
#	setjmp.h
#	strings.h
array.o: /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
array.o: /usr/include/setjmp.h /usr/include/strings.h
boolean.o: main.h /usr/include/stdio.h /usr/include/math.h
boolean.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
cache.o: main.h /usr/include/stdio.h /usr/include/math.h
cache.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
cache.o: graphics.h
# graphics.h includes:
#	device.h
#	point.h
#	font.h
cache.o: device.h
# device.h includes:
#	hard.h
cache.o: hard.h
# hard.h includes:
#	mat.h
#	rop.h
#	colour.h
#	path.h
cache.o: mat.h rop.h colour.h path.h point.h font.h
colour.o: main.h /usr/include/stdio.h /usr/include/math.h
colour.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
colour.o: graphics.h device.h hard.h mat.h rop.h colour.h path.h point.h
colour.o: font.h
config.o: main.h /usr/include/stdio.h /usr/include/math.h
config.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
control.o: /usr/include/signal.h main.h /usr/include/stdio.h
control.o: /usr/include/math.h /usr/include/assert.h /usr/include/setjmp.h
control.o: /usr/include/strings.h graphics.h device.h hard.h mat.h rop.h
control.o: colour.h path.h point.h font.h
device.o: main.h /usr/include/stdio.h /usr/include/math.h
device.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
device.o: graphics.h device.h hard.h mat.h rop.h colour.h path.h point.h
device.o: font.h
dictionary.o: main.h /usr/include/stdio.h /usr/include/math.h
dictionary.o: /usr/include/assert.h /usr/include/setjmp.h
dictionary.o: /usr/include/strings.h
file.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
file.o: /usr/include/setjmp.h /usr/include/strings.h
fill.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
fill.o: /usr/include/setjmp.h /usr/include/strings.h graphics.h device.h
fill.o: hard.h mat.h rop.h colour.h path.h point.h font.h
font.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
font.o: /usr/include/setjmp.h /usr/include/strings.h graphics.h device.h
font.o: hard.h mat.h rop.h colour.h path.h point.h font.h
gsave.o: main.h /usr/include/stdio.h /usr/include/math.h
gsave.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
gsave.o: graphics.h device.h hard.h mat.h rop.h colour.h path.h point.h
gsave.o: font.h
image.o: main.h /usr/include/stdio.h /usr/include/math.h
image.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
image.o: graphics.h device.h hard.h mat.h rop.h colour.h path.h point.h
image.o: font.h
integer.o: /usr/include/signal.h main.h /usr/include/stdio.h
integer.o: /usr/include/math.h /usr/include/assert.h /usr/include/setjmp.h
integer.o: /usr/include/strings.h
main.o: /usr/include/signal.h main.h /usr/include/stdio.h /usr/include/math.h
main.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
mat.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
mat.o: /usr/include/setjmp.h /usr/include/strings.h graphics.h device.h
mat.o: hard.h mat.h rop.h colour.h path.h point.h font.h
math.o: /usr/include/signal.h main.h /usr/include/stdio.h /usr/include/math.h
math.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
matrix.o: main.h /usr/include/stdio.h /usr/include/math.h
matrix.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
matrix.o: graphics.h device.h hard.h mat.h rop.h colour.h path.h point.h
matrix.o: font.h
misc.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
misc.o: /usr/include/setjmp.h /usr/include/strings.h /usr/include/sys/param.h
# /usr/include/sys/param.h includes:
#	machine/machparam.h
#	signal.h
#	sys/types.h
misc.o: /usr/include/machine/machparam.h /usr/include/signal.h
misc.o: /usr/include/sys/types.h /usr/include/sys/times.h
name.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
name.o: /usr/include/setjmp.h /usr/include/strings.h
operator.o: main.h /usr/include/stdio.h /usr/include/math.h
operator.o: /usr/include/assert.h /usr/include/setjmp.h
operator.o: /usr/include/strings.h
pat.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
pat.o: /usr/include/setjmp.h /usr/include/strings.h graphics.h device.h
pat.o: hard.h mat.h rop.h colour.h path.h point.h font.h
path.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
path.o: /usr/include/setjmp.h /usr/include/strings.h graphics.h device.h
path.o: hard.h mat.h rop.h colour.h path.h point.h font.h
poly.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
poly.o: /usr/include/setjmp.h /usr/include/strings.h
property.o: main.h /usr/include/stdio.h /usr/include/math.h
property.o: /usr/include/assert.h /usr/include/setjmp.h
property.o: /usr/include/strings.h
real.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
real.o: /usr/include/setjmp.h /usr/include/strings.h
stack.o: main.h /usr/include/stdio.h /usr/include/math.h
stack.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
state.o: main.h /usr/include/stdio.h /usr/include/math.h
state.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
state.o: graphics.h device.h hard.h mat.h rop.h colour.h path.h point.h
state.o: font.h
string.o: main.h /usr/include/stdio.h /usr/include/math.h
string.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
stroke.o: main.h /usr/include/stdio.h /usr/include/math.h
stroke.o: /usr/include/assert.h /usr/include/setjmp.h /usr/include/strings.h
stroke.o: graphics.h device.h hard.h mat.h rop.h colour.h path.h point.h
stroke.o: font.h
unix.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
unix.o: /usr/include/setjmp.h /usr/include/strings.h
X11.o: main.h /usr/include/stdio.h /usr/include/math.h /usr/include/assert.h
X11.o: /usr/include/setjmp.h /usr/include/strings.h graphics.h device.h
X11.o: hard.h mat.h rop.h colour.h path.h point.h font.h
X11.o: /usr/include/X11/Xlib.h
# /usr/include/X11/Xlib.h includes:
#	sys/types.h
#	X11/X.h
X11.o: /usr/include/sys/types.h /usr/include/X11/X.h /usr/include/X11/Xutil.h
X11.o: /usr/include/X11/Xatom.h
