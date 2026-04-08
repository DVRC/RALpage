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

#define __SEG__ SegI

static int Cvs (), EqEq (), Eq (), ExecName ();
static Object OpExecName;

Object Self;

InitName ()
{
  OpExecName = MakeOp ("exec", ExecName,	1, 1, 0, 4, Name);
  
  TypeInstall (Name, "exec", 	OpExecName);
  
  TypeInstallOp (Name, "==", 	EqEq,	1, 0, 0, 0, Name);
  TypeInstallOp (Name, "cvs", 	Cvs,	2, 1, 0, 0, Name, String);
  TypeInstallOp (Name, "eq", 	Eq, 	2, 1, 0, 0, Name, Name);
}

static int Cvs (v, string)
     Object v, string;
{
  int length;
  
  if (lengthString (string) < (length = lengthName (v)))
    return Error (PRangeCheck);
  VOID Bcopy (BodyString (string), BodyName (v), length);
  return Push (OpStack, getIString (string, 0, length));
}

static int EqEq (v)
     Object v;
{
  if (!xCheck (v))
    putchar ('/');
  PrintName (v);
  return TRUE;
}

static int Eq (a, b)
     Object a, b;
{
#ifdef MACINTOSH
  if (BodyName (a) == BodyName (b) && lengthName (a) == lengthName (b))
    return Push (OpStack, True);
  else
    return Push (OpStack, False);
#else
  return Push (OpStack, MakeBool (BodyName (a) == BodyName (b) && lengthName (a) == lengthName (b)));
#endif
}

static int ExecName (item)
     Object item;
{
  Object v;
  
  v = DictLookup (item);
  if (TypeOf (v) == Condition)
    return Error (PUndefined);
  else
    return Push (ExecStack, v);
}

#if 0
static struct name_struct *FindTreeName (s, length, root)
     unsigned char *s;
     int length;
     struct name_struct **root;
{
  struct name_struct *r;
  int rootlen,cmp;
  
  while (*root) {
    rootlen = (*root)->string_length;
    if(rootlen == length) {
      cmp = strncmp(s, (*root)->string_body, length);
      if (cmp == 0) return *root;
    } else
      cmp = length - rootlen;
    
    root = (cmp < 0) ? &(*root)->prev_name : &(*root)->next_name;
  }

  r = *root = (struct name_struct *) Malloc (sizeof (struct name_struct));
  
  r->next_name = r->prev_name = NULL;
  Bcopy(r->string_body = (unsigned char *)Malloc((unsigned)length),s,length);
  r->string_length = length;

  return r;
}

#define HASH_NAME_SIZE	1024
int name_tries = 0, name_hits = 0;
static struct name_struct *hash_name [HASH_NAME_SIZE];
static struct name_struct *name_tree = NULL;

static struct name_struct *FindName (s, length)
     unsigned char *s;
     int length;
{
  struct name_struct *p;
  int hash = 0,tmplen = length;

  while(tmplen--) hash += *s++;
  hash &= (HASH_NAME_SIZE - 1);
  
  p = hash_name [hash];
  ++name_tries;
  ++name_hits;
  
  if (p == NULL || p->string_length != length ||
      strncmp (s, p->string_body, length)) {
    p = FindTreeName (s, length, &name_tree);

    if (p != NULL) {
      hash_name [hash] = p;
      --name_hits;
    }
  }
  return p;
}
#else

#define HASH_NAME_SIZE	1009
int name_tries = 0, name_hits = 0;
static struct name_struct *hash_name [HASH_NAME_SIZE];

static struct name_struct *FindName (s, length)
     unsigned char *s;
     int length;
{
  struct name_struct *p,**slot;
  unsigned char *t1,*t2;
  unsigned int hash,shift,tmplen;

  /* Convert each character to a 5 bit number between 3 and 28.
   * Xor these into a 32 bit integer in the following pattern:
   * --------------------------------
   *        4444433333222221111100000   <-- first 5 bits go in 00000
   *      9999988888777776666655555
   *    4444433333222221111100000
   *       9999988888777776666655555
   *     4444433333222221111100000
   */
  t1 = s; hash = 0; shift = 0; tmplen = length;
  while (tmplen--) {
    hash ^= *t1++ << shift;
    if((shift += 5) >= 22) shift -= 22;
  }
  hash %= HASH_NAME_SIZE;
  slot = &hash_name[hash];
  p = *slot;
  ++name_tries;
  ++name_hits;

  while(p != NULL) {
    if(p->string_length == length) {
      tmplen = length;
      t1 = p->string_body; t2 = s;
      while(*t1++ == *t2++ && --tmplen > 0);
      if(tmplen == 0) return p;
    }
    slot = &p->next;
    p = *slot;
    name_tries++;
  }

  --name_hits;
  *slot = p = (struct name_struct *) Malloc (sizeof (struct name_struct));
  p->next = NULL;
  Bcopy(p->string_body = (unsigned char *)Malloc((unsigned)length),s,length);
  p->string_length = length;
  return p;
}
#endif

Object NameFrom (s)
     unsigned char *s;
{
  Object res;
  
  MakeObject (res, Name);
  res.u.Name = FindName (s, strlen (s));
  return res;
}

Object Cvn (o)
     Object o;
{
  Object res;
  
  MakeObject (res, Name);
  res.u.Name = FindName (BodyString (o), lengthString (o));
  
  return res;
}

Object StringName (o)
     Object o;
{
  return MakeString (BodyName (o), lengthName (o));
}

PrintName (n)
     Object n;
{
  printf ("%.*s", lengthName (n), BodyName (n));
}

Object ParseId (o)
     Object o;
{
  unsigned char buf[BUFSIZE], *p = buf;
  int c, length;
  Object res;
	
  if(!StatusFile(o)) return Absent;
  if(BodyFile(o)->file_type == StreamFile) {
    FILE *fp = BodyFile(o)->f.f_ptr;
    for (;;) {
      switch (c = getc(fp)) {
	case EOF: case ' ': case '\t': case '\n': break;
	
	case '/': case '<': case '>': case '(': case ')':
	case '%': case '{': case '}': case '[': case ']':
/*	  BodyFile(o)->available++; */
	  ungetc(c,fp);
	  break;
        default: *p++ = c; continue;
      }
      break;
    }
  }
  else {
    for (;;) {
      switch (c = GeneralGetch(o)) {
	case EOF: case ' ': case '\t': case '\n': break;
	
	case '/': case '<': case '>': case '(': case ')':
	case '%': case '{': case '}': case '[': case ']':
	  Ungetch (o, c);
	  break;
        default: *p++ = c; continue;
      }
      break;
    }
  }

  length = (int) (p - buf);
  if (length == 0) return Absent;
	
  /* Only attempt to parse as a number if it could possibly be one */
  c = buf[0];
  if((c >= '0' && c <= '9') || c == '.' || c == '+' || c == '-') {
    res = ParseNumber (buf, length);
    if (TypeOf (res) != Null) return res;
  }

  /* Not a number, so place it in the name space */
  MakeObject(res, Name);
  res.u.Name = FindName(buf,length);
  return res;
}
