/*
 * Copyright (C) Rutherford Appleton Laboratory 1987
 * 
 * This source may be copied, distributed, altered or used, but not
 * sold for profit or incorporated into a product except under licence
 * from the author. 
 * It is not in the public domain.
 * This notice should remain in the source unaltered, and any changes
 * to the source made by persons other than the author should be
 * marked as such. 
 * 
 *	Crispin Goswell @ Rutherford Appleton Laboratory caag@uk.ac.rl.vd
 */
#include "main.h"
#include "graphics.h"

#define __SEG__ SegH

Path NewPath ();
UserPoint NewUserPoint (), IntToExt ();

HardPoint NewHardPoint (), ExtToInt ();

struct path_element *free_pelem = NULL;

#ifdef __STDC__
int Bezier(float,float,float,float,float,float,float,float);
#endif

#ifndef NewElem
void NewElem (type, p)
     enum pelem_type type;
     Path *p;
{
  p = free_pelem;
  
  if (p != NULL)
    free_pelem = p->next;
  else
    p = (Path) Malloc (sizeof (struct path_element));
  p->ptype = type;
}
#endif

#ifndef ElemFree
void ElemFree (p)
     Path p;
{
  PanicIf (p == NULL, "ElemFree given NULL");
  p->next = free_pelem;
  free_pelem = p;
}
#endif

Path NewPath ()
{
  Path res;
  
  NewElem (EHeader, res);
  
  return res->next = res->last = res;
}

#ifndef PathFree
PathFree (p)
     Path p;
{
  while (!EmptyPath (p))
    PathDelete (p);
  ElemFree (p);
}
#endif

Path PathCopy (path)
     Path path;
{
  Path p, tmp, new;
  
  NewElem (EHeader, new);
  new->next = new->last = new;
  
  for (p = path->next; p != path; p = p->next) {
    NewElem (p->ptype, tmp);
    tmp->pe = p->pe;
    PathInsert (new, tmp);
  }
  return new;
}

#ifndef CurEType
enum pelem_type CurEType (p)
     Path p;
{
  return p->last->ptype;
}
#endif

#ifndef EmptyPath
int EmptyPath (p)
     Path p;
{
  return p->next == p;
}
#endif

#ifndef PathDelete
PathDelete (p)
     Path p;
{
  Path res = p->last, ret;
  
  PanicIf (p->next == p, "delete from empty path");
  ret = PathRemove (res);
  ElemFree (ret);
  
  return TRUE;
}
#endif

Path PathRemove (p)
     Path p;
{
  p->last->next = p->next;
  p->next->last = p->last;
  
  return p;
}

#ifndef PathInsert
PathInsert (p, i)
     Path p, i;
{
  PanicIf (i == NULL, "PathInsert given NULL");
  i->next = p;
  i->last = p->last;
  p->last->next = i;
  p->last = i;
}
#endif

int GMoveTo (p, np)
     Path p;
     HardPoint np;
{
  Path res;
  
  NewElem (EMove, res);
  res->pe.point = np;
  
  if (!EmptyPath (p) && CurEType (p) == EMove)
    PathDelete (p);
  gstate->cp_defined = TRUE;
  gstate->cp = np;
  PathInsert (p, res);
  return TRUE;
}

int GLineTo (p, np)
     Path p;
     HardPoint np;
{
  Path res;
  
  NewElem (ELine, res);
  res->pe.point = np;
  
  gstate->cp = np;
  PathInsert (p, res);
  
  return TRUE;
}

int CurveTo (p, p0, p1, p2)
     Path p;
     HardPoint p0, p1, p2;
{
  Path new;
  
  NewElem (ECurve, new);
  
  new->pe.curve.x0 = p0;
  new->pe.curve.x1 = p1;
  new->pe.curve.x2 = p2;
  gstate->cp = p2;
  PathInsert (p, new);
  return TRUE;
}

float Normalise (ang)
     float ang;
{
  while (ang < -PI || ang > PI)
    if (ang < PI)  ang += 2 * PI;
    else           ang -= 2 * PI;
  return ang;
}

int Arc (p, dir, centre, radius, ang1, ang2)
     Path p;
     int dir;
     UserPoint centre;
     float radius, ang1, ang2;
{
  Path new;
  HardPoint P1,P2,P3;
  Matrix m;
  float diff,ca,sa,x,y,t1,t2;
  int quarters;

  /* Find absolute difference between the two angles */
  diff = dir > 0 ? ang2 - ang1 : ang1 - ang2;
  while (diff < (float) 0.0) diff += (float) (2 * PI);

  /* Translate center of arc to origin */
  m = gstate->CTM;
  m.tx += centre.x * m.A + centre.y * m.C;
  m.ty += centre.x * m.B + centre.y * m.D;

  /* Don't bother rotating if starting angle is 0 */
  if(ang1 != (float) 0.0) {
    ca = cos(ang1); sa = sin(ang1);
    t1 = m.A;  t2 = m.B;
    m.A = m.A * ca + m.C * sa; m.B = m.B * ca + m.D * sa;
    m.C = m.C * ca - t1 * sa;  m.D = m.D * ca - t2 * sa;
  }

  /* For arcs larger than a quarter circle, draw quarter circles first */
  quarters = (int) (diff / (float) (PI/2 - .00001));
  if(quarters > 0) {
    diff -= quarters * (float) (PI/2);
    t2 = ((float) .5522847) * radius;  /* (4/3)*(sqrt(2) - 1) */
    if(dir > 0)
      do {
	/* Add a quarter circle, rotate m by 90 degrees */
	P1.hx = m.tx + radius*m.A + t2*m.C; P1.hy = m.ty + radius*m.B + t2*m.D;
	P3.hx = m.tx + radius*m.C;          P3.hy = m.ty + radius*m.D;
	P2.hx = P3.hx + t2*m.A;             P2.hy = P3.hy + t2*m.B;

	NewElem (ECurve, new);
 	new->pe.curve.x0 = P1;	new->pe.curve.x1 = P2;	new->pe.curve.x2 = P3;
 	gstate->cp = P3; PathInsert (p, new);

	RotatePosQ(m,t1);
      } while(--quarters > 0);
    else
      do {
	/* Add a quarter circle, rotate m by -90 degrees */
	P1.hx = m.tx + radius*m.A - t2*m.C; P1.hy = m.ty + radius*m.B - t2*m.D;
	P3.hx = m.tx - radius*m.C;          P3.hy = m.ty - radius*m.D;
	P2.hx = P3.hx + t2*m.A;             P2.hy = P3.hy + t2*m.B;

	NewElem (ECurve, new);
 	new->pe.curve.x0 = P1;	new->pe.curve.x1 = P2;	new->pe.curve.x2 = P3;
 	gstate->cp = P3; PathInsert (p, new);

	RotateNegQ(m,t1);
      } while(--quarters > 0);
  }

  /* Draw the rest of the arc, if there is any left */
  if(diff <= (float) 0.0) return TRUE;
 	
  /* Compute cosine and sine of half the angle just once */
  ca = cos(diff/2);  sa = sin(diff/2);
  if (dir < 0) {
    /* Rotate m by -diff/2 and invert x axis */
    t1 = m.A; t2 = m.B;
    m.A = m.A * ca - m.C * sa;  m.B = m.B * ca - m.D * sa;
    m.C = -m.C * ca - t1 * sa;  m.D = -m.D * ca - t2 * sa;
    x = radius * ca; y = -radius * sa;
    P3.hx = x * m.A - y * m.C + m.tx;  P3.hy = x * m.B - y * m.D + m.ty;
  }
  else {
    /* Rotate m by diff/2 */
    t1 = m.A; t2 = m.B;
    m.A = m.A * ca + m.C * sa;  m.B = m.B * ca + m.D * sa;
    m.C = m.C * ca - t1 * sa;   m.D = m.D * ca - t2 * sa;
    x = radius * ca; y = radius * sa;
    P3.hx = x * m.A + y * m.C + m.tx;  P3.hy = x * m.B + y * m.D + m.ty;
  }

  /* Compute P1 and P2 and P3 from x, y and m */
  x = (4 * radius - x) / 3;
  y = sa * (radius / ca - x) / ca;
  P1.hx = x * m.A - y * m.C + m.tx;  P1.hy = x * m.B - y * m.D + m.ty;
  P2.hx = x * m.A + y * m.C + m.tx;  P2.hy = x * m.B + y * m.D + m.ty;

  NewElem (ECurve, new);
  new->pe.curve.x0 = P1;  new->pe.curve.x1 = P2;  new->pe.curve.x2 = P3;
  gstate->cp = P3; PathInsert (p, new);
  return TRUE;
}

ArcTo (path, cp, x1, y1, x2, y2, r, xt1, yt1, xt2, yt2) 
     Path path;
     HardPoint cp;
     float x1, y1, x2, y2, r;
     float *xt1, *yt1, *xt2, *yt2;
{
  UserPoint p0;
  float x0, y0, x3, y3, dist,
  ang, anga, angb, absang, delta;
  int dir;
  
  p0 = IntToExt (cp); x0 = p0.x, y0 = p0.y;
  angb = atan2 (y2 - y1, x2 - x1);
  if (angb <= 0)
    angb += 2 * PI;
  anga = atan2 (y0 - y1, x0 - x1);
  if (anga <= 0)
    anga += 2 * PI;
  ang = angb - anga;
  if (ang <= 0)
    ang += 2 * PI;
  if (ang >= 2 * PI)
    ang -= 2 * PI;
  if (ang >= PI)
    ang -= 2 * PI;
  dir = ang <= 0 ? 1 : -1;
  dist = fabs (r / sin (ang / 2));
  absang = ang / 2 + anga;
  x3 = dist * cos (absang) + x1;
  y3 = dist * sin (absang) + y1;
  absang += PI;
  delta = (ang + (ang > 0 ? - PI : PI)) / 2;
  *xt1 = x3 + r * cos (absang - delta);
  *yt1 = y3 + r * sin (absang - delta);
  *xt2 = x3 + r * cos (absang + delta);
  *yt2 = y3 + r * sin (absang + delta);
  GLineTo (path, ExtToInt (NewUserPoint (*xt1, *yt1)));
  Arc (path,
       dir,
       NewUserPoint (x3, y3),
       r, absang - delta,
       absang + delta);
  return TRUE;
}

int ClosePath (p)
     Path p;
{
  Path pp, res;
  
  if (!gstate->cp_defined) return TRUE;
  
  NewElem (EClose, res);
  res->pe.point.hx = gstate->CTM.tx;
  res->pe.point.hy = gstate->CTM.ty;
  
  for (pp = p; pp->ptype != EMove; pp = pp->last)
    ;
  PathInsert (p, res);

  NewElem (EMove, res);
  gstate->cp = res->pe.point = pp->pe.point;
  PathInsert (p, res);
  return TRUE;
}

UserPoint NewUserPoint (x, y)
     float x, y;
{
  UserPoint res;
  
  res.x = x; res.y = y;
  return res;
}

HardPoint NewHardPoint (x, y)
     float x, y;
{
  HardPoint res;
  
  res.hx = x; res.hy = y;
  return res;
}

HardPoint MoveHardPoint (a, b)
     HardPoint a, b;
{
  return NewHardPoint (a.hx + b.hx, a.hy + b.hy);
}

UserPoint MoveUserPoint (a, b)
     UserPoint a, b;
{
  return NewUserPoint (a.x + b.x, a.y + b.y);
}

float PointX (p)
     UserPoint p;
{
  return p.x;
}

float PointY (p)
     UserPoint p;
{
  return p.y;
}

/*
 * This algorithm subdivides a Bezier curve.
 * The algorithm is taken from:
 *
 *	"The Beta2-split: A special case of the Beta-spline Curve and
 *	 Surface Representation." B. A. Barsky and A. D. DeRose. IEEE, 1985.
 *
 * A vector of control points W is transformed by two matrices into two
 * Vectors which describe the sub-polygons. This is done twice, once each
 * for the X and Y coordinates. The matrices required for the 'left' and
 * 'Right' sub-polygons are as follows:
 *
 *	 /  1    0    0    0  \			 / 1/8  3/8  3/8  1/8 \
 * W  =	|  1/2  1/2   0    0   |	W  =	|   0   1/4  1/2  1/4  |
 *  L	|  1/4  1/2  1/4   0   |	 R	|   0    0   1/2  1/2  |
 *	 \ 1/8  3/8  3/8  1/8 /			 \  0    0    0    1  /
 *
 * The algorithm subdivides the curve recursively until each sub-polygon
 * is deemed to be flat to within the required tolerance. Flatness is
 * defined as the greater distance of the inner control points (P1 & P2)
 * from the line through the outer ones (P0 & P3):
 *
 *						       P2
 *				       /-------------X
 *	      P1	   ------------    	     |\_
 *	       _X---------/			     |	\_
 *	     _/	|				     |	  \_
 *	   _/	|				     |	    \_
 *       _/	|				     |	      \_
 *     _/	| D1				  D2 |		\_
 *   _/		|				     |		  \_
 *  /		|				     |		    \
 * X-----------------------------------------------------------------X
 * P0
 * 							      P3
 */

static Path path;

#ifdef __STDC__
int Bezier (float x0,float y0,float x1,float y1,
	    float x2,float y2,float x3,float y3)
#else
int Bezier (x0,y0, x1, y1, x2, y2, x3, y3)
     float x0, y0, x1, y1, x2, y2, x3, y3;
#endif
{
  Path new;
  HardPoint P3;
  float xsave,ysave,y,opp,adj;
  float flatness;
	
  /* If bezier curve is a point, return a path to (x3,y3) */
  opp = y3-y0;    adj = x3-x0;
  if(opp == (float) 0.0 && adj == (float) 0.0) {
    P3.hx = x3;   P3.hy = y3;
    NewElem (ELine, new);
    gstate->cp = new->pe.point = P3;
    PathInsert (path, new);
    return TRUE;
  }

  /* If flatness of (x1,y1) and (x2,y2) are good, return a path to (x3,y3) */
  flatness = gstate->flatness * sqrt(opp*opp + adj*adj);
  y = - opp*(x1-x0) + adj*(y1-y0);
  if (y >= -flatness && y <= flatness) {
    y = - opp*(x2-x0) + adj*(y2-y0);
    if (y >= -flatness && y <= flatness) {
      P3.hx = x3;   P3.hy = y3;
      NewElem (ELine, new);
      gstate->cp = new->pe.point = P3;
      PathInsert (path, new);
      return TRUE;
    }
  }

  /* Subdivide bezier curve into two smaller ones and recur */
  xsave = (x0+x3)/8 + 3*(x1+x2)/8;
  ysave = (y0+y3)/8 + 3*(y1+y2)/8;
  if (!Bezier(x0, 		   y0,
	      (x0+x1)/2, 	   (y0+y1)/2,
	      (x0+x2)/4 + x1/2,    (y0+y2)/4 + y1/2,
	      xsave,               ysave))
    return 0;
  return Bezier(xsave,            ysave,
		(x1+x3)/4 + x2/2, (y1+y3)/4 + y2/2,
		(x2+x3)/2,        (y2+y3)/2,
		x3,               y3);
}

static int RevSeg (new, p, next)
     Path new, next;
     HardPoint p;
{
  switch (next->ptype) {
  case EClose:
  case EHeader:
  case EMove:	return GMoveTo (new, p);
  case ELine:	return GLineTo (new, p);
  case ECurve:	return CurveTo (new, next->pe.curve.x1, next->pe.curve.x0, p);
  }
  Panic ("RevSeg: unknown pe type");
  return TRUE;
}

Path ReversePath (path)
     Path path;
{
  Path p, new, pnew;
  int closed = FALSE;
  
  NewElem (EHeader, new);
  new->next = new->last = new;
  pnew = new;
  
  for (p = path->last; p != path; p = p->last)
    switch (p->ptype) {
    case EClose:
      closed = TRUE;
      break;
      
    case EMove:
      if (!RevSeg (new, p->pe.point, p->next)) {
	PathFree (new);
	return NULL;
      }
      if (closed) {
	if (!ClosePath (new)) {
	  PathFree (new);
	  return NULL;
	}
      }
      new = new->next;
      closed = FALSE;
      break;
      
    case ELine:
      if (!RevSeg (new, p->pe.point, p->next)) {
	PathFree (new);
	return NULL;
      }
      
      break;
      
    case ECurve:
      if (!RevSeg (new, p->pe.curve.x2, p->next)) {
	PathFree (new);
	return NULL;
      }
      
      break;
      
    default:
      Panic ("ReversePath: unknown element type");
    }
  PathFree (p);
  
  return pnew;
}

Path FlattenPath (arg)
     Path arg;
{
  Path p, pp, new, add;
  HardPoint lastp;

  NewElem (EHeader, new);
  new->next = new->last = new;
  path = new;
  for (p = arg->next; p != arg; p = p->next)
    switch (p->ptype) {
      case EMove:
        lastp = p->pe.point;
	NewElem (EMove, add);
 	add->pe.point = lastp;
 	if (!EmptyPath (new) && CurEType (new) == EMove) PathDelete (new);
 	gstate->cp_defined = TRUE;
 	gstate->cp = lastp;
 	PathInsert (new, add);
	break;
      case ELine:
	lastp = p->pe.point;
	NewElem (ELine, add);
 	add->pe.point = lastp;
  	gstate->cp = lastp;
 	PathInsert (new, add);
	break;
      case EClose:
	if (gstate->cp_defined) {
	  NewElem (EClose, add);
	  add->pe.point.hx = gstate->CTM.tx;
	  add->pe.point.hy = gstate->CTM.ty;
  
	  for (pp = new; pp->ptype != EMove; pp = pp->last)
	    ;
	  PathInsert (new, add);

	  NewElem (EMove, add);
	  gstate->cp = add->pe.point = pp->pe.point;
	  PathInsert (new, add);
	}
	break;
      case ECurve:
	if (!Bezier(lastp.hx,          lastp.hy,
		    p->pe.curve.x0.hx, p->pe.curve.x0.hy,
		    p->pe.curve.x1.hx, p->pe.curve.x1.hy,
		    p->pe.curve.x2.hx, p->pe.curve.x2.hy)) {
	  PathFree (new);
	  return NULL;
	}
	lastp = p->pe.curve.x2;
	break;
      default:
	Panic ("Flattenpath discovers unknown path element type");
	break;
    }
  return path;
}

int CloseAll (path)
     Path path;
{
  /* closes all open portions of a path *in place* */
  Path p;
  enum pelem_type last_type = EHeader;
  
  for (p = path->next; p != path; last_type = p->ptype, p = p->next)
    if (p->ptype == EMove && last_type != EClose && last_type != EHeader)
      if (!ClosePath (p))
	return Error (PLimitCheck);
  if (last_type == EMove) {
    PathDelete (p);
    return TRUE;
  }
  if (last_type == EHeader || last_type == EClose)
    return TRUE;
  return ClosePath (p);
}

SetPath (p, v)
     Path *p, v;
{
  PathFree ((*p));
  *p = v;
}

Bound (left, right, top, bottom, p)
     float *left, *right, *top, *bottom;
     HardPoint p;
{
  if (p.hx < *left) 	*left 	= p.hx;
  if (p.hx > *right) 	*right 	= p.hx;
  if (p.hy < *bottom) 	*bottom = p.hy;
  if (p.hy > *top) 	*top 	= p.hy;
}

int PathBBox (left, right, top, bottom)
     float *left, *right, *top, *bottom;
{
  Path p;
  
  if (!gstate->cp_defined || EmptyPath (gstate->path))
    return Error (PNoCurrentPoint);
  
  *left = *right = gstate->cp.hx;
  *top = *bottom = gstate->cp.hy;
  
  for (p = gstate->path->next; p != gstate->path; p = p->next)
    switch (p->ptype) {
    case EMove:
    case ELine:
      Bound (left, right, top, bottom, p->pe.point);
      break;
    case EClose:
      break;
    case ECurve:
      Bound (left, right, top, bottom, p->pe.curve.x0);
      Bound (left, right, top, bottom, p->pe.curve.x1);
      Bound (left, right, top, bottom, p->pe.curve.x2); 
      break;
    case EHeader:
      Panic ("PathBBox, header found.");
      break;
    default:
      Panic ("PathBBox, unknown path element type.");
      break;
    }
  
  return TRUE;
}

UserBound (left, right, top, bottom, p)
     float *left, *right, *top, *bottom;
     UserPoint p;
{
  if (p.x < *left) 	*left 	= p.x;
  if (p.x > *right) 	*right 	= p.x;
  if (p.y < *bottom) 	*bottom = p.y;
  if (p.y > *top) 	*top 	= p.y;
}
