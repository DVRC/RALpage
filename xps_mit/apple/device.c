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
#include "main.h"
#include "graphics.h"

#define __SEG__ SegA

static int NullDevice ();
static int FrameDevice ();
static int GrabBits ();
static int PWindow ();

Colour Black = { 0.0, 0.0, 0.0 }, White = { 0.0, 0.0, 1.0 };

struct device *NewWindowDevice (), *NewBitmapDevice (), *NewCacheDevice ();
DevicePoint HardToDevice ();
HardPoint DeviceToHard ();
extern DevicePoint ConvertToDevicePoint ();
extern float hppp, vppp;

InitDevices ()
{
  struct hardware *h;
  DevicePoint extent;
 	
  InstallOp ("framedevice",	FrameDevice, 		4, 0, 0, 0,
	     Array, Integer, Integer, Array);
  InstallOp ("nulldevice",	NullDevice,		0, 0, 0, 0);
  InstallOp ("grabbits",	GrabBits,		4, 0, 0, 0,
	     Float, Float, Float, Float);
  InstallOp ("window",		PWindow,		5, 0, 0, 0,
	     Float, Float, Float, Float, Integer);
  
  extent = HardwareExtent (h = InitHardware ());
  gstate->device = NULL; gstate->clipdevice = NULL;
  SetDevice (LinkDevice (DeviceFrom (h, DeviceMatrix (extent.dx,
						      extent.dy))));
}

static int GrabBits (x, y, w, h)
     Object x, y, w, h;
{
  DevicePoint or,ul,lr;
  HardPoint origin, corner;
  int width, height, size;
  unsigned char *s;
  struct hardware *dev;
 	
  origin = ExtToInt (NewUserPoint (BodyReal (x), BodyReal (y)));
  corner = ExtToInt (NewUserPoint (BodyReal (x) + BodyReal (w),
			       BodyReal (y) + BodyReal (h)));
  width = (int) (corner.hx - origin.hx);
  height = (int) (corner.hy - origin.hy);
 	
  fprintf (stderr, "width = %d, height = %d\n", width, height);
 	
  dev = NewBitmapHardware (width, height);
  or.dx = (int) origin.hx; or.dy = (int) origin.hy;
  ul.dx = 0;               ul.dy = 0;
  lr.dx = width;           lr.dy = height;

  BitBlt (gstate->device->dev, dev, or, ul, lr, ROP_SOURCE);
  s = StringFromHardware (dev);
  DestroyHardware (dev);
  size = (width + 7) / 8 * height;
 	
  Push (OpStack, MakeString (s, size));
  Free (s);
  Push (OpStack, MakeInteger (width));
  Push (OpStack, MakeInteger (height));
 	
  return TRUE;
}

static int NullDevice ()
{
  extern Matrix identity;

  SetDevice (NewDevice (
 			NewClipPath (0.0, 0.0, 0.0, 0.0),
 			identity,
 			(struct hardware *) NULL));
  return TRUE;
}

/*ARGSUSED*/
static int FrameDevice (mat, width, height, proc)
     Object mat, width, height, proc;
{
  Matrix m;
 	
  if (lengthArray (mat) != 6 || !ExtractMatrix (&m, mat))
    return Error (PTypeCheck);
  if (BodyInteger (width) < 0 || BodyInteger (height) < 0)
    return Error (PRangeCheck);
  SetDevice (NewWindowDevice (BodyInteger (width) * 8, BodyInteger (height),
			      m));
  ErasePage ();
 	
  return TRUE;
}

/*ARGSUSED*/
static int PWindow (x, y, wid, hgt, typ)
     Object x, y, wid, hgt, typ;
{
  int wd, ht;
  Matrix m;
  struct device *w;
  
  wd = (int) (BodyReal (wid) * hppp);
  ht = (int) (BodyReal (hgt) * vppp);

  NewMatrix(m, hppp, 0, 0, -vppp, 0, ht);
  w = NewWindowDevice (wd, ht, m);
  SetDevice (w);
  ErasePage ();

  return Push (OpStack, MakeInteger (w));
}

struct device *DeviceFrom (h, m)
     struct hardware *h; Matrix m;
{
  DevicePoint extent;
	
  extent = HardwareExtent (h);
  return NewDevice (NewClipPath (0.0,
				 (float) extent.dx,
				 (float) extent.dy, 0.0),
		    m, h);
}

struct device *NewWindowDevice (width, height, m)
     int width, height;
     Matrix m;
{
  return DeviceFrom (NewWindowHardware (width, height),
		     m);
}

struct device *NewBitmapDevice (width, height, m)
     int width, height;
     Matrix m;
{
  return DeviceFrom (NewBitmapHardware (width, height),
		     m);
}

struct device *NewCacheDevice (m, width, height, swidth, sheight)
     Matrix m; int width, height, swidth, sheight;
{
  m.tx = (float) swidth;
  m.ty = (float) (height - sheight);
  return NewDevice (NewClipPath (0.0, (float) width,
				 (float) height, 0.0),
		    m,
		    NewBitmapHardware (width, height));
}

Path NewClipPath (left, right, top, bottom)
     float left, right, top, bottom;
{
  Path p = NewPath ();
  HardPoint cp;
  int cp_def = gstate->cp_defined;
 	
  cp = gstate->cp;
  VOID GMoveTo (p, NewHardPoint (left,  bottom));
  VOID GLineTo (p, NewHardPoint (right, bottom));
  VOID GLineTo (p, NewHardPoint (right, top));
  VOID GLineTo (p, NewHardPoint (left,  top));
  ClosePath (p);
 	
  gstate->cp = cp; gstate->cp_defined = cp_def;
 	
  return p;
}

struct device *NewDevice (clip, m, dev)
     Path clip;
     Matrix m;
     struct hardware *dev;
{
  struct device *res = (struct device *) Malloc (sizeof (struct device));
 	
  res->link_count = 0;
  res->default_clip = clip;
  res->default_matrix = m;
  res->dev = dev;
 	
  return res;
}

SetDevice (d)
     struct device *d;
{
  UnlinkDevice (gstate->device);
  gstate->device = LinkDevice (d);
  gstate->CTM = d->default_matrix;
  InitClip ();
}

int IsCache (d)
     struct device *d;
{
  return !IsWindowHardware (d->dev);
}

struct device *LinkDevice (d)
     struct device *d;
{
  if (d)
    ++d->link_count;
 	
  return d;
}

UnlinkDevice (d)
     struct device *d;
{
  if (d == NULL)
    return;
  if (--d->link_count != 0)
    return;
  if (d->dev != NULL)
    DestroyHardware (d->dev);	
  Free ((char *) d);
}

struct device *UniqueDevice (d)
     struct device *d;
{
  struct device *res;
  DevicePoint ex;
  
  if (d && d->link_count == 1)
    return d;
  ex = HardwareExtent (gstate->device->dev);
  res = LinkDevice (NewBitmapDevice (ex.dx, ex.dy, gstate->CTM));
  /*if (d)
   * {
   * BitBlt (d->dev, res->dev, NewDevicePoint (0, 0), NewDevicePoint (0, 0),
   *         ex, ROP_SOURCE);
   * UnlinkDevice (d);
   * }
   * else
   */
  BitBlt ((struct hardware *)NULL, res->dev,
	  NewDevicePoint (0, 0), NewDevicePoint (0, 0),
	  ex, ROP_FALSE);
  
  return res;
}

HardPoint DeviceToHard (p)
     DevicePoint p;
{
  HardPoint res;

  res.hx = (float) p.dx;
  res.hy = (float) p.dy;
      
  return res;
}

DevicePoint NewDevicePoint (x, y)
     int x, y;
{
  DevicePoint res;
 	
  res.dx = x;
  res.dy = y;
	
  return res;
}

/*
 * DevicePaintLine() gets called when a thin line (linewidth 0) is to
 * be drawn.
 */
DevicePaintLine (d, fromPoint, toPoint, colour)
     struct device *d;
     HardPoint fromPoint, toPoint;
     Colour colour;
{
  DevicePoint fp,tp;
  fp.dx = (int) fromPoint.hx;  fp.dy = (int) fromPoint.hy;
  tp.dx = (int) toPoint.hx;    tp.dy = (int) toPoint.hy;
  PaintLine (d->dev, fp, tp, colour);
}


#ifdef notdef
void ColourLine (h, fromPoint, toPoint, rop, colour) 
     struct hardware *h;
     DevicePoint fromPoint, toPoint;
     int rop;
     Colour colour;
{
  int x1 = fromPoint.dx, y1 = fromPoint.dy, x2 = toPoint.dx, y2 = toPoint.dy;
  int col = IsWindowHardware (h) ? HardColour (colour) : HardBlack;
	
  if (col == HardBlack) {
    ddbm = h->bm;
    dd->d_line = rop_map [rop];
    line (x1, y1, LNMOVEABS);
    line (x2, y2, LNDRAWABS);
  }
  else {
    box b;
 	 	
    b = boxbuild (Min (x1, x2), Min (y1, y2), Max (x1, x2), Max (y1, y2));
 	 	
    NeedAux (h);
    GraySync (h, col);
    bmxcopy (h->bm, b, h->bm, b, WWXOR);
 	 	
    ddbm = h->aux;
    dd->d_line = WWOR;
    line (x1, y1, LNMOVEABS);
    line (x2, y2, LNDRAWABS);
 		
    bmxcopy (h->aux,  b, h->bm,  b, WWAND | WWNOT);
    bmxcopy (h->gray, b, h->aux, b, WWAND);
    bmxcopy (h->aux,  b, h->bm,  b, rop_map [rop]);
  }
}

void ColourBitBlt (from, to, fromPoint, toPoint, extent, rop, colour)
	struct hardware *from, *to;
	DevicePoint fromPoint, toPoint, extent;
	int rop;
	Colour colour;
 {
 	box frombox, tobox;
 	int col = IsWindowHardware (to) ? HardColour (colour) : HardBlack;
 	int op = rop_map [rop];
 	
 	
 	frombox = boxbuild (fromPoint.dx, fromPoint.dy,
 		fromPoint.dx + extent.dx - 1, fromPoint.dy + extent.dy - 1);
 	tobox = boxbuild (toPoint.dx, toPoint.dy,
 		toPoint.dx + extent.dx - 1, toPoint.dy + extent.dy - 1);
 	
	if (to == NULL)
 		return;
 	
 	if (rop == ROP_FALSE)
		bmxcopy (to->bm, tobox, to->bm, tobox, WWXOR);
 	else if (rop == ROP_TRUE)
		if (col == HardBlack)
 			bmxcopy (to->bm, tobox, to->bm, tobox, WWXOR | WWNOT);
 		else
		 {
 			GraySync (to, col);
			bmxcopy (to->gray, tobox, to->bm, tobox, WWCOPY);
		 }
	else if (col == HardBlack)
		bmxcopy (from->bm, frombox, to->bm, tobox, op);
	else
	 {
 		GraySync (to, col);
		NeedAux (to);
		bmxcopy (to->gray, tobox, to->aux, tobox, WWCOPY);
		bmxcopy (from->bm, frombox, to->aux, tobox, WWAND);
		bmxcopy (to->aux, tobox, to->bm, tobox, op);
	 }
 }
#endif
