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

#include <vfont.h>
#ifndef VFONT_MAGIC
#define VFONT_MAGIC	0436
#endif
#ifndef NUM_DISPATCH
#define NUM_DISPATCH	256
#endif

int LoadVfontCache (string) Object string;
 {
 	Matrix m; float bbox[4];
 	FILE *fp;
 	char disk_name [BUFSIZE];
 	int i;
	struct header head;
	struct dispatch glyphs [NUM_DISPATCH];
	unsigned char *fdata;
	int size, swap = 0;
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
 	
 	if (fread ((char *) &head, sizeof (struct header), 1, fp) != 1)
 	 {
 	 	fclose (fp);
 	 	Message ("failed to read font header");
 	 	return Push (OpStack, False);
 	 }
 	if (head.magic != VFONT_MAGIC)
 	 {
 	 	struct header headswap;
 	 	swab ((char *) &head, (char *) &headswap, sizeof (struct header));
 	 	head = headswap;
 	 	if (head.magic != VFONT_MAGIC)
 	 	 {
 	 	 	fclose (fp);
 	 	 	Message ("not a vfont");
 	 	 	return Push (OpStack, False);
 	 	 }
 	 	swap = 1;
 	 }
 	if (fread ((char *) glyphs, sizeof (struct dispatch), NUM_DISPATCH, fp) != NUM_DISPATCH)
 	 {
 	 	fclose (fp);
 	 	Message ("failed to read font glyphs");
 	 	return Push (OpStack, False);
 	 }
 	if (swap)
 	 {
 	 	struct dispatch glyphsswap [NUM_DISPATCH];
 	 	swab ((char *) glyphs, (char *) glyphsswap, sizeof (glyphs));
 	 	for (i = 0; i < NUM_DISPATCH; i++)
 	 	 {
 	 		glyphs[i] = glyphsswap[i];
 	 		glyphs[i].up = glyphsswap[i].down;
 	 		glyphs[i].down = glyphsswap[i].up;
 	 		glyphs[i].left = glyphsswap[i].right;
 	 		glyphs[i].right = glyphsswap[i].left;
 	 	 }
 	 }
 	size = head.size;
	
	fdata = (unsigned char *) Malloc (size);
	
	if (fread ((char *) fdata, 1, size, fp) != size)
  	 {
		Message ("failed to read char data");
  	 	fclose (fp);
	 	Free (fdata);
  	 	return Push (OpStack, False);
 	 }
	VOID fclose (fp);
	
 	for (i = 0; i < NUM_DISPATCH; i++)
 	 {
 	 	struct hardware *newdev;
 	 	int width, height;
 	 	extern Object *encodingCache;
 	 	DevicePoint ex;
 	 	
 	 	if (glyphs[i].nbytes == 0)
 	 		continue;
 	 	width = glyphs[i].left + glyphs[i].right;
 	 	height = glyphs[i].up + glyphs[i].down;
 	 	if (width <= 0 || height <= 0)
 	 		continue;
 	 	newdev = HardwareFromString (&fdata [glyphs[i].addr], width, height);
 	 	sidev = IDTransform (NewVector ((float) -glyphs[i].left,
 	 				(float) -glyphs[i].down, 1.0), m);
  	 	exv = IDTransform (NewVector ((float) glyphs[i].right + 1,
  	 				(float) glyphs[i].up + 0.5, 1.0), m);
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
 	Free (fdata);
 	VOID GRestore ();
 	Message ("load completed");
 	
 	return Push (OpStack, True);
 }
