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

#include "hard.h"

struct device
 {
 	Matrix default_matrix;
 	Path default_clip;
 	int link_count;
	Object output_routine; /* jgm */
 	struct hardware *dev;
 };

extern struct device *NewDevice (), *NewCacheDevice (), *LinkDevice (), *DeviceFrom (), *UniqueDevice ();
extern struct device *NewBitmapDevice ();
