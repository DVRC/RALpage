/* Copyright (C) Rutherford Appleton Laboratory 1987
 * 
 * This source may be copied, distributed, altered or used, but not sold for profit
 * or incorporated into a product except under licence from the author.
 * It is not in the public domain.
 * This notice should remain in the source unaltered, and any changes to the source
 * made by persons other than the author should be marked as such.
 * 
 *	Crispin Goswell @ Rutherford Appleton Laboratory caag@uk.ac.rl.vd
 */
#include "main.h"
#include "graphics.h"
#include "canon.h"
#include "/sys/ins/base.ins.c"
#include "/sys/ins/gpr.ins.c"
#include "/sys/ins/error.ins.c"
#include "/sys/ins/pad.ins.c"

static void rasterop ();
static void check_status ();
static stream_$id_t old_stream;

int mess = 0;

       /* An error checking routine */
#define check_status(status)     { if ((status).all != status_$ok) \
         {\
         error_$print (status);\
         exit (9999);\
         }\
 }

gpr_$bitmap_desc_t current_bm;
status_$t gstatus;


#define SetBitmap(bm) { if (current_bm != (bm)) { \
	gpr_$set_bitmap ((bm), gstatus); current_bm = (bm); check_status (gstatus); } }


struct hardware *InitHardware ()
 {
    gpr_$disp_char_t     disp;
    gpr_$bitmap_desc_t   temp_bitmap;
    gpr_$offset_t        size;
    pad_$window_desc_t   window;
    stream_$id_t         temp_stream;
    status_$t            status;
    int                  no_pixels, disp_len, disp_len_ret; 
 
    window.top = 0;
    window.left = 0;
    window.height = 20;
    window.width = 20;
                   
      /* create a window in which GPR may be initialised */
    pad_$create_window (' ', 0, pad_$transcript, (short)1, window, temp_stream, status);
    check_status (status);

    size.x_size = 1024;
    size.y_size = 1024;                                 

       /* Initialise GPR in the newly created window */
    gpr_$init (gpr_$direct, temp_stream, size, (short)0, temp_bitmap, status);
    check_status (status);
                                                       
       /* Make the new window invisible */
    pad_$make_invisible (temp_stream, (short)1, status);
    check_status (status);

    disp_len = 56;
                                          
       /* Inquire about the display */
    gpr_$inq_disp_characteristics (gpr_$direct, temp_stream, disp_len, disp, disp_len_ret, status);
    check_status (status);

    no_pixels = disp.x_pixels_per_cm * 2.56;
/* this is wrong for some reason */
   no_pixels = 100;
	InitTransfer (no_pixels);
  
	return NULL;
 }

static struct hardware *NewHardware (bits, width, height, flags) 
 gpr_$bitmap_desc_t  bits;                           
 int                 width, height, flags;
 {
 	struct hardware *d = (struct hardware *) Malloc (sizeof (struct hardware));
	if (bits == 0)
		return NULL;
 	d->hard.handle = bits;
 	d->flags = flags;
 	d->aux = d->clip = NULL;
 	d->extent = NewDevicePoint (width, height);

 	return d;
 }

struct hardware *NewWindowHardware (width, height) int width, height;
 {                        
       gpr_$offset_t        size;
       pad_$window_desc_t   window;    
       status_$t            status;
       boolean              unobscured;
       gpr_$plane_t         plane_id;
       gpr_$bitmap_desc_t   main_bitmap;
static stream_$id_t         main_stream;

/*    printf ("NewWindowHardware : width = %d, height = %d\n",width,height);*/
 
    old_stream = main_stream;   /* If a previos window existed remember it's stream id */

    if (width == 0)
           width == 1;

    if (height == 0)
           height = 1;
    
    window.top    = 0;
    window.left   = 0;
    window.width  = width;
    window.height = height;
                                                                                         
       /* Create a window of the requested size */
    pad_$create_window (' ', 0, pad_$transcript, (short)1, window, main_stream, status);
    if (status.all != status_$ok)
         return NULL;
                                                  
       /* Remove the border around the new window
    pad_$set_border (main_stream, (short)1, false, status);
    check_status (status);*/
                                                    
       /* If pad behind window is ever closed then delete the window */
    pad_$set_auto_close (main_stream, 1, true, status);
    check_status (status);

    size.x_size = 1024;
    size.y_size = 1024;
                                                                  
       /* Initialise GPR in this new window */
    gpr_$init (gpr_$direct, main_stream, size, (short)0, main_bitmap, status);
    check_status (status);
                                              
       /* If the window is ever hidden when it's needed for displaying 'pop' it first */
    gpr_$set_obscured_opt (gpr_$pop_if_obs, status);
    check_status (status); /* caag - Yuk! */

       /* Automatically refresh the window if obscured */
    gpr_$set_auto_refresh (true, status);
    check_status (status);

    printf ("new bitmap = %d\n",main_bitmap);

    gpr_$set_acq_time_out (5, status);
    check_status (status);
                                 
 	return NewHardware (main_bitmap, width, height, ISWIN);
 }
                                                                                 
struct hardware *NewBitmapHardware (width, height) int width, height;
 {                                 
    gpr_$offset_t         size;
    gpr_$plane_t          hi_plane_id;
    gpr_$attribute_desc_t attrib_block;
    gpr_$bitmap_desc_t    bitmap_desc;
    status_$t             status;

/*    printf ("NewBitmapHardware : width = %d, height = %d\n",width,height); */

    if (width == 0) 
        width = 1;
    if (height == 0)
        height = 1;

    size.x_size = width;
    size.y_size = height;

    hi_plane_id = 0;

       /* Create a bitmap */ 
    gpr_$allocate_attribute_block (attrib_block, status);
    check_status (status);
                            
       /* Create an attribute block to go with the bitmap */
    gpr_$allocate_bitmap (size, hi_plane_id, attrib_block, bitmap_desc, status);
    if (status.all != status_$ok)
        return NULL;

/*    printf ("\tbitmap_desc = %d\n",bitmap_desc);*/

 	return NewHardware (bitmap_desc, width, height, 0);
 }

static void DestroyBits (bitmap) gpr_$bitmap_desc_t bitmap;                                  
{
printf ("DestroyBits :bitmap = %d\n",bitmap);
 }

void DestroyHardware (dev) struct hardware *dev;
 {
    status_$t    status;

 	if (dev == NULL)
 		return;
 
/* NOT FULLY WORKING YET */                                                           
 
/* Need to check for a descriptor of 0 .             */
/* Need to deallocate the bitmap.                    */
/* Need to remove window if one is related to bitmap

    pad_$make_invisible (old_stream, 1, status);
    check_status (status);
 
    gpr_$deallocate_bitmap (dev->hard.handle, status);
    check_status (status); */

/* 	DestroyBits (dev->hard.handle);  */

 	if (dev->aux)
 		DestroyHardware (dev->aux); 
 	Free ((char *) dev);           
 }

struct hardware *HardwareFromString (st, width, height) unsigned char *st; int width, height;
 {  
	unsigned char *p = st;
	pad_$window_desc_t   source;    
	gpr_$position_t    dest_origin;
	struct hardware *res = NewBitmapHardware (width, height);
	gpr_$bitmap_desc_t r = res->hard.handle;
                           
	int x, y, i, bytes = (width + 7) / 8;
	
    	source.top    = 0;
    	source.left   = 0;
    	source.width  = 1;
    	source.height = 1;
	for (y = 0; y < height; y++)
	 {
		dest_origin.y_coord = y;
		dest_origin.x_coord = 0;
		for (x = 0; x < bytes; x++)
		 {
			int w = *p++;
			for (i = 0; i < 8; i++)
			 {
				if (w & 0x80)
			 	 {
					rasterop (r, r, source, dest_origin, 15);
					dest_origin.x_coord++;
				 }
				dest_origin.x_coord++;
				w >>= 1;
			 }
		}
	 }
 	Free (st);
 	return res;
 }

char *StringFromHardware (dev) struct hardware *dev;
 {                             
      /* Not implemented yet */
    printf ("\n\n\n\nStringFromHardware : dev = %d\n\n\n\n\n\n",dev->hard.handle);

 	return Malloc ((dev->extent.dx + 7) / 8 *dev->extent.dy);
 }

static int update_count = 0;

void UpdateControl (h, flag) struct hardware *h; int flag;
 {                             
	boolean unobscured;
	status_$t status;
          /* GPR has no method of implementing */
	/* reducing locking should improve performance, however - caag */
    	if (flag)
		update_count--;
	else
		update_count++;
	if (flag && update_count == 0)
		gpr_$release_display (status);
    	else if (!flag && update_count == 1)
		unobscured = gpr_$acquire_display (status);
}

/*
void RasterTileAux ();

#define FACTOR	16

void RasterTileX (from, to, toPoint, extent, rop)
	struct hardware *from, *to;
	DevicePoint toPoint, extent;
	int rop;
 {
	DevicePoint ex;
	
	ex = HardwareExtent (from);
	if (ex.dx < FACTOR || ex.dy < FACTOR)
	 {
		DevicePoint tex;
		struct hardware *temp;
		
		tex = NewDevicePoint ((FACTOR / ex.dx) * ex.dx * 2, (FACTOR / ex.dy) * ex.dy * 2);
		temp = NewBitmapHardware (tex.dx, tex.dy);
		RasterTileAux (from, temp, NewDevicePoint (0, 0), tex, ROP_SOURCE);
		RasterTileAux (temp, to, toPoint, extent, rop);
		DestroyHardware (temp);
	 }
	else
		RasterTileAux (from, to, toPoint, extent, rop);
 }
*/

void steps_across (), step_across (), steps_down (), step_down ();
void step_place (), step_copy ();

void RasterTile (from, to, toPoint, extent, rop)
	struct hardware *from, *to;
	DevicePoint toPoint, extent;
	gpr_$raster_op_t rop;
 {   
    gpr_$bitmap_desc_t original_bitmap;                             
    gpr_$position_t    dest_origin;
    gpr_$window_t      old_window, window, source;
    gpr_$mask_t        plane;
    status_$t          status;
    boolean            active, unobscured;
    short              x_inc, y_inc, x_count, y_count;
    DevicePoint sp;
                           
/*    printf ("RasterTile : from= %d to= %d\n",from->hard.handle,to->hard.handle);
    printf ("\t toPoint= %d %d extent= %d %d\n",toPoint.dx,toPoint.dy,extent.dx,extent.dy);*/

    if (to == NULL || extent.dx == 0 || extent.dy == 0)
        return;

    source.window_base.x_coord = 0;
    source.window_base.y_coord = 0;
    source.window_size.x_size = from->extent.dx;
    source.window_size.y_size = from->extent.dy;

    window.window_base.x_coord = toPoint.dx;
    window.window_base.y_coord = toPoint.dy;
    window.window_size.x_size = /* toPoint.dx + */ extent.dx ;  /* possibly have -1 at end of these */
    window.window_size.y_size = /* toPoint.dy + */ extent.dy ;  /* statements */
                                                 
	UpdateControl (to, FALSE);
      /* Find the old clipping window */
    gpr_$inq_constraints (old_window, active, plane, status);
        check_status (status);
                                        
      /* Find the original bitmap description */
    gpr_$inq_bitmap (original_bitmap, status);
                                                
      /* Set the clipping window */
    gpr_$set_clip_window (window, status);
        check_status (status);
 
      /* Make clipping active */
    gpr_$set_clipping_active (true, status);
        check_status (status);

    	x_inc = from->extent.dx;
    	y_inc = from->extent.dy;


      if (rop != ROP_SOURCE)
       {
      		/* Repeatedly copy the smaller bitmap into the larger bitmap */
     		/* GPR cannot implement bitmap replication                   */
        	for (y_count = toPoint.dy - toPoint.dy % y_inc; y_count <= toPoint.dy + extent.dy - 1 ; y_count += y_inc)
    		 {
    			for (x_count = toPoint.dx - toPoint.dx % x_inc; x_count <= toPoint.dx + extent.dx - 1 ; x_count += x_inc)
        		 {
            			dest_origin.x_coord = x_count;
            			dest_origin.y_coord = y_count;
            			rasterop (from->hard.handle, 
					to->hard.handle,
					source,
					dest_origin,
					rop);
        		 }
    		 }
	 }
	else
	 {
		if (toPoint.dy % y_inc)
		 {
			steps_across (from, to, toPoint.dx, toPoint.dy - toPoint.dy % y_inc, toPoint.dx + extent.dx, y_inc, x_inc, y_inc);
			toPoint = NewDevicePoint (toPoint.dx, toPoint.dy + y_inc);
		 }
		steps_across (from, to, toPoint.dx, toPoint.dy - toPoint.dy % y_inc, toPoint.dx + extent.dx, y_inc, x_inc, y_inc);
		steps_down (from, to, toPoint.dx, toPoint.dy, toPoint.dx + extent.dx, toPoint.dy + extent.dy, x_inc, y_inc);
	 }

       /* Make the original bitmap current again */
	SetBitmap (original_bitmap);
                                                   
       /* Restore the previous clipping window */
    gpr_$set_clip_window (old_window, status);
        check_status (status); 

    gpr_$set_clipping_active (true, status);
        check_status (status);

	UpdateControl (to, TRUE);
 }

void step_place (from, to, x, y, x_inc, y_inc)
	struct hardware *from, *to;
	int x, y, x_inc, y_inc;
 {
    gpr_$window_t source;
    gpr_$position_t    dest_origin;

    source.window_base.x_coord = 0;
    source.window_base.y_coord = 0;
    source.window_size.x_size = x_inc;
    source.window_size.y_size = y_inc;
	dest_origin.x_coord = x;
	dest_origin.y_coord = y;
	rasterop (from->hard.handle, to->hard.handle, source, dest_origin, ROP_SOURCE);
 }

void step_copy (to, fx, fy, w, h, tx, ty)
	struct hardware *to;
	int fx, fy, w, h, tx, ty;
 {
    gpr_$window_t source;
    gpr_$position_t    dest_origin;

    source.window_base.x_coord = fx;
    source.window_base.y_coord = fy;
    source.window_size.x_size = w;
    source.window_size.y_size = h;
	dest_origin.x_coord = tx;
	dest_origin.y_coord = ty;
	rasterop (to->hard.handle, to->hard.handle, source, dest_origin, ROP_SOURCE);
 }

void steps_across (from, to, sx, sy, width, height, x_inc, y_inc)
	struct hardware *from, *to;
	int sx, sy, width, height, x_inc, y_inc;
 {
	if (sx % x_inc)
	 {
		sx -= sx % x_inc;
		step_place (from, to, sx, sy, x_inc, y_inc);
		step_across (from, to, sx + x_inc, sy, width, height, x_inc, y_inc);
	 }
	else
		step_across (from, to, sx, sy, width, height, x_inc, y_inc);
 }

void step_across (from, to, sx, sy, width, height, x_inc, y_inc)
	struct hardware *from, *to;
	int sx, sy, width, height, x_inc, y_inc;
 {
	int i, step = x_inc;

	step_place (from, to, sx, sy, x_inc, y_inc);
	for (i = sx; i < width; i += step, step <<= 1)
		step_copy (to, sx, sy, i - sx + step, y_inc, i + x_inc, sy);
 }

void steps_down (from, to, sx, sy, width, height, x_inc, y_inc)
	struct hardware *from , *to;
	int sx, sy, width, height, x_inc, y_inc;
 {
	if (sy % y_inc)
	 {
		sy -= sy % y_inc;
		steps_across (from, to, sx, sy, width, height, x_inc, y_inc);
		step_down (from, to, sx, sy + y_inc, width, height, x_inc, y_inc);
	 }
	else
		step_down (from, to, sx, sy + y_inc, width, height, x_inc, y_inc);
 }

void step_down (from, to, sx, sy, width, height, x_inc, y_inc)
	struct hardware *from, *to;
	int sx, sy, width, height, x_inc, y_inc;
 {
	int i, step = y_inc;

	steps_across (from, to, sx, sy, width, y_inc, x_inc, y_inc);
	for (i = sy; i < height; i += step, step <<= 1)
		step_copy (to, sx, sy, width, i - sy + step, sx, i + y_inc);
 }

void BitBlt (from, to, fromPoint, toPoint, extent, rop)
	struct hardware  *from, *to;
	DevicePoint      fromPoint, toPoint, extent;
	gpr_$raster_op_t rop;
 {
	int up;
	DevicePoint ex;
	gpr_$window_t    source;
	gpr_$position_t  dest_origin;

	if (to == NULL)
		return;
	if (fromPoint.dx < 0)
	 {
		extent.dx += fromPoint.dx;
		toPoint.dx -= fromPoint.dx;
		fromPoint.dx = 0;
	 }
	if (fromPoint.dy < 0)
	 {
		extent.dy += fromPoint.dy;
		toPoint.dy -= fromPoint.dy;
		fromPoint.dy = 0;
	 }
	if (toPoint.dx < 0)
	 {
		extent.dx += toPoint.dx;
		fromPoint.dx -= toPoint.dx;
		toPoint.dx = 0;
	 }
	if (toPoint.dy < 0)
	 {
		extent.dy += toPoint.dy;
		fromPoint.dy -= toPoint.dy;
		toPoint.dy = 0;
	 }
	if (extent.dx <= 0 || extent.dy <= 0)
        	return;

    	if (from == NULL)
        	rop = single_rop [rop];
        source.window_base.x_coord = fromPoint.dx;
        source.window_base.y_coord = fromPoint.dy;
	source.window_size.x_size = extent.dx;
	source.window_size.y_size = extent.dy;
    
	dest_origin.x_coord = toPoint.dx;
	dest_origin.y_coord = toPoint.dy;
                                            
	up = update_count;
	if (update_count == 0)
		UpdateControl (to, FALSE);
	rasterop ((from == NULL ? NULL : from->hard.handle), to->hard.handle, source, dest_origin, rop);
	if (up == 0)
		UpdateControl (to, TRUE);
 }

void BitBltLine (h, fromPoint, toPoint, rop) 
  	struct hardware *h;
	DevicePoint fromPoint, toPoint;
	gpr_$raster_op_t rop;
 {                                
    gpr_$coordinate_t   x1,y1,x2,y2;
    boolean             unobscured;
    gpr_$plane_t        plane_id;
    status_$t           status;
 
    if (h == NULL)
        return;
 
    x1 = fromPoint.dx;
    y1 = fromPoint.dy;
    x2 = toPoint.dx;
    y2 = toPoint.dy;
                                                 
      /* Make the specified bitmap mcurrent */
	SetBitmap (h->hard.handle);

      /* Set the raster op */
    plane_id = 0;
    gpr_$set_raster_op (plane_id, rop, status);

	UpdateControl (h, FALSE);
  
    gpr_$move (x1, y1, status);
    check_status (status);

    gpr_$line (x2, y2, status);
    check_status (status);
  
	UpdateControl (h, TRUE);
 }

void BitBltBlob (to, top, height, left, right, rop) struct hardware *to; int top, height, *left, *right; gpr_$raster_op_t  rop;
 {                             
    int                 i, offset;
    gpr_$bitmap_desc_t  to_desc, from_desc;
    gpr_$position_t     dest_origin;
    gpr_$window_t       source;
    gpr_$plane_t        plane_id;
    status_$t           status;

/*    printf ("BitBltBlob : to= %d top= %d height= %d\n",to->hard.handle,top,height); */
/*    printf ("\tleft= %d right= %d rop= %d\n",*left,*right,rop);*/

    height += top;
    offset = top;

    if (rop == ROP_DEST)
         return;

    to_desc = to->hard.handle;
    from_desc = to_desc;

    plane_id = 0;
       
    UpdateControl (to, FALSE);

      /* Make specified bitmap current */
	SetBitmap (to_desc);

    gpr_$set_raster_op (plane_id, rop, status);
    check_status (status);

    source.window_base.x_coord = 0;
    source.window_base.y_coord = 0;
                                  
    for (i = top; i < height ; i++)
     {
        source.window_size.x_size = (right [i - offset] - left [i - offset]);
        source.window_size.y_size = 1;                                       

        dest_origin.x_coord = left [i - offset];
        dest_origin.y_coord = i;
                                         
          /* Raster op from from_desc bitmap to current bitmap                      */
          /* source is original size and dest_origin is origin of where to place it */
        gpr_$pixel_blt (from_desc, source, dest_origin, status);
      }

    UpdateControl (to, TRUE);
 }

static void rasterop (from_desc, to_desc, source, dest_origin, rop)
    gpr_$bitmap_desc_t  from_desc, to_desc;
    gpr_$window_t       source;
    gpr_$position_t     dest_origin;
    gpr_$raster_op_t    rop;
 {
    gpr_$offset_t       size;
    gpr_$plane_t        plane,plane_id;
    status_$t           status;

    if (from_desc == NULL)
        from_desc = to_desc;

    plane_id = 0;
      /* Make specified bitmap current */
	SetBitmap (to_desc);

    gpr_$set_raster_op (plane_id, rop, status);
    check_status (status);
                                         
    gpr_$pixel_blt (from_desc, source, dest_origin, status);
    check_status (status);
}

