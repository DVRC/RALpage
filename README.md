# RALpage
RALpage is a PostScript interpreter written by Crispin Goswell at Rutherford Appleton Laboratories
in the 80s.

https://www.chilton-computing.org.uk/inf/se/mmi/p004.htm

## What's in the directory?
- Shar: contains the OG thing from comp.sources.unix, including original messages
- dist: the same of Shar thing but stripped of the headers,
- OG: pristine RALpage, without patches
- MOD: John G. Myers' patches applied to the original interpreter
- xps: from "stuff.mit.edu/afs/athena.mit.edu/astaff/project/xps/".
  It has been ported to Sun, VAX, Mac (A/UX and Mac OS?) and IBM RT.
  I created the "obj" and "gmon" directories to tidy up the thing.
- Ralpage_X11R5: from "https://ftp.gwdg.de/pub/x11/x.org/R5contrib/"
  It is slightly different in the sources organization, and seems to have
  different X11 frontend, and also drivers for Apollo Domain (two different ones) and
  Sunview (from Julian Onions).
  It also has many more fonts compared to other versions and more stuff in
  the initialization
- ps-sunview: from "https://www.scnresearch.com/pub/sun/ps-sunview.tar.Z"
  It has the Onions SunView frontend, with slight patches by David Evans
- SCN: from "https://www.scnresearch.com/pub/graphics/postscript.tar.Z"
  Probably it's the same one from Shars, but with the John G. Myers patches pre-applied
  and some minor modification
- Docs: related documentation and papers about this PostScript interpreter

## What's missing?
- RALpage 1.7
  https://www.chilton-computing.org.uk/inf/literature/newsletters/ecn/p042.htm#s2
  Here an ftp site (unix.hensa.ac.uk) is mentioned, but unfortunately it's down and
  hasn't been archived

- ww: RAL's portable window toolkit. There isn't much about it online, apart from
  documents at chilton-computing.org
  - https://www.chilton-computing.org.uk/inf/pdfs/ui_research.pdf
  - https://www.chilton-computing.org.uk/inf/literature/inf_annual_reports/p011.htm
  - https://www.chilton-computing.org.uk/inf/literature/leaflets/p003.htm

  Maybe it rested in the same ftp site as RALpage 1.7

- Xinterface: a GUI rapid prototyping environment for X11R3 developed at University of
  Calgary, which includes a modified version of RALpage to "draw inside objects'
  window" (a lá Display PostScript/NeWS ?).
  The resting place would be cs-sun-fsa.cpsc.ucalgary.ca and ftp.cpsc.ucalgary.ca, but
  they're both dead and there is no archived copy.
