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

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <setjmp.h>
#ifdef SYS5
#include <string.h>
#include <memory.h>
#define bcopy(s,d,n) memcpy(d,s,n)
#else
#include <strings.h>
#endif

#define NONE		(-2)
#define EOS		'\0'
#define TRUE		1
#define FALSE		0

#define BUFSIZE	     1000

#define VOID	(void)

#define READABLE	 01
#define WRITEABLE	 02
#define EXECUTABLE	 04
#define PERMANENT	010
#define PIPED		020

#define TYPES	16

typedef struct dict_struct *Type;

extern Type type_table [];

typedef struct object
 {
	unsigned int Length : 23;
 	unsigned int flags : 5;
	unsigned int type : 4;
	union
	 {
		int Integer, Boolean, Font;
	 	float Real;
	 	Type Dictionary;
	 	unsigned char *String;
	 	struct name_struct *Name;
	 	struct op_struct *Operator;
	 	struct file_struct *File;
	 	struct object *Array;
	 } u;
	
 } Object;
 
enum file_type { StringFile, StreamFile };

struct file_struct
 {
 	enum file_type file_type;
 	int available;
 	union
 	 {
 	 	unsigned char	*c_ptr;
 	 	FILE	*f_ptr;
 	 } f;
 };

struct dict_entry
 {
 	Object entry_key, entry_value;
 };

struct dict_struct
 {
 	int type_index;
 	int dict_flags, dict_size, dict_fill;
 	struct dict_entry *dict_body;
 };

typedef struct stack
 {
 	int stack_fill, stack_size;
 	Object overflow, underflow, *stack_body;
 } *Stack, StackOb;
 
Object SameFlags (), MakeObject (), Cvx (), Cvlit (), ReadOnly (), WriteOnly (), ExecOnly (), NoAccess ();
int OpCheck (), min (), rCheck (), wCheck (), xCheck ();
Object MakeArray (), ParseArray (), getArray (), getIArray (), *BodyArray ();
Object MakeBoolean ();


Object MakeDict (), DictLoad (), Lookup (), DictFrom (), Load ();
Type MakeType (), TypeOf (), BodyDict ();
void EmptyDict ();

extern int EqTrue (), Equal ();

Object FileFrom (), FileString ();

int Getch ();
struct file_struct *BodyFile ();
Object MakeInteger (), IntReal ();

Object ParseNumber ();
float Deg (), Rad ();
Object ParseId (), NameFrom (), MakeName (), Cvn (), StringName ();
unsigned char *BodyName ();
Object MakeOp (), NameOperator ();
Object Parse ();


int PolyFirst (), PolySecond (), PolyThird (), PolyPair ();

Object MakeReal (), RealInteger ();

float BodyReal (), BodyFloat ();

char *Malloc ();
Object Pop (), Top (), Where (), DictLookup ();
int Push ();

extern Object MakeString (), StringFrom (), getIString (), ParseString (), ParseHexString ();
unsigned char *BodyString ();
int lengthString ();
char *sprintf ();

extern Object PDictFull;
extern Object PDictOverflow,		PInvFont,		PSyntaxError;
extern Object PDictUnderflow,		PInvRestore,		PTypeCheck;
extern Object PExecOverflow,		PIOError,		PUndefined;
extern Object PExecUnderflow,		PLimitCheck,		PUnFilename;
extern Object PInterrupt,		PNoCurrentPoint,	PUnResult;
extern Object PInvAccess,		PRangeCheck,		PUnMatched;
extern Object PInvExit,			POpOverflow,		PUnregistered;
extern Object PInvFileAccess,		POpUnderflow,		PVMError;

extern Type Boolean, Mark, String, Real, Poly, Operator;
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
extern unsigned char *Bcopy ();
extern FILE *vfp, *Fopen ();
extern void Fclose ();


#define TypeOf(a) 	(type_table [(a).type])

#define Push(stack, object) (((stack)->stack_fill != (stack)->stack_size) ?	\
		((stack)->stack_body[(stack)->stack_fill] = (object), (stack)->stack_fill++, TRUE) : FALSE)

#define Pop(stack) 	((stack)->stack_body[--(stack)->stack_fill])

#define Top(stack)	((stack)->stack_body[(stack)->stack_fill - 1])

#define Height(stack)	((stack)->stack_fill)

#define MaxStack(stack)		((stack)->stack_size)

extern int getchbuf;

#define BodyFile(file) ((file).u.File)

#define StatusFile(file) (BodyFile(file)->available != 0)

#define Getch(file) ((StatusFile(file) && BodyFile(file)->file_type == StreamFile) ?\
	((getchbuf = getc (BodyFile(file)->f.f_ptr)), \
		((getchbuf != EOF) ? getchbuf : ((BodyFile(file)->available = 0), Close (file), EOF))) \
	: GeneralGetch (file))

/*
 * BZS - macro-ify some things
 */
Object *DictFind();
#define DictLoad(DICT,KEY) (*DictFind((DICT.u.Dictionary)->dict_body,KEY,\
				    (DICT.u.Dictionary)->dict_size))
/* Next line --jgm */
#define PanicIf(flag,s) do { if (flag) Panic(s); } while (0)
