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

#define ROP_FALSE	0	/* F		*/
#define ROP_AND		1	/* S & D	*/
#define ROP_ANDNOT	2	/* S & ~D	*/
#define ROP_SOURCE	3	/* S		*/
#define ROP_NOTAND	4	/* ~S & D	*/
#define ROP_DEST	5	/* D		*/
#define ROP_XOR		6	/* S ^ D	*/
#define ROP_OR		7	/* S | D	*/
#define ROP_NOR		8	/* ~(S | D)	*/
#define ROP_NXOR	9	/* ~(S ^ D)	*/
#define ROP_NOTDEST	10	/* ~D		*/
#define ROP_ORNOT	11	/* S | ~D	*/
#define ROP_NOTSOURCE	12	/* ~S		*/
#define ROP_NOTOR	13	/* ~S | D	*/
#define ROP_NAND	14	/* ~(S & D)	*/
#define ROP_TRUE	15	/* T		*/
