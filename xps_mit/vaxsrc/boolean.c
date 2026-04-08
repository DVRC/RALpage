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

#define __SEG__ SegD

Object True, False;
static Object OpNot;
static int Cvs (), And (), Or (), Not (), Xor ();
static int PEq (), PNeq (), EqBool ();

InitBool ()
{
  MakeObject (True, Bool);
  MakeObject (False, Bool);
  
  True.u.Bool = TRUE;
  False.u.Bool = FALSE;
  
  Install ("true",	True);
  Install ("false",	False);
  
  TypeInstallOp (Bool, "cvs", 	Cvs,  		2, 1, 0, 0, Bool, String);
  TypeInstallOp (Bool, "and", 	And,  		2, 1, 0, 0, Bool, Bool);
  TypeInstallOp (Bool, "or", 	Or,  		2, 1, 0, 0, Bool, Bool);
  TypeInstallOp (Bool, "xor", 	Xor,  		2, 1, 0, 0, Bool, Bool);
  TypeInstallOp (Bool, "not", 	Not,  		1, 1, 0, 0, Bool);
  TypeInstallOp (Bool, "eq", 	EqBool,  	2, 1, 0, 0, Bool, Bool);
  
  OpNot = Lookup (Bool, NameFrom ("not"));
  
  InstallOp ("eq",	PEq,  		2, 1, 0, 0, Poly, Poly);
  InstallOp ("ne",	PNeq, 		2, 1, 0, 0, Poly, Poly);
  InstallOp ("ge", 	PolyPair, 	2, 2, 0, 0, Poly, Poly);
  InstallOp ("gt", 	PolyPair, 	2, 2, 0, 0, Poly, Poly);
  InstallOp ("le", 	PolyPair, 	2, 2, 0, 0, Poly, Poly);
  InstallOp ("lt", 	PolyPair, 	2, 2, 0, 0, Poly, Poly);
  InstallOp ("not", 	PolyFirst, 	1, 1, 0, 0, Poly);
  InstallOp ("and", 	PolyPair, 	2, 2, 0, 0, Poly, Poly);
  InstallOp ("or", 	PolyPair, 	2, 2, 0, 0, Poly, Poly);
  InstallOp ("xor", 	PolyPair, 	2, 2, 0, 0, Poly, Poly);
  InstallOp ("bitshift", 	PolyPair, 	2, 2, 0, 0, Poly, Poly);
}

static int Cvs (v, string)
     Object v, string;
{
  char *choice = BodyBool (v) ? "true" : "false";
  int length = strlen (choice);
  
  if (lengthString (string) < length)
    return Error (PRangeCheck);
  VOID Bcopy (BodyString (string), choice, length);
  return Push (OpStack, getIString (string, 0, length));
}

static int PEq (a, b)
     Object a, b;	/* any any --- Bool */
{
  Object fn;

  if (!rCheck (a) || !rCheck (b)) return Error (PInvAccess);
  if (TypeOf (a) == Name && TypeOf (b) == String)       a = StringName (a);
  else if (TypeOf (b) == Name && TypeOf (a) == String)  b = StringName (b);
  else if (TypeOf (a) == Real && TypeOf (b) == Integer) RealInteger(b);
  else if (TypeOf (a) == Integer && TypeOf (b) == Real) RealInteger(a);
  if (TypeOf (a) == TypeOf (b)) {
    VOID Push2(OpStack, a, b);
    fn = Lookup (TypeOf(a), Self);
    if (TypeOf (fn) == Condition) return Error (PTypeCheck);
    else        return Push (ExecStack, fn);
  }
  else
    return Push (OpStack, False);
}

static int PNeq (a, b)
     Object a, b;		/* any any --- Bool */
{
  if (!rCheck (a) || !rCheck (b))
    return Error (PInvAccess);
  if (TypeOf (a) == Name && TypeOf (b) == String)       a = StringName (a);
  else if (TypeOf (b) == Name && TypeOf (a) == String)  b = StringName (b);
  else if (TypeOf (a) == Real && TypeOf (b) == Integer) RealInteger(b);
  else if (TypeOf (a) == Integer && TypeOf (b) == Real) RealInteger(a);
  if (TypeOf (a) == TypeOf (b)) {
    VOID Push (OpStack, a);
    VOID Push (OpStack, b);
    VOID Push (ExecStack, OpNot);
    Self = NameFrom ("eq");
    return Apply (TypeOf (a));
  } else
    return Push (OpStack, True);
}

static int EqBool (a, b)
     Object a, b;
{
#ifdef MACINTOSH
  if (BodyBool (a) == BodyBool (b))
    return Push (OpStack, True);
  else
    return Push (OpStack, False);
#else
  return Push (OpStack, MakeBool (BodyBool (a) == BodyBool (b)));
#endif
}

static int Not (bool)
     Object bool;
{
#ifdef MACINTOSH
  if (!BodyBool (bool))
    return Push (OpStack, True);
  else
    return Push (OpStack, False);
#else
  return Push (OpStack, MakeBool (!BodyBool (bool)));
#endif
}

static int And (a, b)
     Object a, b;
{
#ifdef MACINTOSH
  if (BodyBool (a) && BodyBool (b))
    return Push (OpStack, True);
  else
    return Push (OpStack, False);
#else
  return Push (OpStack, MakeBool (BodyBool (a) && BodyBool (b)));
#endif
}

static int Or (a, b)
     Object a, b;
{
#ifdef MACINTOSH
  if (BodyBool (a) || BodyBool (b))
    return Push (OpStack, True);
  else
    return Push (OpStack, False);
#else
  return Push (OpStack, MakeBool (BodyBool (a) || BodyBool (b)));
#endif
}

static int Xor (a, b)
     Object a, b;
{
#ifdef MACINTOSH
  if (BodyBool (a) != BodyBool (b))
    return Push (OpStack, True);
  else
    return Push (OpStack, False);
#else
  return Push (OpStack, MakeBool (BodyBool (a) != BodyBool (b)));
#endif
}
