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
#include "graphics.h"
#include "canon.h"

static int transfer [TRANSFER_SIZE];

void InitTransfer (ppi) int ppi;
 {
 	int i;
 	
 	pixels_per_inch = ppi;
 	for (i = 0; i < TRANSFER_SIZE; i++)
 		transfer [i] = i;
 }

int HardColour (colour) Colour colour;
 {
	return transfer [(int) ((TRANSFER_SIZE - 1) * colour.brightness + .5)];
 }

int TransferSize ()
 {
	return TRANSFER_SIZE;
 }

void SetTransfer (tran) float *tran;
 {
 	int i;
 	
 	for (i = 0; i < TRANSFER_SIZE; i++)
 		transfer [i] = (TRANSFER_SIZE - 1) * tran[i] + .5;
 }
