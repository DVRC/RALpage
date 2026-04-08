#include <X/Xlib.h>
#define FALSE 0
#define TRUE 1
     
main()
{
     Display *dpy;
     XGCValues values;
     GC gc;
     Window w;
     XEvent rep;
     int leave=FALSE;
     
     dpy = XOpenDisplay("");
     w = XCreateSimpleWindow(dpy, RootWindow(dpy, 0), 0, 0,
			     100, 100, 2, BlackPixel(dpy, 0),
			     WhitePixel(dpy, 0));
     values.function = GXcopy;
     values.foreground = BlackPixel(dpy, 0);
     values.background = WhitePixel(dpy, 0);
     values.line_width = 5;
     gc = XCreateGC(dpy, w, GCFunction|GCForeground|GCBackground|GCLineWidth,
		    &values);

     XSelectInput(dpy, w, ButtonPressMask|ExposureMask);
     XMapWindow(dpy, w);
     do {
	  XNextEvent(dpy, &rep);
	  switch(rep.type) {
	  case Expose:
	       XDrawLine(dpy, w, gc, 10, 10, 90, 90);
	       break;
	  case ButtonPress:
	       leave = TRUE;
	       break;
	  default:
	       printf("Losing horribly...\n");
	       break;
	  }
     } while (leave == FALSE);
     XDestroyWindow(dpy, w);
}
     
     
