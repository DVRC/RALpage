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

#define __SEG__ SegG

int PStrokePath ();

int in_stroke = FALSE;

int Stroke ();

InitStroke ()
{
  InstallOp ("strokepath",	PStrokePath,	0, 0, 0, 0);
  InstallOp ("stroke",		Stroke,		0, 0, 0, 0);
}

int Stroke ()
{
  int res;
  
  if (!PFlattenPath ())            return FALSE;
  if (EmptyPath (gstate->path))    return TRUE;
  if (gstate->device->dev == NULL) return PNewPath ();
  if (ThinStroke ())               return TRUE;
  in_stroke = TRUE;
  res = PStrokePath () && Fill ();
  VOID PNewPath ();
  in_stroke = FALSE;
  
  return res;
}

float Magnitude (v)
     Vector v;
{
  return sqrt (v.vx * v.vx + v.vy * v.vy);
}

#define Magnitude(v) sqrt (v.vx * v.vx + v.vy * v.vy)

void Dash ()
{
  Matrix m;
  Path p, new = NewPath (), last_move, last_dash;
  HardPoint move, here, next;
  Vector v, unit;
  int marking, start_marking = TRUE, index, start_index = 0;
  float dash_left, start_dash_left = gstate->dash_offset;
  float umag, dmag;
  
  m = gstate->CTM;
  m.tx = 0.0;  m.ty = 0.0;
  
  while (start_dash_left > gstate->dash_array [start_index]) {
    start_dash_left -= gstate->dash_array [start_index];
    if (++start_index == gstate->dash_length)
      start_index = 0;
    start_marking = !start_marking;
  }
  start_dash_left = gstate->dash_array [start_index] - start_dash_left;
  
  for (p = gstate->path->next; p != gstate->path; p = p->next)
    switch (p->ptype) {
    case EMove:
      index = start_index;
      marking = start_marking;
      dash_left = start_dash_left;
      here = move = p->pe.point;
      VOID GMoveTo (new, here);
      last_move = last_dash = new->last;
      break;
      
    case ELine:
    case EClose:
      if (p->ptype == ELine)
	next = p->pe.point;
      else
	next = move;
      v = NewVector (next.hx - here.hx, next.hy - here.hy, 1.0);
      dmag = Magnitude (v);
      if (dmag == 0) {
	here = next;
	break;
      }
      unit = ITransform (v, m);
      umag = Magnitude (unit);
      unit = NewVector (v.vx / umag, v.vy / umag, 1.0);
      while (umag > dash_left) {
	here.hx += unit.vx * dash_left;
	here.hy += unit.vy * dash_left;
	(*(marking ? GLineTo : GMoveTo)) (new, here);
	if (!marking)
	  last_dash = new->last;
	marking = !marking;
	umag -= dash_left;
	if (++index == gstate->dash_length)
	  index = 0;
	dash_left = gstate->dash_array [index];
      }
      if (p->ptype == ELine) {
	dash_left -= umag;
	if (marking)
	  VOID GLineTo (new, next);
      } else {
	if (marking) {
	  if (start_marking)
	    if (last_dash == last_move)
	      ClosePath (new);
	    else {
	      /*	GLineTo (new, move);	*/
	      MoveChunk (last_move,last_dash,new->last);
	    } else
	      VOID GLineTo (new, move);
	}
      }
      here = next;
      break;
      
    default:
      Panic ("Dash: unknown Path Element type");
    }
  PathFree (gstate->path);
  gstate->path = new;
  gstate->cp = new->next->pe.point;
}

MoveChunk (dest, begin, end)
     Path dest, begin, end;
{
  dest->ptype = ELine;
  end->next->last = begin->last;
  begin->last->next = end->next;
  dest->last->next = begin;
  
  end->next = dest;
  begin->last = dest->last;
  dest->last = end;
}

/*
 * is 'b' to the left of 'a' ? if 'a' carries into 'b' ?
 *
 *			   /
 *			  /
 *			 b
 *			/
 *		       /
 * --------- a -------o
 * 
 */

leftof (a, b)
     float a, b;
{
  return Normalise (b - a) > 0;
}

#define leftof(a, b) Normalise (b - a) > 0

Miter (new, last_angle, angle, width)
     Path new;
     float last_angle, angle, width;
{
  Matrix old;
  float diff;
  
  old = gstate->CTM;
  if (leftof (last_angle, angle)) {
    diff = angle - last_angle;
    VOID GMoveTo (new, ExtToInt (NewUserPoint (0.0, 0.0)));
    gstate->CTM = Rotate (gstate->CTM, -diff);
    VOID GLineTo (new, ExtToInt (NewUserPoint (0.0, -width / 2)));
    gstate->CTM = Rotate (gstate->CTM, diff / 2);
    VOID GLineTo (new, ExtToInt (NewUserPoint (0.0, -(width / 2) / cos (diff / 2))));
    gstate->CTM = old;
    VOID GLineTo (new, ExtToInt (NewUserPoint (0.0, -width / 2)));
  } else {
    diff = last_angle - angle;
    VOID GMoveTo (new, ExtToInt (NewUserPoint (0.0, 0.0)));
    VOID GLineTo (new, ExtToInt (NewUserPoint (0.0, width / 2)));
    gstate->CTM = Rotate (gstate->CTM, diff / 2);
    VOID GLineTo (new, ExtToInt (NewUserPoint (0.0, (width / 2) / cos (diff / 2))));
    gstate->CTM = Rotate (gstate->CTM, diff / 2);
    VOID GLineTo (new, ExtToInt (NewUserPoint (0.0, width / 2)));
    gstate->CTM = old;
  }
  ClosePath (new);
}

Bevel (new, last_angle, angle, width)
     Path new;
     float last_angle, angle, width;
{
  HardPoint P1;
  Matrix *mp;
  float diff,ca,sa,x,y;
  
  mp = &gstate->CTM;
  width /= 2;
  diff = last_angle - angle;
  x = mp->tx;   y = mp->ty;
  P1.hx = x;    P1.hy = y;
  VOID GMoveTo (new, P1);

  ca = cos(diff);   sa = sin(diff);

  if (leftof (last_angle, angle)) {
    P1.hx = -width * (mp->C*ca - mp->A*sa) + x;
    P1.hy = -width * (mp->D*ca - mp->B*sa) + y;
    VOID GLineTo (new, P1);

    P1.hx = -width*mp->C + x;
    P1.hy = -width*mp->D + y;
    VOID GLineTo (new, P1);
  } else {
    P1.hx = width*mp->C + x;
    P1.hy = width*mp->D + y;
    VOID GLineTo (new, P1);

    P1.hx = width * (mp->C*ca - mp->A*sa) + x;
    P1.hy = width * (mp->D*ca - mp->B*sa) + y;
    VOID GLineTo (new, P1);
  }
  ClosePath (new);
}

BeginCap (new, width)
     Path new;
     float width;
{
  Path add;
  HardPoint P1,P2,P3;
  Matrix *mp;
  float x,y,t2,w2,wa,wb,wc,wd;

  switch (gstate->line_cap) {
  case CBUTT:
    break;
    
  case CSQUARE:
    mp = &gstate->CTM;  w2 = width/2; wc = w2*mp->C; wd = w2*mp->D;
    P1.hx = -wc + mp->tx;  P1.hy = -wd + mp->ty;  VOID GMoveTo (new, P1);
    wc *= 2;  wd *= 2;
    P1.hx += wc;           P1.hy += wd;           VOID GLineTo (new, P1);
    P1.hx -= w2*mp->A;     P1.hy -= w2*mp->B;     VOID GLineTo (new, P1);
    P1.hx += wc;           P1.hy += wd;           VOID GLineTo (new, P1);
    ClosePath (new);
    break;
    
  case CROUND:
    mp = &gstate->CTM;  w2 = width/2;
    P1.hx = w2*mp->C + mp->tx;   P1.hy = w2*mp->D + mp->ty;
    VOID GMoveTo (new, P1);

    /* Arc (new, 1, P1, w2, PI / 2, 3 * PI / 2); */
    wa = w2*gstate->CTM.C;  wb = w2*gstate->CTM.D;
    wc = -w2*gstate->CTM.A; wd = -w2*gstate->CTM.B;
    x = gstate->CTM.tx;     y = gstate->CTM.ty;
    t2 = ((float) .5522847);  /* (4/3)*(sqrt(2) - 1) */

    P1.hx = x + wa + t2*wc; P1.hy = y + wb + t2*wd;
    P3.hx = x + wc;         P3.hy = y + wd;
    P2.hx = P3.hx + t2*wa;  P2.hy = P3.hy + t2*wb;
    NewElem (ECurve, add);
    add->pe.curve.x0 = P1;	add->pe.curve.x1 = P2;	add->pe.curve.x2 = P3;
    gstate->cp = P3; PathInsert (new, add);

    P1.hx = x + wc - t2*wa; P1.hy = y + wd - t2*wb;
    P3.hx = x - wa;         P3.hy = y - wb;
    P2.hx = P3.hx + t2*wc;  P2.hy = P3.hy + t2*wd;
    NewElem (ECurve, add);
    add->pe.curve.x0 = P1;	add->pe.curve.x1 = P2;	add->pe.curve.x2 = P3;
    gstate->cp = P3; PathInsert (new, add);

    ClosePath (new);
    break;
    
  default:
    Panic ("BeginCap  - unknown line cap encountered");
  }
}

LineJoin (new, width, last_angle, angle)
     Path new;
     float width, last_angle, angle;
{
  float ang, sa;
  
  switch (gstate->line_join) {
  case JMITRE:
    ang = Normalise (leftof (last_angle, angle) ? angle - last_angle : last_angle - angle);
    ang = ang < 0 ? -ang : ang;
    ang = ang > PI / 2 ? PI - ang : ang;
    sa = sin (ang / 2);
    if (sa != 0 && 1 / sa <= gstate->miter_limit &&
	           1 / sa >= -gstate->miter_limit)
      Miter (new, last_angle, angle, width);
    else
      Bevel (new, last_angle, angle, width);
    
    break;
    
  case JROUND:
    VOID GMoveTo (new, ExtToInt (NewUserPoint (width / 2, 0.0)));
    Arc (new, 1, NewUserPoint (0.0, 0.0), width / 2, 0.0, 2 * PI);
    ClosePath (new);
    break;
    
  case JBEVEL:
    Bevel (new, last_angle, angle, width);
    break;
    
  default:
    Panic ("LineJoin  - unknown line join encountered");
  }
}

EndCap (new, width, length)
     Path new;
     float width, length;
{
  Path add;
  HardPoint P1,P2,P3;
  Matrix *mp;
  float x,y,t2,w2,wa,wb,wc,wd;

  switch (gstate->line_cap) {
    case CBUTT:
      break;
    
    case CSQUARE:
      mp = &gstate->CTM;  w2 = width/2;  wa = w2*mp->A;  wb = w2*mp->B;
      P1.hx = length*mp->A + w2*mp->C + mp->tx;
      P1.hy = length*mp->B + w2*mp->D + mp->ty;    VOID GMoveTo (new, P1);
      P1.hx += wa;          P1.hy += wb;           VOID GLineTo (new, P1);
      P1.hx += width*mp->C; P1.hy += width*mp->D;  VOID GLineTo (new, P1);
      P1.hx -= wa;          P1.hy -= wb;           VOID GLineTo (new, P1);
      ClosePath (new);
      break;
    
    case CROUND:
      mp = &gstate->CTM;  w2 = width/2;
      P1.hx = length*mp->A - w2*mp->C + mp->tx;
      P1.hy = length*mp->B - w2*mp->D + mp->ty;
      
      VOID GMoveTo (new, P1);

      /* Arc (new, 1, P1, w2, -PI / 2, PI / 2); */
      wa = -w2*gstate->CTM.C; wb = -w2*gstate->CTM.D;
      wc = w2*gstate->CTM.A;  wd = w2*gstate->CTM.B;
      x = gstate->CTM.tx;     y = gstate->CTM.ty;
      t2 = ((float) .5522847);  /* (4/3)*(sqrt(2) - 1) */

      P1.hx = x + wa + t2*wc; P1.hy = y + wb + t2*wd;
      P3.hx = x + wc;         P3.hy = y + wd;
      P2.hx = P3.hx + t2*wa;  P2.hy = P3.hy + t2*wb;
      NewElem (ECurve, add);
      add->pe.curve.x0 = P1;	add->pe.curve.x1 = P2;	add->pe.curve.x2 = P3;
      gstate->cp = P3; PathInsert (new, add);

      P1.hx = x + wc - t2*wa; P1.hy = y + wd - t2*wb;
      P3.hx = x - wa;         P3.hy = y - wb;
      P2.hx = P3.hx + t2*wc;  P2.hy = P3.hy + t2*wd;
      NewElem (ECurve, add);
      add->pe.curve.x0 = P1;	add->pe.curve.x1 = P2;	add->pe.curve.x2 = P3;
      gstate->cp = P3; PathInsert (new, add);

      ClosePath (new);
      break;
    
    default:
      Panic ("StrokeLineEnd  - unknown line cap encountered");
  }
}

static float move_angle;
static Matrix move_matrix;

float LineSegment (p, new, ehere, enow, width, last_angle, last_type)
     Path p, new;
     UserPoint ehere, enow;
     float width, last_angle;
     enum pelem_type last_type;
{
  HardPoint P1;
  float angle,length,ca,sa;
  Matrix old,m;
	
  /* Compute angle, length, sin(angle) and cos(angle) of line segment */
  sa = enow.y - ehere.y;   ca = enow.x - ehere.x;

  angle = atan2 (sa,ca);   length = sqrt (sa*sa + ca*ca);
  if(length > (float) 0.0) {  sa /= length;   ca /= length; }
  else {                      sa = 0;         ca = 1; }

  /* Translate CTM by ehere and rotate by angle */
  old = gstate->CTM;
  m.A  = ca * old.A + sa * old.C;  m.B  = ca * old.B + sa * old.D;
  m.C  = ca * old.C - sa * old.A;  m.D  = ca * old.D - sa * old.B;
  m.tx = ehere.x * old.A + ehere.y * old.C + old.tx;
  m.ty = ehere.x * old.B + ehere.y * old.D + old.ty;
  gstate->CTM = m;

  /* Create a path for the outline of the line segment */
  sa = -width/2;
  P1.hx = sa*m.C + m.tx; P1.hy = sa*m.D + m.ty; VOID GMoveTo (new, P1);
  P1.hx += length*m.A;   P1.hy += length*m.B;   VOID GLineTo (new, P1);
  P1.hx += width*m.C;    P1.hy += width*m.D;    VOID GLineTo (new, P1);
  P1.hx -= length*m.A;   P1.hy -= length*m.B;   VOID GLineTo (new, P1);
  ClosePath (new);

  if (last_type == EMove) {
    move_angle = angle;
    move_matrix = gstate->CTM;
  }
  else if (last_type == ELine)
    LineJoin (new, width, last_angle, angle);
	
  if (p->ptype == EClose) {
    if (last_type == ELine) {
      gstate->CTM = move_matrix;
      LineJoin (new, width, angle, move_angle);
    }
  }
  else if (p->next->ptype == EMove || p->next->ptype == EHeader) {
    EndCap (new, width, length);
    gstate->CTM = move_matrix;
    BeginCap (new, width);
  }
	
  gstate->CTM = old;
  return angle;
}

int PStrokePath ()
{
  Matrix *mp;
  Path p, new;
  HardPoint here, move;
  UserPoint f,t;
  enum pelem_type last_type = EHeader;
  float x, y, det, angle, last_angle, width = gstate->line_width;
  
  NewElem(EHeader,new);
  new->next = new->last = new;

  PFlattenPath ();
  if (gstate->dash_length != 0) Dash ();

  /* Precompute determinant of CTM for later use */
  mp = &gstate->CTM;
  det = mp->A * mp->D - mp->B * mp->C;

  for (p = gstate->path->next; p != gstate->path;
       last_type = p->ptype, last_angle = angle, p = p->next) {
    switch (p->ptype) {
    case EMove:
      move = here = p->pe.point;
      break;
      
    case EClose:
      if (last_type == EMove) break;
      
      x = here.hx - mp->tx;  y = here.hy - mp->ty;
      f.x = (x * mp->D - y * mp->C) / det; f.y = (y * mp->A - x * mp->B) / det;
      x = move.hx - mp->tx;  y = move.hy - mp->ty;
      t.x = (x * mp->D - y * mp->C) / det; t.y = (y * mp->A - x * mp->B) / det;
      angle = LineSegment (p, new, f, t, width, last_angle, last_type);
      here = move;
      last_type = EHeader;
      break;
      
    case ELine:
      x = here.hx - mp->tx;  y = here.hy - mp->ty;
      f.x = (x * mp->D - y * mp->C) / det; f.y = (y * mp->A - x * mp->B) / det;
      x = p->pe.point.hx - mp->tx;  y = p->pe.point.hy - mp->ty;
      t.x = (x * mp->D - y * mp->C) / det; t.y = (y * mp->A - x * mp->B) / det;
      angle = LineSegment (p, new, f, t, width, last_angle, last_type);
      here = p->pe.point;
      break;
      
    default:
      Panic ("unknown path element type in StrokePath");
    }
  }
  PathFree (gstate->path);
  gstate->path = new;
  return TRUE;
}

#ifndef X11
int ThinStroke ()
{
  Path p;
  DevicePoint fp,tp;
  HardPoint here, prev, cur;
  float x,y,w;
  
  if (stroke_method != STROKE_THIN) return FALSE;
  if((w = gstate->line_width) != 0.0) {
    w = 1.1/w;
    x = (gstate->CTM.A + gstate->CTM.C);
    y = (gstate->CTM.B + gstate->CTM.D);
    if(x < -w || x > w || y < -w || y > w) return FALSE;
  }
  if (gstate->dash_length != 0) Dash ();

  for (p = gstate->path->next; p != gstate->path; p = p->next) {
    if(p->ptype == EMove) here = prev = p->pe.point;
    else {
#ifdef MACINTOSH
      if (p->ptype == ELine)
	cur = p->pe.point;
      else
	cur = here;
#else
      cur = (p->ptype == ELine) ? p->pe.point : here;
#endif
      
/*    DevicePaintLine (gstate->device, prev, cur, gstate->colour); */

      fp.dx = (int) prev.hx;  fp.dy = (int) prev.hy;
      tp.dx = (int) cur.hx;   tp.dy = (int) cur.hy;
      PaintLine (gstate->device->dev, fp, tp, gstate->colour);

      prev = cur;
    }
  }

  VOID PNewPath ();
  return TRUE;
}
#endif
