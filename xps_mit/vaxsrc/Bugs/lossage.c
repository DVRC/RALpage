#include <X/Xlib.h>
#include <stdio.h>
#include <signal.h>
#define FALSE 0
#define TRUE 1

main(argc, argv)
     int argc;
     char **argv;
{
      Display *dpy;
      Window w;
      Pixmap bitmap1, bitmap2, bitmap3;
      XEvent rep;
      char buf[5];
      GC bitmapgc, gc1, gc2, gc3;
      XGCValues values;
      XSetWindowAttributes attribs;
      
      if (argc < 1) {
	    printf("Usage: %s [ host:display ]\n", argv[0]);
	    exit(1);
      }
      if (argc < 2) {
	    if ((dpy = XOpenDisplay(NULL)) == NULL) {
		  printf("Couldn't open display.\n");
		  exit(1);
	    } 
      } else
	if ((dpy = XOpenDisplay(argv[1])) == NULL) {
	      printf("Couldn't open display %s.\n", argv[1]);
	      exit(1);
	}

      bitmap1 = XCreatePixmap(dpy, RootWindow(dpy, 0), 100, 100, 1);
      bitmap2 = XCreatePixmap(dpy, RootWindow(dpy, 0), 100, 100, 1);
      bitmap3 = XCreatePixmap(dpy, RootWindow(dpy, 0), 100, 100, 1);
      
      values.function = GXclear;
      values.foreground = 1;
      values.background = 0;
      bitmapgc = XCreateGC(dpy, bitmap1,
			   GCFunction|GCForeground|GCBackground, &values);
      
      XFillRectangle(dpy, bitmap2, bitmapgc, 0, 0, 100, 100);
      XFillRectangle(dpy, bitmap3, bitmapgc, 0, 0, 100, 100);

      XSetFunction(dpy, bitmapgc, GXset);
      XFillRectangle(dpy, bitmap1, bitmapgc, 0, 0, 100, 100);

      XSetFunction(dpy, bitmapgc, GXcopy);
      XDrawPoint(dpy, bitmap3, bitmapgc, 50, 50);
      
      attribs.background_pixel = WhitePixel(dpy, 0);
      attribs.border_pixel = WhitePixel(dpy, 0);
      
      XSynchronize(dpy, 1);
      
      w = XCreateWindow(dpy, RootWindow(dpy, 0), 0, 0, 300, 100, 1,
			DefaultDepth(dpy, 0), InputOutput, CopyFromParent,
			CWBackPixel|CWBorderPixel,
			&attribs);
      XStoreName(dpy, w, argv[0]);

      values.function = GXcopy;
      values.foreground = BlackPixel(dpy,0);
      values.background = WhitePixel(dpy,0);
      values.clip_mask = bitmap1;
      values.clip_x_origin = 0;
      values.clip_y_origin = 0;
      gc1 = XCreateGC(dpy, w,
		     GCFunction|GCForeground|GCBackground|GCClipMask|GCClipXOrigin|GCClipYOrigin,
		     &values);

      values.clip_mask = bitmap2;
      values.clip_x_origin = 100;
      gc2 = XCreateGC(dpy, w,
		     GCFunction|GCForeground|GCBackground|GCClipMask|GCClipXOrigin|GCClipYOrigin,
		     &values);

      values.clip_mask = bitmap3;
      values.clip_x_origin = 200;
      gc3 = XCreateGC(dpy, w,
		     GCFunction|GCForeground|GCBackground|GCClipMask|GCClipXOrigin|GCClipYOrigin,
		     &values);

      XSelectInput(dpy, w, (ButtonPressMask|KeyPressMask|ExposureMask));
      XMapWindow(dpy, w);
      do {
	   XNextEvent(dpy, &rep);
	   switch(rep.type) {
	   case Expose:
		XFillRectangle(dpy, w, gc1, 0, 0, 100, 100);
		XFillRectangle(dpy, w, gc2, 100, 0, 100, 100);
		XFillRectangle(dpy, w, gc3, 200, 0, 100, 100);
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
      XFreeGC(dpy, gc1);
      XFreeGC(dpy, gc2);
      XFreeGC(dpy, gc3);
      XFreeGC(dpy, bitmapgc);
      XFreePixmap(dpy, bitmap1);
      XFreePixmap(dpy, bitmap2);
      XFreePixmap(dpy, bitmap3);
}
