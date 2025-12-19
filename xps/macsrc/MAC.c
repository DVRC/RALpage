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
 *    Crispin Goswell @ Rutherford Appleton Laboratory caag@uk.ac.rl.vd
 *
 * Macintosh graphics routines written by David C. Jedlinsky.
 * Copyright (C) Massachusetts Institute of Technology 1989
 */

#include "main.h"
#include "graphics.h"

#define __SEG__ SegA

#include <Types.h>
#include <QuickDraw.h>
#include <Windows.h>
#include <Desk.h>
#include <Events.h>
#include <fonts.h>
#include <ToolUtils.h>

#define ISWIN   1
#define ISBITMAP 2

typedef int ColorType;
#define ISPIXEL         1
#define ISHALFTONE      2

struct screen
{
  float val;
  int sx, sy;
  struct hardware *shade;
} *screen = NULL;

static int screen_size;

static int transfer [TRANSFER_SIZE];

int pixels_per_inch;

float hppp, vppp;

int single_rop [] =
{ -1, 7, -1, 0, 3, -1, 2, 1, -1, 6, -1, -1, 4, 5, -1, -1 };

void InitTransfer (ppi) int ppi;
{
  int i;
  
  pixels_per_inch = ppi;
  for (i = 0; i < TRANSFER_SIZE; i++)
    transfer [i] = i;
}

static void Punt(str)
char *str;
{
  fprintf(stderr, "%s\n", str);
  exit(1);
}

static int SCREEN;
static struct hardware *NewHardware ();
struct hardware *GraySync ();

extern HardPoint DeviceToHard ();

#define SETRECT(rectp, _left, _top, _right, _bottom) \
  (rectp)->left = (_left), (rectp)->top = (_top), \
  (rectp)->right = (_right), (rectp)->bottom = (_bottom)

typedef struct _HardwareRec {
  WindowPtr w;
  WindowRecord wRecord;
  int last_used;
} HardwareRec, *Hardware;

/*
 * This file describes the interface that PostScript requires to the
 * graphics system at Version 1.4.
 *      
 * ''Hardware'' in this context refers to a pointer to windows and/or
 * bitmaps and is the lowest level of access that PostScript is
 * interested in. Any Hardware parameter may be expected to be NULL.
 */

/****************** CREATION OF WINDOWS AND BITMAPS *******************/

#ifndef MIN
#define MIN(x, y)       (((x) < (y)) ? (x) : (y))
#endif

/*
 * InitHardware () returns a default device which PostScript may use
 * immediately (or NULL if not appropriate).  Its size and shape are
 * not defined. Most typically the user will want to start up another
 * device before it is used anyway. No attempt will be made by
 * PostScript to Destroy the resulting device.
 */
struct hardware *InitHardware ()
{
  Rect screenRect;
  short w, h, hpi, vpi;
  
  InitGraf(&qd.thePort);
  InitFonts();
  FlushEvents(everyEvent, 0);
  InitWindows();

  screenRect = qd.screenBits.bounds;
  w = screenRect.right - screenRect.left - 8;
  h = screenRect.bottom - screenRect.top - 46;

  /*
   * Try to set pixels per point exactly, but re-scale if an 8.5x11
   * piece of paper won't fit on the screen.
   */
  ScreenRes(&hpi, &vpi);
  if (vpi*11 > h) {
    /* Assume square pixels. */
    hppp = vppp = (float) h / (792.0);
  } else {
    hppp = (float) hpi / 72.0;
    vppp = (float) vpi / 72.0;
  }
  InitTransfer(h/11);
  
  return NewHardware(w, h);
}

static struct hardware *NewHardware(width, height)
int width, height;
{
    struct hardware *to;
    Hardware hard;

    to = (struct hardware *) malloc(sizeof(struct hardware));
    hard = (Hardware) malloc(sizeof(HardwareRec));
    to->hard.addr = (char *) hard;
    to->flags = 0;
    to->aux = to->clip = (struct hardware *) NULL;
    to->extent = NewDevicePoint(width, height);
    to->touched = FALSE;
    to->changed = TRUE;
    hard->last_used = 0;
    return to;
}

/*
 * NewBitmapHardware () is expected to create a new bitmap. Only one
 * plane will be needed.
 *      
 * NewWindowHardware () is expected to create a window on the screen.
 * On a colour system this will be expected to support full colour.
 */
struct hardware *NewBitmapHardware (width, height)
int width, height;
{
  struct hardware *to = NewHardware(width, height);
  Hardware hard = (Hardware) to->hard.addr;
  BitMap *bm;
  Rect bnds;
  GrafPtr tmpgraf;
  
  if (width == 0 || height == 0) {
    return NULL;
  }

  bm = (BitMap *) Malloc(sizeof(BitMap));
  bm->baseAddr = Malloc((height*width/8)+1);
  bm->rowBytes = width/8;
  SETRECT(&bnds, 0, 0, width, height);
  Bcopy(&(bm->bounds), &bnds, sizeof(Rect));

  hard->w = (GrafPtr) Malloc(sizeof(GrafPort));
  OpenPort((GrafPtr) hard->w);
  GetPort(&tmpgraf);
  SetPort((GrafPtr) hard->w);
  SetPortBits(bm);

  SetPort(tmpgraf);
  
  to->flags = ISBITMAP;
  
  return to;
}

struct hardware *NewWindowHardware (width, height)
int width, height;
{
  struct hardware *to = NewHardware(width, height);
  Hardware hard = (Hardware) to->hard.addr;
  Rect bnds;
  EventRecord myEvent;
    
  SETRECT(&bnds, 4, 42, width+4, height+42);

  hard->w = NewWindow (&(hard->wRecord), &bnds, "PostScript", true,
                       documentProc, (WindowPtr) -1, false, 128);
  SystemTask();
  
  if (GetNextEvent(everyEvent, &myEvent))
    ;
    
  to->flags = ISWIN;
  return to;
}

/*
 * IsWindowHardware () should return TRUE if the hardware is a window,
 * FALSE otherwise.
 * 
 * IsBitmapHardware () should return TRUE if the hardware is a bitmap,
 * FALSE otherwise.
 */

int IsWindowHardware (h) struct hardware *h;
{
  return h->flags & ISWIN;
}

#define IsWindowHardware(h) ((h)->flags & ISWIN)

#define IsBitmapHardware(h) ((h)->flags & ISBITMAP)

/*
 * DestroyHardware () should release the resources required by the
 * hardware, bitmap or window.  This should cause a window device to
 * vanish. NULL is not an error (does nothing).
 */

void DestroyHardware (h)
struct hardware *h;
{
  if (h) {
    Hardware hard = (Hardware) h->hard.addr;
    if (IsWindowHardware(h))
      CloseWindow(hard->w);
    else {
      Free (hard->w->portBits.baseAddr);
      Free (&(hard->w->portBits));
      Free (hard->w);
    }
  }
}

/*
 * DeviceMatrix () should return a matrix appropriate to a device of
 * the given height and width.  For a typical display with a graphics
 * origin at the top left of a window, an appropriate definition would
 * be:
 *      
 * Matrix DeviceMatrix (width, height)
 * int width, height;
 * {
 *     return NewMatrix (PIXELS_PER_INCH / 72.0, 0.0, 0.0,
 *                       -PIXELS_PER_INCH / 72.0, 0.0, (float) height);
 * }
 */

Matrix DeviceMatrix (width, height) int width, height;
{
  Matrix m;
  NewMatrix (m, pixels_per_inch / 72.0, 0.0, 0.0,
             -pixels_per_inch / 72.0, 0.0, (float) height);
  return m;
}

/*
 * HardwareExtent () returns a DevicePoint describing the width and
 * height of the argument.  NULL has extent NewDevicePoint (0, 0).
 */

DevicePoint HardwareExtent (h) struct hardware *h;
{
  if (h)
    return h->extent;
  else
    return NewDevicePoint (0, 0);
}

static NeedAux (h) struct hardware *h;
{
  DevicePoint p;
  
  if (h->aux)
    return;
  p = HardwareExtent(h);
  h->aux = NewBitmapHardware (p.dx, p.dy);
}

static void RasterThreeAdd (from, aux, to, fromPoint, toPoint,
                            extent, rop)
     struct hardware *from, *aux, *to;
     DevicePoint fromPoint, toPoint, extent;
     int rop;
{
  BitBlt (from, aux, fromPoint, toPoint, extent, ROP_AND);
  BitBlt (aux, to, toPoint, toPoint, extent, rop);
}

static void RasterFour (from, mask, aux, to, fromPoint,
                        toPoint, extent, rop)
     struct hardware *from, *mask, *aux, *to;
     DevicePoint fromPoint, toPoint, extent;
     int rop;
{
  if (mask == NULL)
    BitBlt (from, to, fromPoint, toPoint, extent, rop);
  else if (from == NULL)
    BitBlt (mask, to, toPoint, toPoint, extent, rop);
  else {
    BitBlt (from, aux, fromPoint, toPoint, extent, ROP_SOURCE);
    RasterThreeAdd (mask, aux, to, toPoint, toPoint, extent, rop);
  }
}

/********************** OUTPUT PRIMITIVES *****************************/
/*
 * BitBlt () is a full function RasterOp. The 'rop' argument will have
 * values as described in the header file hard.h. If the from argument
 * is NULL it is taken to be a bitmap full of ones the shape of the
 * fromPoint and extent. If the to argument is NULL, this is a no-op.
 * 
 * Paint () is an addition to BitBlt. Bits that are set in the source
 * are Painted into the destination in the given colour with a copying
 * rasterop so that they replace pixels previously there. If the
 * machine does not support colour windows, half-toning should be
 * performed.  Colour values have hue, saturation and brightness
 * components. On a black and white or greyscale system the brightness
 * value will be a FP value between 0.0 (black) and 1.0 (white), which
 * can be used as a grey level.
 *      
 * Paint is expected to mask with the clip mask. BitBlt is not.
 */

void BitBlt (from, to, fromPoint, toPoint, extent, rop)
struct hardware *from, *to;
DevicePoint toPoint, fromPoint, extent;
int rop;
{
  Hardware fromhard, tohard;
  GrafPtr tmpgraf;
  Rect src, dest;
  
  if (to == NULL) return;
  /*
   * Copying from full color window to monochrome bitmap is the
   * wrong idea.
   */
  if (IsWindowHardware(from) && !IsWindowHardware(to)) return;

  tohard = (Hardware) to->hard.addr;
  
  GetPort(&tmpgraf);
  SetPort(tohard->w);

  SETRECT(&dest, toPoint.dx, toPoint.dy,
          extent.dx+toPoint.dx, extent.dy+toPoint.dy);
  SETRECT(&src, fromPoint.dx, fromPoint.dy,
          extent.dx+fromPoint.dx, extent.dy+fromPoint.dy);
  
  if (from == NULL) {
    if (single_rop[rop] != -1)
      PenMode(single_rop[rop]+8);
    else
      PenMode(patCopy);
    PaintRect(&dest);
  } else {
    fromhard = (Hardware) from->hard.addr;
    if (single_rop[rop] != -1)
      CopyBits (&(((GrafPtr) fromhard->w)->portBits),
                &(((GrafPtr) tohard->w)->portBits), &src, &dest,
                single_rop[rop], (RgnHandle) NULL);
    else {
      CopyBits (&(((GrafPtr) fromhard->w)->portBits),
                &(((GrafPtr) tohard->w)->portBits), &src, &dest,
                srcCopy, (RgnHandle) NULL);
    }
  }
  
  tohard = (Hardware) to->hard.addr;
  to->touched = TRUE;
  to->changed = TRUE;

  SetPort(tmpgraf);
}

void Paint (from, to, fromPoint, toPoint, extent, colour)
     struct hardware *from, *to;
     DevicePoint fromPoint, toPoint, extent;
     Colour colour;
{
  int col;
  
  if (to == NULL)
    return;
  
  UpdateControl (to, FALSE);
  
  col = IsWindowHardware (to) ? HardColour (colour) : 0;
  
  if (to->clip )
    NeedAux (to);
  if (col == 0)
    if (to->clip == NULL && from != NULL)
      BitBlt (from, to, fromPoint, toPoint, extent, ROP_OR);
    else
      RasterFour (from, to->clip, to->aux, to, fromPoint, toPoint,
                  extent, ROP_OR);
  else if (col == TransferSize () - 1)
    RasterFour (from, to->clip, to->aux, to, fromPoint, toPoint,
                extent, ROP_NOTAND);
  else { 
    struct hardware *gray = GraySync (col);
    NeedAux (to);
    if (from) {
      BitBlt (from, to->aux, fromPoint, toPoint, extent, ROP_SOURCE);
      if (to->clip)
        BitBlt (to->clip, to->aux, toPoint, toPoint, extent, ROP_AND);
      BitBlt (to->aux, to, toPoint, toPoint, extent, ROP_NOTAND);
      RasterTile (gray, to->aux, toPoint, extent, ROP_AND);
      BitBlt (to->aux, to, toPoint, toPoint, extent, ROP_OR);
    } else {
      if (to->clip) {
        BitBlt (to->clip, to->aux, toPoint, toPoint, extent,
                ROP_SOURCE);
        BitBlt (to->aux, to, toPoint, toPoint, extent, ROP_NOTAND);
        RasterTile (gray, to->aux, toPoint, extent, ROP_AND);
        BitBlt (to->aux, to, toPoint, toPoint, extent, ROP_OR);
      } else
        RasterTile (gray, to, toPoint, extent, ROP_SOURCE);
    }
  }
  UpdateControl (to, TRUE);
}

/*
 * FillPath () is the a new procedure which is not implemented yet.
 * It takes a path and a fill rule, and fills the path using the
 * capabilities of the display hardware.
 */
#ifdef notdef
int FillPath(to, path, rule)
struct hardware *to;
Path path;
int rule;
{
  register Path p;
  int ret, ok = FALSE, i = 0, valuemask = 0;
  struct hardware *gray = NULL;
  Hardware hard, grayhard;
  
/*  if (!vertices) {
    if ((vertices = (XPoint *) malloc(MAXPATHELEMENTS * sizeof(XPoint)))
        == NULL)
      Punt("Not enough memory for filling.\n");
  }            
  */
     
  if (to->clip)
    printf("Can't clip (yet).\n");
  
  if (EmptyPath (path))
    return TRUE;
  
  path = FlattenPath (path);
  
  if (!CloseAll (path)) {
    PathFree (path);
    
    return FALSE;
  }
  
  UpdateControl (to, FALSE);
  
  for (p = path->next; p != path; p = p->next)
    switch (p->ptype) {
    case EMove:
      if (i > 1) { }
      /* XFillPolygon(dpy, hard->w, gc, vertices, i,
                            Complex, CoordModeOrigin);*/
      /*          vertices[0].x = (short) p->pe.point.hx;
       * vertices[0].y = (short) p->pe.point.hy;
       */  i = 1;
      break;
    case ELine:
      /*  vertices[i].x = (short) p->pe.point.hx;
       * vertices[i++].y = (short) p->pe.point.hy;
       */  break;
    case EClose:
      /* XFillPolygon(dpy, hard->w, gc, vertices, i,
       * Complex, CoordModeOrigin);*/
      i = 0;
      break;
    }
  
  UpdateControl (to, TRUE);
  
  PathFree (path);
  
  to->touched = TRUE;
  to->changed = TRUE;
  
  return TRUE;
}
#endif

static short Xvalue (ax, ay, bx, by, cy) int ax, ay, bx, by, cy;
{
  if (ay - by == 0) {
    printf("Xvalue: ax=%d, ay=%d, bx=%d, by=%d, cy=%d\n",
           ax, ay, bx, by, cy);
    return bx + (cy - by) * (ax - bx);
  }
  return bx + (cy - by) * (ax - bx) / (float) (ay - by);
}

/*
 * BitBltTrapezoid () and PaintTrapezoid () render a complete
 * trapezoidal shape.  The corners of the trapezoid may lie far
 * outside the range of interesting scan-lines, but the slope of the
 * line should be clipped by the top and bottom. The coordinates are
 * half-open.
 */

void BitBltTrapezoid (to, lefttop, leftbottom, righttop, rightbottom,
                      top, bottom, rop)
struct hardware *to;
DevicePoint lefttop, leftbottom, righttop, rightbottom;
int top, bottom, rop;
{
  int i, j, temp;
  static int *left = NULL, *right = NULL;

  int
    ltx = Xvalue (lefttop.dx, lefttop.dy, leftbottom.dx,
                  leftbottom.dy, top),
    rtx = Xvalue (righttop.dx, righttop.dy, rightbottom.dx,
                  rightbottom.dy, top),
    lbx = Xvalue (lefttop.dx, lefttop.dy, leftbottom.dx,
                  leftbottom.dy, bottom),
    rbx = Xvalue (righttop.dx, righttop.dy, rightbottom.dx,
                  rightbottom.dy, bottom);
  
  if (left == NULL) {
    left = Malloc (1024*sizeof(int));
    right = Malloc (1024*sizeof(int));
  }
  
  if (ltx == lbx && rtx == rbx) {
    if (rtx < ltx)
      temp = rtx, rtx = ltx, ltx = temp;
    
    BitBlt ((struct hardware *) NULL, to,
            NewDevicePoint (0, 0), NewDevicePoint (ltx, top),
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
  BitBltBlob (to, top, bottom - top, left, right, rop);
  
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
  int col;
  
  if (to == NULL)
    return;
  
  col = IsWindowHardware (to) ? HardColour (colour) : 0;
  
  UpdateControl (to, FALSE);
  
  if (to->clip)
    NeedAux (to);
  if (col == 0)
    BitBltTrapezoid (to, lefttop, leftbottom, righttop, rightbottom,
                     top, bottom, ROP_TRUE);
  else if (col == TransferSize () - 1)
    BitBltTrapezoid (to, lefttop, leftbottom, righttop, rightbottom,
                     top, bottom, ROP_FALSE);
  else {
    DevicePoint origin, extent;
    struct hardware *gray;
    
    int left = lefttop.dx < leftbottom.dx ? lefttop.dx : leftbottom.dx,
    right = righttop.dx > rightbottom.dx ? righttop.dx : rightbottom.dx;
    
    if (righttop.dx < left)
      left = righttop.dx;
    if (rightbottom.dx < left)
      left = rightbottom.dx;
    if (lefttop.dx > right)
      right = lefttop.dx;
    if (leftbottom.dx > right)
      right = leftbottom.dx;
    origin = NewDevicePoint (left, top);
    extent = NewDevicePoint (right - left, bottom - top);
    
    gray = GraySync (col);
    NeedAux (to);
    BitBlt ((struct hardware *) NULL, to->aux, origin, origin,
            extent, ROP_FALSE);
    BitBltTrapezoid (to->aux, lefttop, leftbottom, righttop,
                     rightbottom, top, bottom, ROP_TRUE);
    if (to->clip)
      BitBlt (to->clip, to->aux, origin, origin, extent, ROP_AND);
    BitBlt (to->aux, to, origin, origin, extent, ROP_NOTAND);
    RasterTile (gray, to->aux, origin, extent, ROP_AND);
    BitBlt (to->aux, to, origin, origin, extent, ROP_OR);
  }
  UpdateControl (to, TRUE);
}

/*
 * BitBltLine () is expected to draw a line between the given points
 * with the given RasterOp and colour masking.  The line should be one
 * pixel wide and half-open.  [Thicker lines are done with BitBlt.]
 *      
 * PaintLine () is expected to Paint a line by analogy with Paint and
 * BitBlt.
 */

void BitBltLine (h, fromPoint, toPoint, rop)
struct hardware *h;
DevicePoint fromPoint, toPoint;
int rop;
{
  GrafPtr tmpgraf;

  if (h) {
    Hardware hard = (Hardware) h->hard.addr;
    GetPort(&tmpgraf);
    SetPort(hard->w);
    
    if (single_rop[rop] != -1)
      PenMode(single_rop[rop]+8);
    else
      PenMode(patCopy);

    MoveTo(fromPoint.dx, fromPoint.dy);
    LineTo(toPoint.dx, toPoint.dy);
    
    h->touched = TRUE;
    h->changed = TRUE;
    SetPort(tmpgraf);
  }
}

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
  
  if (h->clip == NULL && (col == 0 || col == TransferSize () - 1))
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
    BitBlt ((struct hardware *) NULL, h->aux, orig, orig, ex,
            ROP_FALSE);
    
    BitBltLine (h->aux, fromPoint, toPoint, ROP_TRUE);
    
    BitBlt (h->clip, h->aux, orig, orig, ex, ROP_AND);
    BitBlt (h->aux, h, orig, orig, ex, ROP_NOTAND);
    
    if (col != 0 && col != TransferSize () - 1) {
      struct hardware *gray = GraySync (col);
      RasterTile (gray, h->aux, orig, ex, ROP_AND);
    }
    BitBlt (h->aux, h, orig, orig, ex, ROP_OR);
  }
  UpdateControl (h, TRUE);
  
  h->touched = TRUE;
  h->changed = TRUE;
}

/*
 * BitBltBlob () takes a set of pixel coordinates and fills the
 * trapezon figure half open.
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
 * RasterTile () replicates the whole of ``from'' over ``to'', but
 * clipped by the rectangle bounded by ``toPoint'' and ``extent''.
 */
void RasterTile (from, to, toPoint, extent, rop)
struct hardware *from, *to;
DevicePoint toPoint, extent;
int rop;
{
  Hardware fromhard, tohard;
  
  if (to == NULL) return;
  if (from == NULL || IsWindowHardware(from))
    Punt("Can only RasterTile from Bitmap.");
  
  fromhard = (Hardware) from->hard.addr;
  tohard = (Hardware) to->hard.addr;
  to->touched = TRUE;
  to->changed = TRUE;
}

/**************** BRIGHTNESS TRANSFER FUNCTION ************************/
/*
 * TransferSize () and SetTransfer () control the mapping function
 * between user brightnesses and device brightnesses. The interface is
 * expected to perform this mapping of brightnesses to a sufficient
 * resolution.  SetTransfer takes a table of floating point numbers
 * between 0 and 1. User brightnesses are scaled to the size of this
 * table and mapped through it.  The argument table given to
 * SetTransfer () will be deleted after use.  TransferSize () simply
 * enquires the required size of the table.
 *      
 * It may be appropriate to half-tone on a grayscale or colour device
 * to improve rendering if it is not too expensive. TransferSize ()
 * returns the size of the pattern table.
 */

ColorType RealColor (color, pixelvalue, halftone)
Colour color;
unsigned long *pixelvalue;
struct hardware **halftone;
{
  int bright;
  
  bright = transfer[(int)((TRANSFER_SIZE - 1) * color.brightness + .5)];
  
  *halftone = GraySync(bright);
  return ISHALFTONE;
}

struct hardware *GraySync (col) int col;
{
  col = col * (float) screen_size / TRANSFER_SIZE + 0.5;
  
  return screen[col].shade;
}

int HardColour (colour) Colour colour;
{
  return transfer [(int) ((TRANSFER_SIZE-1) * colour.brightness + .5)];
}

int TransferSize ()
{
  return TRANSFER_SIZE;
}

#define TransferSize() TRANSFER_SIZE
     
void SetTransfer (tran) float *tran;
{
  int i;
  
  for (i = 0; i < TRANSFER_SIZE; i++)
    transfer [i] = (TRANSFER_SIZE - 1) * tran[i] + .5;
}

/******************* BITMAP CONVERSION ********************************/
/*
 * StringFromHardware () produces a string from its argument which
 * describes the bitmap.  The bitmap is returned in row-major order
 * with the leftmost bit of each byte in the most significant
 * position. Rows are padded to byte boundaries. Only single plane
 * bitmaps are used.
 *      
 * HardwareFromString () performs the inverse mapping, generating a
 * bitmap from a set of bits, given a width and height. Only single
 * plane bitmaps are used.
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
}

struct hardware *HardwareFromString (s, width, height)
unsigned char *s;
int width, height;
{
}

/********************** HALF-TONE SCREEN ******************************/
/*
 * ScreenSize () allows PostScript to determine how large an array of
 * sample points to expect.  It should return the length of the side
 * of the sample square.
 *      
 * BuildScreen () returns a set of sampling coordinates to PostScript
 * to hand to the users spot-function
 *      
 * SetScreen () allows PostScript to set the thresholds for each
 * sample point so that half-tone bitmaps can be made.
 */

static int FreqSize (freq) float freq;
{
  int i = pixels_per_inch / freq + 0.5;
  
  if (i < 2)
    return 2;
  return i;
}

int ScreenSize (freq, rot) float freq, rot;
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

static int sgn (a) float a;
{
  if (a == 0)
    return 0;
  else if (a < 0)
    return -1;
  else
    return 1;
}

static int screen_cmp (a, b) char *a, *b;
{
  struct screen *aa = (struct screen *) a, *bb = (struct screen *) b;
  return sgn (aa->val - bb->val);
}

void SetScreen (freq, rotation, thresh)
     float freq, rotation, *thresh;
{
  struct hardware *temp;
  int i, j, size = FreqSize (freq);
  struct screen *p;
  Hardware hard;
  
  if (screen) {
    for (i = 0; i < screen_size; i++)
      DestroyHardware (screen [i].shade);
    free ((char *) screen);
  }
  p = screen = (struct screen *) Malloc
    ((unsigned) (((screen_size = size * size) + 1) *
                 sizeof (struct screen)));
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++) {
      p->val = *thresh++;
      p->sx = i;
      p->sy = j;
      ++p;
    }
  qsort ((char *) screen, screen_size, sizeof (struct screen),
         screen_cmp);
  temp = NewBitmapHardware (size, size);
  BitBlt ((struct hardware *) NULL, temp, NewDevicePoint (0, 0),
          NewDevicePoint (0, 0), NewDevicePoint (size, size), ROP_TRUE);
  
  for (i = 0; i < screen_size; i++) {
    screen [i].shade = NewBitmapHardware (size, size);
    BitBlt (temp, screen[i].shade,
            NewDevicePoint (0, 0), NewDevicePoint (0, 0),
            NewDevicePoint (size, size), ROP_SOURCE);
    BitBlt ((struct hardware *) NULL, temp,
            NewDevicePoint (0, 0),
            NewDevicePoint (screen[i].sx, screen[i].sy),
            NewDevicePoint (1, 1), ROP_FALSE);
    screen[i].shade->touched = TRUE;
    screen[i].shade->changed = TRUE;
  }
  screen[screen_size].shade = temp;
  screen[screen_size].shade->touched = TRUE;
  screen[screen_size].shade->changed = TRUE;
}

/************************* CLIPPING ***********************************/
/*
 * SetClipHardware sets hardware which is a clip mask for BitBlt. This
 * mask should be ANDed with any output operation. If clip is NULL,
 * masking will not be needed.
 */

void SetClipHardware (h, clip) struct hardware *h, *clip;
{
  if (h) {
    h->clip = clip;
    if (h->clip)
      h->clip->changed = TRUE;
  }
}

/*********************** UPDATE CONTROLS ******************************/
/*
 * HardUpdate is a hook to allow devices which do output buffering to
 * flush that buffering when appropriate.  This allows an interactive
 * user to see completed graphics between prompts (it is called as a
 * side-effect of the PostScript flush operator). Typically is is a
 * no-op.
 */

void HardUpdate ()
{
}

/*
 * This call can be used to enable batching of output operations.
 * UpdateControl (h, FALSE) means ``start of batching'' UpdateControl
 * (h, TRUE) means ``end of batching''. It is used to improve
 * performance on machines where screen updates have a high locking
 * overhead. It may be a no-op.  The operation should nest if batching
 * is already in progress: FALSE increments a counter, TRUE decrements
 * a counter. Display changes are allowed when the counter is
 * non-zero.
 */

void UpdateControl (h, on)
struct hardware *h;
int on;
{
}

#define UpdateControl (h, on) {}

DevicePoint ConvertToDevicePoint (x, y) int x, y;
{
  DevicePoint res;
  
  res.dx = (int) ((float) x * (float) pixels_per_inch / 72.0);
  res.dy = (int) ((float) y * (float) pixels_per_inch / 72.0);
  
  return res;
}
