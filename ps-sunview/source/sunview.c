/*
 * Copyright (C) Rutherford Appleton Laboratory 1987, All Rights Reserved.
 * 
 * This source may be copied, distributed, altered or used, but not sold for profit
 * or incorporated into a product except under licence from the author.
 * It is not in the public domain.
 * This notice should remain in the source unaltered, and any changes to the source
 * made by persons other than the author should be marked as such.
 * 
 *	Crispin Goswell @ Rutherford Appleton Laboratory caag@uk.ac.rl.vd
 */
/*
 * Some rather crude hacks made to this module to get it to run under
 * sunwindows as a separate window. This code is not very pretty but seems
 * to do the job.
 *
 * 	Julian Onions @ Nottingham University <jpo@uk.ac.nott.cs>
 * 					April 1988
 *			More hacks	May 1988
 */
/*
 * Made window appear at size asked for, eg A4, and by default centred on
 * left-hand side of screen so that it fits on
 *
 * #defined WIN_MAX_HEIGHT and WIN_MAX_WIDTH below so that window is not too big
 *
 *      David Evans @ Nottingham University <dre@uk.ac.nott.cs>
 *                    May 1988
 */

#include "main.h"
#include "graphics.h"
#include "canon.h"
#include <pixrect/pixrect_hs.h>
#include <suntool/sunview.h>
#include <suntool/canvas.h>

static short icon_image[] = {
#include "postscript.icon"
};
DEFINE_ICON_FROM_IMAGE (postscript_icon, icon_image);


int rop_map [] =
 {
 	PIX_SRC & PIX_NOT (PIX_SRC),
 	PIX_SRC & PIX_DST,
 	PIX_SRC & PIX_NOT (PIX_DST),
 	PIX_SRC,
 	PIX_NOT (PIX_SRC) & PIX_DST,
 	PIX_DST,
 	PIX_SRC ^ PIX_DST,
 	PIX_SRC | PIX_DST,
 	PIX_NOT (PIX_SRC | PIX_DST),
 	PIX_NOT (PIX_SRC ^ PIX_DST),
 	PIX_NOT (PIX_DST),
 	PIX_SRC | PIX_NOT (PIX_DST),
 	PIX_NOT (PIX_SRC),
 	PIX_NOT (PIX_SRC) | PIX_DST,
 	PIX_NOT (PIX_SRC & PIX_DST),
 	PIX_SRC | PIX_NOT (PIX_SRC)
 };

static struct hardware *NewHardware ();

static struct pixrect *screen;
static	Frame	frame;
static	Canvas	canvas;
static	Pixwin	*pixw;
static  Rect	wr;
static	int my_client_object;
static int	*me = &my_client_object;

#define THESCREEN	((Pixrect *)-1)

#define	WIN_MAX_HEIGHT	900
#define	WIN_MAX_WIDTH	1152

char *malloc ();

setpgrp (n)	/* HAH - this stops sunview from messing things up! */
int	n;
{
	return 0;
}


DeviceArguments (argcp, argvp)
int	*argcp;
char	***argvp;
{
	if (frame == NULL)
		return;
	window_set (frame, FRAME_ARGC_PTR_ARGV, argcp, *argvp, 0);
}

static Notify_value my_signal_handler (me, signal, when)
int	*me;
int	signal;
Notify_signal_mode when;
{
	extern int interrupted;

	if (signal == SIGINT)
		interrupted = TRUE;
	notify_stop ();
	return NOTIFY_DONE;
}

static Notify_value my_notice_destroy (frame, status)
Frame	frame;
Destroy_status status;
{
	if (status != DESTROY_CHECKING) {
		(void) notify_stop ();
		notify_next_destroy_func (frame, status);
		Cleanup ();
		exit (0);
	}
	return notify_next_destroy_func (frame, status);
}

struct hardware *InitHardware ()
 {
	 

	screen = THESCREEN;
	frame = window_create (NULL, FRAME,
			       FRAME_ICON,	&postscript_icon,
			       FRAME_LABEL,	"Postscript Display",
			       WIN_ERROR_MSG,	"Can't Create Window",
			       0);

	canvas = window_create (frame, CANVAS,
				CANVAS_AUTO_SHRINK,	FALSE,
				CANVAS_AUTO_EXPAND,	FALSE,
				WIN_VERTICAL_SCROLLBAR,	scrollbar_create (0),
				WIN_HORIZONTAL_SCROLLBAR, scrollbar_create (0),
				WIN_ERROR_MSG,	"Can't Create Window",
				0);
	pixw = canvas_pixwin (canvas);

	notify_do_dispatch ();
	(void) notify_set_signal_func (me, my_signal_handler, SIGINT, NOTIFY_ASYNC);
	(void) notify_interpose_destroy_func (frame, my_notice_destroy);

	InitTransfer (82);	/* XXX */
	
	return NULL;
 }

struct hardware *NewWindowHardware (width, height) int width, height;
 {
 	DevicePoint real_extent;
 	struct pixrect *w, *xpr_region ();

	real_extent = NewDevicePoint (width, height);

	if ((w = xpr_region (screen, 0, 0, width, height)) == NULL)
 		return NULL;
	wr.r_left = 0;
	wr.r_top = 0;
	wr.r_width = width;
	wr.r_height = height;
	window_set (canvas, CANVAS_HEIGHT, height, CANVAS_WIDTH, width, 0);
	window_set (frame,
		    WIN_HEIGHT, height <= WIN_MAX_HEIGHT ? height : WIN_MAX_HEIGHT,
		    WIN_WIDTH, width <= WIN_MAX_WIDTH ? width : WIN_MAX_WIDTH,
		    WIN_SHOW, TRUE,
		    WIN_X, 0,
		    WIN_Y, height < WIN_MAX_HEIGHT ? (WIN_MAX_HEIGHT-height)/2 : 0,
		    0);
 	return NewHardware (w, real_extent, ISWIN);
 }

struct hardware *NewBitmapHardware (width, height) int width, height;
 {
 	DevicePoint real_extent;
 	struct pixrect *bm;
 	
 	real_extent = NewDevicePoint (width, height);
 	
 	if ((bm = mem_create (width, height, 1)) == NULL)
 	 {
 	 	fprintf (stderr, "mem_create (%d, %d, 1)\n", width, height);
 		Panic ("failed to create bitmap");
 	 }
 	
 	return NewHardware (bm, real_extent, 0);
 }

#define DeviceBitmap(h) ((struct pixrect *) ((h)->hard.addr))

static void DestroyHard (dev) struct hardware *dev;
 {
 	struct pixrect *bm = DeviceBitmap (dev);
 	
 	if (bm && bm != THESCREEN)
 		pr_destroy (bm);
 }

void DestroyHardware (dev) struct hardware *dev;
 {
 	if (dev == NULL)
 		return;
 	DestroyHard (dev);
 	if (dev->aux)
 		DestroyHardware (dev->aux);
 	Free ((char *) dev);
 }

static struct hardware *NewHardware (bm, extent, flags) struct pixrect *bm; DevicePoint extent; int flags;
 {
 	struct hardware *d = (struct hardware *) Malloc (sizeof (struct hardware));
 	
 	d->flags = flags;
 	d->hard.addr = (char *) bm;
 	d->aux = d->clip = NULL;
 	d->extent = extent;
 	
 	return d;
 }

struct hardware *HardwareFromString (s, width, height) unsigned char *s; int width, height;
 {
	int words = (width + 15) / 16;
	struct pixrect *pr = mem_create (width, height, 1);
	short *d = mpr_d (pr)->md_image;
	int odd = ((width + 7) / 8) & 1;
	int i, j;

	for (i = 0; i < height; i++)
	 {
		for (j = 0; j < words - odd; j++)
		 {
			short word = *s++;

			*d++ = (word << 8) | *s++;
		 }
		if (odd)
			*d++ = *s++ << 8;
	 }
	return NewHardware (pr, NewDevicePoint (width, height), 0);
 }

char *StringFromHardware (h) struct hardware *h;
 {
	int words = (h->extent.dx + 15) / 16;
	char *string = malloc ((h->extent.dx + 7) / 8 * h->extent.dy), *s = string;
	int i, j, odd = ((h->extent.dx + 7) / 8) & 1;
	short *d = mpr_d (DeviceBitmap (h))->md_image;

	for (i = 0; i < h->extent.dy; i++)
	 {
		for (j = 0; j < words - odd; j++)
		 {
			short word = *d++;

			*s++ = (word >> 8) & 0xFF;
			*s++ = word & 0xFF;
		 }
		if (odd)
			*s++ = (*d++ >> 8) & 0xFF;
	 }
	return string;
 }

void UpdateControl (h, flag) struct hardware *h; int flag;
 {
	static int count = 0;

	if (count == 0)
		pw_lock (pixw, &wr);
	if (flag)
		count--;
	else
		count++;
	if (count == 0)
	 {
		pw_unlock (pixw);
		notify_dispatch ();
	 }
 }

void RasterTile (from, to, toPoint, extent, rop)
	struct hardware *from, *to;
	DevicePoint toPoint, extent;
	int rop;
 {
	struct pixrect *fr;
	
 	if (to == NULL || extent.dx == 0 || extent.dy == 0)
 		return;
 	if (from)
 		fr = DeviceBitmap (from);
 	else
 		fr = NULL;
	
 	xpr_replrop (DeviceBitmap (to), toPoint.dx, toPoint.dy, extent.dx, extent.dy, rop_map [rop], fr, toPoint.dx, toPoint.dy);
 }

void BitBlt (from, to, fromPoint, toPoint, extent, rop)
	struct hardware *from, *to;
	DevicePoint fromPoint, toPoint, extent;
	int rop;
 {
	struct pixrect *fr;
	
	if (to == NULL || extent.dx == 0 || extent.dy == 0)
 		return;
 		
 	if (from)
 		fr = DeviceBitmap (from);
 	else
 	 {
 		fr = NULL;
 		rop = single_rop [rop];
 	 }
	
	xpr_rop (DeviceBitmap (to), toPoint.dx, toPoint.dy, extent.dx, extent.dy, rop_map [rop], fr, fromPoint.dx, fromPoint.dy);
 }

void BitBltLine (h, fromPoint, toPoint, rop) 
  	struct hardware *h;
	DevicePoint fromPoint, toPoint;
	int rop;
 {
 	if (h == NULL)
 		return;
 	
	switch (single_rop [rop])
	 {
	 	case ROP_FALSE: 	rop = PIX_NOT (PIX_SET); 	break;
	 	case ROP_TRUE:  	rop = PIX_SET;  	break;
	 	case ROP_NOTDEST: 	rop = PIX_NOT (PIX_SRC); 	break;
	 	case ROP_DEST: 		return; 	break;
	 	
	 	default:
	 		fprintf (stderr, "illegal rasterop\n");
	 		exit (1);
	 }
	
	xpr_vector (DeviceBitmap (h), fromPoint.dx, fromPoint.dy, toPoint.dx, toPoint.dy, rop, ~0);
 }

void BitBltBlob (to, top, height, left, right, rop) struct hardware *to; int top, height, *left, *right, rop;
 {
 	int i, op, offset = top;
 	struct pixrect *bm;
 	height += top;
 	switch (rop)
 	 {
 	 	case ROP_FALSE: 	op = PIX_NOT (PIX_SET); break;
 	 	case ROP_DEST: 		return;
 	 	case ROP_NOTDEST: 	op = PIX_NOT (PIX_SRC); break;
 	 	case ROP_TRUE: 		op = PIX_SET; break;
 	 }
 	bm = DeviceBitmap (to);
 	rop = rop_map [rop];
 	UpdateControl (to, FALSE);
 	for (i = top; i < height; i++)
 		xpr_rop (bm, left[i - offset], i, right[i - offset] - left[i - offset], 1, op, bm, 0, 0);
 	UpdateControl (to, TRUE);
 }

Pixrect *xpr_region (pw, a, b, c, d)
Pixrect	*pw;
int	a, b, c, d;
{
	if (pw == THESCREEN) {
		pw_region (pixw, a, b, c, d);
		return THESCREEN;
	}
	else
		return pr_region (pw, a, b, c, d);
}

xpr_replrop (pw1, tx, ty, ex, ey, op, pw2, fx, fy)
Pixrect	*pw1, *pw2;
{
	if (pw1 == THESCREEN)
		pw_replrop (pixw, tx, ty, ex, ey, op, pw2, fx, fy);
	else	pr_replrop (pw1, tx, ty, ex, ey, op, pw2, fx, fy);
}

xpr_rop (pw1, tx, ty, ex, ey, op, pw2, fx, fy)
Pixrect	*pw1, *pw2;
{
	if (pw1 == THESCREEN && pw2 == THESCREEN)
		pw_copy (pixw, tx, ty, ex, ey, op, pixw, fx, fy);
	else if (pw1 == THESCREEN && pw2 != THESCREEN)
		pw_rop (pixw, tx, ty, ex, ey, op, pw2, fx, fy);
	else if (pw1 != THESCREEN && pw2 == THESCREEN)
		fprintf (stderr, "ARGGGHHHHHH\n");
	else	pr_rop (pw1, tx, ty, ex, ey, op, pw2, fx, fy);
}

xpr_vector (pw, fx, fy, tx, ty, op, type)
Pixrect	*pw;
{
	if (pw == THESCREEN)
		pw_vector (pixw, fx, fy, tx, ty, op, type);
	else	pr_vector (pw, fx, fy, tx, ty, op, type);
}
