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

#include "mat.h"
#include "rop.h"
#include "colour.h"
#include "path.h"

typedef struct device_point { int dx, dy; } DevicePoint;

extern struct hardware *HardwareFromString (), *NewBitmapHardware (), *NewWindowHardware (), *InitHardware ();
extern DevicePoint HardwareExtent ();
extern char *StringFromHardware ();

extern void BitBlt (), BitBltBlob (), BitBltLine (), DestroyHardware ();
extern void HardUpdate (), UpdateControl ();
extern Matrix DeviceMatrix ();

extern void RasterTile (), BitBltTrapezoid ();

extern int IsWindowHardware (), TransferSize ();
extern void SetTransfer ();
extern void Paint (), PaintLine (), PaintTrapezoid ();
extern int ScreenSize ();
extern void BuildScreen (), SetScreen ();
extern DevicePoint NewDevicePoint ();
