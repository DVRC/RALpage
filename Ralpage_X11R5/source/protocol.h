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

#define NEW_WINDOW	'W'
#define NEW_BITMAP	'B'
#define BITBLT		'b'
#define	SEND_BITMAP	's'
#define GET_BITMAP	'g'
#define	DESTROY_HARDWARE	'D'
#define LINE		'l'
#define GET_MATRIX	'm'
#define GET_TRANSFERSIZE	't'
#define SET_TRANSFER	'T'
#define PAINT		'P'
#define PAINT_LINE	'L'
#define HARD_FLUSH	'F'
#define SCREEN_SIZE	'Z'
#define BUILD_SCREEN	'U'
#define SET_SCREEN	'S'
#define SET_CLIP_HARDWARE	'c'
#define BITBLT_TRAPEZOID	'r'
#define PAINT_TRAPEZOID	'R'
#define SET_UPDATE_CONTROL	'u'

void master_protocol ();
void slave_protocol ();
void send_colour ();
Colour recv_colour ();
void send_small ();
float recv_small ();
void send_point ();
DevicePoint recv_point ();
void send_byte ();
unsigned char recv_byte ();
void send_short ();
short recv_short ();
void send_float ();
float recv_float ();
void send_string ();
void recv_string ();
void flush_protocol ();
int can_recv ();
