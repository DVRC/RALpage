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

void Paint  (from, to, fromPoint, toPoint, extent, colour)
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
			RasterFour (from, to->clip, to->aux, to, fromPoint, toPoint, extent, ROP_OR);
	else if (col == TransferSize () - 1)
		RasterFour (from, to->clip, to->aux, to, fromPoint, toPoint, extent, ROP_NOTAND);
 	else
 	 { 
 	 	struct hardware *gray = GraySync (col);
  		NeedAux (to);
	 	if (from)
	 	 {
	 		BitBlt (from, to->aux, fromPoint, toPoint, extent, ROP_SOURCE);
	 		if (to->clip)
	 			BitBlt (to->clip, to->aux, toPoint, toPoint, extent, ROP_AND);
	 		BitBlt (to->aux, to, toPoint, toPoint, extent, ROP_NOTAND);
	 		RasterTile (gray, to->aux, toPoint, extent, ROP_AND);
	 		BitBlt (to->aux, to, toPoint, toPoint, extent, ROP_OR);
	 	 }
	 	else
	 	 {
	 	 	if (to->clip)
	 	 	 {
	 	 		BitBlt (to->clip, to->aux, toPoint, toPoint, extent, ROP_SOURCE);
	 			BitBlt (to->aux, to, toPoint, toPoint, extent, ROP_NOTAND);
	 			RasterTile (gray, to->aux, toPoint, extent, ROP_AND);
	 			BitBlt (to->aux, to, toPoint, toPoint, extent, ROP_OR);
	 		 }
	 		else
	 		 	RasterTile (gray, to, toPoint, extent, ROP_SOURCE);
	 	 	
	 	 }
 	 }
 	UpdateControl (to, TRUE);
 }
