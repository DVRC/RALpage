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

void PaintTrapezoid (to, lefttop, leftbottom, righttop, rightbottom, top, bottom, colour)
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
		BitBltTrapezoid (to, lefttop, leftbottom, righttop, rightbottom, top, bottom, ROP_TRUE);
	else if (col == TransferSize () - 1)
		BitBltTrapezoid (to, lefttop, leftbottom, righttop, rightbottom, top, bottom, ROP_FALSE);
 	else
 	 {
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
 		BitBlt ((struct hardware *) NULL, to->aux, origin, origin, extent, ROP_FALSE);
		BitBltTrapezoid (to->aux, lefttop, leftbottom, righttop, rightbottom, top, bottom, ROP_TRUE);
 		if (to->clip)
 			BitBlt (to->clip, to->aux, origin, origin, extent, ROP_AND);
 		BitBlt (to->aux, to, origin, origin, extent, ROP_NOTAND);
 		RasterTile (gray, to->aux, origin, extent, ROP_AND);
 		BitBlt (to->aux, to, origin, origin, extent, ROP_OR);
 	 }
  	UpdateControl (to, TRUE);
 }
