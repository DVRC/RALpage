/*
 * Copyright (C) Crispin Goswell 1987, All Rights Reserved.
 */

typedef struct point { float x, y; } UserPoint;

extern Matrix PointTranslate ();
extern UserPoint NewUserPoint (), IntToExt (), MoveUserPoint ();
extern int GMoveTo (), GLineTo ();
extern Object AssignMatrix ();
