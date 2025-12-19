#include <X/Xlib.h>
#include <stdio.h>
#define gray_width 4
#define gray_height 4
static char gray_bits[] = {
   0x05, 0x0a, 0x05, 0x0a};

main(argc, argv)
int argc;
char **argv;
{
     Display *dpy;
     Window w;
     Pixmap gray;
     XEvent rep;
     char buf[5];
     int valuemask = 0;
     GC gc;
     XGCValues values;
     XPoint coord[4];
      
     if (argc < 2) {
	  if ((dpy = XOpenDisplay(NULL)) == NULL) {
	       printf("Couldn't open display.\n");
	       exit(1);
	  }
     } else {
	  if ((dpy = XOpenDisplay(argv[1])) == NULL) {
	       printf("Couldn't open display %s.\n", argv[1]);
	       exit(1);
	  }
     }

     w = XCreateSimpleWindow(dpy, RootWindow(dpy,0), 0, 0, 100, 100, 2,
			     BlackPixel(dpy, 0), WhitePixel(dpy, 0));

     gray = XCreatePixmapFromBitmapData(dpy, RootWindow(dpy, 0), gray_bits,
					gray_width, gray_height, 1, 0, 1);
     
     coord[0].x = 10;
     coord[0].y = 90;

     coord[1].x = 30;
     coord[1].y = 10;

     coord[2].x = 70;
     coord[2].y = 10;

     coord[3].x = 90;
     coord[3].y = 90;

     values.function = GXcopy;
     values.foreground = BlackPixel(dpy, 0);
     values.background = WhitePixel(dpy, 0);
     values.fill_style = FillOpaqueStippled;
     values.stipple = gray;
     valuemask = valuemask|GCFunction|GCForeground|GCBackground|GCFillStyle|GCStipple;
     
     gc = XCreateGC(dpy, RootWindow(dpy, 0), valuemask, &values);
     
     XSelectInput(dpy, w, (ButtonPressMask|KeyPressMask|ExposureMask));
     XMapWindow(dpy, w);
     do {
	  XNextEvent(dpy, &rep);
	  switch(rep.type) {
	  case Expose:
	       XFillPolygon(dpy, w, gc, coord, 4, Complex, CoordModeOrigin);
	       break;
	  case KeyPress:
	       XLookupString(&rep.xkey, buf, 1, NULL, NULL);
	       break;
	  case ButtonPress:
	       buf[0] = 'q';
	       break;
	  default:
	       printf("I'm losing here...\n");
	       break;
	  }
     } while (buf[0] != 'q');
     XDestroyWindow(dpy, w);
     XFreePixmap(dpy, gray);
     XFreeGC(dpy, gc);
}
