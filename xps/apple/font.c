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

#define __SEG__ SegF

#define MOVETO	0x80
#define BYTE	0xFF

#define FONT_USER	3
#define MakeVector(V, X, Y, T) V->vx = X; V->vy = Y; V->vt = T;

/* Also change #define in X11.c */
#define UpdateControl(h, on) {}

static struct show_context show_stack [MAXGSAVES];
 
int GMoveTo (), GLineTo ();
extern Matrix identity;

static int PCurrentFont ();
static int PDefineFont ();
static int PFindFont ();
static int PScaleFont ();
static int PMakeFont ();
static int PSetFont ();

static int BuildHershey ();
static int PShow ();
static int PWidthShow ();
static int PAShow ();
static int PAWidthShow ();
static int PStringWidth ();
static int PAdjust ();

static Vector *GetMetrics ();
extern Vector NewVector ();

static int ShowString ();
static int ShowStop ();
static int ShowStopped ();

Object Fid, FD, FontMatrix, FontType, Encoding, FontBBox;
Object OpShowStop, OpShowString, OpStopped;
Object UserShow, CharStrings, Metrics, BuildChar, FontName;

static last_fid = 0;

static int Update ();

HardPoint zeroHardPoint;
DevicePoint zeroDevicePoint = { 0, 0 };

extern Matrix HardPointTranslate ();

InitFont ()
{
  Fid 		= NameFrom ("FID");
  FontMatrix 	= NameFrom ("FontMatrix");
  FontType	= NameFrom ("FontType");
  Encoding	= NameFrom ("Encoding");
  FontBBox 	= NameFrom ("FontBBox");
  UserShow 	= NameFrom (".show");
  CharStrings	= NameFrom ("CharStrings");
  Metrics	= NameFrom ("Metrics");
  BuildChar	= NameFrom ("BuildChar");
  FontName	= NameFrom ("FontName");
  
  OpShowStop = MakeOp ("(showstop)",		ShowStop,	1, 0, 0, 0,
		       Bool);
  OpShowString = MakeOp ("(showstring)",	ShowString,	1, 0, 0, 0,
			 String);
  OpStopped = MakeOp ("(showstopped)",		ShowStopped,0, 1, 1, 0);
  
  InstallOp ("currentfont",	PCurrentFont,	0, 1, 0, 0);
  InstallOp ("definefont",	PDefineFont,	2, 1, 0, 0,
	     Poly, Dictionary);
  InstallOp ("findfont",	PFindFont,	1, 1, 0, 0, Poly);
  InstallOp ("scalefont",	PScaleFont,	2, 1, 0, 0,
	     Dictionary, Float);
  InstallOp ("makefont",	PMakeFont,	2, 1, 0, 0,
	     Dictionary, Array);
  InstallOp ("setfont",		PSetFont,	1, 0, 0, 0,
	     Dictionary);
  
  InstallOp ("show",		PShow,		1, 0, 0, 0, String);
  InstallOp ("widthshow",	PWidthShow,	4, 0, 0, 0,
	     Float, Float, Integer, String);
  InstallOp ("ashow",		PAShow,		3, 0, 0, 0,
	     Float, Float, String);
  InstallOp ("awidthshow",	PAWidthShow,	6, 0, 0, 0,
	     Float, Float, Integer, Float, Float, String);
  InstallOp ("stringwidth",	PStringWidth,	1, 2, 0, 0, String);
  InstallOp (".adjust",		PAdjust,	1, 0, 0, 0, Integer);
  InstallOp ("buildhershey",	BuildHershey,	2, 0, 0, 0,
	     Dictionary, Integer);
  InstallOp ("update",		Update,		1, 0, 0, 0, Bool);
  
  Install ("FontDirectory",	FD = MakeDict (20));
  
  zeroHardPoint = NewHardPoint (0.0, 0.0);
}

HardPoint DExtToInt (p)
     UserPoint p;
{
  Matrix m;
  HardPoint res;
  
  m = gstate->CTM;
  res.hx = p.x * m.A + p.y * m.C;
  res.hy = p.x * m.B + p.y * m.D;
  
  return res;
}

static int update_state = TRUE;

static int Update (bool)
     Object bool;
{
  int new = BodyBool (bool);
  
  if (new != update_state)
    UpdateControl (gstate->device->dev, update_state = new);
  return TRUE;
}

InitShowContext ()
{
  gstate->show = show_stack;
}

static int PAdjust (code)
     Object code;
{
  /*
  ** gstate->CTM = HardPointTranslate (gstate->CTM,
  **				    Adjust (ExtToInt (NewUserPoint (0.0,
  **								    0.0)),
  **					    BodyInteger (code)));
  ** Bug fix:  This appears to add tx and ty to themselves in gstate
  **           which seems wrong.  It is changed in the new code.
  **           
  */

  gstate->CTM.tx += gstate->show->shim.hx;
  gstate->CTM.ty += gstate->show->shim.hy;
  if (BodyInteger(code) == gstate->show->space) {
    gstate->CTM.tx += gstate->show->space_shim.hx;
    gstate->CTM.ty += gstate->show->space_shim.hy;
  }
  return TRUE;
}

static int PCurrentFont ()
{
  return Push (OpStack, gstate->font);
}

static int PFindFont (key)
     Object key;
{
  Object res;
  
  res = DictLoad (FD, key);
  if (TypeOf (res) == Condition)
    return Error (PInvFont);
  return Push (OpStack, res);
}

static Object *encodingCache = NULL;
static Matrix fontmatrixCache;
static int fontmatrixScale;
static int fonttypeCache;
int fidCache;
float fontbboxCache[4];

static int PSetFont (font)
     Object font;
{
  Object tmp;
  
  if (!CheckFont (font))
    return Error (PInvFont);
  gstate->font = font;
  ExtractMatrix (&fontmatrixCache, DictLoad (font, FontMatrix));
  fontmatrixScale = ((fontmatrixCache.B == 0.0) && (fontmatrixCache.C == 0.0)
	     && (fontmatrixCache.tx == 0.0) && (fontmatrixCache.ty == 0.0));

  tmp = DictLoad (font, Encoding);
  encodingCache = BodyArray (tmp);
  tmp = DictLoad (font, FontType);
  fonttypeCache = BodyInteger (tmp);
  fidCache = BodyFontID (DictLoad (font, Fid));
  ExtractBBox (fontbboxCache, DictLoad (font, FontBBox));
  return TRUE;
}

int CheckFont (font)
     Object font;
{
  Object mat, bbox, enc, fonttype;
  Matrix dummy;
  float bboxdummy[4];
  
  mat = DictLoad (font, FontMatrix);
  if (TypeOf (mat) == Condition || !ExtractMatrix (&dummy, mat))
    return FALSE;
  fonttype = DictLoad (font, FontType);
  if (TypeOf (fonttype) != Integer)
    return FALSE;
  bbox = DictLoad (font, FontBBox);
  if (TypeOf (bbox) != Array || lengthArray (bbox) != 4 || !ExtractBBox (bboxdummy, bbox))
    return FALSE;
  enc = DictLoad (font, Encoding);
  if (TypeOf (enc) != Array)
    return FALSE;
  /*
   * DCJ: Since LaserWriter allows smaller arrays, we should
   * too...
   */
  if ( lengthArray (enc) != 256 )
    Message("Warning: Encoding array does not contain 256 elements.");
  
  switch (BodyInteger (fonttype)) {
  case FONT_USER: return TRUE;
    
  default: return TRUE;
  }
}

int CheckHershey (font, encoding)
     Object font, *encoding;
{
  Object cs, metrics;
  int i;
  cs = DictLoad (font, CharStrings);
  if (TypeOf (cs) != Dictionary)
    return FALSE;
  metrics = DictLoad (font, Metrics);
  if (TypeOf (metrics) != Dictionary)
    return FALSE;
  
  for (i = 0; i < 256; i++) {
    Object met;
    
    met = DictLoad (metrics, encoding[i]);
    if (TypeOf (met) == Array) {
      Object *m = BodyArray (met);
      int j, l = lengthArray (met);
      
      if (l != 1 && l != 2 && l != 4)
	return FALSE;
      
      for (j = 0; j < l; j++)
	if (TypeOf (m[j]) != Integer && TypeOf (m[j]) != Real)
	  return FALSE;
    }
    else if (TypeOf (metrics) != Integer && TypeOf (metrics) != Real)
      return FALSE;
  }
  return TRUE;
}

int ExtractBBox (bbox, array)
     float *bbox;
     Object array;
{
  Object num;
  int i;
  
  if (lengthArray (array) != 4)
    return FALSE;
  for (i = 0; i < 4; i++) {
    num = getArray (array, i);
    
    if (TypeOf (num) == Integer)
      bbox [i] = BodyInteger (num);
    else if (TypeOf (num) == Real)
      bbox [i] = BodyReal (num);
    else
      return FALSE;
  }
  return TRUE;
}

static Object MakeFontID ()
{
  Object res;
  
  MakeObject (res, FontID);
  res.u.Font = last_fid++; /* just for uniqueness */
  
  return res;
}

int BodyFontID (fid)
     Object fid;
{
  return fid.u.Font;
}

static int PDefineFont (key, dict)
     Object key, dict;
{
  if (maxDict (dict) == lengthDict (dict) || maxDict (FD) == lengthDict (FD))
    return Error (PDictFull);
  
  if (!CheckFont (dict))
    return Error (PInvFont);
  
  DictStore (FD, key, ReadOnly (dict));
  DictStore (dict, Fid, MakeFontID ());
  return Push (OpStack, ReadOnly (dict));
}

struct font_entry
{
  Object font;
  Matrix mat;
  int fid;
  struct font_entry *font_next;
 } *font_list = NULL;

#define EqFont(m,n) (m.A == n.A && m.B == n.B && m.C == n.C && m.D == n.D)

Object LookupFont (font, matrix)
     Object font;
     Matrix matrix;
{
  Object res, mat, new;
  Matrix m, n;
  struct font_entry *p, *fe;
  int fid = BodyFontID (DictLoad (font, Fid));
  
  mat = DictLoad (font, FontMatrix);
  ExtractMatrix (&m, mat);
  n = MatMult (matrix, m);
  
  for (p = font_list; p != NULL; p = p->font_next)
    if (p->fid == fid && EqFont (p->mat, n)) {
      Message ("font matched");
      return p->font;
    }
  Message ("building a font dict");
  
  copyDict (font, res = MakeDict (maxDict (font)));
  new = MakeArray ((Object *) Malloc ((unsigned) 6 * sizeof (Object)), 6);
  VOID AssignMatrix (new, n);
  DictStore (res, FontMatrix, new);
  
  res = ReadOnly (res);
  
  fe = (struct font_entry *) Malloc (sizeof (struct font_entry));
  fe->mat = n;
  fe->font = res;
  fe->font_next = font_list;
  fe->fid = fid;
  font_list = fe;
  
  return res;
}

static int PScaleFont (font, scale)
     Object font, scale;
{
  Object res;
  float s = BodyReal (scale);
  Matrix m;
  
  if (!CheckFont (font))
    return Error (PInvFont);
  NewMatrix(m, s, 0.0, 0.0, s, 0.0, 0.0);
  res = LookupFont (font, m);
  
  return Push (OpStack, res);
}

static int PMakeFont (font, matrix)
     Object font, matrix;
{
  Object res;
  Matrix m;
  
  if (!CheckFont (font))
    return Error (PInvFont);
  ExtractMatrix (&m, matrix);
  res = LookupFont (font, m);
  
  return Push (OpStack, res);
}

static Vector *GetMetrics (v, ob)
     Vector *v;
     Object ob;
{
  Object aobj,aobj2;

  if (TypeOf (ob) == Integer || TypeOf (ob) == Real) {
    MakeVector (v, BodyFloat (ob), 0.0, 0.0);
  } else if (TypeOf (ob) != Array) {
    MakeVector (v, 0.0, 0.0, 1.0);
  } else if (lengthArray (ob) == 2) {
    aobj = getArray(ob,1);
    MakeVector (v, BodyFloat (aobj), 0.0, 0.0);
  } else if (lengthArray (ob) == 4) {
    aobj = getArray(ob,2);
    aobj2 = getArray(ob,3);
    MakeVector (v, BodyFloat (aobj), BodyFloat (aobj2), 0.0);
  } else {
    MakeVector (v, 0.0, 0.0, 1.0);
  }
  
  return v;
}
/*
 * BZS - some small changes to allow macrification of DictLoad()
 */
static int BuildHershey (font, code)
     Object font, code;
{
  Vector vtmp, *met;
  Object *bbox, string, nm, tmp;
  unsigned char *s;
  Path p;
  Matrix *mp;
  int (*func)();
  register float a,b,c,d,tx,ty;
  register int x,y, i, l;
  
  tmp = DictLoad (font, FontBBox);
  bbox 	= BodyArray (tmp);
  tmp = DictLoad (font, Encoding);
  nm 	= BodyArray (tmp) [BodyInteger (code)];
  /*
   * met 	= GetMetrics (DictLoad (DictLoad (font, Metrics), nm));
   */
  tmp = DictLoad(font,Metrics);
  met = GetMetrics(&vtmp, DictLoad(tmp,nm));
  met->vx -= 2; /* hershey bodge - they look better closer */
  /*
   * string 	= DictLoad (DictLoad (font, CharStrings), nm);
   */
  tmp = DictLoad(font,CharStrings);
  string = DictLoad(tmp,nm);
  
  SetCacheDevice (nm, NewUserPoint (met->vx, met->vy),
		  BodyReal (bbox[0]), BodyReal (bbox[1]),
		  BodyReal (bbox[2]), BodyReal (bbox[3]));
  
  if (TypeOf (string) != String) return TRUE;
  gstate->line_width = 1.5;
  gstate->line_join = 2;
  gstate->line_cap = 0;
  gstate->dash_length = 0;
  
  s = BodyString (string);
  l = lengthString (string) / 2;
  p = gstate->path;
  
  VOID PNewPath ();
  mp = &gstate->CTM;
  a = mp->A; b = mp->B; c = mp->C; d = mp->D; tx = mp->tx; ty = mp->ty;
  for (i = 0; i < l; i++) {
    HardPoint po;
    func = (*s & MOVETO) ? GMoveTo : GLineTo;
    x = (*s++ & ~MOVETO) - 64;
    y = (*s++ & ~MOVETO) - 73;
    po.hx = x*a + y*c + tx;
    po.hy = x*b + y*d + ty;
    (*func)(p,po);
  }
  return Push (ExecStack, Cvx (NameFrom ("stroke")));
}

/*
	Persistent state important across a BuildChar call-back
		InShow 		- the fact that we're in a call back
		gstate 		- graphics state
		ccache 		- the current cache device
		gstate->device 	- whether a cache device has been set or not.
				  The old device if a cache device is
				  installed.
				  The old CTM.
		Width 		- current width information - used for
				   moving cp
		awx, awy	- shim adjustment parameters
		scode,
		axcode, aycode	- spacing adjustment parameters
	
	XXXXXXX         XXXXXX         XXXXXXXXXX         XXXXXXXXX
	XXXXXXX         XXXXXX         XXXXXXXXXX         XXXXXXXXX
	XXXXXXX         XXXXXX         XXXXXXXXXX         XXXXXXXXX
	XXXXXXX         XXXXXX         XXXXXXXXXX         XXXXXXXXX
	XXXXXXX         XXXXXX         XXXXXXXXXX         XXXXXXXXX
	       |_______|      |_______|          |_______|
		   |
		ashow/widthshow width vector
	
	ShowUser shows the first character (to avoid a leading gap).
			It may have to call buildchar if the character
			is not cached. 
	ShowString finishes off a BuildChar (if necessary)
			then shows gaps and characters until it encounters
			one which is not cached.
		It then stacks:
			a stopped context to reset things if an error occurs.
			another ShowString to continue output.
			A buildchar to build a character
*/

static int Show (string, space, shim, space_shim)
     Object string;
     char space;
     HardPoint shim, space_shim;
{
  unsigned char *s;
  int l;

  if (!gstate->cp_defined) return Error (PNoCurrentPoint);
  if (fonttypeCache != FONT_USER) return Error (PInvFont);

  if((l = lengthString (string)) == 0) return TRUE;
  s = BodyString (string);

  ++gstate->show;
  gstate->show->InShow = TRUE;
  gstate->show->shim = shim;
  gstate->show->space_shim = space_shim;
  gstate->show->space = space;
  gstate->show->mat = gstate->CTM;
  
  gstate->CTM.tx = gstate->cp.hx;
  gstate->CTM.ty = gstate->cp.hy;
  
  if(fontmatrixScale) {
    gstate->CTM.A *= fontmatrixCache.A;
    gstate->CTM.B *= fontmatrixCache.A;
    gstate->CTM.C *= fontmatrixCache.D;
    gstate->CTM.D *= fontmatrixCache.D;
  }
  else gstate->CTM = MatMult (fontmatrixCache, gstate->CTM);

  FindCache ();
  
  UpdateControl (gstate->device->dev, FALSE);
  
  gstate->cp.hx = gstate->CTM.tx;
  gstate->cp.hy = gstate->CTM.ty;
  if (CacheShow (encodingCache [*s], gstate->cp))
    return ShowStringAux (string, s, l, gstate->show->Width);
  
  UpdateControl (gstate->device->dev, TRUE);
  
  GSave ();
  VOID Push (ExecStack, OpShowStop);
  PStopped (OpShowString);
  VOID Push (OpStack, string);
  VOID Push (OpStack, gstate->font);
  VOID Push (OpStack, MakeInteger (*s));
  VOID Push (ExecStack, DictLoad (gstate->font, BuildChar));
  return TRUE;
}

static int ShowStopped ()
{
  VOID Pop (ExecStack);
  VOID Push (OpStack, False);
  return TRUE;
}

static int ShowString (string)
     Object string;
{
  int l = lengthString (string);
  unsigned char *st = BodyString (string);
  UserPoint last_width;
  
  Pop (ExecStack);
  Pop (ExecStack);
  Pop (ExecStack);
  GRestore ();
  
  UpdateControl (gstate->device->dev, FALSE);
  
  last_width = gstate->show->Width;
  CacheShow (encodingCache [*st], gstate->cp);
  
  return ShowStringAux (string, st, l, last_width);
}

ShowStringAux (string, st, l, last_width)
     Object string;
     unsigned char *st;
     int l;
     UserPoint last_width;
{
  Matrix m;
  unsigned char *s = st;
  HardPoint loc;
  float x,y;

  loc.hx = gstate->CTM.tx; loc.hy = gstate->CTM.ty;
  for (;;) {
    int code = *++s;
    
    m = gstate->CTM;
    x = last_width.x * m.A + last_width.y * m.C + gstate->show->shim.hx;
    y = last_width.x * m.B + last_width.y * m.D + gstate->show->shim.hy;
    if (code == gstate->show->space) { 
      x += gstate->show->space_shim.hx; 
      y += gstate->show->space_shim.hy; 
    }
    loc.hx += x;
    loc.hy += y;

    if (--l == 0) break;

    if (!CacheShow (encodingCache [code], loc)) {
      gstate->cp = loc;
      gstate->CTM.tx = loc.hx; gstate->CTM.ty = loc.hy;
      
      VOID Push (ExecStack, OpShowStop);
      PStopped (OpShowString);
      
      VOID Push (OpStack, getIString (string, s - st, l));
      
      VOID Push (OpStack, gstate->font);
      VOID Push (OpStack, MakeInteger (*s));
      VOID Push (ExecStack, DictLoad (gstate->font,
				      BuildChar));
      UpdateControl (gstate->device->dev, TRUE);
      
      GSave ();
      
      return TRUE;
    }
    last_width = gstate->show->Width;
  }
  gstate->CTM.tx = loc.hx; gstate->CTM.ty = loc.hy;
  UpdateControl (gstate->device->dev, TRUE);
  
  return ShowStopAux ();
}

static int ShowStop (st)
     Object st;
{
  GRestore ();
  ShowStopAux ();
  if (BodyBool (st))
    return PStop ();
  return TRUE;
}

ShowStopAux ()
{
  HardPoint cp;
  
  cp.hx = gstate->CTM.tx;
  cp.hy = gstate->CTM.ty;
  /* GRestore (); */
  gstate->CTM = gstate->show->mat;
  --gstate->show;
  gstate->cp = cp;
  
  return TRUE;
}

static int PShow (string)
     Object string;
{
  return Show (string, 0, zeroHardPoint, zeroHardPoint);
}

static int PWidthShow (x, y, code, string)
     Object x, y, code, string;
{
  return Show (string, BodyInteger (code), zeroHardPoint,
	       DExtToInt (NewUserPoint (BodyReal (x), BodyReal (y))));
}

static int PAShow (ax, ay, string)
     Object ax, ay, string;
{
  return Show (string, 0, DExtToInt (NewUserPoint (BodyReal (ax),
						   BodyReal (ay))),
	       zeroHardPoint);
}

static int PAWidthShow (x, y, code, ax, ay, string)
     Object x, y, code, ax, ay, string;
{
  return Show (string,
	       BodyInteger (code),
	       DExtToInt (NewUserPoint (BodyReal (x), BodyReal (y))),
	       DExtToInt (NewUserPoint (BodyReal (ax), BodyReal (ay))));
}

static int PStringWidth (string)
     Object string;
{
  Matrix m;
  Vector *v, vtmp;
  Object *enc, met, tmp;
  int i, l = lengthString (string);
  unsigned char *s = BodyString (string);
  float x = 0, y = 0;
  
  if (!CheckFont (gstate->font))
    return Error (PInvFont);
  tmp = DictLoad (gstate->font, FontType);
  if (BodyInteger (tmp) == 3) {
    VOID Push (OpStack, string);
    VOID Push (ExecStack, Cvx (NameFrom (".stringwidth")));
    return TRUE;
  }
  tmp = DictLoad (gstate->font, Encoding);
  enc = BodyArray (tmp);
  met = DictLoad (gstate->font, Metrics);
  for (i = 0; i < l; i++) {
    v = GetMetrics (&vtmp, DictLoad (met, enc[s[i]]));
    x += v->vx;
    y += v->vy;
  }
  ExtractMatrix (&m, DictLoad (gstate->font, FontMatrix));
  vtmp = Transform (NewVector (x, y, 0.0), m);
  return Push (OpStack, MakeReal (vtmp.vx)), Push (OpStack,
						   MakeReal (vtmp.vy));
}
