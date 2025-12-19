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

#include "main.h"

#define SMALL_SIZE	4096

char *Malloc (u) unsigned u;
 {
 	char *malloc (), *res;

 	res = malloc (u);
 	if (res == NULL)
 	 {
 	 	fprintf (stderr, "malloc failed - out of memory\n");
 	 	exit (1);
 	 }
 	return res;
 }

/*ARGSUSED*/
Free (s) char *s;
 {
	/* free (s);	*/
 }
