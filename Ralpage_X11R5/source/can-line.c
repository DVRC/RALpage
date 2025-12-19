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

#include "main.h"
#include "graphics.h"
#include "canon.h"

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
	 	BitBltLine (h, fromPoint, toPoint, (col == 0 ? ROP_TRUE : ROP_FALSE));
	else
 	 {
		int mx = Min (fromPoint.dx, toPoint.dx), my = Min (fromPoint.dy, toPoint.dy),
			Mx = Max (fromPoint.dx, toPoint.dx), My = Max (fromPoint.dy, toPoint.dy);
  		DevicePoint orig, ex;
  		
  		orig = NewDevicePoint (mx, my);
  		ex = NewDevicePoint (Mx - mx + 1, My - my + 1);
  		
		NeedAux (h);
 		BitBlt ((struct hardware *) NULL, h->aux, orig, orig, ex, ROP_FALSE);
 		
 		BitBltLine (h->aux, fromPoint, toPoint, ROP_TRUE);
		
		BitBlt (h->clip, h->aux, orig, orig, ex, ROP_AND);
		BitBlt (h->aux, h, orig, orig, ex, ROP_NOTAND);
		
		if (col != 0 && col != TransferSize () - 1)
		 {
		 	struct hardware *gray = GraySync (col);
		 	RasterTile (gray, h->aux, orig, ex, ROP_AND);
		 }
		BitBlt (h->aux, h, orig, orig, ex, ROP_OR);
	 }
	UpdateControl (h, TRUE);
 }
