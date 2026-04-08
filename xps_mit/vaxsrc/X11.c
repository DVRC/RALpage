/*
 * Copyright (C) Rutherford Appleton Laboratory 1987
 * 
 * This source may be copied, distributed, altered or used, but not
 * sold for profit or incorporated into a product except under licence
 * from the author. 
 * It is not in the public domain.
 * This notice should remain in the source unaltered, and any changes
 * to the source made by persons other than the author should be
 * marked as such. 
 * 
 *	Crispin Goswell @ Rutherford Appleton Laboratory caag@uk.ac.rl.vd
 */
/* X11R2 modifications and fixes made from sources by Crispin Goswell,
 * Barry Shein of BU, and Jean Marie Diaz of MIT.  The resulting
 * melange, along with other fixes for color workstations, is entirely
 * my fault.
 *
 * David Jedlinsky
 * MIT Project Athena Watchmaker
 * opus@athena.mit.edu
 * 
 * Andrew Marc Greene
 * MIT Project Athena Watchmaker
 * amgreene@athena.mit.edu
 *
 */
#include "main.h"
#include "graphics.h"
#include "cache.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <stdio.h>

#define ISWIN	1
#define ISBITMAP 2
#define MAXSYNC 400
#define FLUSH if (--synccount <= 0) {XFlush(dpy, 0); synccount=MAXSYNC;}
typedef int ColorType;
#define ISPIXEL		1
#define ISHALFTONE	2

/* Also change UpdateControl in font.c */
#define UpdateControl(h, on) {}

#define GraySync(C) \
  (screen[(int)((float)(C)*(float)screen_size/(float)TRANSFER_SIZE+0.5)].shade)

struct screen
{
  float val;
  int sx, sy;
  struct hardware *shade;
} *screen = NULL;

static int screen_size;
static int transfer [TRANSFER_SIZE];

extern int RUNSYNC, TESTING;

/* Externals for the inlined procedures found here */
extern DevicePoint zeroDevicePoint;
extern void Dash();
extern struct char_table *CharTable;
extern int cmax;

int pixels_per_inch;
static int synccount = MAXSYNC;
static unsigned long blackpixel,whitepixel;
static int bwvalid;

float hppp, vppp;

int single_rop [] =
{
  ROP_FALSE, ROP_DEST, ROP_NOTDEST, ROP_TRUE,
  ROP_FALSE, ROP_DEST, ROP_NOTDEST, ROP_TRUE,
  ROP_FALSE, ROP_DEST, ROP_NOTDEST, ROP_TRUE,
  ROP_FALSE, ROP_DEST, ROP_NOTDEST, ROP_TRUE
  };

void InitTransfer (ppi)
     int ppi;
{
  int i;
     
  pixels_per_inch = ppi;
  for (i = 0; i < TRANSFER_SIZE; i++)
    transfer [i] = i;
}

static void Punt(str)
     char *str;
{
  /* fprintf(stderr, "%s\n", str); */
  exit(1);
}

static Display *dpy;
static int SCREEN;
static struct hardware *NewHardware ();

extern HardPoint DeviceToHard ();

typedef struct _HardwareRec {
  Drawable w;
  GC gc;
  union last_color {
    unsigned long pix;
    Pixmap halftone;
  } last_color;
  int last_used;
} HardwareRec, *Hardware;

/*
 * This file describes the interface that PostScript requires to the graphics
 * system at Version 1.4.
 * 	
 * ''Hardware'' in this context refers to a pointer to windows and/or bitmaps
 * and is the lowest level of access that PostScript is interested in. Any
 * Hardware parameter may be expected to be NULL.
 */

/********************* CREATION OF WINDOWS AND BITMAPS *******************/

#define MIN(x, y)	(((x) < (y)) ? (x) : (y))

static GC GlobalGC = NULL;
static int GlobalMask = 0;
static GC fillgc[16];
static GC BitmapGC[16];
static GC whitegc;
static GC whitebitgc;
void ModifyGC ();

/*
 * InitHardware () returns a default device which PostScript may use
 * immediately (or NULL if not appropriate).  Its size and shape are
 * not defined. Most typically the user will want to start up another
 * device before it is used anyway. No attempt will be made by
 * PostScript to Destroy the resulting device.
 */
struct hardware *InitHardware ()
{
  XGCValues values;
  int i;
  DevicePoint p;
    
  if ((dpy = XOpenDisplay("")) == NULL)
    Punt("Could not open display");
  SCREEN = DefaultScreen(dpy);

  /* AMG: debugging code */
  /* fprintf(stderr,"inithardware\n"); */

  /* DCJ:
   * This is the real ppi.  However, we might be using a screen that
   * is less than 11 inches tall, which makes life difficult.  So we
   * cheat a bit.
   */

  if (DisplayHeightMM(dpy, SCREEN) < 280) {
    hppp = (float) DisplayWidth(dpy,SCREEN) / ((279.4 / (float) DisplayHeightMM(dpy, SCREEN)) * (float) DisplayWidthMM(dpy, SCREEN) * 2.83);
    vppp = (float) DisplayHeight(dpy, SCREEN) / 792.0;
    InitTransfer(DisplayHeight(dpy, SCREEN) / 11);
  } else {
    hppp = (float) DisplayWidth(dpy, SCREEN) /
      ((float) DisplayWidthMM(dpy, SCREEN) * 2.83);
    vppp = (float) DisplayHeight(dpy, SCREEN) /
      ((float) DisplayHeightMM(dpy, SCREEN) * 2.83);
    InitTransfer((int) ((float) DisplayHeight(dpy, SCREEN) * (float) 25.4 / (float) DisplayHeightMM(dpy, SCREEN)));
  }

  blackpixel = BlackPixel(dpy, SCREEN);
  whitepixel = WhitePixel(dpy, SCREEN);
  values.foreground = blackpixel;
  values.background = whitepixel;

  /* Turn off graphics exposures, to avoid getting an expose event for each
  ** XCopyArea with a pixmap.
  */
  values.graphics_exposures = False;
  for (i=0 ; i<16 ; i++) {
    BitmapGC[i] = NULL;
    values.function = i;
    fillgc[i] = XCreateGC(dpy, RootWindow(dpy, SCREEN),GCFunction |
			  GCForeground | GCBackground | GCGraphicsExposures,
			  &values);
  }
  values.function = GXcopy;
  values.foreground = whitepixel;
  values.background = blackpixel;
  whitegc = XCreateGC(dpy, RootWindow(dpy, SCREEN),GCFunction|GCForeground|
		      GCBackground|GCGraphicsExposures,&values);

  p.dx = DisplayWidth(dpy, SCREEN);
  p.dy = DisplayHeight(dpy, SCREEN);

  /* AMG: debugging code */
  /* fprintf(stderr,"inithardware returning\n"); */

  return NewHardware(p);

}

static struct hardware *NewHardware(width, height)
     int width, height;
{
  struct hardware *to;
  Hardware hard;

  /* fprintf(stderr,"NewHardware\n"); */ /*AMG*/
  to = (struct hardware *) malloc(sizeof(struct hardware));
  hard = (Hardware) malloc(sizeof(HardwareRec));
  to->hard.addr = (char *) hard;
  to->flags = 0;
  to->aux = to->clip = (struct hardware *) NULL;
  to->extent = NewDevicePoint(width, height);
  hard->w = (Drawable) NULL;
  hard->gc = (GC) NULL;
  to->touched = FALSE;
  to->changed = TRUE;
  hard->last_used = 0;
  return to;
}

/*
 * NewBitmapHardware () is expected to create a new bitmap. Only one plane
 * will be needed.
 * 	
 * NewWindowHardware () is expected to create a window on the screen. On a
 * colour system this will be expected to support full colour.
 */
struct hardware *NewBitmapHardware (width, height)
     int width, height;
{
  XGCValues values;
  struct hardware *to = NewHardware(width, height);
  Hardware hard = (Hardware) to->hard.addr;
  int i;

  to->flags = ISBITMAP;
     
  /* fprintf(stderr,"NewBitmapHardware\n"); */ /*AMG*/

  if (width == 0 || height == 0) {
    return NULL;
  }
  hard->w = XCreatePixmap(dpy, RootWindow(dpy, SCREEN), width, height, 1);

  if (! BitmapGC[GXcopy]) {
    /* Turn off graphics exposures, to avoid getting an expose event for each
    ** XCopyArea with a pixmap.
    */
    values.graphics_exposures = False;
    values.foreground = 1;
    values.background = 0;

    for (i=0; i<16; i++) {
      values.function = i;
      BitmapGC[i] = XCreateGC(dpy, hard->w,GCFunction|GCForeground|
			      GCBackground|GCGraphicsExposures,
			      &values);
    }

    values.foreground = 0;
    values.background = 1;
    values.function = GXcopy;
    whitebitgc = XCreateGC(dpy, hard->w,GCFunction|GCForeground|
			   GCBackground|GCGraphicsExposures,
			   &values);
  }
  XFillRectangle(dpy, hard->w, BitmapGC[GXclear], 0, 0,
		 width, height);
     
  return to;
}

struct hardware *NewWindowHardware (width, height)
     int width, height;
{
  struct hardware *to = NewHardware(width, height);
  Hardware hard = (Hardware) to->hard.addr;
  XEvent event;
  unsigned long vmask;
  XSetWindowAttributes xswa;
  XSizeHints xshints;
     
  /* fprintf(stderr,"NewWindowHardware\n"); */ /*AMG*/

  to->flags = ISWIN;
  vmask = CWBackPixel|CWBorderPixel|CWBackingStore|
    CWBackingPlanes;
  xswa.background_pixel = whitepixel;
  xswa.border_pixel = blackpixel;
  xswa.backing_store = Always;
  xswa.backing_planes = AllPlanes;
  xshints.x = DisplayWidth(dpy, SCREEN) - width;
  xshints.y = 0;
  xshints.width = width;
  xshints.height = height;
  xshints.flags = USPosition|USSize;
     
  if (RUNSYNC) {
    /* fprintf(stderr, "Running synchronized.\n"); */ fflush(stderr);
    XSynchronize(dpy, TRUE);
  }
  hard->w = XCreateWindow(dpy, RootWindow(dpy,SCREEN),
			  xshints.x, xshints.y, width, height,
			  1, DefaultDepth(dpy,SCREEN),
			  InputOutput, DefaultVisual(dpy,SCREEN),
			  vmask, &xswa);

  XChangeProperty(dpy,hard->w,XA_WM_NAME,XA_STRING,8,
		  PropModeReplace,"POSTSCRIPT",10);
  if (TESTING)
    XSetNormalHints(dpy, hard->w, &xshints);
     
  XSelectInput(dpy, hard->w, ExposureMask);
  XMapWindow(dpy, hard->w);
  XFlush(dpy);
  do { /* modified by AMG for testing */
    XNextEvent(dpy, &event);
    /* fprintf(stderr,"Event received of type %d\n",event.type); */
  } while (event.type != Expose);
  XSelectInput(dpy, hard->w, 0);
  return to;
}

/*
 * IsWindowHardware () should return TRUE if the hardware is a window, FALSE
 * otherwise.
 *
 * IsBitmapHardware () should return TRUE if the hardware is a bitmap,
 * FALSE otherwise.
 */

int IsWindowHardware (h)
     struct hardware *h;
{
  return h->flags & ISWIN;
}

#define IsWindowHardware(h) ((h)->flags & ISWIN)
#define IsBitmapHardware(h) ((h)->flags & ISBITMAP)

/*
 * 	
 * DestroyHardware () should release the resources required by the hardware,
 * bitmap or window.  This should cause a window device to vanish. NULL is not
 * an error (does nothing).
 */

void DestroyHardware (h)
     struct hardware *h;
{
  if (h) {
    Hardware hard = (Hardware) h->hard.addr;
    if (IsWindowHardware(h))
      XDestroyWindow(dpy, hard->w);
    else
      if (IsBitmapHardware(h))
	XFreePixmap(dpy, hard->w);
    Free(h);
  }
}

/*
 *
 * DeviceMatrix () should return a matrix appropriate to a device of the given
 * height and width.  For a typical display with a graphics origin at the top
 * left of a window, an appropriate definition would be:
 * 	
 * Matrix DeviceMatrix (width, height)
 * int width, height;
 * {
 *     Matrix m;
 *     NewMatrix (m, PIXELS_PER_INCH / 72.0, 0.0, 0.0,
 * 		         -PIXELS_PER_INCH / 72.0, 0.0, (float) height);
 *     return m;
 * }
 */

Matrix DeviceMatrix (width, height)
     int width, height;
{
  Matrix m;
  NewMatrix(m, pixels_per_inch / 72.0, 0.0, 0.0,
	    -pixels_per_inch / 72.0, 0.0, (float) height);
  return m;
}

/*
 * HardwareExtent () returns a DevicePoint describing the width and height of
 * the argument.  NULL has extent zeroDevicePoint.
 */

DevicePoint HardwareExtent (h)
     struct hardware *h;
{
  if (h)
    return h->extent;
  else
    return zeroDevicePoint;
}

static NeedAux (h)
     struct hardware *h;
{
  DevicePoint p;
     
  if (h->aux)
    return;
  p = HardwareExtent(h);
  h->aux = NewBitmapHardware (p.dx, p.dy);
}

/*************************** OUTPUT PRIMITIVES ******************************/
/*
 * 	
 * BitBlt () is a full function RasterOp. The 'rop' argument will have values
 * as described in the header file hard.h. If the from argument is NULL it is
 * taken to be a bitmap full of ones the shape of the fromPoint and extent. If
 * the to argument is NULL, this is a no-op.
 *
 * Paint () is an addition to BitBlt. Bits that are set in the source are
 * Painted into the destination in the given colour with a copying rasterop so
 * that they replace pixels previously there. If the machine does not support
 * colour windows, half-toning should be performed.  Colour values have hue,
 * saturation and brightness components. On a black and white or greyscale
 * system the brightness value will be a FP value between 0.0 (black) and 1.0
 * (white), which can be used as a grey level.
 * 	
 * Paint is expected to mask with the clip mask. BitBlt is not,
 */

void BitBlt (from, to, fromPoint, toPoint, extent, rop)
     struct hardware *from, *to;
     DevicePoint toPoint, fromPoint, extent;
     int rop;
{
  Hardware fromhard, tohard;
  GC gc = NULL;
     
  /* fprintf(stderr,"BitBlt\n"); */

  if (to == NULL) return;
  /*
   * Copying from full color window to monochrome bitmap is the
   * wrong idea.
   */
  if (IsWindowHardware(from) && !IsWindowHardware(to)) return;
     
  tohard = (Hardware) to->hard.addr;
  if (IsWindowHardware(to)) {
    if (from == NULL) {
      XFillRectangle(dpy, tohard->w, fillgc[rop], toPoint.dx,
		     toPoint.dy, extent.dx, extent.dy);
    } else {
      fromhard = (Hardware) from->hard.addr;
      if (IsWindowHardware(from)) {
	XCopyArea(dpy, fromhard->w, tohard->w, fillgc[rop],
		  fromPoint.dx, fromPoint.dy,
		  extent.dx, extent.dy,
		  toPoint.dx, toPoint.dy);
      } else
	if (IsBitmapHardware (from)) {
	  XCopyPlane(dpy, fromhard->w, tohard->w, fillgc[15-rop],
		     fromPoint.dx, fromPoint.dy,
		     extent.dx, extent.dy,
		     toPoint.dx, toPoint.dy, 1);
	}
    }
  } else
    if (IsBitmapHardware(to)) {
      if (from == NULL) {
	XFillRectangle(dpy, tohard->w, BitmapGC[rop], toPoint.dx,
		       toPoint.dy, extent.dx, extent.dy);
      } else {
	if (IsBitmapHardware(from)) {
	  fromhard = (Hardware) from->hard.addr;
	  XCopyArea(dpy, fromhard->w, tohard->w, BitmapGC[rop],
		    fromPoint.dx, fromPoint.dy,
		    extent.dx, extent.dy,
		    toPoint.dx, toPoint.dy);
	}
      }
    }

  to->touched = to->changed = TRUE;

  if (gc != NULL) XFreeGC(dpy, gc);
  FLUSH
}

 void Paint (from, to, fromPoint, toPoint, extent, colour)
     struct hardware *from, *to;
     DevicePoint fromPoint, toPoint, extent;
     Colour colour;
{
  Hardware tohard,fromhard;
  float b;
     
  /* fprintf(stderr,"Pait\n"); */ /*AMG*/

  if (from != NULL && !IsBitmapHardware(from))
    Punt("Paint() called with non-bitmap source.\n");

  if (to == NULL || !from->touched) return;
  tohard = (Hardware) to->hard.addr;

  b = colour.brightness;
  if(from && bwvalid && to->clip==NULL && (b==0.0 || b==1.0)) {
    fromhard = (Hardware) from->hard.addr;
    XCopyArea(dpy, fromhard->w, tohard->w,
	      (b == 0.0 ? fillgc[GXandInverted] : fillgc[GXor]),
	      0, 0, extent.dx, extent.dy, toPoint.dx, toPoint.dy);
  }
  else {
    ModifyGC (colour, from, to, fromPoint, toPoint, extent);
    XFillRectangle(dpy, tohard->w, GlobalGC, toPoint.dx, toPoint.dy,
		   extent.dx, extent.dy);
  }
     
  to->touched = to->changed = TRUE;

  FLUSH
}	

/*
** Severely optimized for drawing characters using XCopyArea().
*/
int CacheShow (name, cp)
     Object name;
     HardPoint cp;
{
  struct hardware *from, *to;
  Hardware tohard,fromhard;
  struct cache *cache, *cc;
  DevicePoint fromPoint, toPoint, extent;
  float b;
  int i, h;
  
  if (!(cache = gstate->show->ccache)) return FALSE;
  gstate->show->CharName = name;

  /* Search through cache for the character */
  for (i = h = (BodyInteger (name) + (int) cache) % cmax;;) {
    if ((cc = CharTable[i].char_cache) == NULL) return FALSE;
    if (cc == cache && DictEqual (name, CharTable[i].char_key)) break;
    if (++i == cmax) i = 0;
    if (i == h) return FALSE;
  }
  gstate->show->Width = CharTable[i].char_width;
  
  from = CharTable[i].char_device->dev;
  to = gstate->device->dev;
  if (to == NULL || !from->touched) return TRUE;

  tohard = (Hardware) to->hard.addr;
  b = gstate->colour.brightness;
  if(from && bwvalid && to->clip==NULL && (b==0.0 || b==1.0)) {
    fromhard = (Hardware) from->hard.addr;
    XCopyArea(dpy, fromhard->w, tohard->w,
	      (b == 0.0 ? fillgc[GXandInverted] : fillgc[GXor]),
	      0, 0, cache->width, cache->height,
	      (int) cp.hx - cache->swidth,(int) cp.hy - cache->sheight);
  }
  else {
    fromPoint.dx = fromPoint.dy = 0;
    toPoint.dx = (int) cp.hx - cache->swidth;
    toPoint.dy = (int) cp.hy - cache->sheight;
    extent.dx = cache->width;
    extent.dy = cache->height;
    ModifyGC (gstate->colour, from, to, fromPoint, toPoint, extent);
    XFillRectangle(dpy, tohard->w, GlobalGC, toPoint.dx, toPoint.dy,
		   extent.dx, extent.dy);
  }
  
  to->touched = to->changed = TRUE;
  FLUSH
  return TRUE;
}

int FillPath(to, path, rule)
     struct hardware *to;
     Path path;
     int rule;
{
  register Path p;
  static XPoint *vertices = NULL;
  int i = 0;
  Hardware hard;
     
  /* fprintf(stderr,"FillPath\n"); */ /*amg*/

  if (!vertices) {
    if ((vertices = (XPoint *) malloc(MAXPATHELEMENTS * sizeof(XPoint)))
	== NULL)
      Punt("Not enough memory for filling.\n");
  }	       
     
  if (EmptyPath (path))
    return TRUE;
     
  path = FlattenPath (path);

  if (!CloseAll (path)) {
    PathFree (path);

    return FALSE;
  }

  ModifyGC (gstate->colour, (struct hardware *)NULL, to,
	    zeroDevicePoint, zeroDevicePoint, zeroDevicePoint);
     
  hard = (Hardware) to->hard.addr;
     
  for (p = path->next; p != path; p = p->next)
    switch (p->ptype) {
    case EMove:
      if (i > 1)
	XFillPolygon(dpy, hard->w, GlobalGC, vertices, i,
		     Complex, CoordModeOrigin);
      vertices[0].x = (short) p->pe.point.hx;
      vertices[0].y = (short) p->pe.point.hy;
      i = 1;
      break;
    case ELine:
      vertices[i].x = (short) p->pe.point.hx;
      vertices[i++].y = (short) p->pe.point.hy;
      break;
    case EClose:
      XFillPolygon(dpy, hard->w, GlobalGC, vertices, i,
		   Complex, CoordModeOrigin);
      i = 0;
      break;
    }

  PathFree (path);
     
  to->touched = TRUE;
  to->changed = TRUE;

  return TRUE;
}

static short Xvalue (ax, ay, bx, by, cy)
     int ax, ay, bx, by, cy;
{
  if (ay - by == 0) {
    printf("Xvalue: ax=%d, ay=%d, bx=%d, by=%d, cy=%d\n",
	   ax, ay, bx, by, cy);
    return bx + (cy - by) * (ax - bx);
  }
  return bx + (cy - by) * (ax - bx) / (float) (ay - by);
}

/*
 * BitBltTrapezoid () and PaintTrapezoid () render a complete trapezoidal
 * shape.  The corners of the trapezoid may lie far outside the range of
 * interesting scan-lines, but the slope of the line should be clipped by the
 * top and bottom. The coordinates are half-open.
 */

void BitBltTrapezoid (to, lefttop, leftbottom, righttop, rightbottom,
		      top, bottom, rop)
     struct hardware *to;
     DevicePoint lefttop, leftbottom, righttop, rightbottom;
     int top, bottom, rop;
{
  int i, j, temp;
  static int left [1024], right [1024];

  /* fprintf(stderr,"BitBltTrap\n"); */ /* amg*/
     
  int
    ltx = Xvalue (lefttop.dx, lefttop.dy, leftbottom.dx,
		  leftbottom.dy, top),
    rtx = Xvalue (righttop.dx, righttop.dy, rightbottom.dx,
		  rightbottom.dy, top),
    lbx = Xvalue (lefttop.dx, lefttop.dy, leftbottom.dx,
		  leftbottom.dy, bottom),
    rbx = Xvalue (righttop.dx, righttop.dy, rightbottom.dx,
		  rightbottom.dy, bottom);
 
  if (ltx == lbx && rtx == rbx) {
    if (rtx < ltx)
      temp = rtx, rtx = ltx, ltx = temp;
	  
    BitBlt ((struct hardware *) NULL, to,
	    zeroDevicePoint, NewDevicePoint (ltx, top),
	    NewDevicePoint (rtx - ltx + 1, bottom - top + 1), rop);
	  
    return;
  }

  for (i = top, j = 0; i <= bottom; i++, j++) {
    int
      lx = Xvalue (lefttop.dx, lefttop.dy, leftbottom.dx,
		   leftbottom.dy, i),
      rx = Xvalue (righttop.dx, righttop.dy, rightbottom.dx,
		   rightbottom.dy, i);
 	  
    if (rx < lx)
      temp = rx, rx = lx, lx = temp;
	  
    left [j] = lx; right [j] = rx;
  }
  BitBltBlob (to, top, bottom - top, left, right, single_rop [rop]);

  to->touched = TRUE;
  to->changed = TRUE;
}

void PaintTrapezoid (to, lefttop, leftbottom, righttop, rightbottom,
		     top, bottom, colour)
     struct hardware *to;
     DevicePoint lefttop, leftbottom, righttop, rightbottom;
     int top, bottom;
     Colour colour;
{
  struct hardware *gray = NULL;
  Hardware hard, grayhard, tohard;
  int valuemask = 0, ok = FALSE;
  XGCValues values;
  GC gc = NULL;
  unsigned long pix;
  ColorType ret;
  XPoint coord[4];
     
  ret = RealColor(colour, &pix, &gray);

  /* fprintf(stderr,"PaintTrap\n"); */ /*AMG*/

  /* This should be done better: Halftones aren't handled... */
  if (IsBitmapHardware(to)) {
    BitBltTrapezoid(to, lefttop, leftbottom, righttop, rightbottom,
		    top, bottom, (pix == WhitePixel(dpy, SCREEN)) ? ROP_FALSE : ROP_TRUE);
    return;
  }
	  
  if (to == NULL)
    return;
     
  coord[0].x = Xvalue (lefttop.dx, lefttop.dy, leftbottom.dx,
		       leftbottom.dy, bottom);
  coord[0].y = bottom;
  coord[1].x = Xvalue (lefttop.dx, lefttop.dy, leftbottom.dx,
		       leftbottom.dy, top);
  coord[1].y = top;
  coord[2].x = Xvalue (righttop.dx, righttop.dy, rightbottom.dx,
		       rightbottom.dy, top);
  coord[2].y = top;
  coord[3].x = Xvalue (righttop.dx, righttop.dy, rightbottom.dx,
		       rightbottom.dy, bottom);
  coord[3].y = bottom;

  UpdateControl (to, FALSE);
     
  /* Turn off graphics exposures, to avoid getting an expose event for each
  ** XCopyArea with a pixmap.
  */
  values.graphics_exposures = False;
  valuemask |= GCGraphicsExposures;

  if (ret == ISPIXEL) {
    values.function = GXcopy;
    values.foreground = pix;
    valuemask = valuemask|GCFunction|GCForeground;
    if (pix == blackpixel)  gc = fillgc[GXcopy];
    if (pix == whitepixel)  gc = whitegc;
    hard = (Hardware) to->hard.addr;
    if (hard->last_used == ISPIXEL && hard->last_color.pix == pix)
      ok = TRUE;
  } else if (ret == ISHALFTONE) {
    values.function = GXcopy;
    values.foreground = blackpixel;
    values.background = whitepixel;
    values.fill_style = FillOpaqueStippled;
    grayhard = (Hardware) gray->hard.addr;
    values.stipple = grayhard->w;
    valuemask = valuemask|GCFunction|GCForeground|GCBackground|GCFillStyle|GCStipple | GCGraphicsExposures;
    gc = grayhard->gc;
    
    hard = (Hardware) to->hard.addr;
    if (hard->last_used == ISHALFTONE &&
	hard->last_color.halftone == grayhard->w)
      ok = TRUE;
	  
    if (!gc)
      printf("HalfTone GC not set!\n");
	  
  } else return;

  if (to->clip) {
    hard = (Hardware) to->clip->hard.addr;
    values.clip_mask = hard->w;
    valuemask = valuemask|GCClipMask;

    if (ok)
      gc = ((Hardware) to->hard.addr)->gc;
	  
/*	  if (to->clip->changed) {
	       to->clip->changed = FALSE;
	       printf("Clip changed.\n");
	       gc = NULL;
	  }
*/
    gc = NULL;
  }
     
  if (!gc) {
    gc = XCreateGC(dpy, RootWindow(dpy, SCREEN), valuemask, &values);
  }
  tohard = (Hardware) to->hard.addr;
     
  XFillPolygon(dpy, tohard->w, gc, coord, 4, Complex, CoordModeOrigin);
     
  hard = (Hardware) to->hard.addr;
  hard->last_used = ret;
  hard->gc = gc;
     
/*     if (created)
	  XFreeGC(dpy, gc);
	  */
     
  UpdateControl (to, TRUE);

  to->touched = TRUE;
  to->changed = TRUE;
}

/*
 * 	
 * 	BitBltLine () is expected to draw a line between the given points
 * 	with the given RasterOp and colour masking.
 * 	The line should be one pixel wide and half-open.
 * 	[Thicker lines are done with BitBlt.]
 * 	
 * 	PaintLine () is expected to Paint a line by analogy with Paint
 * 	and BitBlt.
 */

void BitBltLine (h, fromPoint, toPoint, rop)
     struct hardware *h;
     DevicePoint fromPoint, toPoint;
     int rop;
{
  /* fprintf(stderr,"BBLine\n"); */ /*AMG*/

  if (h) {
    Hardware hard = (Hardware) h->hard.addr;
    if (IsWindowHardware(h)) {
      XDrawLine(dpy, hard->w, fillgc[rop], fromPoint.dx, fromPoint.dy,
		toPoint.dx, toPoint.dy);
    } else
      if (IsBitmapHardware(h)) {
	XDrawLine(dpy, hard->w, BitmapGC[rop], fromPoint.dx, fromPoint.dy,
		  toPoint.dx, toPoint.dy);
      }

    h->touched = TRUE;
    h->changed = TRUE;
  }
}

void PaintLine (h, fromPoint, toPoint, colour)
     struct hardware *h;
     DevicePoint fromPoint, toPoint;
     Colour colour;
{
  Hardware tohard;
  GC local;
  register float b;

  /* fprintf(stderr,"PLine\n"); */ /*amg*/

  if(h == NULL) return;
  tohard = (Hardware) h->hard.addr;

  UpdateControl (h, FALSE);

  b = colour.brightness;
  if(bwvalid && h->clip==NULL && (b==0.0 || b==1.0))
    local = (IsWindowHardware(h)
	     ? (b == 0.0 ? fillgc[GXcopy]   : whitegc)
	     : (b == 0.0 ? BitmapGC[GXcopy] : whitebitgc));
  else {
    ModifyGC (colour, (struct hardware *)NULL, h,
	      zeroDevicePoint, zeroDevicePoint, zeroDevicePoint);
    local = GlobalGC;
  }

  XDrawLine(dpy, tohard->w, local, fromPoint.dx, fromPoint.dy,
		 toPoint.dx, toPoint.dy);

  h->touched = h->changed = TRUE;

  UpdateControl (h, TRUE);
  FLUSH
}	

/*
** Severely optimized for drawing multi-line strokes using XDrawLines().
*/
int ThinStroke ()
{
  struct hardware *h;
  Hardware tohard;
  Path p;
  HardPoint here, prev, cur;
  float x,y,w;
  GC local;
  register float b;
  
  /* fprintf(stderr,"ThinStroke\n"); */ /*amg*/

  if (stroke_method != STROKE_THIN) return FALSE;
  if((w = gstate->line_width) != 0.0) {
    w = 1.1/w;
    x = (gstate->CTM.A + gstate->CTM.C);
    y = (gstate->CTM.B + gstate->CTM.D);
    if(x < -w || x > w || y < -w || y > w) return FALSE;
  }
  if (gstate->dash_length != 0) Dash ();

  if((h = gstate->device->dev) == NULL) return;
  tohard = (Hardware) h->hard.addr;
  UpdateControl (h, FALSE);
  b = gstate->colour.brightness;

  if(bwvalid && h->clip==NULL && (b==0.0 || b==1.0))
    local = (IsWindowHardware(h)
	     ? (b == 0.0 ? fillgc[GXcopy]   : whitegc)
	     : (b == 0.0 ? BitmapGC[GXcopy] : whitebitgc));
  else {
    ModifyGC (gstate->colour, (struct hardware *)NULL, h,
	      zeroDevicePoint, zeroDevicePoint, zeroDevicePoint);
    local = GlobalGC;
  }

  for (p = gstate->path->next; p != gstate->path; p = p->next) {
    if(p->ptype == EMove) here = prev = p->pe.point;
    else {
      cur = (p->ptype == ELine) ? p->pe.point : here;

      XDrawLine(dpy, tohard->w, local, (int) prev.hx, (int) prev.hy,
		(int) cur.hx, (int) cur.hy);
      FLUSH
      prev = cur;
    }
  }

  h->touched = h->changed = TRUE;
  UpdateControl (h, TRUE);

  VOID PNewPath ();
  return TRUE;
}

#ifdef notdef
void PaintLine (h, fromPoint, toPoint, colour)
     struct hardware *h;
     DevicePoint fromPoint, toPoint;
     Colour colour;
{
  int col;
     
  if (h == NULL)
    return;
     
  col = IsWindowHardware (h) ? HardColour (colour) : 0;
     
  UpdateControl (h, FALSE);
     
  if (h->clip == NULL && (col == 0 || col == TRANSFER_SIZE - 1))
    BitBltLine (h, fromPoint, toPoint,
		(col == 0 ? ROP_TRUE : ROP_FALSE));
  else {
    int mx = Min (fromPoint.dx, toPoint.dx), my = Min (fromPoint.dy,
						       toPoint.dy),
    Mx = Max (fromPoint.dx, toPoint.dx), My = Max (fromPoint.dy,
						   toPoint.dy);
    DevicePoint orig, ex;
	  
    orig = NewDevicePoint (mx, my);
    ex = NewDevicePoint (Mx - mx + 1, My - my + 1);
	  
    NeedAux (h);
    BitBlt ((struct hardware *) NULL, h->aux, orig, orig, ex, ROP_FALSE);
    
    BitBltLine (h->aux, fromPoint, toPoint, ROP_TRUE);
	  
    BitBlt (h->clip, h->aux, orig, orig, ex, ROP_AND);
    BitBlt (h->aux, h, orig, orig, ex, ROP_NOTAND);
	  
    if (col != 0 && col != TRANSFER_SIZE - 1) {
      struct hardware *gray = GraySync (col);
      RasterTile (gray, h->aux, orig, ex, ROP_AND);
    }
    BitBlt (h->aux, h, orig, orig, ex, ROP_OR);
  }
  UpdateControl (h, TRUE);

  h->touched = TRUE;
  h->changed = TRUE;
}
#endif

/*
 * BitBltBlob () takes a set of pixel coordinates and fills the trapezon
 * figure half open.
 */

void BitBltBlob (to, top, height, left, right, rop)
     struct hardware *to;
     int top, height, *left, *right, rop;
{
  int i;
  DevicePoint p1, p2;
  for (i=0 ; i<height ; i++) {
    p1 = NewDevicePoint (left[i], top + i);
    p2 = NewDevicePoint (right[i], top + i);
    BitBltLine(to, p1, p2, rop);
  }
}

/*
 * RasterTile () replicates the whole of ``from'' over ``to'', but clipped by
 * the rectangle bounded by ``toPoint'' and ``extent''.
 */
void RasterTile (from, to, toPoint, extent, rop)
     struct hardware *from, *to;
     DevicePoint toPoint, extent;
     int rop;
{
  Hardware fromhard, tohard;
  static GC wgc = NULL, bgc = NULL;
  XGCValues values;
  int valuemask;
  if (to == NULL) return;
  if (from == NULL || IsWindowHardware(from))
    Punt("Can only RasterTile from Bitmap.");

  fromhard = (Hardware) from->hard.addr;
  tohard = (Hardware) to->hard.addr;

  /* Turn off graphics exposures, to avoid getting an expose event for each
  ** XCopyArea with a pixmap.
  */
  values.graphics_exposures = False;
  values.tile = fromhard->w;
  values.fill_style = FillTiled;
  values.function = rop;
  valuemask = GCFunction | GCTile | GCFillStyle | GCGraphicsExposures;
  if (IsWindowHardware (to)) {
    if (wgc == NULL) {
      wgc = XCreateGC(dpy, RootWindow(dpy, SCREEN),
		      valuemask, &values);
    } else {
      XChangeGC(dpy, wgc, valuemask, &values);
    }
    XFillRectangle(dpy, tohard->w, wgc, toPoint.dx, toPoint.dy,
		   extent.dx, extent.dy);
  } else
    if (IsBitmapHardware(to)){
      if (bgc == NULL) {
	bgc = XCreateGC(dpy, tohard->w, valuemask, &values);
      } else {
	XChangeGC(dpy, bgc, valuemask, &values);
      }
      XFillRectangle(dpy, tohard->w, bgc, toPoint.dx, toPoint.dy,
		     extent.dx, extent.dy);
    }
  to->touched = TRUE;
  to->changed = TRUE;
}

/******************* BRIGHTNESS TRANSFER FUNCTION ************************/
/*
 * 	
 * TRANSFER_SIZE and SetTransfer () control the mapping function between
 * user brightnesses and device brightnesses. The interface is expected to
 * perform this mapping of brightnesses to a sufficient resolution.
 * SetTransfer takes a table of floating point numbers between 0 and 1. User
 * brightnesses are scaled to the size of this table and mapped through it.
 * The argument table given to SetTransfer () will be deleted after use.
 * TRANSFER_SIZE is the required size of the table.
 * 	
 * It may be appropriate to half-tone on a grayscale or colour device to
 * improve rendering if it is not too expensive. TRANSFER_SIZE is the
 * size of the pattern table.
 */

void ModifyGC (color, src, dest, fromPoint, toPoint, extent)
     Colour color;
     struct hardware *src, *dest;
     DevicePoint fromPoint, toPoint, extent;
{
  int bright, ret;
  Hardware hard, grayhard, cliphard, temphard;
  struct hardware *halftone;
  unsigned long pixelvalue = blackpixel;
  XGCValues values;
  int valuemask = 0;
  Drawable root = RootWindow(dpy, SCREEN);
     
  /* Handles only monochrome for now.  Fix for color later. */

  /*
   * This routine should take a color, a destination, and a source
   * hardware.  Simplest case:
   * Solid color, No clip, No from.
   *
   * Worst case:
   * Halftone, to clip, from non-null.
   */

  bright = transfer[(int)((float)(TRANSFER_SIZE - 1) * color.brightness + .5)];

  /* Turn off graphics exposures, to avoid getting an expose event for each
  ** XCopyArea with a pixmap.
  */
  values.graphics_exposures = False;
  valuemask |= GCGraphicsExposures;

  if (bright == 0) {
    pixelvalue = blackpixel;
    ret = ISPIXEL;
  } else if (bright == TRANSFER_SIZE - 1) {
    pixelvalue = whitepixel;
    ret = ISPIXEL;
  } else {
    halftone = GraySync(bright);
    grayhard = (Hardware) halftone->hard.addr;
    ret = ISHALFTONE;
  }

  if (IsBitmapHardware(dest)) {
    root = ((Hardware) dest->hard.addr)->w;
    values.function = GXcopy;
    values.foreground = 1;
    values.background = 0;
    values.fill_style = FillSolid;
    values.clip_mask = None;
    valuemask = GCFunction|GCForeground|GCBackground|GCFillStyle|GCClipMask;
    if (ret == ISHALFTONE) {
      values.fill_style = FillTiled;
      values.tile = grayhard->w;
      values.ts_x_origin = 0;
      values.ts_y_origin = 0;
      valuemask = valuemask|GCFillStyle|GCTile|GCTileStipXOrigin|GCTileStipYOrigin;
    }
    
    if (src) {
      hard = (Hardware) src->hard.addr;
      if (dest->clip) {
	cliphard = (Hardware) dest->clip->hard.addr;
	if (ret == ISHALFTONE) {
	  NeedAux(dest);
	  BitBlt(dest->clip, dest->aux, toPoint,
		 zeroDevicePoint, extent, ROP_SOURCE);
	  BitBlt(src, dest->aux, fromPoint,
		 zeroDevicePoint, extent, ROP_AND);
	  
	  hard = (Hardware) dest->aux->hard.addr;
	  values.clip_mask = hard->w;
	  values.clip_x_origin = toPoint.dx;
	  values.clip_y_origin = toPoint.dy;
	  valuemask = valuemask|GCClipMask|GCClipXOrigin|GCClipYOrigin;
	} else {
	  values.clip_mask = cliphard->w;
	  values.clip_x_origin = 0;
	  values.clip_y_origin = 0;
	  values.fill_style = FillStippled;
	  values.stipple = hard->w;
	  values.ts_x_origin = 0;
	  values.ts_y_origin = 0;
	  valuemask = valuemask|GCClipMask|GCClipXOrigin|GCClipYOrigin|GCFillStyle|GCStipple|GCTileStipXOrigin|GCTileStipYOrigin;
	}
      } else {
	if (ret == ISHALFTONE) {
	  values.clip_mask = hard->w;
	  values.clip_x_origin = toPoint.dx;
	  values.clip_y_origin = toPoint.dy;
	  valuemask = valuemask|GCClipMask|GCClipXOrigin|GCClipYOrigin;
	} else {
	  /*
	   * This should be fixed to use stipple, not
	   * clip.
	   *
	   * values.fill_style = FillStippled;
	   * values.stipple = hard->w;
	   * valuemask = valuemask|GCFillStyle|GCStipple;
	   * gc = MakeGC;
	   */
	  NeedAux(dest);
	  BitBlt(src, dest->aux, fromPoint,
		 zeroDevicePoint, 
		 extent, ROP_SOURCE);
	  hard = (Hardware) dest->aux->hard.addr;
	  values.clip_mask = hard->w;
	  values.clip_x_origin = toPoint.dx;
	  values.clip_y_origin = toPoint.dy;
	  valuemask = valuemask|GCClipMask|GCClipXOrigin|GCClipYOrigin;
	}
      }
    } else { /* (!src) */
      if (dest->clip) {
	cliphard = (Hardware) dest->clip->hard.addr;
	values.clip_mask = cliphard->w;
	values.clip_x_origin = 0;
	values.clip_y_origin = 0;
	valuemask = valuemask|GCClipMask|GCClipXOrigin|GCClipYOrigin;
      }
    }
  } else { /* (IsWindowHardware(dest)) */
    root = RootWindow (dpy, SCREEN);
    values.function = GXcopy;
    values.foreground = pixelvalue;
    values.background = WhitePixel (dpy, SCREEN);
    values.fill_style = FillSolid;
    values.clip_mask = None;
    valuemask = GCFunction|GCForeground|GCBackground|GCFillStyle|GCClipMask;
    if (ret == ISHALFTONE) {
      values.fill_style = FillOpaqueStippled;
      values.stipple = grayhard->w;
      values.ts_x_origin = 0;
      values.ts_y_origin = 0;
      valuemask = valuemask|GCFillStyle|GCStipple|GCTileStipXOrigin|GCTileStipYOrigin;
    }

    if (src) {
      hard = (Hardware) src->hard.addr;
      if (dest->clip) {
	cliphard = (Hardware) dest->clip->hard.addr;
	if (ret == ISHALFTONE) {
	  NeedAux(dest);
	  BitBlt(dest->clip, dest->aux, toPoint,
		 zeroDevicePoint, extent, ROP_SOURCE);
	  BitBlt(src, dest->aux, fromPoint,
		 zeroDevicePoint, extent, ROP_AND);
	  
	  hard = (Hardware) dest->aux->hard.addr;
	  values.clip_mask = hard->w;
	  values.clip_x_origin = toPoint.dx;
	  values.clip_y_origin = toPoint.dy;
	  values.fill_style = FillOpaqueStippled;
	  values.stipple = grayhard->w;
	  valuemask = valuemask|GCClipMask|GCClipXOrigin|GCClipYOrigin|GCFillStyle|GCStipple;
	} else { /* ISPIXEL */
	  if (IsWindowHardware(src)) {
	    values.clip_mask = cliphard->w;
	    values.fill_style = FillTiled;
	    values.tile = hard->w;
	    valuemask = valuemask|GCClipMask|GCFillStyle|GCTile;
	  } else {
	    /* should also handle fromPoint here,
	     * but this would have to be saved in
	     * order for the cache to work properly.
	     */
	    values.clip_mask = cliphard->w;
	    values.fill_style = FillStippled;
	    values.stipple = hard->w;
	    valuemask = valuemask|GCClipMask|GCFillStyle|GCStipple;
	  }
	}
      } else { /* (!dest->clip) */
	if (ret == ISHALFTONE) {
	  values.clip_mask = hard->w;
	  values.clip_x_origin = 0;
	  values.clip_y_origin = 0;
	  valuemask = valuemask|GCClipMask|GCClipXOrigin|GCClipYOrigin;
	} else { /* ISPIXEL */
	  if (IsWindowHardware(src)) {
	    hard = (Hardware) src->hard.addr;
	    values.fill_style = FillTiled;
	    values.tile = hard->w;
	    values.ts_x_origin = 0;
	    values.ts_y_origin = 0;
	    valuemask = valuemask|GCFillStyle|GCTile|GCTileStipXOrigin|GCTileStipYOrigin;
	  } else {
	    values.fill_style = FillStippled;
	    values.stipple = hard->w;
	    values.ts_x_origin = toPoint.dx-fromPoint.dx;
	    values.ts_y_origin = toPoint.dy-fromPoint.dy;
	    valuemask = valuemask|GCFillStyle|GCStipple|GCTileStipXOrigin|GCTileStipYOrigin;
	  }
	}
      }
    } else { /* (!src) */
      hard = (Hardware) dest->hard.addr;
      if (dest->clip) {
	cliphard = (Hardware) dest->clip->hard.addr;
	if (ret == ISHALFTONE) {
/*			 if ((hard->last_used != ISHALFTONE) ||
			     (hard->last_color.halftone != grayhard->w)) {*/
	  values.clip_mask = cliphard->w;
	  values.fill_style = FillOpaqueStippled;
	  values.stipple = grayhard->w;
	  valuemask = valuemask|GCClipMask|GCFillStyle|GCStipple;
/*			      hard->gc = MakeGC;
			      hard->last_used = ISHALFTONE;
			      hard->last_color.halftone = grayhard->w;
			 }
			 gc = hard->gc;
*/
	} else { /* ISPIXEL */
	  values.clip_mask = cliphard->w;
	  valuemask = valuemask|GCClipMask;
	}
      } else { /* (!dest->clip) */
	if (ret == ISHALFTONE) {
	  values.fill_style = FillOpaqueStippled;
	  values.stipple = grayhard->w;
	  valuemask = valuemask|GCFillStyle|GCStipple;
/*		    } else {
			 if (pixelvalue == blackpixel)
			      gc = fillgc[GXcopy];
			 else if (pixelvalue == WhitePixel(dpy, SCREEN))
			      gc = whitegc;
			 else {
			      gc = MakeGC;
			 }
*/
	}
      }
    }
  }
  
  if (!GlobalGC) {
    GlobalGC = XCreateGC(dpy, root, valuemask, &values);
    GlobalMask = valuemask;
  } else {
    XChangeGC(dpy, GlobalGC, valuemask, &values);
    GlobalMask = valuemask;
  }
}

ColorType RealColor (color, pixelvalue, halftone)
     Colour color;
     unsigned long *pixelvalue;
     struct hardware **halftone;
{
  int bright;
  XGCValues values;
     
  bright = transfer[(int)((float)(TRANSFER_SIZE - 1) * color.brightness + .5)];

  if (bright == 0) {
    *pixelvalue = blackpixel;
    return ISPIXEL;
  } else if (bright == TRANSFER_SIZE - 1) {
    *pixelvalue = whitepixel;
    return ISPIXEL;
  }

  *halftone = GraySync(bright);
  return ISHALFTONE;
}

#ifdef notdef
int HardColour (colour)
     Colour colour;
{
  /* DCJ:
   * This is a hack.  This is only a hack.  If this had been real
   * code, you would have been notified of it through email.
   */
  if (blackpixel == 0)
    return (TRANSFER_SIZE - 1 - transfer [(int) ((TRANSFER_SIZE - 1)
						 * colour.brightness + .5)]);
  else
    return transfer [(int) ((float)(TRANSFER_SIZE-1)*colour.brightness + .5)];
}
#endif

void SetTransfer (tran)
     float *tran;
{
  int i;
     
  for (i = 0; i < TRANSFER_SIZE; i++)
    transfer [i] = (int) ((float) (TRANSFER_SIZE - 1) * tran[i] + .5);

  /* Optimize black and white drawing when transfer function allows it */
  bwvalid = transfer[0] == 0 && transfer[TRANSFER_SIZE-1] == TRANSFER_SIZE-1 &&
    (DefaultDepth(dpy, SCREEN) == 1);
}

/********************** BITMAP CONVERSION ********************************/
/*
 * 	
 * StringFromHardware () produces a string from its argument which describes
 * the bitmap.  The bitmap is returned in row-major order with the leftmost
 * bit of each byte in the most significant position. Rows are padded to byte
 * boundaries. Only single plane bitmaps are used.
 * 	
 * HardwareFromString () performs the inverse mapping, generating a bitmap
 * from a set of bits, given a width and height. Only single plane bitmaps are
 * used.
 */

/*char *StringFromHardware (h)
struct hardware *h;
{
    XImage *image;
    Hardware hard;
    char *result, *ptr, c;
    int x, y, i, hdx, hdy;
    if (h == NULL) return NULL;
    hard = (Hardware) h->hard.addr;
    hdx = h->extent.dx;
    hdy = h->extent.dy;
    image = XGetImage(dpy, hard->w, 0, 0, hdx, hdy, AllPlanes, ZPixmap);
    result = Malloc(((hdx + 7) / 8) * hdy);
    ptr = result;
    for (y=0 ; y<hdy ; y++) {
	for (x=0 ; x<hdx ; x+=8) {
	    c = 0;
	    for (i=0 ; i<8 ; i++) {
		c = c << 1;
		if (x+i < hdx)
		    c |= XGetPixel(image, x+i, y);
	    }
	}
	*ptr++ = c;
    }
    Free((char *) image);
    return (char *) result;
}

struct hardware *HardwareFromString (s, width, height)
char *s;
int width, height;
{
    struct hardware *h = NewBitmapHardware(width, height);
    Hardware hard = (Hardware) h->hard.addr;
    XImage *image;
    if (s == NULL) Punt("HardwareFromString called with NULL string!");
    image = XCreateImage(dpy, DefaultVisual(dpy, SCREEN),
			 DefaultDepth(dpy, SCREEN), ZPixmap, 0, s,
			 h->extent.dx, h->extent.dy, 8, 0);
    image->bitmap_bit_order = MSBFirst;
    XPutImage(dpy, hard->w, fillgc[GXcopy], image, 0, 0, 0, 0,
	      h->extent.dx, h->extent.dy);
    free((char *) image);
    return h;
}
*/
unsigned char *StringFromHardware (h)
     struct hardware *h;
{
  XImage *image;
  Hardware hard;
  unsigned char *result, *ptr, c;
  int x, y, i, hdx, hdy;

  if (h == NULL || !IsBitmapHardware(h)) return NULL;

  printf("and is getting the image.\n");
    
  hard = (Hardware) h->hard.addr;
  hdx = h->extent.dx;
  hdy = h->extent.dy;
  image = XGetImage(dpy, hard->w, 0, 0, hdx, hdy, AllPlanes, XYPixmap);
  result = (unsigned char *) Malloc(((hdx + 7) / 8) * hdy);
  ptr = result;
  for (y=0 ; y<hdy ; y++) {
    for (x=0 ; x<hdx ; x+=8) {
      c = 0;
      for (i=0 ; i<8 ; i++) {
	c = c << 1;
	if (x+i < hdx)
	  c |= XGetPixel(image, x+i, y);
      }
    }
    *ptr++ = c;
  }
  Free((char *) image);
  return result;
}

struct hardware *HardwareFromString (s, width, height)
     unsigned char *s;
     int width, height;
{
  struct hardware *h = NewBitmapHardware(width, height);
  Hardware hard = (Hardware) h->hard.addr;
  XImage *image;

  if (s == NULL) Punt("HardwareFromString called with NULL string!");
  image = XCreateImage(dpy, DefaultVisual(dpy, SCREEN),
		       1, XYBitmap, 0, s,
		       h->extent.dx, h->extent.dy, 8, 0);
  image->bitmap_bit_order = MSBFirst;
  XPutImage(dpy, hard->w, BitmapGC[GXcopy], image, 0, 0, 0, 0,
	    h->extent.dx, h->extent.dy);
  free((char *) image);
  h->touched = TRUE;
  h->changed = TRUE;
  return h;
}

/************************* HALF-TONE SCREEN *******************************/
/*
 * ScreenSize () allows PostScript to determine how large an array of sample
 * points to expect.  It should return the length of the side of the sample
 * square.
 * 	
 * BuildScreen () returns a set of sampling coordinates to PostScript to hand
 * to the users spot-function
 * 	
 * SetScreen () allows PostScript to set the thresholds for each sample point
 * so that half-tone bitmaps can be made.
 */

static int FreqSize (freq)
     float freq;
{
  int i = pixels_per_inch / freq + 0.5;
     
  if (i < 2)
    return 2;
  return i;
}

int ScreenSize (freq, rot)
     float freq, rot;
{
  int size = FreqSize (freq);
     
  return size * size;
}

void BuildScreen (freq, rotation, x, y)
     float freq, rotation, *x, *y;
{
  int size = FreqSize (freq);
  int i, j;
     
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++)
      *x++ = (2 * i - size + 1) / (float) size,
      *y++ = (2 * j - size + 1) / (float) size;
}

static int screen_cmp (a, b)
     struct screen *a, *b;
{
  float diff = a->val - b->val;
  if(diff > 0.0) return 1;
  if(diff < 0.0) return -1;
  return 0;
}

void SetScreen (freq, rotation, thresh)
     float freq, rotation, *thresh;
{
  struct hardware *temp;
  int i, j, size = FreqSize (freq);
  struct screen *p;
  int valuemask;
  XGCValues values;
  Hardware hard;
     
  if (screen) {
    for (i = 0; i < screen_size; i++)
      DestroyHardware (screen [i].shade);
    free ((char *) screen);
  }

  p = screen = (struct screen *) Malloc ((unsigned) (((screen_size =
			     size * size) + 1) * sizeof (struct screen)));
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++) {
      p->val = *thresh++;
      p->sx = i;
      p->sy = j;
      ++p;
    }
  qsort ((char *) screen, screen_size, sizeof (struct screen), screen_cmp);
  temp = NewBitmapHardware (size, size);
  XFillRectangle(dpy, ((Hardware) temp->hard.addr)->w, BitmapGC[GXset],
		 0, 0, size, size);

  for (i = 0; i < screen_size; i++) {
    screen [i].shade = NewBitmapHardware (size, size);
    BitBlt (temp, screen[i].shade,
	    zeroDevicePoint, zeroDevicePoint, 
	    NewDevicePoint (size, size), ROP_SOURCE);
    XDrawPoint(dpy, ((Hardware) temp->hard.addr)->w, BitmapGC[ROP_FALSE],
	       screen[i].sx, screen[i].sy);

    /* Turn off graphics exposures, to avoid getting an expose event for each
    ** XCopyArea with a pixmap.
    */
    values.graphics_exposures = False;
    values.function = GXcopy;
    values.foreground = blackpixel;
    values.background = whitepixel;
    values.fill_style = FillOpaqueStippled;
    hard = (Hardware) screen[i].shade->hard.addr;
    values.stipple = hard->w;
    valuemask = GCFunction|GCForeground|GCBackground|GCFillStyle|
                GCStipple|GCGraphicsExposures;
    hard->gc = XCreateGC(dpy, RootWindow(dpy, SCREEN), valuemask,
			 &values);
    screen[i].shade->touched = TRUE;
    screen[i].shade->changed = TRUE;
  }
  screen[screen_size].shade = temp;
  values.graphics_exposures = False;
  values.function = GXcopy;
  values.foreground = blackpixel;
  values.background = whitepixel;
  values.fill_style = FillOpaqueStippled;
  hard = (Hardware) temp->hard.addr;
  values.stipple = hard->w;
  valuemask = GCFunction|GCForeground|GCBackground|GCFillStyle|
              GCStipple|GCGraphicsExposures;
  hard->gc = XCreateGC(dpy, RootWindow(dpy, SCREEN), valuemask,
		       &values);
  screen[screen_size].shade->touched = TRUE;
  screen[screen_size].shade->changed = TRUE;
}

/************************* CLIPPING ******************************************/
/*
 * 	
 * SetClipHardware sets hardware which is a clip mask for BitBlt. This mask
 * should be ANDed with any output operation. If clip is NULL, masking will
 * not be needed.
 */

void SetClipHardware (h, clip)
     struct hardware *h, *clip;
{
  if (h) {
    h->clip = clip;
    if (h->clip)
      h->clip->changed = TRUE;
  }
}

/************************ UPDATE CONTROLS **********************************/
/*
 * HardUpdate is a hook to allow devices which do output buffering to flush
 * that buffering when appropriate.  This allows an interactive user to see
 * completed graphics between prompts (it is called as a side-effect of the
 * PostScript flush operator). Typically is is a no-op.
 */

void HardUpdate ()
{
  XFlush(dpy, 0);
}

/*
 * This call can be used to enable batching of output operations.
 * UpdateControl (h, FALSE) means ``start of batching'' UpdateControl (h,
 * TRUE) means ``end of batching''. It is used to improve performance on
 * machines where screen updates have a high locking overhead. It may be a
 * no-op.  The operation should nest if batching is already in progress: FALSE
 * increments a counter, TRUE decrements a counter. Display changes are
 * allowed when the counter is non-zero.
 */

#ifndef UpdateControl
void UpdateControl (h, on)
     struct hardware *h;
     int on;
{}
#endif

DevicePoint ConvertToDevicePoint (x, y)
     int x, y;
{
  DevicePoint res;

  res.dx = (int) ((float) x * (float) pixels_per_inch / 72.0);
  res.dy = (int) ((float) y * (float) pixels_per_inch / 72.0);
  
  return res;
}

