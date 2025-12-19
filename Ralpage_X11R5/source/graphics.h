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

#include "device.h"
#include "point.h"
#include "font.h"

#define PI 3.14159265358979

#define MAXDASH	11

#define MAXGSAVES	30

#define BBOX_LEFT	0
#define BBOX_TOP	1
#define BBOX_RIGHT	2
#define BBOX_BOTTOM	3

struct show_context
 {
 	char InShow;
 	char space;
	Point Width;
	HardPoint shim, space_shim;
 	Object CharName;
 	struct cache *ccache;
 	Matrix mat;
 };

struct state
 {
 	Matrix CTM;
 	Colour colour;
 	HardPoint cp; int cp_defined;
 	Path path;
 	Path clip;
 	Object font;
 	float line_width;
 	int line_cap;
 	int line_join;
 	struct
 	 {
 	 	float frequency, rotation, *thresh;
 	 	int count;
 	 	Object spot_function;
 	  } screen;
 	struct {
 		Object transfn;
 		float *tran;
 		int tcount;
 	} transfer;
 	int flatness;
 	float miter_limit;
 	float dash_offset, dash_array [MAXDASH];
 	int dash_length;
	struct device *device;
	struct show_context *show;
	struct device *clipdevice;
 };

extern struct state *gstate;

extern int stroke_method, fill_method;
