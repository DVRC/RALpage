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

#define CBUTT	0
#define CROUND	1
#define CSQUARE	2

#define JMITRE	0
#define JROUND	1
#define JBEVEL	2

#define STROKE_FILL 0
#define STROKE_THIN 1

#define FILL_SINGLE		0
#define FILL_CACHE_DOUBLE	1
#define FILL_DOUBLE		2
#define FILL_DOUBLE_STROKE	3

typedef struct hard_point { float hx, hy; } HardPoint;

enum pelem_type { EHeader, EMove, ELine, EArc, ECurve, EClose };

struct path_element
 {
 	enum pelem_type ptype;
 	union {
  		HardPoint point;
		struct arc { int dir; HardPoint centre; float radius, from, to; } arc;
 		struct bezier { HardPoint x0, x1, x2; } curve;
 	} pe;
 	struct path_element *next, *last;
 };

typedef struct path_element *Path;

extern HardPoint NewHardPoint (), ExtToInt (), MoveHardPoint ();
extern Path NewPath (), PathCopy (), NewMove ();
extern int PNewPath (), PInitMatrix ();
extern Path NewClipPath (), PathRemove (), ReversePath (), FlattenPath ();
extern float Normalise (), PointX (), PointY ();
