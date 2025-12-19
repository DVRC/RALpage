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

#ifndef X11

/*ARGSUSED*/
int LoadXCache (string) Object string;
 {
 	Message ("not compiled for X11 fonts");
 	
 	return Push (OpStack, False);
 }

#else

#include <X11/X.h>
#include <X11/Xproto.h>
#include "fontstruct.h"
 	

int LoadXCache (string) Object string;
 {
 	Matrix m; float bbox[4];
 	FILE *fp;
 	char disk_name [BUFSIZE], *bits;
 	FontInfoRec theFont;
 	CharInfoRec theCharInfo [256];
 	int nglyphs, i, fd;
 	Vector sidev, exv, met, GetMetrics ();
 	Object metricsDict;
 	extern Object Metrics, FontBBox;
 	
 	m = gstate->CTM;
 	ExtractBBox (bbox, DictLoad (gstate->font, FontBBox));
 	metricsDict = DictLoad (gstate->font, Metrics);
 	
 	Bcopy (disk_name, BodyString (string), lengthString (string));
  	disk_name [lengthString (string)] = '\0';
	if ((fp = fopen (disk_name, "r")) == NULL)
  		return Push (OpStack, False);
 	Message ("found a disk cache");
 	VOID GSave ();
 	
 	if (fread ((char *) &theFont, sizeof (FontInfoRec), 1, fp) != 1)
 	 {
 	 	fclose (fp);
 	 	Message ("failed to read FontInfoRec");
 	 	return Push (OpStack, False);
 	 }
 	bits = Malloc (theFont.maxbounds.byteOffset);
 	nglyphs = theFont.lastCol - theFont.firstCol + 1;
 	if (theFont.linear != 1)
 	 {
 	 	Message ("non-linear font");
  	 	fclose (fp);
	 	Free (bits);
  	 	return Push (OpStack, False);
 	 }
	 	
 	if (fread ((char *) &theCharInfo[theFont.firstCol], sizeof (CharInfoRec), nglyphs, fp) != nglyphs)
  	 {
		Message ("failed to read CharInfoRecs");
  	 	fclose (fp);
	 	Free (bits);
  	 	return Push (OpStack, False);
 	 }
	if (fread (bits, (unsigned) 1, theFont.maxbounds.byteOffset, fp) != theFont.maxbounds.byteOffset)
  	 {
		Message ("failed to read glyphs");
  	 	fclose (fp);
	 	Free (bits);
  	 	return Push (OpStack, False);
 	 }
	VOID fclose (fp);
	
 	for (i = theFont.firstCol; i <= theFont.lastCol; i++)
 	 {
 	 	struct hardware *newdev;
 	 	int width, height;
 	 	extern Object *encodingCache;
 	 	DevicePoint ex;
 	 	
 	 	if (!theCharInfo[i].exists)
 	 		continue;
 	 	width = theCharInfo[i].metrics.rightSideBearing - theCharInfo[i].metrics.leftSideBearing;
 	 	width = ((width + 31) / 32) * 32;
 	 	height = theCharInfo[i].metrics.ascent + theCharInfo[i].metrics.descent;
 	 	if (width <= 0 || height <= 0)
 	 		continue;
 	 	newdev = HardwareFromString (bits + theCharInfo[i].byteOffset, width, height);
 	 	sidev = IDTransform (NewVector ((float) theCharInfo[i].metrics.leftSideBearing, (float) -theCharInfo[i].metrics.descent, 1.0), m);
  	 	exv = IDTransform (NewVector ((float) theCharInfo[i].metrics.rightSideBearing + 1, (float) theCharInfo[i].metrics.ascent + 0.5, 1.0), m);
	 	met = GetMetrics (DictLoad (metricsDict, encodingCache[i]));
	 	VOID SetCacheDevice (encodingCache [i],
 	 				NewPoint (met.vx, met.vy),
 	 				sidev.vx, -exv.vy,
 	 				exv.vx, -sidev.vy);
	 	ex = HardwareExtent (gstate->device->dev);
	 	VOID BitBlt (newdev, gstate->device->dev,
 	 			NewDevicePoint (0, 0), NewDevicePoint (0, 0),
 	 			NewDevicePoint (width, height),
 	 			ROP_SOURCE);
 	 	DestroyHardware (newdev);
 	 }
 	Free (bits);
 	VOID GRestore ();
 	Message ("load completed");
 	
 	return Push (OpStack, True);
 }

#endif
