/*
 * Copyright (C) Crispin Goswell 1987, All Rights Reserved.
 */

#include <stdio.h>
#include <math.h>

#ifndef macintosh
#include <assert.h>
#endif

#include <setjmp.h>
#include <strings.h>

#define NONE		(-2)
#define EOS		'\0'
#define TRUE		1
#define FALSE		0

#ifdef BUFSIZE
#undef BUFSIZE
#endif
#define BUFSIZE	     1000
#define STRSIZE      4000

#ifndef unix
#define MACINTOSH
#endif

#define VOID	(void)

#define READABLE	 01
#define WRITEABLE	 02
#define EXECUTABLE	 04
#define PERMANENT	010
#define PIPED		020

typedef struct dict_struct *Type;

typedef struct object {
  int flags;
  Type type;
  int Length;
  union {
    int Integer, Bool, Font;
    float Real;
    Type Dictionary;
    unsigned char *String;
    struct name_struct *Name;
    struct op_struct *Operator;
    struct file_struct *File;
    struct object *Array;
  } u;
} Object;
 
struct name_struct
{
  struct name_struct *next;
  int string_length;
  unsigned char *string_body;
};

enum file_type { StringFile, StreamFile };

struct file_struct {
  enum file_type file_type;
  int available;
  union {
    unsigned char *c_ptr;
    FILE	  *f_ptr;
  } f;
};

struct dict_entry { Object entry_key, entry_value; };

struct dict_struct {
  int dict_flags, dict_size, dict_fill;
  struct dict_entry *dict_body;
};

typedef struct stack {
  int stack_fill, stack_size;
  Object overflow, underflow, *stack_body;
} *Stack, StackOb;
 
struct op_struct {
  Object name;
  int (*fn)(), arguments, results, execpop, execpush, execuse;
  Type *argtypes;
};

Object SameFlags (), Cvx (), Cvlit ();
Object ReadOnly (), WriteOnly (), ExecOnly ();
int OpCheck ();
Object MakeArray (), ParseArray (), getIArray ();

Object MakeDict (), DictFrom (), Load ();
Type MakeType ();

extern int EqTrue (), Equal ();

Object FileFrom (), FileString ();

int Getch ();
struct file_struct *BodyFile ();
Object MakeInteger (), IntReal ();

Object ParseNumber ();
float Deg (), Rad ();
Object ParseId (), NameFrom (), MakeName (), Cvn (), StringName ();
Object MakeOp (), NameOperator ();
Object Parse ();

int PolyFirst (), PolySecond (), PolyThird (), PolyPair ();

Object MakeReal ();

extern char *malloc();
extern int free();
#define Malloc(I) malloc(I)
#define Free(S) free(S)

Object Where (), DictLookup ();

extern Object MakeString (), StringFrom (), getIString ();
extern Object ParseString (), ParseHexString ();

extern Object PDictFull;
extern Object PDictOverflow,		PInvFont,		PSyntaxError;
extern Object PDictUnderflow,		PInvRestore,		PTypeCheck;
extern Object PExecOverflow,		PIOError,		PUndefined;
extern Object PExecUnderflow,		PLimitCheck,		PUnFilename;
extern Object PInterrupt,		PNoCurrentPoint,	PUnResult;
extern Object PInvAccess,		PRangeCheck,		PUnMatched;
extern Object PInvExit,			POpOverflow,		PUnregistered;
extern Object PInvFileAccess,		POpUnderflow,		PVMError;

extern Object error_name;

extern Type Bool, Mark, String, Real, Poly, Operator;
extern Type Name, File, Dictionary, Condition, Null, Integer;
extern Type Array, Mark, Condition, Null, Float, FontID;

extern Object SysDict, Absent, Nil;
extern Object True, False, Marker, Self;
extern Object OpInterp, Lbracket, Rbracket;
extern Object StatementEdit, Fstdin, Fstdout, Fstderr;

extern Stack OpStack, ExecStack, DictStack;
extern jmp_buf env;
extern int interactive, verbose;
extern char default_library[], *library;
extern FILE *vfp, *Fopen ();
extern void Fclose ();

/* Body macros */
#define BodyInteger(O)  ((O).u.Integer)
#define BodyReal(O)     ((O).u.Real)
#define BodyFloat(O)    ((O).type==Integer?(float)BodyInteger(O):BodyReal(O))
#define BodyOperator(O)	((O).u.Operator)
#define BodyString(O)	((O).u.String)
#define BodyArray(O)    ((O).u.Array)
#define BodyDict(O)     ((O).u.Dictionary)
#define BodyName(O)     ((O).u.Name->string_body)
#define BodyBool(O)     ((O).u.Bool)
#define BodyFile(O)     ((O).u.File)
#define lengthName(O)   ((O).u.Name->string_length)
#define lengthString(O) ((O).Length)
#define MakeBool(B)     ((B)?True:False)

/* Stack macros */
#define Top(S)	      ((S)->stack_body[(S)->stack_fill - 1])
#define Height(S)     ((S)->stack_fill)
#define MaxStack(S)   ((S)->stack_size)
#define Space(S)      ((S)->stack_size - (S)->stack_fill)
#define Pop(S) 	      ((S)->stack_body[--(S)->stack_fill])
#define FastPush(S,O) ((S)->stack_body[(S)->stack_fill]=(O),(S)->stack_fill++)
#define Push(S,O)     (((S)->stack_fill != (S)->stack_size) \
            ? ((S)->stack_body[(S)->stack_fill]=(O), (S)->stack_fill++,TRUE) \
	    : FALSE)
#define Push2(S,O1,O2) (((S)->stack_fill < (S)->stack_size - 1) \
            ? ((S)->stack_body[(S)->stack_fill]=(O1), (S)->stack_fill++, \
               (S)->stack_body[(S)->stack_fill]=(O2), (S)->stack_fill++,TRUE) \
            : (Push(S,O1), FALSE))
#define Push3(S,O1,O2,O3) (((S)->stack_fill < (S)->stack_size - 2) \
            ? ((S)->stack_body[(S)->stack_fill]=(O1), (S)->stack_fill++, \
               (S)->stack_body[(S)->stack_fill]=(O2), (S)->stack_fill++, \
               (S)->stack_body[(S)->stack_fill]=(O3), (S)->stack_fill++,TRUE) \
            : (Push(S,O1), Push(S,O2), FALSE))
#define Push4(S,O1,O2,O3,O4) (((S)->stack_fill < (S)->stack_size - 3) \
            ? ((S)->stack_body[(S)->stack_fill]=(O1), (S)->stack_fill++ \
               (S)->stack_body[(S)->stack_fill]=(O2), (S)->stack_fill++ \
               (S)->stack_body[(S)->stack_fill]=(O3), (S)->stack_fill++ \
               (S)->stack_body[(S)->stack_fill]=(O4), (S)->stack_fill++,TRUE) \
            : (Push(S,O1), Push(S,O2), Push(S,O3), FALSE))

/* File macros */
extern int getchbuf;
#define StatusFile(file) (BodyFile(file)->available != 0)
#define Getch(file) \
  ((StatusFile(file) && BodyFile(file)->file_type == StreamFile) \
  ? ((getchbuf = getc (BodyFile(file)->f.f_ptr)), \
    ((getchbuf != EOF) ? getchbuf : ((BodyFile(file)->available = 0), \
				     Close (file), EOF))) \
  : GeneralGetch (file))

/* Dictionary macros */
Object DictFind();
#define DictLoad(DICT,KEY) DictFind((DICT.u.Dictionary)->dict_body,KEY,\
				    (DICT.u.Dictionary)->dict_size)
#define DictEqual(A,B) ((TypeOf(A) == TypeOf(B)) && \
		       (A.Length == B.Length) && \
		       ((A.u.Integer == B.u.Integer)|| \
		        (TypeOf(A) == Array) && (A.Length == 0)))
#define Lookup(DICT,KEY) DictFind((DICT)->dict_body,KEY,(DICT)->dict_size)
#define maxDict(D)     (BodyDict(D)->dict_size)
#define lengthDict(D)  (BodyDict(D)->dict_fill)

/* Flag macros */
#define rCheck(O) ((O.type == Dictionary) \
		    ? (O.u.Dictionary->dict_flags & READABLE) \
		    : (O.flags & READABLE))
#define wCheck(O) ((O.type == Dictionary) \
		    ? (O.u.Dictionary->dict_flags & WRITEABLE) \
		    : (O.flags & WRITEABLE))
#define xCheck(O) (O.flags & EXECUTABLE)

/* Path macros */
extern struct path_element *free_pelem;
#define NewElem(T, P) { P = free_pelem; if (P) free_pelem = P->next;\
     else P = (Path) Malloc (sizeof (struct path_element));\
     P->ptype = T; }
#define CurEType(P) ((P)->last->ptype)
#define EmptyPath(P) ((!(P))?TRUE:((P)->next == (P)))
#define ElemFree(P)  { PanicIf ((P) == NULL, "ElemFree given NULL");\
		       (P)->next = free_pelem; free_pelem = (P); }
#define PathInsert(P, I) { PanicIf ((I) == NULL, "PathInsert given NULL");\
				(I)->next = (P); (I)->last = (P)->last;\
				(P)->last->next = (I); (P)->last = (I); }
#define PathFree(P) { PanicIf((P) == NULL, "PathFree given NULL");\
			   (P)->last->next = free_pelem; \
				(P)->last = (P); \
			   free_pelem = (P); }
#define PathDelete(P) { Path R;\
			PanicIf ((P)->next == (P), "delete from empty path");\
                        R=(P)->last; R->last->next = (P); (P)->last = R->last;\
			R->next = free_pelem; free_pelem = R; }


/* Object macros */
extern Object InitialObject,InitialExecObject;
#define MakeObject(O, T) { O = InitialObject; O.type = T;}
#define MakeExecObject(O, T) { O = InitialExecObject; O.type = T;}

/* Matrix macros */
#define NewMatrix(m, pA, pB, pC, pD, ptx, pty) { \
  (m).A = pA; (m).B = pB; (m).C = pC; (m).D = pD; (m).tx = ptx; (m).ty = pty; }
#define RotatePosQ(m,t) { t=m.A; m.A=m.C; m.C = -t; t=m.B; m.B=m.D; m.D = -t; }
#define RotateNegQ(m,t) { t=m.A; m.A = -m.C; m.C=t; t=m.B; m.B = -m.D; m.D=t; }

/* Miscellaneous macros */
#define lengthArray(A)   ((A).Length)
#define getArray(A,I)    ((A).u.Array[I])
#define NameOperator(O)  ((O).u.Operator->name)
#define TypeOf(O)        ((O).type)
#define Message(S) { if (vfp != NULL) fprintf (vfp, "%s\n", S), fflush (vfp); }
#define PanicIf(COND,STR) if(COND) Panic(STR)
#define Max(a, b) ((a > b) ? a : b)
#define Min(a, b) ((a < b) ? a : b)
#define RealInteger(O) (O.type = Real, O.u.Real = (float) O.u.Integer)
#define Error(O) (error_name=(O),FALSE)

/* Bcopy macro */
#ifdef AUX
#define Bcopy(D,S,N) memcpy(D,S,N)
#else
#ifdef MACINTOSH
#define Bcopy(D,S,N) BlockMove(S,D,N)
#else
#define Bcopy(D,S,N) bcopy(S,D,N)
#endif
#endif
