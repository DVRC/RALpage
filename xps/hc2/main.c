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
#ifndef macintosh
#include <signal.h>
#endif

#include "main.h"

#define __SEG__ SegI

#define ERRORDICTSIZE	 25

Object PDictFull;
Object PDictOverflow,		PInvFont,		PSyntaxError;
Object PDictUnderflow,		PInvRestore,		PTypeCheck;
Object PExecOverflow,		PIOError,		PUndefined;
Object PExecUnderflow,		PLimitCheck,		PUnFilename;
Object PInterrupt,		PNoCurrentPoint,	PUnResult;
Object PInvAccess,		PRangeCheck,		PUnMatched;
Object PInvExit,		POpOverflow,		PUnregistered;
Object PInvFileAccess,		POpUnderflow,		PVMError;

Object Lbracket, Rbracket;
Object OpInterp, ErrorDict;

/* External variables for hashing statistics */
extern int hash_tries, hash_collisions, hash_attempts;

static int ErrorFn ();
static Object InsErr ();
static void InitErrors (), InitArguments (), Interpreter ();
static Object OpError, execName;
Object error_name;

static catchint ();
static int EnableInterrupts ();

char *library = NULL;

Object InitialObject = {READABLE|WRITEABLE};
Object InitialExecObject = {READABLE|WRITEABLE|EXECUTABLE};

static int files_present = 0, interrupted = FALSE, protected = 0;
int interactive = FALSE, verbose = FALSE, RUNSYNC = FALSE, TESTING = FALSE;
FILE *vfp = NULL;

main (argc, argv)
     int argc;
     char **argv;
{
  char profile[BUFSIZE], rcfile [BUFSIZE];
#ifndef MACINTOSH
  char *getenv ();
#endif
  FILE *fp;
  int here;
 	
#ifndef MACINTOSH
  library = getenv ("POSTSCRIPTLIB");
#endif
  if (library == NULL) library = default_library;
	
#ifdef MACINTOSH
  VOID sprintf (rcfile, "psrc", library);
#else
  VOID sprintf (rcfile, "%s/psrc", library);
#endif
  
#ifdef MACINTOSH
#ifndef macintosh
  vfp = stderr;
#endif
#else
  if (close (dup (9)) == 0) {
    verbose = TRUE;
    vfp = fdopen (9, "w");
  }
#endif
  
  Message ("POSTSCRIPT");

  Init ();
  Message ("Init Errors");
  InitErrors ();
  Message ("Init Arguments");
	
  VOID Push (ExecStack, Cvx (NameFrom ("executive")));
	
  here = Height (ExecStack);
	
  Install ("Library", StringFrom (library));
	
  InitArguments (argc, argv);
#ifdef MACINTOSH
  interactive = interactive || !files_present;
#else
  interactive = interactive || isatty (0) && !files_present;
#endif
  ReverseStack (ExecStack, Height (ExecStack) - here);
	
#ifndef MACINTOSH
  VOID strcpy (profile, getenv ("HOME") ? getenv ("HOME") : "/etc");
#endif
  VOID strcat (profile, "/.postscript");
  
  if (fp = fopen (profile, "r"))
    VOID Push (ExecStack, Cvx (FileFrom (fp)));
		
  if (!interactive)
    VOID Push (ExecStack,
	       Cvx (StringFrom (files_present ?
				"/executive null def" :
				"/executive (%stdin) (r) file cvx def")));

  if (TESTING)
    VOID Push (ExecStack,
      Cvx (StringFrom ("/showpage { copypage initgraphics erasepage } def")));
      
  if ((fp = fopen (rcfile, "r")) == NULL)
    fprintf (stderr, "%s: cannot open %s\n", argv[0], rcfile),
    exit (1);
  else
    VOID Push (ExecStack, Cvx (FileFrom (fp)));
  InstallOp ("enableinterrupts",	EnableInterrupts, 0, 0, 0, 0);
/*	SysDict = ReadOnly (SysDict);	*/
  Message ("Entering Interpreter");
  Interpreter ();
}

Protect ()
{
  ++protected;
}

Unprotect ()
{
  --protected;
}

static int EnableInterrupts ()
{
#ifndef macintosh
  if (interactive)
    VOID signal (SIGINT, catchint);
#endif
  return TRUE;
}

static void InitArguments (argc, argv)
     int argc;
     char **argv;
{
  int i, command = FALSE;
      
  for (i = 1; i < argc; i++) {
    FILE *fp;
	 	
    if (command) {
      command = FALSE;
      VOID Push (ExecStack, Cvx (StringFrom (argv[i])));
    }
    else if (*argv[i] == '-')
      switch (argv[i][1]) {
      case 'n':
	RUNSYNC = TRUE;
	break;
		
      case 't':
	TESTING = TRUE;
	break;
		
      case 'i':
	interactive = TRUE;
	break;
		
      case 'c':
	++files_present; 
	command = TRUE;
	break;
		
      case 's': case '\0':
	++files_present;
	VOID Push (ExecStack, Cvx (Fstdin));
	break;
	 		 	
      default:
	fprintf (stderr, "%s: unknown option '%c'\n", argv[0],
		 argv[i][1]);
	exit (1);
      }
    else {
      ++files_present;
      if (fp = Fopen (argv[i], "r"))
	VOID Push (ExecStack, Cvx (FileFrom (fp)));
      else
	fprintf (stderr, "%s: cannot open %s\n", argv[0],
		 argv[i]);
    }
  }
}

static void Interpreter ()
{
  register Stack ExecSt,OpSt,DictSt;

  Self = execName = NameFrom ("exec");
  ExecSt = ExecStack;
  OpSt = OpStack;
  DictSt = DictStack;

  while (Height (ExecSt) != 0) {
    int res;
    Object item, exop;
    Type itype;
 		
    item = Pop (ExecSt);
    if (!xCheck (item))
      res = Push (OpSt, item);
    else if ((itype = TypeOf (item)) == Operator)
      /* Marginally slower when inlined... */
      res = ExecOperator(item);
    else if (itype == Array) {
      /* Arrays get special treatment for speed */
      int l = lengthArray (item);
      register Object *rp;
      Type t;

      if (l != 0) {
	if(ExecSt->stack_fill != ExecSt->stack_size) {
	  rp = &ExecSt->stack_body[ExecSt->stack_fill++];
	  rp->type = Array;
	  rp->u.Integer = 0;
	  rp->u.Array = item.u.Array + 1;
	  rp->Length = l - 1;
	  rp->flags = item.flags;
	}

	rp = BodyArray(item);
	t = TypeOf(*rp);
	if(t == Name || t == Operator) Push (ExecSt, *rp);
	else                           Push (OpSt, *rp);
      }
      res = TRUE;
    }
    else if (itype == Name) {
      /* Names get special treatment for speed */
      Type dict;
      struct dict_entry *hash;
      int level,i,h,size,hashval;

      /* Try to load the object from the dictionary */
      /* Initialize information based solely on the item */
      hashval = item.Length + BodyInteger(item) + (int) itype;

      /* Search through the levels of the dict stack from the top down */
      for (level = Height (DictSt) - 1; level >= 0; level--) {
	dict = BodyDict(DictSt->stack_body[level]);
	if ((size=dict->dict_size) != 0) {
	  hash = dict->dict_body;
	  ++hash_attempts;
	  i = h = hashval % size;

	  for(;;) {
	    if (TypeOf (hash[i].entry_key) == Null) break;
	    if (DictEqual (item, hash[i].entry_key)) {
	      ++hash_tries;
	      item = hash[i].entry_value;
	      level = 0;
	      break;
	    }
	    if (++i == size) i = 0;
	    if (i == h) break;
	    ++hash_collisions;
	  }
	}
      }

      if (!xCheck (item))  res = Push (OpSt, item);
      else if ((itype = TypeOf (item)) == Operator) res = ExecOperator (item);
      else if (itype == Array)	res = ExecArray (item);
      else if (itype == File)   res = ExecFile (item);
      else {
	res = Push (OpSt, item);
	exop = Lookup (TypeOf (item), execName);
	if (TypeOf (exop) != Condition)
	  VOID Push (ExecSt, exop);
      }
    }
    else if (itype == File)
      res = ExecFile (item);
    else {
      res = Push (OpSt, item);
      exop = Lookup (TypeOf (item), execName);
      if (TypeOf (exop) != Condition)
	VOID Push (ExecSt, exop);
    }
    
    if (interrupted && !protected) {
      interrupted = FALSE;
      error_name = PInterrupt;
      res = FALSE;
    }
 		
    if (!res) {
      Object error_op;
  		 	
      VOID Push (OpSt, Self);
      error_op = DictLoad (ErrorDict, error_name);
      if (TypeOf (error_op) == Condition)
	VOID Push (ExecSt, OpError);
      else
	VOID Push (ExecSt, error_op);
      Cbreak (TRUE);
    }
  }
}

static void InitErrors ()
{
  ErrorDict = MakeDict (ERRORDICTSIZE);
  PDictFull = InsErr (ErrorDict, "dictfull",		      	ErrorFn);
  PDictOverflow	= InsErr (ErrorDict, "dictstackoverflow",	ErrorFn);
  PDictUnderflow = InsErr (ErrorDict, "dictstackunderflow",	ErrorFn);
  PExecOverflow	= InsErr (ErrorDict, "execstackoverflow",	ErrorFn);
  PExecUnderflow = InsErr (ErrorDict, "execstackunderflow",	ErrorFn);
  PInterrupt = InsErr (ErrorDict, "interrupt",			ErrorFn);
  PInvAccess = InsErr (ErrorDict, "invalidaccess",		ErrorFn);
  PInvExit = InsErr (ErrorDict, "invalidexit",			ErrorFn);
  PInvFileAccess = InsErr (ErrorDict, "invalidfileaccess",	ErrorFn);
  PInvFont = InsErr (ErrorDict, "invalidfont",			ErrorFn);
  PInvRestore = InsErr (ErrorDict, "invalidrestore",		ErrorFn);
  PIOError = InsErr (ErrorDict, "ioerror",			ErrorFn);
  PLimitCheck = InsErr (ErrorDict, "limitcheck",		ErrorFn);
  PNoCurrentPoint = InsErr (ErrorDict, "nocurrentpoint",	ErrorFn);
  PRangeCheck = InsErr (ErrorDict, "rangecheck",		ErrorFn);
  POpOverflow = InsErr (ErrorDict, "stackoverflow",		ErrorFn);
  POpUnderflow = InsErr (ErrorDict, "stackunderflow",		ErrorFn);
  PSyntaxError = InsErr (ErrorDict, "syntaxerror",		ErrorFn);
  PTypeCheck = InsErr (ErrorDict, "typecheck",			ErrorFn);
  PUndefined = InsErr (ErrorDict, "undefined",			ErrorFn);
  PUnFilename = InsErr (ErrorDict, "undefinedfilename",		ErrorFn);
  PUnResult	= InsErr (ErrorDict, "undefinedresult",		ErrorFn);
  PUnMatched = InsErr (ErrorDict, "unmatchedmark",		ErrorFn);
  PUnregistered = InsErr (ErrorDict, "unregistered",		ErrorFn);
  PVMError = InsErr (ErrorDict, "VMerror",			ErrorFn);
  Install ("errordict", ErrorDict);
  OpError = MakeOp ("(errorfn)", ErrorFn, 1, 0, 0, 1, Name);
}

/*
 * This is the code to deal with user interrupts.
 * In order to do this cleanly, we effectively poll for interrupts in
 * the main interpreter loop (above). Possibly any routine which could
 * take an inordinate amount of time out of this loop should also poll
 * the `interrupted' variable and generate an error if one is found.
 * Currently none do - "run"ning a file is done by the interpreter
 * above.
 *
 */

void BZero(p,n)
     register char *p;
     register unsigned n;
{
  while (n--)
    *p++ = '\0';
}

static catchint ()
{
#ifndef macintosh
  VOID signal (SIGINT, catchint);
  interrupted = TRUE;
#endif
}

int Interrupted ()
{
  return interrupted && !protected;
}

/*
 *	An intrinsic operator generates an error condition by
 *	returning FALSE.  The usual way to do this is to call Error
 *	with the PostScript name of the error condition.
 *	
 */

static Object InsErr (dict, name, fn)
     Object dict;
     unsigned char *name;
     int (*fn)();
{
  Object op;
 	
  DictStore (dict, op = NameFrom (name), MakeOp (name, fn, 1, 0, 0, 1, Name));
  return op;
}

static ErrorFn (name)
     Object name;
{
  PrintName (Self);
  printf (" in operator ");
  PrintName (name);
  putchar ('\n');
  return Push (ExecStack, Cvx (NameFrom ("stop")));
}

Panic (s)
     char *s;
{
  fprintf (stderr, "PostScript panic: %s\n", s);
  fprintf (stderr, "Please report this fault to the support person for this program\n");
  Cleanup ();
  exit (1);
}

Object Cvx (o)
     Object o;
{
  o.flags |= EXECUTABLE;
  return o;
}

Object Cvlit (o)
     Object o;
{
  o.flags &= ~EXECUTABLE;
  return o;
}

Object ExecOnly (o)
     Object o;
{
  if (o.type == Dictionary)
    o.u.Dictionary->dict_flags &= ~(READABLE | WRITEABLE);
  else
    o.flags &= ~(READABLE | WRITEABLE);
  return ReadOnly (o);
}

Object ReadOnly (o)
     Object o;
{
  if (o.type == Dictionary)
    o.u.Dictionary->dict_flags &= ~WRITEABLE;
  else
    o.flags &= ~WRITEABLE;
  return o;
}

Object WriteOnly (o)
     Object o;
{
  if (o.type == Dictionary)
    o.u.Dictionary->dict_flags &= ~READABLE;
  else
    o.flags &= ~READABLE;
  return o;
}

Object SameFlags (a, b)
     Object a, b;
{
  b.flags = a.flags;
  return b;
}
