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

NeedAux (h) struct hardware *h;
 {
 	DevicePoint extent;
 	
 	if (h->aux)
 		return;
 	extent = HardwareExtent (h);
 	h->aux = NewBitmapHardware (extent.dx, extent.dy);
 }

void RasterThreeAdd (from, aux, to, fromPoint, toPoint, extent, rop)	/* depends on aux */
	struct hardware *from, *aux, *to;
	DevicePoint fromPoint, toPoint, extent;
	int rop;
 {
 	BitBlt (from, aux, fromPoint, toPoint, extent, ROP_AND);
 	BitBlt (aux, to, toPoint, toPoint, extent, rop);
 }

void RasterFour (from, mask, aux, to, fromPoint, toPoint, extent, rop)
	struct hardware *from, *mask, *aux, *to;
	DevicePoint fromPoint, toPoint, extent;
	int rop;
 {
 	if (mask == NULL)
 		BitBlt (from, to, fromPoint, toPoint, extent, rop);
 	else if (from == NULL)
 		BitBlt (mask, to, toPoint, toPoint, extent, rop);
 	else
 	 {
 	 	BitBlt (from, aux, fromPoint, toPoint, extent, ROP_SOURCE);
 	 	RasterThreeAdd (mask, aux, to, toPoint, toPoint, extent, rop);
 	 }
 }
