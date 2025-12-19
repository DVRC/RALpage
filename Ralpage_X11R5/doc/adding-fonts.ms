.ds Ps P\s-2OST\s+2S\s-2CRIPT\0\s+2
.na
.TL
Adding fonts to RALpage
.AU
Crispin A. A. Goswell
.AI
Rutherford Appleton Laboratory
Chilton, Didcot
OXON OX11 0QX
.NH
THE DISK LIBRARY
.LP
.I RALpage
keeps font descriptions in files on disk, rather than loading them all when the
interpreter starts up.
.LP
.I RALpage
has a well-known directory which it uses to keep font information and various pieces of
startup code.
This may be set in the environment variable, ``RALPAGELIB''.
.NH
HOW FONTS WORK WITH RALPAGE
.LP
The normal
.Ps
language
.B findfont
operator looks for a font in an internal directory called
.B FontDirectory.
If it fails to find it, it raises an
.B invalidfont
error.
.LP
.I RALpage
goes on to look in a directory called ``$RALPAGELIB/font''
for a file having the name of the given font
and attempts to load that file if present.
The file should define the font and return it on the stack.
It will find the font name on the stack on entry.
.LP
Fonts may be added to RALpage system simply by placing a piece of
.Ps
source in this directory which defines the font.
It should do so given a name and leave the resulting font dictionary on the stack.
.NH
ADDING SCREEN FONTS
.LP
Because outline fonts cannot be rendered effectively at low resolution, it useful to provide
screen fonts at popular sizes.
.I RALpage
can then load a screen font from disk straight into the cache,
if one can be found of a suitable size and orientation.
The size is flexible:
RALpage will use a smaller screen font if one of the correct size is not available.
There is a threshold on this, however.
.LP
To add a set of screen fonts:
.LP
Edit the file ``$RALPAGELIB/screen-fonts'' to describe the
screen fonts available (there is a comment at the top which describes the format).
.LP
Run
.I scr-to-cache-list
in ``$RALPAGELIB''.
.LP
If the screen fonts are for a font which is not already available
(i.e. visible in ``$RALPAGELIB/font'') then you'll need to dummy up a font:
.DS
\fC\s-2$ ln $RALPAGELIB/font/outline-proforma $RALPAGELIB/font/new-font-name
.DE
This provides a framework so that RALpage believes that the font exists.
.DS
\fC\s-2$ $RALPAGELIB/afm-to-rpfm new-font-name.afm >$RALPAGELIB/metrics/new-font-name
.DE
This takes standard Adobe font metrics and translates them into a form that RALpage
can use (see below for details).
.DS
\fC\s-2$ ln -s $RALPAGELIB/fonts/font-with-similar-metrics $RALPAGELIB/fonts/new-font-name
.DE
This provides a set of glyphs for RALpage to fall back on if there is no screen font
of the desired size.
.NH
LAZY FONT DEFINITIONS
.LP
The existing internal fonts are structured so that they can be loaded lazily.
Since they are all similar, most of the work is done by executing a common proforma
file in ``$RALPAGELIB/font/proforma''.
This proforma is linked to the various font names, and uses the font name it is given as a
parameter to enable it to locate the remainder of the font information (namely the glyphs
themselves and the font metrics).
.LP
``$RALPAGELIB/proforma'' will load fonts provided in an internal hershey format, which is
reasonably compact, and quick to load.
.LP
``$RALPAGE/outline-proforma'' will load fonts provided in postscript outline format.
It constructs a file name from the font name of the form ``$RALPAGELIB/fonts/<font-name>''.
It expects the file to contain definitions of the form:
.DS
\fC\s-2CharProcs /A { 346 398 moveto 373 387 lineto 37 34 lineto 38 38 lineto closepath } put
.DE
which are procedures which define an outline for each character in the font.
.B CharProcs
is a dictionary in the font which contains the definitions of the characters.
This is used by
.B BuildChar
to render the characters on request.
If outline fonts can be converted to this format, they can be easily loaded lazily by RALpage.
.LP
A definition of
.B BuildChar
is installed which will read this file when it is first
executed (i.e. the first character needs to be rendered) and then replace itself with the normal
.B BuildChar.
.NH 2
DEALING WITH FONT METRICS
.LP
The supplied proformas load the character metrics immediately the font is requested
(i.e. not lazily).
These are loaded from ``$RALPAGELIB/metrics/<font name>''.
The format is:
.DS
\fC\s-2Metrics /A 423 put
.DE
.LP
The proformas map the given font name through dictionary called
.B MetricMap.
This allows fonts to be used with different metrics from their own, so that they can ``stand in''
for standard fonts.
The file ``$RALPAGELIB/metric-map'' contains a list of definitons for this.
The format is:
.DS
\fC\s-2MetricMap /substitute-font-name /standard-font-name put
.DE
If no definition for a name is present, the identity mapping is assumed.
.NH
FONT CACHING
.LP
Normally, when
.I RALpage
renders a character from a font, it creates a cache internally so that it only has to render
each character once.
When it creates a new cache for a particular font size, it calls a
.Ps
procedure called
.B loaddiskcache
which is expected to find a file containing an appropriate screen font and load it if possible.
.B loaddiskcache
takes no arguments and returns no results.
It is called with the
.B currentmatrix
in character coordinates and the
.B currentfont
set to the font to load, and is expected to find a screen font from
that.
.LP
Procedures are provided to load files in each of the supported disk cache formats
into the current cache.
Currently
.B loadxcache
loads fonts in X11 Server Natural Format (snf)
and
.B loadvcache
loads fonts in
.I vfont(5)
format.
Each of these takes a file name as an argument and returns
.B true
or
.B false
on success or failure.
.LP
A definition of
.B loaddiskcache
is provided in ``$RALPAGELIB/disk-cache''.
This will read ``$RALPAGELIB/disk-cache-list'' which defines a dictionary of font names
and what screen fonts are available for them.
``$RALPAGELIB/disk-cache-list'' is generated by
.I scr-to-cache-list.
This is a shell script which should be run with no arguments to generate the list of screen
fonts from ``$RALPAGELIB/screen-fonts''.
.NH
FONT NAME MAPPING
.LP
A general mechanism is provided for mapping font names.
.B findfont
will pass requested font names through a dictionary called
.B FontMap
which maps
.Ps
font names to alternative font names.
This can be used to deal with font names which are too long for the file system
(normally only relevant on System 5),
or for mapping font names from standard names to some locally available equivalent.
.LP
The format of entries in this file is:
.DS
\fC\s-2FontMap /postscript-name /base-file-name put
.DE
in ``$RALPAGELIB/font-map''.
If postscript-name is not found there, the identity mapping is assumed.
.LP
Note that if alternative fonts are being used, there will be a problem if the font metrics
do not match \- see the section on metric mapping.
