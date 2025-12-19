.nr PS 11
.TL
An Overview of the source of RALpage
.AU
Crispin A. A. Goswell
.AI
Rutherford Appleton Laboratory,
Chilton, Didcot,
OXON OX11 0QX
.SH
INTRODUCTION
.LP
This document describes the program source of the
.B RALpage
postscript language interpreter.
It is mostly a road map to enable the reader to quickly become familiar with the source.
It is described in terms of source files, because these are conceptual modules in the ``C'' language.
.NH
THE MAIN INTERPRETER
.LP
.NH 2
config.c
.LP
This contains a function,
.B Init ,
which is called once during startup.
It initialises the other modules.
The order of initialisation is sometimes important, but this has been kept to a minimum.
This file also contains most of the implementation limits.
.NH 2
main.h main.c
.LP
These contain the
.B main
program and the body of the interpreter.
During startup, a number of files are read, notably
.I ../postscript/psrc ,
which completes the initialisation in the postscript language itself.
Some operators are defined in the language itself in this file.
.NH 2
control.c stack.c
.LP
.B control.c
contains most of the flow control operators for executing loops and conditions.
.LP
.B stack.c
contains code for handling stacks.
There are four stacks in the interpreter, all described in the Reference manual for the
language.
Three of them, the
.B
Operand, Execution
.R
and
.B Dictionary
stacks are stacks of PostScript objects, and use the same code.
The stack of
.B
Graphics states
.R
is handled specially in the (separate) graphics code, since there is no PostScript type to
represent graphics states.
.SH
Fundamental data types
.LP
PostScript is a complete programming language, having a conventional set of data types.
Graphics are treated as an extension.
.LP
.B
array.c boolean.c file.c dictionary.c math.c integer.c operator.c poly.c name.c real.c save.c string.c
.LP
The above files contain code for handling the type which forms the file name.
.B math.c
handles
.I polymorphic
maths functions, which take arguments of varying type.
.NH 2
Miscellaneous operators
.LP
There are a few miscellaneous PostScript operators which are confined to the files:
.B property.c ,
.B unix.c
and
.B misc.c .
.B unix.c
is a file of language extensions which are useful in a
.UX
environment.
.NH 2
Miscellaneous support functions.
.LP
.B str.h
declares string functions for systems which do not ordinarily have a file declaring them.
.B bcopy.c
is a definition of a functions in most system libraries, but not available universally.
.B malloc.c
is an implementation of the storage management functions for systems for which they are
very inefficient.
.NH
GRAPHICS FUNCTIONS
.LP
.B graphics.h
defines all the data types needed for graphics.
.NH 2
Colour
.LP
.B colour.c
and
.B colour.h
define colour conversion functions, which convert between RGB and HSL.
This is useful also for getting a grey value for a given colour, which can subsequently be
half-toned on a Black and White display.
.NH 2
Generation of Paths
.LP
Paths describe the outlines of shapes to be rendered.
These are either drawn as lines or filled with colour
.LP
.B
pat.c, path.c, point.h
and
.B path.h
define functions needed for building path data structures (outlines).
.LP
.B fill.c
defines routines for filling paths with colour.
The same code is used to handle clipping to arbitrary regions.
.LP
.B stroke.c
defines routines to draw around a path so that it can then be filled.
This is how lines are drawn.
A line has a finite thickness, and is thus an area fill.
Line drawing is done by area fill \- this is exposed to the PostScript programmer by an
operator which will return a path description of the outline of a line.
.B stroke.c
contains an optimisation to do thin lines using a line-drawing algorithm.
.LP
.B stroke.c
also contains code to do dashing of lines.
.NH 2
Drawing bitmapped images
.LP
.B image.c
contains the code to draw bitmapped gray-scale images at arbitrary scaling or rotation.
Normal cases are optimised so that they can be done quickly.
Rotated images are very slow.
.NH 2
Graphics state control
.LP
.B state.c
and
.B gsave.c
contains the routines which alter the graphics state, and manage the graphics context stack,
respectively.
.NH 2
Font support
.LP
.B font.c
and
.B font.h
contain the code which manages the definition and use of fonts within the interpreter.
.LP
.B cache.c
contains the code to manage the character cache.
Once a character outline has been rendered once into a bitmap, it can be copies into the framebuffer
whenever it is requested.
.NH 2
Device abstraction
.LP
.B device.c
and
.B device.h
provide the abstraction of an output context, to allow output primitives to work identically
in both the character cache and in the frame buffer.
.NH 2
Transformation matrix manipulation
.LP
All coordinates are in floating point, and are controlled with a transformation matrix.
This accumulates translations, rotations and scaling functions and provides a convenient
representation for those functions.
.LP
.B
mat.c, mat.h
.R
and
.B matrix.c
provide functions to manipulate these matrices.
.NH
DEVICE INDEPENDENCE
.LP
.B hard-interface.c
is actually documentation of a set of function calls which the interpreter needs to make to
drive a given device.
The functions it asks of the device are trivial on much hardware, in particular, most bitmapped
graphics frame buffers with an implementation of RasterOp (BitBlt).
This interface assumes that the device knows about colour.
The interpreter reduces all of the PostScript imaging model into rasterops and (optionally)
thin line drawing.
This makes it very easy to port.
.LP
.B hard.h
and
.B rop.h
define types and constants which are needed to communicate with the rest of the interpreter.
.LP
.B null.c
is a sample driver which does nothing.
It can be used to excercise the non-graphic parts of the interpreter.
.LP
.B pixrect.c
is a sample implementation for Sun frame buffers.
It writes straight onto the screen, without cooperating with any window system.
.LP
A driver for the X Window System Version 11 is available.
.NH
USING POSTSCRIPT ON A REMOTE FRAME BUFFER
.LP
.B adapter.c
is a driver for the interpreter, which instead of driving a frame buffer, serialises the
requests into a simple two-way byte-stream protocol.
This can be sent over any reliable connection to a program called
.B viewer ,
which interprets that byte-stream protocol back into standard driver requests.
Any driver which will drive the interpreter proper will drive the viewer on a remote device.
This allows the main part of the interpreter to run on a machine which does not have a display,
by allowing a network connection to be placed between interpreter and frame buffer.
.LP
.B protocol.c
and
.B protocol.h
define generic mechanisms for encapsulating driver requests into a byte stream.
.NH
LOW LEVEL PORTING LIBRARY
.LP
In order to making porting the interpreter easier,
a library of ``helper'' routines is available.
These are designed to provide trapezoid rendering, half-toning and multi-way rasterop
on devices which cannot perform these functions directly.
.LP
.B canon.c
and
.B canon.h
are generic implementations of simple requests which are the same almost everywhere.
.B paint.c
does multiway rasterop in colour.
.LP
.B screen.c
manages half-toning
.LP
.B trapezoid.c
renders trapezoids into scanline lists.
