/*
 * Copyright (C) Rutherford Appleton Laboratory 1987
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
   Apollo driver created by Henk Tullemans
   Philips Research Laboratories
   Eindhoven, The Netherlands
   tullemans@apolloway.prl.philips.nl
*/

#include "main.h"
#include "graphics.h"
#include "canon.h"

#include "/sys/ins/base.ins.c"
#include "/sys/ins/gpr.ins.c"
#include "/sys/ins/error.ins.c"
#include "/sys/ins/pad.ins.c"
#include "/sys/ins/streams.ins.c"

#define HIGHEST_PLANE 0

#define DeviceBitmap(h) (((struct pixrect *)((h)->hard.handle))->ident)

typedef struct pixrect
{
	int	ident;
} pixrect;

static  struct hardware *NewHardware ();

static struct pixrect *screen;

static gpr_$bitmap_desc_t orig_bitmap;

extern	void	Message ();

struct hardware *NewHardware (bm, extent, flags)
struct pixrect *bm;
DevicePoint extent; int flags;
 {
 	struct hardware *d;

	d = (struct hardware *) Malloc (sizeof (struct hardware));
 	
 	d->flags = flags;
 	d->hard.addr = (char *) bm;
 	d->aux = d->clip = NULL;
 	d->extent = extent;
 	return d;
 }

/*
 *	This file describes the interface that PostScript requires to the graphics system at Version 1.4.
 *	
 *	''Hardware'' in this context refers to a pointer to windows and/or bitmaps and is the lowest level
 *	of access that PostScript is interested in. Any Hardware parameter may be expected to be NULL.
 */

/*********************************** CREATION OF WINDOWS AND BITMAPS *******************/

struct hardware *InitHardware () 
{
/*
 *	InitHardware () returns a default device which PostScript may use immediately (or NULL if not appropriate).
 *	Its size and shape are not defined. Most typically the user will want to start up another device
 *	before it is used anyway. No attempt will be made by PostScript to Destroy the resulting
 *	device.
 */
	gpr_$offset_t		display_size;
	gpr_$plane_t          	hi_plane_id;
/*	gpr_$bitmap_desc_t	orig_bitmap;*/
	pad_$window_desc_t	window;
	stream_$id_t		stream;
	status_$t		status;

	Message ("InitHardware");

	InitTransfer (72);

	pad_$inq_full_window (stream_$stdout, (short) 1, window, status);
	check_status (status);

	pad_$create ("",
		    (short) 0,
		    pad_$transcript,
		    stream_$stdout,
		    pad_$top,
		    (short) 0,
		    (short) 1,
		    stream,
		    status);

	display_size.x_size = 1024;
	display_size.y_size = 1024;
	hi_plane_id = HIGHEST_PLANE;

	gpr_$init (gpr_$direct, stream, display_size, hi_plane_id, orig_bitmap, status);
	check_status (status);

	gpr_$set_cursor_active (true, status);
	check_status (status);

	return NULL;
}
struct hardware *NewBitmapHardware (width, height)
int width, height;
{
/*
 *	NewBitmapHardware () is expected to create a new bitmap. Only one plane will be needed.
 */
 	DevicePoint 		real_extent;
 	struct pixrect 		*bm;
	gpr_$plane_t    	hi_plane_id;
	gpr_$offset_t		size;
	gpr_$bitmap_desc_t	bitmap_desc;
	gpr_$attribute_desc_t	attribs_desc;
	status_$t		status;
	
	Message ("NewBitmapHardware");

	gpr_$allocate_attribute_block (attribs_desc, status);
	check_status (status);

	size.x_size = (short) width;
	size.y_size = (short) height;
	hi_plane_id = HIGHEST_PLANE;

	gpr_$allocate_bitmap (size, hi_plane_id, attribs_desc, bitmap_desc, status);
	check_status (status);

/*	gpr_$set_bitmap (bitmap_desc, status);
	check_status (status);
*/
 	real_extent = NewDevicePoint (width, height);

	bm = (struct pixrect *) Malloc (sizeof (struct pixrect));
	bm->ident = (int)bitmap_desc;

	return NewHardware (bm, real_extent, 0);
}

struct hardware *NewWindowHardware (width, height)
int width, height;
{
/*
 *	NewWindowHardware () is expected to create a window on the screen. On a colour system this will
 *	be expected to support full colour.
 */
 	DevicePoint		real_extent;
 	struct pixrect 		*w;
	name_$pname_t		graphics_window;
	gpr_$offset_t		display_size;
	gpr_$plane_t          	hi_plane_id;
	gpr_$bitmap_desc_t    	init_bitmap;
	status_$t               status;
	pad_$window_desc_t	window;
	stream_$id_t		stream_win;
	gpr_$attribute_desc_t	attribs_desc;

	Message ("NewWindowHardware");

	strcpy (graphics_window, "postscript_graphics");
	
	window.top    = 0;
	window.left   = 0;
	window.width  = width;
	window.height = height;
 
	pad_$create_window ("",
			    (short) 0,
			    pad_$transcript,
			    (short)1,
			    window,
			    stream_win,
			    status);
	check_status (status);

	pad_$set_auto_close (stream_win, (short) 1, true, status);
	check_status (status);
                              
	display_size.x_size = width;
	display_size.y_size = height;
	hi_plane_id = HIGHEST_PLANE;

	gpr_$init (gpr_$direct, stream_win, display_size, hi_plane_id, init_bitmap, status);
	check_status (status);

	gpr_$set_bitmap (init_bitmap, status);
	check_status (status);
/*
	gpr_$set_fill_background_value ((int) 0, status);
	check_status (status);
*/
	gpr_$set_draw_value ((int) 0, status);
	check_status (status);

	gpr_$set_auto_refresh (true, status);
	check_status (status);

	gpr_$set_obscured_opt (gpr_$pop_if_obs, status);
	check_status (status);

	gpr_$set_cursor_active (true, status);
	check_status (status);

 	real_extent = NewDevicePoint (width, height);
 
	w = (struct pixrect *) Malloc (sizeof (struct pixrect));
	w->ident = (int)init_bitmap;

	return NewHardware (w, real_extent, ISWIN);
}


static void DestroyHard (h) struct hardware *h;
{
	gpr_$attribute_desc_t	attribs_desc;
	status_$t		status;


	if (h->flags != ISWIN)
	{
		Message ("DestroyHard: Bitmap destroyed");
		attribs_desc = gpr_$attribute_block ((gpr_$bitmap_desc_t)DeviceBitmap(h), status);
		check_status (status);
		gpr_$deallocate_bitmap ((gpr_$bitmap_desc_t)DeviceBitmap(h), status);
		check_status (status);
		gpr_$deallocate_attribute_block (attribs_desc, status);
		check_status (status);
	}
	else
	{ 
		Message ("DestroyHard: Window destroyed");
/*
		stream_$delete ((stream_$id_t)DeviceBitmap(h), status);
		check_status (status);
*/
	}
}

void DestroyHardware (h)
struct hardware *h;
{
/*	
 *	DestroyHardware () should release the resources required by the hardware, bitmap or window.
 *	This should cause a window device to vanish. NULL is not an error (does nothing).
 */

	status_$t	status;
	boolean		delete_display;

	if (h == (struct hardware *)NULL)
		return;
 	DestroyHard (h);
 	if (h->aux)
 		DestroyHardware (h->aux);
}

/******************************************* OUTPUT PRIMITIVES ******************************/	

void BitBlt (from, to, fromPoint, toPoint, extent, rop)
	struct hardware *from, *to;
	DevicePoint toPoint, fromPoint, extent;
	int rop;
{
/*	
 *	BitBlt () is a full function RasterOp. The 'rop' argument
 *	will have values as described in the header file hard.h. If the from argument is NULL it is taken to be
 *	a bitmap full of ones the shape of the fromPoint and extent. If the to argument is NULL, this is a no-op.
 */

	gpr_$bitmap_desc_t	from_desc, to_desc;
	gpr_$window_t		from_window;
	short			from_plane, to_plane;
	gpr_$position_t		to_origin;
	status_$t               status;
	boolean			unobscured;
	struct hardware		*from_tmp;

	Message ("BitBlt");

	if (from == NULL)
	{
		from_desc = NULL;
		rop = single_rop[rop];
	}
	else
		from_desc = (gpr_$bitmap_desc_t)DeviceBitmap(from);

	if (to == NULL)
		to_desc = NULL;
	else
		to_desc = (gpr_$bitmap_desc_t)DeviceBitmap(to);

	if (to_desc == NULL || extent.dx == 0 || extent.dy == 0)
		return;

	if (from_desc == NULL)
	{
		gpr_$set_draw_value ((int) 0, status);
		check_status (status);

		from_tmp = NewBitmapHardware (extent.dx, extent.dy);
		from_desc = (gpr_$bitmap_desc_t)DeviceBitmap(from_tmp);

		gpr_$set_bitmap (from_desc, status);
		check_status (status);

		from_window.window_base.x_coord = (short)0;
		from_window.window_base.y_coord = (short)0;
		from_window.window_size.x_size = (short)extent.dx;
		from_window.window_size.y_size = (short)extent.dy;

                gpr_$rectangle (from_window, status);
		check_status (status);
	}
	else
	{
		from_window.window_base.x_coord = (short)fromPoint.dx;
		from_window.window_base.y_coord = (short)fromPoint.dy;
		from_window.window_size.x_size = (short)extent.dx;
		from_window.window_size.y_size = (short)extent.dy;
	}

	gpr_$set_bitmap (to_desc, status);
	check_status (status);

	gpr_$set_plane_mask ((short)1, status);
	check_status (status);

	gpr_$set_raster_op ((short)0, (short)rop, status);
	check_status (status);

	from_plane = (short)0;
	to_plane = (short)0;
	to_origin.x_coord = (short)toPoint.dx;	
	to_origin.y_coord = (short)toPoint.dy;	

	gpr_$bit_blt (from_desc, from_window, from_plane, to_origin, to_plane, status);
	check_status (status);

	if (from == NULL)
		DestroyHardware (from_tmp);
}

void BitBltLine (h, fromPoint, toPoint, rop)
	struct hardware *h;
	DevicePoint fromPoint, toPoint;
	int rop;
{
/*	
 *	BitBltLine () is expected to draw a line between the given points
 *	with the given RasterOp and colour masking.
 *	The line should be one pixel wide and half-open.
 *	[Thicker lines are done with BitBlt.]
 */
	gpr_$bitmap_desc_t	to_desc;
	boolean			unobscured;
	status_$t               status;
	int			new_rop;

	Message ("BitBltLine");

	if (h == NULL)
		to_desc = NULL;
	else
		to_desc = (gpr_$bitmap_desc_t)DeviceBitmap(h);

	if (to_desc == NULL)
		return;

 	switch (single_rop[rop])
 	 {
 	 	case ROP_DEST: 		return;

 	 	case ROP_NOTDEST: 	new_rop = ROP_NOTSOURCE; break;

		default:		new_rop = rop;
 	 }

	gpr_$set_bitmap (to_desc, status);
	check_status (status);

	gpr_$set_plane_mask ((short)1, status);
	check_status (status);

	gpr_$set_raster_op ((short)0, (short)new_rop, status);
	check_status (status);

	gpr_$set_draw_value ((int) 0, status);
	check_status (status);

	gpr_$move ((short)fromPoint.dx, (short)fromPoint.dy, status);
	check_status (status);

	gpr_$line ((short)toPoint.dx, (short)toPoint.dy, status);
	check_status (status);
}

void BitBltBlob (to, top, height, left, right, rop)
	struct hardware *to;
	int top, height, *left, *right, rop;
{
 /*
  *	BitBltBlob () takes a set of pixel coordinates and fills the trapezon figure
  *	half open.
  */
	gpr_$bitmap_desc_t	to_desc;
	boolean			unobscured;
	status_$t               status;
	int			new_rop;

	int			i, offset = top;
	gpr_$window_t		rectangle;

	Message ("BitBltBlob");

	height = height + top;

 	switch (rop)
 	 {
 	 	case ROP_DEST: 		return;

 	 	case ROP_NOTDEST: 	new_rop = ROP_NOTSOURCE; break;

		default:		new_rop = rop;
 	 }

	to_desc = (gpr_$bitmap_desc_t)DeviceBitmap(to);

	gpr_$set_bitmap (to_desc, status);
	check_status (status);

	gpr_$set_raster_op ((short)0, (short)new_rop, status);
	check_status (status);

	gpr_$set_plane_mask ((short)1, status);
	check_status (status);

 	for (i = top; i < height; i++)
	{
		rectangle.window_base.x_coord = (short)left [i-offset];
		rectangle.window_base.y_coord = (short)i;
		rectangle.window_size.x_size = (short)(right[i-offset]-left[i-offset]);
		rectangle.window_size.y_size = (short)1;

		gpr_$rectangle (rectangle, status);
		check_status (status);
	}

}


void RasterTile (from, to, toPoint, extent, rop)
	struct hardware *from, *to;
	DevicePoint toPoint, extent;
	int rop;
{
/*
 *	RasterTile () replicates the whole of ``from'' over ``to'', but clipped by the
 *	rectangle bounded by ``toPoint'' and ``extent''.
 */
	struct hardware		*from_tmp, *pattern;
	gpr_$bitmap_desc_t	from_desc, to_desc, from_tmp_desc, pattern_desc;
	gpr_$offset_t		from_size, to_size;
	gpr_$plane_t          	hi_plane_id;
	gpr_$window_t		to_window, from_window, clip_window;
	short			from_plane, to_plane;
	gpr_$position_t		to_origin;
	boolean			unobscured;
	status_$t               status;
	int			nx, ny, i, j;
	int			pixel_array[16];

	Message ("RasterTile");

	if (from == NULL)
		from_desc = NULL;
	else
		from_desc = (gpr_$bitmap_desc_t)DeviceBitmap(from);

	if (to == NULL)
		to_desc = NULL;
	else
		to_desc = (gpr_$bitmap_desc_t)DeviceBitmap(to);

	if (to_desc == NULL || extent.dx == 0 || extent.dy == 0)
		return;

	if (from_desc == NULL)
	{
/*		gpr_$set_draw_value ((int) 0, status);
		check_status (status);

		from_desc = to_desc;
*/
		Message ("RasterTile: from = nill-pointer");
		return;
	}

/*
   Inquire 'from' and 'to' bitmap dimensions
*/
	gpr_$inq_bitmap_dimensions (from_desc, from_size, hi_plane_id, status);
	check_status (status);

	from_window.window_base.x_coord = (short)0;
	from_window.window_base.y_coord = (short)0;
	from_window.window_size.x_size = (short)from_size.x_size;
	from_window.window_size.y_size = (short)from_size.y_size;

	gpr_$inq_bitmap_dimensions (to_desc, to_size, hi_plane_id, status);
	check_status (status);

	to_window.window_base.x_coord = (short)0;
	to_window.window_base.y_coord = (short)0;
	to_window.window_size.x_size = (short)to_size.x_size;
	to_window.window_size.y_size = (short)to_size.y_size;

	from_plane = (short)0;
	to_plane = (short)0;

/*
	Create a (32x32) bitmap for the pattern in 'from'
	See GPR Call "gpr_$set_fill_pattern" for restrictions
*/

	pattern = NewBitmapHardware (32, 32);
	pattern_desc = (gpr_$bitmap_desc_t)DeviceBitmap(pattern);

	nx = 32 / from_size.x_size; /* nx = 8 from_size.x_size = 4 i.e. int(72/20+.5) */
	ny = 32 / from_size.y_size; /* ny = 8 from_size.x_size = 4 i.e. int(72/20+.5) */

	gpr_$set_bitmap (pattern_desc, status);
	check_status (status);

	to_origin.x_coord = (short)0;
	to_origin.y_coord = (short)0;

	for (i = 0; i < ny; i++)
	{
		for (j = 0; j < nx; j++)
		{
			gpr_$bit_blt (from_desc, from_window, from_plane, to_origin, to_plane, status);
			check_status (status);

			to_origin.x_coord = (short)(to_origin.x_coord + from_size.x_size);
		} 

		to_origin.x_coord = (short)0;
		to_origin.y_coord = (short)(to_origin.y_coord + from_size.y_size);
	}

/*
	Create a temporary bitmap with the dimensions of 'to' and filled with all ones
	Set for this temporary bitmap the fill pattern as created above 
*/

	from_tmp = NewBitmapHardware (to_size.x_size, to_size.y_size);
	from_tmp_desc = (gpr_$bitmap_desc_t)DeviceBitmap(from_tmp);

	gpr_$set_bitmap (from_tmp_desc, status);
	check_status (status);

	gpr_$set_fill_pattern (pattern_desc, (short)1, status);
	check_status (status);

	gpr_$rectangle (to_window, status);
	check_status (status);

/*
	BitBlt of 'from_tmp' to 'to'
*/
	gpr_$set_bitmap (to_desc, status);
	check_status (status);

	to_origin.x_coord = (short)0;
	to_origin.y_coord = (short)0;

	gpr_$set_plane_mask ((short)1, status);
	check_status (status);

	gpr_$set_raster_op ((short)0, (short)rop, status);
	check_status (status);

	clip_window.window_base.x_coord = (short)toPoint.dx;
	clip_window.window_base.y_coord = (short)toPoint.dy;
	clip_window.window_size.x_size = (short)extent.dx;
	clip_window.window_size.y_size = (short)extent.dy;

	gpr_$set_clip_window (clip_window, status);
	check_status (status);

	gpr_$set_clipping_active (true, status);
	check_status (status);

	gpr_$bit_blt (from_tmp_desc, to_window, from_plane, to_origin, to_plane, status);
	check_status (status);

	gpr_$set_clipping_active (false, status);
	check_status (status);

	DestroyHardware (from_tmp);
	DestroyHardware (pattern);
}

/********************************** BITMAP CONVERSION ********************************/

char *StringFromHardware (h) struct hardware *h;
{
/*	
 *	StringFromHardware () produces a string from its argument which describes the bitmap.
 *	The bitmap is returned in row-major order with the leftmost bit of each byte in the most significant
 *	position. Rows are padded to byte boundaries. Only single plane bitmaps are used.
 */
/*
	int words = (h->extent.dx + 15) / 16;
	char *string = malloc ((h->extent.dx + 7) / 8 * h->extent.dy), *s = string;
	int i, j, odd = ((h->extent.dx + 7) / 8) & 1;
	short *d = mpr_d (DeviceBitmap (h))->md_image;
*/
	Message ("StringFromHardware");
/*
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
*/
}

struct hardware *HardwareFromString (s, width, height) unsigned char *s; int width, height;
{
/*
 *	
 *	HardwareFromString () performs the inverse mapping, generating a bitmap from a set of bits, given
 *	a width and height. Only single plane bitmaps are used.
 */

 	DevicePoint 			real_extent;
	gpr_$display_mode_t     	mode;
	gpr_$plane_t          		hi_plane_id;
	gpr_$offset_t			size;
	gpr_$attribute_desc_t		attribs_desc;
	gpr_$bitmap_desc_t		desc;
	gpr_$window_t			window;
/*	static gpr_$pixel_array_t	pixel_array;*/
	static int			pixel_array[300000];
	int				array_index;
	short				line_width;
	unsigned short			word;
	struct pixrect			*bm;
	status_$t			status;
	int words = (width + 15) / 16;
	int odd = ((width + 7) / 8) & 1;
	int i, j, k;

	Message ("HardwareFromString");

	gpr_$allocate_attribute_block (attribs_desc, status);
	check_status (status);

	size.x_size = (short)width;
	size.y_size = (short)height;
	hi_plane_id = HIGHEST_PLANE;

	gpr_$allocate_bitmap (size, hi_plane_id, attribs_desc, desc, status);
	check_status (status);

	gpr_$set_bitmap (desc, status);
	check_status (status);

 	real_extent = NewDevicePoint (width, height);

	window.window_base.x_coord = (short)0;
	window.window_base.y_coord = (short)0;
	window.window_size.x_size = (short)width;
	window.window_size.y_size = (short)height;

	array_index = 0;
	for (i = 0; i < height; i++)
	 {
		for (j = 0; j < words - odd; j++)
		{
			word = *s++ & 0x00FF;
			word = (word << 8) & 0xFF00;
			word = word | (*s++ & 0x00FF);

			for (k=0; k<16; k++)
			{
				pixel_array[array_index++] = (word >> (16-k-1)) & 0x0001 ;
			}
		}
		if (odd)
		{
			word = *s++ & 0x00FF;
			word = (word << 8) & 0xFF00;
			word = word | (*s++ & 0x00FF);

			for (k=0; k<16; k++)
			{
				pixel_array[array_index++] = (word >> (16-k-1)) & 0x0001;
			}
		}
	 }

	gpr_$write_pixels (pixel_array, window, status);
	check_status (status);

	bm = (struct pixrect *) Malloc (sizeof (struct pixrect));
	bm->ident = (int)desc;

	return (struct hardware *) NewHardware (bm, real_extent, 0);
}

/**************************************** UPDATE CONTROLS ******************************************/

void UpdateControl (h, on) struct hardware *h; int on;
{
/*
 * 	This call can be used to enable batching of output operations. UpdateControl (h, FALSE) means ``start of
 *	batching'' UpdateControl (h, TRUE) means ``end of batching''. It is used to improve performance on machines
 *	where screen updates have a high locking overhead. It may be a no-op.
 *	The operation should nest if batching is already in progress: FALSE increments a counter,
 *	TRUE decrements a counter. Display changes are allowed when the counter is non-zero.
 */

	boolean			unobscured;
	status_$t               status;

	Message ("UpdateControl");

	if (on)
	{
		gpr_$release_display (status);
		check_status (status);
	}
	else
	{
		unobscured = gpr_$acquire_display (status);
		check_status (status);
	}

}
/********************************** CANONICAL IMPLEMENTATION LIBRARY ******************************/

/*
 *	Some parts of the above interface can be supported by a canonical library.
 *	This library contains:

SetClipHardware
HardUpdate
IsWindowHardware
HardwareExtent

PaintTrapezoid
BitBltTrapezoid

Paint
PaintLine

DeviceMatrix
InitTransfer
TransferSize
SetTransfer
ScreenSize
BuildScreen
SetScreen

 *
 *	As the driver matures, the user may provide his own versions of the canonical routines.
 *	This leaves the following for implementation by the user.
 *

InitHardware
NewBitmapHardware
NewWindowHardware
DestroyHardware
HardwareFromString
StringFromHardware
UpdateControl
RasterTile
BitBlt
BitBltLine
BitBltBlob

 *	There is a pedagogical implementation in null.c
 *	
 *	There are a number of interface issues concerning the canonical driver.
 *	Firstly, a canonical struct hardware is defined, which contains a union of
 *	a char * and an int handle. The remainder are expected to use this to store
 *	device specific information.
 *
 *	InitTransfer() should be called during InitHardware with the number of pixels
 *	per inch on the display as an argument.
 */

check_status (status)
status_$t	status;
{
	if (status.all != status_$ok)
	{
		error_$print (status);
		exit (1);
	}
}
