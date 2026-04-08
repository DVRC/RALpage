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
#include <signal.h>
#include "main.h"

#define __SEG__ SegJ

#ifndef PI
#define PI 3.14159265358979
#endif

static int PRand (), PSrand (), PRrand (), Idiv ();
static int ParseInteger ();

static catchmath ();
extern int errno;
jmp_buf penv;
extern void BZero ();

InitMath ()
{
  InstallOp ("abs",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("add",	PolyPair,	2, 1, 0, 0, Poly, Poly);
  InstallOp ("div",	PolyPair,	2, 1, 0, 0, Poly, Poly);
  InstallOp ("mod",	PolyPair,	2, 1, 0, 0, Poly, Poly);
  InstallOp ("mul",	PolyPair,	2, 1, 0, 0, Poly, Poly);
  InstallOp ("neg",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("sub",	PolyPair,	2, 1, 0, 0, Poly, Poly);
  InstallOp ("sqrt",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("exp",	PolyPair,	2, 1, 0, 0, Poly, Poly);
  InstallOp ("ceiling",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("floor",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("round",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("truncate",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("atan",	PolyPair,	2, 1, 0, 0, Poly, Poly);
  InstallOp ("cos",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("sin",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("ln",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("log",	PolyFirst,	1, 1, 0, 0, Poly);
  InstallOp ("rand",	PRand,		0, 1, 0, 0);
  InstallOp ("srand",	PSrand,		1, 0, 0, 0, Integer);
  InstallOp ("rrand",	PRrand,		0, 1, 0, 0);
  InstallOp ("idiv", 	Idiv,		2, 1, 0, 0, Float, Float);
	
  /*	PanicIf (setjmp (penv), "Unexpected floating point error");
	VOID signal (SIGFPE, catchmath);
*/
}

static catchmath ()
{
#ifndef macintosh
  VOID signal (SIGFPE, catchmath);
  longjmp (penv, TRUE);
#endif
}

float Deg (r)
     float r;
{
  float res = 360 * r / (2 * PI);
  return res < 0 ? res + 360 : res;
}

float Rad (d)
     float d;
{
  return 2 * PI * d / 360;
}

static float pow10[]={1,1e1,1e2,1e3,1e4,1e5,1e6,1e7,1e8,1e9,1e10,1e11};

#define RetInt(V) {res.type=Integer; res.u.Integer=sign?-(V):(V); return res;}
#define RetReal(V) {res.type=Real; res.u.Real = sign?-(V):(V); return res;}

Object ParseNumber (s, length) unsigned char *s; int length;
{
  Object res;
  int c, sign, ival, dval, olength;
  float fval;
 	
  if (length == 0) return Nil;

#ifdef MACINTOSH
#ifdef macintosh
  BZero(&res, sizeof(Object));
#else
  setmem(&res, sizeof(Object), '\0');
#endif /* macintosh */
#else
  bzero(&res, sizeof(Object));
#endif
  
  res.flags =  READABLE|WRITEABLE;
  if ((c = *s) == '+') { sign = 0;  c = *++s; --length; }
  else if (c == '-') { sign = 1; c = *++s; --length; }
  else sign = 0;

  if (c == '.') {
    if ((++s, --length) == 0 || (c = *s) < '0' || c > '9') return Nil;
    olength = length;  dval = c - '0';
    while ((++s, --length) > 0 && (c = *s) >= '0' && c <= '9')
      dval = dval*10 + c-'0';
    fval = (float) dval / pow10[olength-length];
    if (length == 0) RetReal(fval);
  }
  else {
    if (length == 0 || c < '0' || c > '9') return Nil;
    ival = c - '0';
    while ((++s, --length) > 0 && (c = *s) >= '0' && c <= '9')
      ival = ival*10 + c-'0';
    if (length == 0) RetInt(ival);

    if ((c = *s) == '.') {
      if ((s++, --length) == 0 || (c = *s) < '0' || c > '9') return Nil;
      olength = length;  dval = c - '0';
      while ((++s, --length) > 0 && (c = *s) >= '0' && c <= '9')
	dval = dval*10 + c-'0';
      fval = ival + (float) dval / pow10[olength-length];
      if (length == 0) RetReal(fval);
    }
    else if (c == '#') {
      if (ival < 2 || ival > 36) return Nil;
      s++; --length;
      ival = ParseInteger (&s, &length, ival);
      if (length >= 0) RetInt(ival);
      return Nil;
    }
    else fval = (float) ival;
  }

  if ((c = *s) == 'e' || c == 'E') {
    if ((s++, --length) == 0) return Nil;

    if (sign) fval = -fval;
    if ((c = *s) == '+') { sign = 0;  c = *++s; --length; }
    else if(c == '-') { sign = 1; c = *++s; --length; }
    else sign = 0;

    if (length == 0 || c < '0' || c > '9') return Nil;
    ival = c - '0';
    while ((++s, --length) > 0 && (c = *s) >= '0' && c <= '9')
      ival = ival*10 + c-'0';
    if (sign) ival = -ival;
    res.type = Real; res.u.Real = fval * pow(10.0,(double) ival);
    return res;
  }
  return Nil;
}

static int ParseInteger (p, length, base)
     unsigned char **p;
     int *length, base;
{
  int present = FALSE, ival = 0, digit;
  if (*length == 0) {
    *length = -1;
    return 0;
  }
  for (;;) {
    int c = *(*p)++;
    
    if (c >= '0' && c <= '9')      digit = c - '0';
    else if (c >= 'a' && c <= 'z') digit = c - 'a' + 10;
    else if (c >= 'A' && c <= 'Z') digit = c - 'A' + 10;
    else break;
    
    if (digit < base) ival = ival * base + digit;
    else break;
    ++present;
    if (--*length == 0) return ival;
  }
  --*p;
  if (!present) *length = -1;
  return ival;
}

static int seed = 1;

static int random ()
{
  return seed = ((seed * 1103515245 + 12345) & 0x7fffffff);
}

static int PRand ()		/* --- integer */
{
  VOID random ();
  return Push (OpStack, MakeInteger (seed));
}

static int PSrand (see)
     Object see;
{
  srand ((unsigned) (seed = BodyInteger (see)));
  return TRUE;
}

static int PRrand ()
{
  return Push (OpStack, MakeInteger (seed));
}

static int Idiv (a, b)
     Object a, b;
{
  if (BodyReal (b) == 0)
    return Error (PUnResult);
  return Push (OpStack, MakeInteger ((int) BodyReal (a) / (int) BodyReal (b)));
}
