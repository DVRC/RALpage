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

static Object OpForAll;

static int PLength (), PArray (), PRBracket (), PAload (), PAstore ();

static int forAll (), ForAll (), PReadOnly (), PExecOnly (), PrCheck ();
static int PwCheck (), PutInterval (), GetInterval (), Put (), Get ();
static int Length (), Copy (), Eq ();

int ExecArray ();
Object Marker;

static int autobind = 0;

static int PSetAutoBind (), PGetAutoBind ();

InitArray ()
{
  TypeInstallOp (Mark, 	"eq", EqTrue, 2, 1, 0, 0, Mark, Mark);
  
  MakeObject (Marker, Mark);
  
  OpForAll = MakeOp ("(forallarray)", forAll, 0, 0, 3, 5);
  
  Install ("[",		Marker);
  Install ("null",	Nil);
  
  InstallOp ("array", 	PArray,		1, 1, 0, 0, Integer);
  InstallOp ("]", 	PRBracket,	0, 1, 0, 0);
  InstallOp ("aload", 	PAload,		1, 0, 0, 0, Array);
  InstallOp ("astore",	PAstore,	1, 1, 0, 0, Array);
  
  InstallOp ("setautobind",	PSetAutoBind,	1, 0, 0, 0, Bool);
  InstallOp ("getautobind",	PGetAutoBind,	0, 1, 0, 0);
  
  TypeInstallOp (Array, "exec",		ExecArray,	1, 1, 0, 2, Array);
  TypeInstallOp (Array, "eq", 		Eq,		2, 1, 0, 0, Array,
		 Array);
  TypeInstallOp (Array, "put", 		Put,		3, 0, 0, 0, Array,
		 Integer, Poly);
  TypeInstallOp (Array, "get", 		Get,		2, 1, 0, 0, Array,
		 Integer);
  TypeInstallOp (Array, "putinterval", 	PutInterval,	3, 0, 0, 0, Array,
		 Integer, Array);
  TypeInstallOp (Array, "getinterval", 	GetInterval,	3, 1, 0, 0, Array,
		 Integer, Integer);
  TypeInstallOp (Array, "length", 	PLength,	1, 1, 0, 0, Array);
  TypeInstallOp (Array, "copy", 	Copy,		2, 0, 0, 0, Array,
		 Array);
  TypeInstallOp (Array, "forall", 	ForAll,		2, 0, 0, 4, Array,
		 Array);
  TypeInstallOp (Array, "executeonly", 	PExecOnly,	1, 1, 0, 0, Array);
  TypeInstallOp (Array, "readonly", 	PReadOnly,	1, 1, 0, 0, Array);
  TypeInstallOp (Array, "rcheck", 	PrCheck,	1, 1, 0, 0, Array);
  TypeInstallOp (Array, "wcheck", 	PwCheck,	1, 1, 0, 0, Array);
}


Object MakeArray (p, length)
     Object *p;
     int length;
{
  Object res;
  
  MakeObject (res, Array);
  res.u.Array = p;
  res.Length = length;
  
  return res;
}


static Object Make (p, length)
     Object *p;
     int length;
{
  Object res;
  
  MakeObject (res, Array);
  res.u.Array = p;
  res.Length = length;
  
  return res;
}

#define Body(a) ((a).u.Array)

#ifndef Body
static Object *Body (item)
     Object item;
{
  return item.u.Array;
}
#endif

static Object ArrayStore (n)
     int n;
{
  Object *body = (Object *) Malloc ((unsigned) sizeof (Object) * n);
  int i, h = Height (OpStack);
  
  for (i = h - n; i < h; i++)
    body[i-h+n] = OpStack->stack_body[i];
  OpStack->stack_fill -= n;
  return Make (body, n);
}

/*VARARGS
Object ArrayFrom (length, args)
 int length; Object args;
 {
 	Object *argv = &args, *array = (Object *) Malloc ((unsigned) (sizeof (Object) * length));
 	int i;
 	
 	for (i = 0; i < length; i++)
 		array[i] = argv[i];
 	return Cvx (Make (array, length));
 }
*/

static int PLength (object)
     Object object;
{
  return Push (OpStack, MakeInteger (lengthArray(object)));
}

int ExecArray (item)
     Object item;
{
  int l = lengthArray (item);
  register Object *rp;
  Type t;

  if (l == 0) return TRUE;

  /*
   * BZS - try some open-coding here
   * TMD - try some real open-coding here...
   Push (ExecStack, SameFlags (item, Make (Body (item) + 1, l - 1)));
   */
  if(ExecStack->stack_fill != ExecStack->stack_size) {
    rp = &ExecStack->stack_body[ExecStack->stack_fill++];
    rp->type = Array;
    rp->u.Integer = 0;
    rp->u.Array = item.u.Array + 1;
    rp->Length = l - 1;
    rp->flags = item.flags;
  }

  rp = Body(item);
  t = TypeOf(*rp);
  if(t == Name || t == Operator) Push (ExecStack, *rp);
  else                           Push (OpStack, *rp);
		
  return TRUE;
}

static int Get (object, key)
     Object object, key;
{
  int index;
  
  if (TypeOf (key) == Integer)
    index = BodyInteger (key);
  else if (TypeOf (key) == Real)
    index = (int) BodyReal (key);
  else
    return Error (PTypeCheck);
  if (index >= 0 && index < lengthArray(object)) {
    Object t;
    
    t = Body (object) [index]; /* VAX Compiler Broken */
    return Push (OpStack, t);
  } else
    return Error (PRangeCheck);
}

static int GetInterval (object, begin, length)
     Object object, begin, length;
{
  int b = BodyInteger (begin), l = BodyInteger (length);
  
  if (l < 0 || b < 0 || b + l > lengthArray(object))
    return Error (PRangeCheck);
  return Push (OpStack, Make (Body (object) + b, l));
}

Object getIArray (object, begin, length)
     Object object;
     int begin, length;
{
  return Make (Body (object) + begin, length);
}

static int Put (object, key, value)
     Object object, key, value;
{
  int index = BodyInteger (key);
  
  if (index < 0 || index >= lengthArray(object))
    return Error (PRangeCheck);
  Body (object) [index] = value;
  return TRUE;
}
 
int putArray (object, index, value)
     Object object, value;
     int index;
{
  if (index < 0 || index >= lengthArray(object))
    return Error (PRangeCheck);
  Body (object) [index] = value;
  return TRUE;
}
 
static int PutInterval (object1, begin, object2)
     Object object1, begin, object2;
{
  int i, l1 = lengthArray(object1), l2 = lengthArray (object2),
  b = BodyInteger (begin);
  
  if (b < 0 || l2 + b > l1)
    return Error (PRangeCheck);
  for (i = b; i < b + l2; i++)
    Body (object1) [i] = Body (object2) [i - b];
  return TRUE;
}

static int Copy (object1, object2)
     Object object1, object2;
{
  if (lengthArray (object1) <= lengthArray (object2))
    return PutInterval (object2, MakeInteger (0), object1)
      && Push (OpStack, Make (Body (object2),
			      lengthArray (object1)));
  else
    return Error (PRangeCheck);
}

static Eq (a, b)
     Object a, b;
{
#ifdef MACINTOSH
  if (lengthArray (a) == lengthArray (b) &&
      (lengthArray (a) == 0 || Body (a) == Body (b)))
    return Push (OpStack, True);
  else
    return Push (OpStack, False);
#else
  return Push (OpStack,
	       MakeBool (lengthArray (a) == lengthArray (b) &&
			 (lengthArray (a) == 0 || Body (a) == Body (b))));
#endif
}

static int ForAll (array, proc)
     Object array, proc;
{
  return Push (ExecStack, array)
    && Push (ExecStack, MakeInteger (0))
      && Push (ExecStack, proc)
	&& Push (ExecStack, OpForAll);
}

static forAll ()
{
  Object array, index, proc;
  
  proc  = Pop (ExecStack);
  index = Pop (ExecStack);
  array = Pop (ExecStack);
  
  if (BodyInteger (index) >= lengthArray (array))
    return TRUE;
  return Push (ExecStack, array)
    && Push (ExecStack, MakeInteger (BodyInteger (index) + 1))
      && Push (ExecStack, proc)
 	&& Push (ExecStack, OpForAll)
	  && Push (ExecStack, proc)
	    && Push (OpStack, Body (array) [BodyInteger (index)]);
}

static int PSetAutoBind (bool)
     Object bool;
{
  autobind = BodyBool (bool);
  
  return TRUE;
}

static int PGetAutoBind ()
{
#ifdef MACINTOSH
  if (autobind)
    return Push (OpStack, True);
  else
    return Push (OpStack, False);
#else
  return Push (OpStack, MakeBool (autobind));
#endif
}

Object ParseArray (o)
     Object o;
{
  Object res, *array;
  int i, length = 0;
  struct acc
    {
      Object item;
      struct acc *prev;
    } *p = NULL;
  
  for (;;) {
    struct acc *item = (struct acc *) Malloc (sizeof (struct acc));
    res = Parse (o);
    
    if (TypeOf (res) == Condition)
      return Absent;
    if (TypeOf (res) == Bool)
      if (BodyBool (res))
	break;
      else
	return Absent;
    else if (TypeOf (res) == Null)
      return res;
    
    if (autobind && TypeOf (res) == Name && xCheck (res)) {
      Object binding;
      
      binding = Load (res);
      
      if (TypeOf (binding) == Operator)
	res = binding;
    }
    item->item = res;
    item->prev = p;
    p = item;
    ++length;
  }
  array = (Object *) Malloc ((unsigned) sizeof (Object) * length);
  
  for (i = length - 1; i >= 0; i--) {
    struct acc *prev = p->prev;
    
    array[i] = p->item;
    Free ((char *)p);
    p = prev;
  }
  PanicIf (p != NULL, "Something badly wrong in Array literal parser\n");
  return Make (array, length);
}

static int PArray (size)
     Object size;
{
  if (BodyInteger (size) < 0)
    return Error (PRangeCheck);
  else
    {
      int i, s = BodyInteger (size);
      Object *body = (Object *) Malloc ((unsigned) sizeof (Object) * s);
      
      for (i = 0; i < s; i++)
	body[i] = Nil;
      return Push (OpStack, Make (body, s));
    }
}

static int PRBracket ()	/* [ a1 ... an --- array */
{
  int n = CountTo (Mark, OpStack);
  
  if (n < 0)
    return Error (PUnMatched);
  else {
    Object res;
    
    res = ArrayStore (n);
    VOID Pop (OpStack);
    return Push (OpStack, res);
  }
}

static int PAload (array)
     Object array;
{
  Object *body = Body (array);
  int i, l = lengthArray (array);
  
  if (MaxStack (OpStack) - Height (OpStack) < l + 1)
    return Error (POpOverflow);
  else {
    for (i = 0; i < l; i++)
      if (!Push (OpStack, body[i]))
	break;
    return Push (OpStack, array);
  }
}

static int PAstore (array)
     Object array;
{
  if (lengthArray (array) > Height (OpStack))
    return Error (POpUnderflow);
  else {
    Object *body = Body (array);
    int i, l = lengthArray (array);
    
    for (i = l-1; i >= 0; i--)
      body[i] = Pop (OpStack);
    return Push (OpStack, array);
  }
}

static int PExecOnly (item)
     Object item;
{
  return Push (OpStack, ExecOnly (item));
}

static int PReadOnly (item)
     Object item;
{
  return Push (OpStack, ReadOnly (item));
}

static int PrCheck (v)
     Object v;
{
#ifdef MACINTOSH
  if (rCheck (v))
    return Push (OpStack, True);
  else
    return Push (OpStack, False);
#else
  return Push (OpStack, MakeBool (rCheck (v)));
#endif
}

static int PwCheck (v)
     Object v;
{
#ifdef MACINTOSH
  if (wCheck (v))
    return Push (OpStack, True);
  else
    return Push (OpStack, False);
#else
  return Push (OpStack, MakeBool (wCheck (v)));
#endif
}
