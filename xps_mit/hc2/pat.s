	.globl	.oVncs
	.set	.oVncs,0
	.globl	.oVhc2.1d
	.set	.oVhc2.1d,0
	.globl	.oVO
	.set	.oVO,0
	.data
	.align	2
L00DATA:
	.align	2
	.globl	_free_pelem
_free_pelem:	.long	0x0
	.align	2
_NewPath:	.long	_.NewPath
	.text
	.align	2
L00TEXT:
	.globl	_.NewPath
	.globl	_NewPath
	.text
#--------------| NewPath |-----------------------#
_.NewPath:
	stm	r14,-44(sp)
	mr	r14,r0
	l	r2,_free_pelem-_NewPath(r14)
	cal	sp,-44(sp)
	cis	r2,0
	jeq	L01343
	ls	r0,28(r2)
	bx	L01344
	st	r0,_free_pelem-_NewPath(r14)
L01343:
	cal	r2,36(r0)
	.data
	.align	2
LF1:	.long	_malloc
	.text
	balix	r15,_.malloc
	l	r0,LF1-_NewPath(r14)
L01344:
	lis	r0,0
	sts	r0,0(r2)
	sts	r2,32(r2)
	sts	r2,28(r2)
	lm	r14,0(sp)
	brx	r15
	cal	sp,44(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for NewPath
	.byte	0xe8	# First nonvolatile register saved is r14
	.byte	0x01	# npars=0 words
	.byte	0xb	# lcl_offset=44
	.align	1
	.data
	.align	2
_PathCopy:	.long	_.PathCopy
	.text
	.data
	.align	2
L1:	.ascii	"PathInsert given NULL\0"
	.text
	.globl	_.PathCopy
	.globl	_PathCopy
	.text
#--------------| PathCopy |-----------------------#
_.PathCopy:
	stm	r10,-60(sp)
	mr	r14,r0
	cal	sp,-60(sp)
	mr	r13,r2
	l	r12,_free_pelem-_PathCopy(r14)
	cis	r12,0
	jeq	L01353
	ls	r0,28(r12)
	bx	L01354
	st	r0,_free_pelem-_PathCopy(r14)
L01353:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_PathCopy(r14)
	mr	r12,r2
L01354:
	lis	r0,0
	sts	r0,0(r12)
	sts	r12,32(r12)
	sts	r12,28(r12)
	ls	r10,28(r13)
	c	r10,r13
	jeq	L013511
L01355:
	l	r11,_free_pelem-_PathCopy(r14)
	cis	r11,0
	jeq	L01357
	ls	r0,28(r11)
	bx	L01358
	st	r0,_free_pelem-_PathCopy(r14)
L01357:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_PathCopy(r14)
	mr	r11,r2
L01358:
	ls	r0,0(r10)
	sts	r0,0(r11)
	cal	r2,4(r11)
	cal	r3,4(r10)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	ls	r0,8(r3)
	sts	r0,8(r2)
	ls	r0,12(r3)
	sts	r0,12(r2)
	ls	r0,16(r3)
	sts	r0,16(r2)
	ls	r0,20(r3)
	sts	r0,20(r2)
	cis	r11,0
	jne	L013510
	cal	r2,L1-_PathCopy(r14)
	.data
	.align	2
LF2:	.long	_Panic
	.text
	balix	r15,_.Panic
	l	r0,LF2-_PathCopy(r14)
L013510:
	sts	r12,28(r11)
	ls	r2,32(r12)
	sts	r2,32(r11)
	ls	r2,32(r12)
	sts	r11,28(r2)
	sts	r11,32(r12)
	ls	r10,28(r10)
	c	r10,r13
	jne	L01355
L013511:
	mr	r2,r12
	lm	r10,0(sp)
	brx	r15
	cal	sp,60(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for PathCopy
	.byte	0xa8	# First nonvolatile register saved is r10
	.byte	0x11	# npars=1 words
	.byte	0xf	# lcl_offset=60
	.align	1
	.data
	.align	2
_PathRemove:	.long	_.PathRemove
	.text
	.globl	_.PathRemove
	.globl	_PathRemove
	.text
#--------------| PathRemove |-----------------------#
_.PathRemove:
	ls	r3,32(r2)
	ls	r4,28(r2)
	sts	r4,28(r3)
	ls	r4,28(r2)
	ls	r3,32(r2)
	brx	r15
	sts	r3,32(r4)
	.long	0xdf02df00	#Trace table (no stack frame)
	.data
	.align	2
_MoveTo:	.long	_.MoveTo
	.text
	.data
	.align	2
L2:	.ascii	"delete from empty path\0"
	.align	2
L3:	.ascii	"PathInsert given NULL\0"
	.text
	.globl	_.MoveTo
	.globl	_MoveTo
	.text
#--------------| MoveTo |-----------------------#
_.MoveTo:
	.using	_MoveTo,r14
	stm	r11,-56(sp)
	mr	r14,r0
	cal	sp,-56(sp)
	sts	r3,44(sp)	# np
	sts	r4,48(sp)	# np
	mr	r11,r2
	l	r12,_free_pelem-_MoveTo(r14)
	cis	r12,0
	jeq	L01813
	ls	r0,28(r12)
	bx	L01814
	st	r0,_free_pelem-_MoveTo(r14)
L01813:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_MoveTo(r14)
	mr	r12,r2
L01814:
	lis	r13,1
	sts	r13,0(r12)
	cal	r2,4(r12)
	cal	r3,44(sp)	# np
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r11,0
	jne	L01816
	bx	L01817
	mr	r0,r13
L01816:
	ls	r0,28(r11)
	c	r0,r11
	beqx	LL18117
	lis	r0,1
	lis	r0,0
LL18117:
L01817:
	cis	r0,0
	jne	L018112
	ls	r3,32(r11)
	ls	r0,0(r3)
	cis	r0,1
	jne	L018112
	ls	r0,28(r11)
	c	r0,r11
	jne	L018111
	cal	r2,L2-_MoveTo(r14)
	balix	r15,_.Panic
	l	r0,LF2-_MoveTo(r14)
	ls	r3,32(r11)
L018111:
	ls	r2,32(r3)
	l	r0,_free_pelem-_MoveTo(r14)
	sts	r11,28(r2)
	ls	r2,32(r3)
	sts	r2,32(r11)
	sts	r0,28(r3)
	st	r3,_free_pelem-_MoveTo(r14)
L018112:
	load	r2,_gstate
	sts	r13,44(r2)
	cal	r2,36(r2)
	cal	r3,44(sp)	# np
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r12,0
	jne	L018114
	cal	r2,L3-_MoveTo(r14)
	balix	r15,_.Panic
	l	r0,LF2-_MoveTo(r14)
L018114:
	sts	r11,28(r12)
	ls	r2,32(r11)
	sts	r2,32(r12)
	ls	r2,32(r11)
	sts	r12,28(r2)
	sts	r12,32(r11)
	mr	r2,r13
	lm	r11,0(sp)
	brx	r15
	cal	sp,56(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for MoveTo
	.byte	0xb8	# First nonvolatile register saved is r11
	.byte	0x31	# npars=3 words
	.byte	0xe	# lcl_offset=56
	.align	1
	.data
	.align	2
_LineTo:	.long	_.LineTo
	.text
	.data
	.align	2
L4:	.ascii	"PathInsert given NULL\0"
	.text
	.globl	_.LineTo
	.globl	_LineTo
	.text
#--------------| LineTo |-----------------------#
_.LineTo:
	.using	_LineTo,r14
	stm	r12,-52(sp)
	mr	r14,r0
	cal	sp,-52(sp)
	sts	r3,40(sp)	# np
	sts	r4,44(sp)	# np
	mr	r12,r2
	l	r13,_free_pelem-_LineTo(r14)
	cis	r13,0
	jeq	L01823
	ls	r0,28(r13)
	bx	L01824
	st	r0,_free_pelem-_LineTo(r14)
L01823:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_LineTo(r14)
	mr	r13,r2
L01824:
	lis	r0,2
	sts	r0,0(r13)
	cal	r2,4(r13)
	cal	r3,40(sp)	# np
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	load	r2,_gstate
	cal	r2,36(r2)
	cal	r3,40(sp)	# np
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r13,0
	jne	L01826
	cal	r2,L4-_LineTo(r14)
	balix	r15,_.Panic
	l	r0,LF2-_LineTo(r14)
L01826:
	sts	r12,28(r13)
	ls	r2,32(r12)
	sts	r2,32(r13)
	ls	r2,32(r12)
	sts	r13,28(r2)
	sts	r13,32(r12)
	lis	r2,1
	lm	r12,0(sp)
	brx	r15
	cal	sp,52(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for LineTo
	.byte	0xc8	# First nonvolatile register saved is r12
	.byte	0x31	# npars=3 words
	.byte	0xd	# lcl_offset=52
	.align	1
	.data
	.align	2
_CurveTo:	.long	_.CurveTo
	.text
	.data
	.align	2
L5:	.ascii	"PathInsert given NULL\0"
	.text
	.globl	_.CurveTo
	.globl	_CurveTo
	.text
#--------------| CurveTo |-----------------------#
_.CurveTo:
	.using	_CurveTo,r14
	stm	r12,-52(sp)
	mr	r14,r0
	cal	sp,-52(sp)
	sts	r3,40(sp)	# p0
	sts	r4,44(sp)	# p0
	sts	r5,48(sp)	# p1
	mr	r12,r2
	l	r13,_free_pelem-_CurveTo(r14)
	cis	r13,0
	jeq	L01863
	ls	r0,28(r13)
	bx	L01864
	st	r0,_free_pelem-_CurveTo(r14)
L01863:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_CurveTo(r14)
	mr	r13,r2
L01864:
	lis	r0,4
	sts	r0,0(r13)
	cal	r2,4(r13)
	cal	r3,40(sp)	# p0
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,12(r13)
	cal	r3,48(sp)	# p1
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,20(r13)
	cal	r3,56(sp)	# p2
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	load	r2,_gstate
	cal	r2,36(r2)
	cal	r3,56(sp)	# p2
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r13,0
	jne	L01866
	cal	r2,L5-_CurveTo(r14)
	balix	r15,_.Panic
	l	r0,LF2-_CurveTo(r14)
L01866:
	sts	r12,28(r13)
	ls	r2,32(r12)
	sts	r2,32(r13)
	ls	r2,32(r12)
	sts	r13,28(r2)
	sts	r13,32(r12)
	lis	r2,1
	lm	r12,0(sp)
	brx	r15
	cal	sp,52(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for CurveTo
	.byte	0xc8	# First nonvolatile register saved is r12
	.byte	0x71	# npars=7 words
	.byte	0xd	# lcl_offset=52
	.align	1
	.data
	.align	2
_Normalise:	.long	_.Normalise
	.text
	.globl	_.Normalise
	.globl	_Normalise
	.text
#--------------| Normalise |-----------------------#
_.Normalise:
	.using	_Normalise,r14
	stm	r11,-56(sp)
	mr	r14,r0
	cal	sp,-128(sp)
	cal	r15,L31-_Normalise(r14)
	balr	r15,r15	# STOREM	<fr2/fr3>,0(sp)
	cal	r15,L32-_Normalise(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L33-_Normalise(r14)
	balr	r15,r15	# MOVE	F:fr3,D:fr0
	cal	r15,L34-_Normalise(r14)
	balr	r15,r15	# MOVE	F:fr2,F:-3.141593e+00
	cal	r15,L35-_Normalise(r14)
	balr	r15,r15	# CMPT	F:fr3,F:fr2
	jl	L014310
	cal	r15,L36-_Normalise(r14)
	balr	r15,r15	# MOVE	F:fr1,F:3.141593e+00
	cal	r15,L37-_Normalise(r14)
	balr	r15,r15	# CMPT	F:fr3,F:fr1
	jle	L01438
	cal	r15,L38-_Normalise(r14)
	balr	r15,r15	# MOVE	F:fr0,F:6.283185e+00
L01433:
	cal	r15,L37-_Normalise(r14)
	balr	r15,r15	# CMPT	F:fr3,F:fr1
	jhe	L01435
	cal	r15,L39-_Normalise(r14)
	balr	r15,r15	# ADD	F:fr3,F:fr0
	j	L01436
L01435:
	cal	r15,L40-_Normalise(r14)
	balr	r15,r15	# SUB	F:fr3,F:fr0
L01436:
	cal	r15,L41-_Normalise(r14)
	balr	r15,r15	# CMPT	F:fr3,F:fr2
	jl	L01433
	cal	r15,L42-_Normalise(r14)
	balr	r15,r15	# CMPT	F:fr3,F:fr1
	jh	L01433
L01438:
	cal	r15,L43-_Normalise(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,F:fr3
	cal	r15,L44-_Normalise(r14)
	balr	r15,r15	# LOADM	<fr2/fr3>,0(sp)
	lm	r11,72(sp)
	brx	r15
	cal	sp,128(sp)
L014310:
	cal	r15,L45-_Normalise(r14)
	balr	r15,r15	# MOVE	F:fr1,F:3.141593e+00
	cal	r15,L46-_Normalise(r14)
	balr	r15,r15	# MOVE	F:fr0,F:6.283185e+00
	j	L01433
	.byte	0xdf,0x07,0xdf	#Trace table for Normalise
	.byte	0xba	# First nonvolatile register saved is r11
	.byte	0x21	# npars=2 words
	.byte	0x30	# rtfl regs saved
	.byte	0x20	# lcl_offset=128
	.align	1
	.data
	.align	2
L31:	# STOREM	<fr2/fr3>,0(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1d,2
	.short	LSI31
	.byte	36
	.byte	LSF31
	.short	0xff30	# opd
	.short	0xff01	# opd
	.long	0
	.space	16
	.align	2
L32:	# MOVE	D:fr0,D:<r2,r3>
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI32
	.byte	84
	.byte	LSF32
	.short	0x1300	# opd
	.short	0x0323	# opd
	.space	68
	.align	2
L33:	# MOVE	F:fr3,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI33
	.byte	24
	.byte	LSF33
	.short	0x1203	# opd
	.short	0x1300	# opd
	.space	8
	.align	2
L34:	# MOVE	F:fr2,F:-3.141593e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI34
	.byte	108
	.byte	LSF34
	.short	0x1202	# opd
	.short	0x2200	# opd
	.long	0xc0490fdb	# -3.14159
	.space	88
	.align	2
L35:	# CMPT	F:fr3,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI35
	.byte	84
	.byte	LSF35
	.short	0x1203	# opd
	.short	0x1202	# opd
	.space	68
	.align	2
L36:	# MOVE	F:fr1,F:3.141593e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI36
	.byte	108
	.byte	LSF36
	.short	0x1201	# opd
	.short	0x2200	# opd
	.long	0x40490fdb	# 3.14159
	.space	88
	.align	2
L37:	# CMPT	F:fr3,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI37
	.byte	84
	.byte	LSF37
	.short	0x1203	# opd
	.short	0x1201	# opd
	.space	68
	.align	2
L38:	# MOVE	F:fr0,F:6.283185e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI38
	.byte	108
	.byte	LSF38
	.short	0x1200	# opd
	.short	0x2200	# opd
	.long	0x40c90fdb	# 6.28319
	.space	88
	.align	2
L39:	# ADD	F:fr3,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI39
	.byte	36
	.byte	LSF39
	.short	0x1203	# opd
	.short	0x1200	# opd
	.space	20
	.align	2
L40:	# SUB	F:fr3,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,2
	.short	LSI40
	.byte	36
	.byte	LSF40
	.short	0x1203	# opd
	.short	0x1200	# opd
	.space	20
	.align	2
L41:	# CMPT	F:fr3,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI41
	.byte	84
	.byte	LSF41
	.short	0x1203	# opd
	.short	0x1202	# opd
	.space	68
	.align	2
L42:	# CMPT	F:fr3,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI42
	.byte	84
	.byte	LSF42
	.short	0x1203	# opd
	.short	0x1201	# opd
	.space	68
	.align	2
L43:	# MOVE	D:<r2,r3>,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI43
	.byte	52
	.byte	LSF43
	.short	0x0323	# opd
	.short	0x1203	# opd
	.space	36
	.align	2
L44:	# LOADM	<fr2/fr3>,0(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0f,2
	.short	LSI44
	.byte	44
	.byte	LSF44
	.short	0xff30	# opd
	.short	0xff01	# opd
	.long	0
	.space	24
	.align	2
L45:	# MOVE	F:fr1,F:3.141593e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI45
	.byte	108
	.byte	LSF45
	.short	0x1201	# opd
	.short	0x2200	# opd
	.long	0x40490fdb	# 3.14159
	.space	88
	.align	2
L46:	# MOVE	F:fr0,F:6.283185e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI46
	.byte	108
	.byte	LSF46
	.short	0x1200	# opd
	.short	0x2200	# opd
	.long	0x40c90fdb	# 6.28319
	.space	88
	.text
	.data
	.align	2
_Arc:	.long	_.Arc
	.text
	.data
	.align	2
L6:	.ascii	"PathInsert given NULL\0"
	.align	2
L7:	.ascii	"PathInsert given NULL\0"
	.align	2
L8:	.ascii	"PathInsert given NULL\0"
	.text
	.globl	_.Arc
	.globl	_Arc
	.text
#--------------| Arc |-----------------------#
_.Arc:
	.using	_Arc,r14
	stm	r8,-68(sp)
	mr	r14,r0
	cal	sp,-220(sp)
	cal	r15,L47-_Arc(r14)
	balr	r15,r15	# STOREM	<fr2/fr3/fr4/fr5/fr6/fr7>,80(sp)
	st	r4,212(sp)	# centre
	st	r5,216(sp)	# centre
	mr	r8,r2
	mr	r9,r3
	cal	r15,L48-_Arc(r14)
	balr	r15,r15	# MOVE	F:20(sp),D:220(sp)
	cal	r15,L49-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr2,D:228(sp)
	cal	r15,L50-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr0,D:236(sp)
	cis	r9,0
	jle	L01873
	cal	r15,L51-_Arc(r14)
	balr	r15,r15	# SUB	F:fr6,F:fr0,F:fr2
	j	L01874
L01873:
	cal	r15,L52-_Arc(r14)
	balr	r15,r15	# SUB	F:fr6,F:fr2,F:fr0
L01874:
	cal	r15,L53-_Arc(r14)
	balr	r15,r15	# MOVE	F:12(sp),F:0.000000e+00
	cal	r15,L54-_Arc(r14)
	balr	r15,r15	# CMPT	F:fr6,F:12(sp)
	jhe	L01876
	cal	r15,L55-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr0,F:6.283185e+00
L01875:
	cal	r15,L56-_Arc(r14)
	balr	r15,r15	# ADD	F:fr6,F:fr0
	cal	r15,L54-_Arc(r14)
	balr	r15,r15	# CMPT	F:fr6,F:12(sp)
	jl	L01875
	mr	r0,r0	# no op to fix APC bug
	mr	r0,r0	# no op to fix APC bug
	mr	r0,r0	# no op to fix APC bug
L01876:
	lda	r12,_gstate
	ls	r3,0(r12)
	cal	r2,32(sp)	# m
	cal	r3,0(r3)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	ls	r0,8(r3)
	sts	r0,8(r2)
	ls	r0,12(r3)
	sts	r0,12(r2)
	ls	r0,16(r3)
	sts	r0,16(r2)
	ls	r0,20(r3)
	sts	r0,20(r2)
	cal	r15,L57-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr1,F:212(sp)
	cal	r15,L58-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr5,F:32(sp)
	cal	r15,L59-_Arc(r14)
	balr	r15,r15	# MUL	F:fr4,F:fr5,F:fr1
	cal	r15,L60-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr0,F:216(sp)
	cal	r15,L61-_Arc(r14)
	balr	r15,r15	# MOVE	F:16(sp),F:40(sp)
	cal	r15,L62-_Arc(r14)
	balr	r15,r15	# MUL	F:fr3,F:16(sp),F:fr0
	cal	r15,L63-_Arc(r14)
	balr	r15,r15	# ADD	F:fr3,F:fr4
	cal	r15,L64-_Arc(r14)
	balr	r15,r15	# ADD	F:28(sp),F:48(sp),F:fr3
	cal	r15,L65-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr4,F:36(sp)
	cal	r15,L66-_Arc(r14)
	balr	r15,r15	# MUL	F:fr1,F:fr4
	cal	r15,L67-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr3,F:44(sp)
	cal	r15,L68-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr3
	cal	r15,L69-_Arc(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr1
	cal	r15,L70-_Arc(r14)
	balr	r15,r15	# ADD	F:24(sp),F:52(sp),F:fr0
	cal	r15,L71-_Arc(r14)
	balr	r15,r15	# CMP	F:fr2,F:12(sp)
	jeq	L01878
	cal	r15,L72-_Arc(r14)
	balr	r15,r15	# MOVE	D:fr7,F:fr2
	cal	r15,L73-_Arc(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:fr7
	.data
	.align	2
LF3:	.long	_cos
	.text
	balix	r15,_.cos
	l	r0,LF3-_Arc(r14)
	cal	r15,L74-_Arc(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L75-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr2,D:fr0
	cal	r15,L76-_Arc(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:fr7
	.data
	.align	2
LF4:	.long	_sin
	.text
	balix	r15,_.sin
	l	r0,LF4-_Arc(r14)
	cal	r15,L77-_Arc(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L78-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr7,D:fr0
	cal	r15,L79-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr1,F:fr5
	cal	r15,L80-_Arc(r14)
	balr	r15,r15	# MOVE	F:8(sp),F:fr4
	cal	r15,L81-_Arc(r14)
	balr	r15,r15	# MUL	F:fr5,F:fr2
	cal	r15,L82-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:16(sp),F:fr7
	cal	r15,L83-_Arc(r14)
	balr	r15,r15	# ADD	F:fr5,F:fr0
	cal	r15,L84-_Arc(r14)
	balr	r15,r15	# MUL	F:fr4,F:fr2
	cal	r15,L85-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr3,F:fr7
	cal	r15,L86-_Arc(r14)
	balr	r15,r15	# ADD	F:fr4,F:fr0
	cal	r15,L87-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:16(sp),F:fr2
	cal	r15,L88-_Arc(r14)
	balr	r15,r15	# MUL	F:fr1,F:fr7
	cal	r15,L89-_Arc(r14)
	balr	r15,r15	# SUB	F:16(sp),F:fr0,F:fr1
	cal	r15,L90-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr3,F:fr2
	cal	r15,L91-_Arc(r14)
	balr	r15,r15	# MUL	F:fr1,F:8(sp),F:fr7
	cal	r15,L92-_Arc(r14)
	balr	r15,r15	# SUB	F:fr3,F:fr0,F:fr1
L01878:
	cal	r15,L93-_Arc(r14)
	balr	r15,r15	# DIV	F:fr0,F:fr6,F:1.570786e+00
	cal	r15,L94-_Arc(r14)
	balr	r15,r15	# MOVE	I:r11,F:fr0
	cis	r11,0
	ble	L018723
	cal	r15,L95-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,I:r11,F:1.570796e+00
	cal	r15,L96-_Arc(r14)
	balr	r15,r15	# SUB	F:fr6,F:fr0
	cal	r15,L97-_Arc(r14)
	balr	r15,r15	# MUL	F:fr2,F:5.522847e-01,F:20(sp)
	l	r10,_free_pelem-_Arc(r14)
	lis	r13,4
	cis	r9,0
	ble	L018717
L018710:
	cal	r15,L98-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr5,F:20(sp)
	cal	r15,L99-_Arc(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr0,F:28(sp)
	cal	r15,L87-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:16(sp),F:fr2
	cal	r15,L69-_Arc(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr1
	cal	r15,L100-_Arc(r14)
	balr	r15,r15	# MOVE	F:72(sp),F:fr0
	cal	r15,L101-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr4,F:20(sp)
	cal	r15,L102-_Arc(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr0,F:24(sp)
	cal	r15,L90-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr3,F:fr2
	cal	r15,L69-_Arc(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr1
	cal	r15,L103-_Arc(r14)
	balr	r15,r15	# MOVE	F:76(sp),F:fr0
	cal	r15,L104-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:16(sp),F:20(sp)
	cal	r15,L99-_Arc(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr0,F:28(sp)
	cal	r15,L105-_Arc(r14)
	balr	r15,r15	# MOVE	F:56(sp),F:fr1
	cal	r15,L106-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr3,F:20(sp)
	cal	r15,L107-_Arc(r14)
	balr	r15,r15	# ADD	F:fr0,F:24(sp)
	cal	r15,L108-_Arc(r14)
	balr	r15,r15	# MOVE	F:60(sp),F:fr0
	cal	r15,L109-_Arc(r14)
	balr	r15,r15	# MUL	F:fr7,F:fr5,F:fr2
	cal	r15,L110-_Arc(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr7
	cal	r15,L111-_Arc(r14)
	balr	r15,r15	# MOVE	F:64(sp),F:fr1
	cal	r15,L112-_Arc(r14)
	balr	r15,r15	# MUL	F:fr1,F:fr4,F:fr2
	cal	r15,L69-_Arc(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr1
	cal	r15,L113-_Arc(r14)
	balr	r15,r15	# MOVE	F:68(sp),F:fr0
	cis	r10,0
	jeq	L018712
	ls	r0,28(r10)
	bx	L018713
	st	r0,_free_pelem-_Arc(r14)
L018712:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_Arc(r14)
	mr	r10,r2
L018713:
	ls	r4,0(r12)
	sts	r13,0(r10)
	cal	r2,4(r10)
	cal	r3,72(sp)	# P1
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,12(r10)
	cal	r3,64(sp)	# P2
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,20(r10)
	cal	r3,56(sp)	# P3
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,36(r4)
	cal	r3,56(sp)	# P3
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r10,0
	jne	L018715
	cal	r2,L6-_Arc(r14)
	balix	r15,_.Panic
	l	r0,LF2-_Arc(r14)
L018715:
	sts	r8,28(r10)
	ls	r2,32(r8)
	sts	r2,32(r10)
	ls	r2,32(r8)
	sts	r10,28(r2)
	sts	r10,32(r8)
	cal	r15,L114-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr0,F:fr5
	cal	r15,L115-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr5,F:16(sp)
	cal	r15,L116-_Arc(r14)
	balr	r15,r15	# NEG	F:fr0,F:fr0
	cal	r15,L117-_Arc(r14)
	balr	r15,r15	# MOVE	F:16(sp),F:fr0
	cal	r15,L118-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr0,F:fr4
	cal	r15,L119-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr4,F:fr3
	cal	r15,L120-_Arc(r14)
	balr	r15,r15	# NEG	F:fr3,F:fr0
	sis	r11,1
	ble	L018723
	bx	L018710
	l	r10,_free_pelem-_Arc(r14)
L018717:
	cal	r15,L98-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr5,F:20(sp)
	cal	r15,L99-_Arc(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr0,F:28(sp)
	cal	r15,L87-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:16(sp),F:fr2
	cal	r15,L121-_Arc(r14)
	balr	r15,r15	# SUB	F:fr0,F:fr1,F:fr0
	cal	r15,L100-_Arc(r14)
	balr	r15,r15	# MOVE	F:72(sp),F:fr0
	cal	r15,L101-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr4,F:20(sp)
	cal	r15,L102-_Arc(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr0,F:24(sp)
	cal	r15,L90-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr3,F:fr2
	cal	r15,L121-_Arc(r14)
	balr	r15,r15	# SUB	F:fr0,F:fr1,F:fr0
	cal	r15,L103-_Arc(r14)
	balr	r15,r15	# MOVE	F:76(sp),F:fr0
	cal	r15,L104-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:16(sp),F:20(sp)
	cal	r15,L122-_Arc(r14)
	balr	r15,r15	# SUB	F:fr1,F:28(sp),F:fr0
	cal	r15,L105-_Arc(r14)
	balr	r15,r15	# MOVE	F:56(sp),F:fr1
	cal	r15,L106-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr3,F:20(sp)
	cal	r15,L123-_Arc(r14)
	balr	r15,r15	# SUB	F:fr0,F:24(sp),F:fr0
	cal	r15,L108-_Arc(r14)
	balr	r15,r15	# MOVE	F:60(sp),F:fr0
	cal	r15,L109-_Arc(r14)
	balr	r15,r15	# MUL	F:fr7,F:fr5,F:fr2
	cal	r15,L110-_Arc(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr7
	cal	r15,L111-_Arc(r14)
	balr	r15,r15	# MOVE	F:64(sp),F:fr1
	cal	r15,L112-_Arc(r14)
	balr	r15,r15	# MUL	F:fr1,F:fr4,F:fr2
	cal	r15,L69-_Arc(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr1
	cal	r15,L113-_Arc(r14)
	balr	r15,r15	# MOVE	F:68(sp),F:fr0
	cis	r10,0
	jeq	L018719
	ls	r0,28(r10)
	bx	L018720
	st	r0,_free_pelem-_Arc(r14)
L018719:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_Arc(r14)
	mr	r10,r2
L018720:
	ls	r4,0(r12)
	sts	r13,0(r10)
	cal	r2,4(r10)
	cal	r3,72(sp)	# P1
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,12(r10)
	cal	r3,64(sp)	# P2
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,20(r10)
	cal	r3,56(sp)	# P3
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,36(r4)
	cal	r3,56(sp)	# P3
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r10,0
	jne	L018722
	cal	r2,L7-_Arc(r14)
	balix	r15,_.Panic
	l	r0,LF2-_Arc(r14)
L018722:
	sts	r8,28(r10)
	ls	r2,32(r8)
	sts	r2,32(r10)
	ls	r2,32(r8)
	sts	r10,28(r2)
	sts	r10,32(r8)
	cal	r15,L114-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr0,F:fr5
	cal	r15,L124-_Arc(r14)
	balr	r15,r15	# NEG	F:fr5,F:16(sp)
	cal	r15,L117-_Arc(r14)
	balr	r15,r15	# MOVE	F:16(sp),F:fr0
	cal	r15,L118-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr0,F:fr4
	cal	r15,L125-_Arc(r14)
	balr	r15,r15	# NEG	F:fr4,F:fr3
	cal	r15,L126-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr3,F:fr0
	sis	r11,1
	bh	L018737
L018723:
	cal	r15,L127-_Arc(r14)
	balr	r15,r15	# CMPT	F:fr6,F:12(sp)
	jh	L018725
	lis	r2,1
	cal	r15,L128-_Arc(r14)
	balr	r15,r15	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,80(sp)
	lm	r8,152(sp)
	brx	r15
	cal	sp,220(sp)
L018725:
	lis	r13,4
	cal	r15,L129-_Arc(r14)
	balr	r15,r15	# DIV	F:fr0,F:fr6,F:2.000000e+00
	cal	r15,L130-_Arc(r14)
	balr	r15,r15	# MOVE	D:fr6,F:fr0
	cal	r15,L131-_Arc(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:fr6
	balix	r15,_.cos
	l	r0,LF3-_Arc(r14)
	cal	r15,L74-_Arc(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L75-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr2,D:fr0
	cal	r15,L132-_Arc(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:fr6
	balix	r15,_.sin
	l	r0,LF4-_Arc(r14)
	cal	r15,L77-_Arc(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L133-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr1,D:fr0
	cis	r9,0
	jhe	L018727
	cal	r15,L134-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr6,F:fr5
	cal	r15,L135-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr0,F:fr4
	cal	r15,L109-_Arc(r14)
	balr	r15,r15	# MUL	F:fr7,F:fr5,F:fr2
	cal	r15,L136-_Arc(r14)
	balr	r15,r15	# MUL	F:fr5,F:16(sp),F:fr1
	cal	r15,L137-_Arc(r14)
	balr	r15,r15	# SUB	F:fr5,F:fr7,F:fr5
	cal	r15,L138-_Arc(r14)
	balr	r15,r15	# MUL	F:fr7,F:fr4,F:fr2
	cal	r15,L139-_Arc(r14)
	balr	r15,r15	# MUL	F:fr4,F:fr3,F:fr1
	cal	r15,L140-_Arc(r14)
	balr	r15,r15	# SUB	F:fr4,F:fr7,F:fr4
	cal	r15,L141-_Arc(r14)
	balr	r15,r15	# NEG	F:fr7,F:16(sp)
	cal	r15,L142-_Arc(r14)
	balr	r15,r15	# MUL	F:fr7,F:fr2
	cal	r15,L143-_Arc(r14)
	balr	r15,r15	# MUL	F:fr6,F:fr1
	cal	r15,L144-_Arc(r14)
	balr	r15,r15	# SUB	F:16(sp),F:fr7,F:fr6
	cal	r15,L145-_Arc(r14)
	balr	r15,r15	# NEG	F:fr3,F:fr3
	cal	r15,L146-_Arc(r14)
	balr	r15,r15	# MUL	F:fr3,F:fr2
	cal	r15,L147-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr1
	cal	r15,L40-_Arc(r14)
	balr	r15,r15	# SUB	F:fr3,F:fr0
	cal	r15,L148-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr2,F:20(sp)
	cal	r15,L149-_Arc(r14)
	balr	r15,r15	# NEG	F:fr6,F:20(sp)
	cal	r15,L150-_Arc(r14)
	balr	r15,r15	# MUL	F:fr7,F:fr6,F:fr1
	cal	r15,L151-_Arc(r14)
	balr	r15,r15	# MUL	F:0(sp),F:fr5,F:fr0
	cal	r15,L152-_Arc(r14)
	balr	r15,r15	# MUL	F:fr6,F:16(sp),F:fr7
	cal	r15,L153-_Arc(r14)
	balr	r15,r15	# SUB	F:fr6,F:0(sp),F:fr6
	cal	r15,L154-_Arc(r14)
	balr	r15,r15	# ADD	F:fr6,F:28(sp)
	cal	r15,L155-_Arc(r14)
	balr	r15,r15	# MOVE	F:56(sp),F:fr6
	cal	r15,L156-_Arc(r14)
	balr	r15,r15	# MUL	F:fr6,F:fr4,F:fr0
	cal	r15,L157-_Arc(r14)
	balr	r15,r15	# MUL	F:fr7,F:fr3
	cal	r15,L158-_Arc(r14)
	balr	r15,r15	# SUB	F:fr6,F:fr7
	cal	r15,L159-_Arc(r14)
	balr	r15,r15	# ADD	F:fr6,F:24(sp)
	cal	r15,L160-_Arc(r14)
	balr	r15,r15	# MOVE	F:60(sp),F:fr6
	j	L018728
L018727:
	cal	r15,L161-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr6,F:fr5
	cal	r15,L162-_Arc(r14)
	balr	r15,r15	# MOVE	F:fr7,F:fr4
	cal	r15,L81-_Arc(r14)
	balr	r15,r15	# MUL	F:fr5,F:fr2
	cal	r15,L163-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:16(sp),F:fr1
	cal	r15,L83-_Arc(r14)
	balr	r15,r15	# ADD	F:fr5,F:fr0
	cal	r15,L84-_Arc(r14)
	balr	r15,r15	# MUL	F:fr4,F:fr2
	cal	r15,L164-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr3,F:fr1
	cal	r15,L86-_Arc(r14)
	balr	r15,r15	# ADD	F:fr4,F:fr0
	cal	r15,L87-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:16(sp),F:fr2
	cal	r15,L143-_Arc(r14)
	balr	r15,r15	# MUL	F:fr6,F:fr1
	cal	r15,L165-_Arc(r14)
	balr	r15,r15	# SUB	F:16(sp),F:fr0,F:fr6
	cal	r15,L146-_Arc(r14)
	balr	r15,r15	# MUL	F:fr3,F:fr2
	cal	r15,L166-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr7,F:fr1
	cal	r15,L40-_Arc(r14)
	balr	r15,r15	# SUB	F:fr3,F:fr0
	cal	r15,L148-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr2,F:20(sp)
	cal	r15,L167-_Arc(r14)
	balr	r15,r15	# MUL	F:fr7,F:fr1,F:20(sp)
	cal	r15,L168-_Arc(r14)
	balr	r15,r15	# MUL	F:4(sp),F:fr5,F:fr0
	cal	r15,L152-_Arc(r14)
	balr	r15,r15	# MUL	F:fr6,F:16(sp),F:fr7
	cal	r15,L169-_Arc(r14)
	balr	r15,r15	# ADD	F:fr6,F:4(sp)
	cal	r15,L154-_Arc(r14)
	balr	r15,r15	# ADD	F:fr6,F:28(sp)
	cal	r15,L155-_Arc(r14)
	balr	r15,r15	# MOVE	F:56(sp),F:fr6
	cal	r15,L156-_Arc(r14)
	balr	r15,r15	# MUL	F:fr6,F:fr4,F:fr0
	cal	r15,L157-_Arc(r14)
	balr	r15,r15	# MUL	F:fr7,F:fr3
	cal	r15,L170-_Arc(r14)
	balr	r15,r15	# ADD	F:fr6,F:fr7
	cal	r15,L159-_Arc(r14)
	balr	r15,r15	# ADD	F:fr6,F:24(sp)
	cal	r15,L160-_Arc(r14)
	balr	r15,r15	# MOVE	F:60(sp),F:fr6
L018728:
	cal	r15,L171-_Arc(r14)
	balr	r15,r15	# MUL	F:fr6,F:4.000000e+00,F:20(sp)
	cal	r15,L172-_Arc(r14)
	balr	r15,r15	# SUB	F:fr0,F:fr6,F:fr0
	cal	r15,L173-_Arc(r14)
	balr	r15,r15	# DIV	F:fr0,F:3.000000e+00
	cal	r15,L174-_Arc(r14)
	balr	r15,r15	# DIV	F:fr6,F:20(sp),F:fr2
	cal	r15,L96-_Arc(r14)
	balr	r15,r15	# SUB	F:fr6,F:fr0
	cal	r15,L175-_Arc(r14)
	balr	r15,r15	# MUL	F:fr1,F:fr6
	cal	r15,L176-_Arc(r14)
	balr	r15,r15	# DIV	F:fr6,F:fr1,F:fr2
	cal	r15,L177-_Arc(r14)
	balr	r15,r15	# MUL	F:fr2,F:fr5,F:fr0
	cal	r15,L178-_Arc(r14)
	balr	r15,r15	# MUL	F:fr1,F:16(sp),F:fr6
	cal	r15,L179-_Arc(r14)
	balr	r15,r15	# SUB	F:fr5,F:fr2,F:fr1
	cal	r15,L180-_Arc(r14)
	balr	r15,r15	# ADD	F:fr5,F:28(sp)
	cal	r15,L181-_Arc(r14)
	balr	r15,r15	# MOVE	F:72(sp),F:fr5
	cal	r15,L182-_Arc(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr4
	cal	r15,L183-_Arc(r14)
	balr	r15,r15	# MUL	F:fr4,F:fr3,F:fr6
	cal	r15,L184-_Arc(r14)
	balr	r15,r15	# SUB	F:fr3,F:fr0,F:fr4
	cal	r15,L185-_Arc(r14)
	balr	r15,r15	# ADD	F:fr3,F:24(sp)
	cal	r15,L186-_Arc(r14)
	balr	r15,r15	# MOVE	F:76(sp),F:fr3
	cal	r15,L187-_Arc(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr2
	cal	r15,L188-_Arc(r14)
	balr	r15,r15	# ADD	F:fr1,F:28(sp)
	cal	r15,L111-_Arc(r14)
	balr	r15,r15	# MOVE	F:64(sp),F:fr1
	cal	r15,L189-_Arc(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr4
	cal	r15,L107-_Arc(r14)
	balr	r15,r15	# ADD	F:fr0,F:24(sp)
	cal	r15,L113-_Arc(r14)
	balr	r15,r15	# MOVE	F:68(sp),F:fr0
	l	r11,_free_pelem-_Arc(r14)
	cis	r11,0
	jeq	L018730
	ls	r0,28(r11)
	bx	L018731
	st	r0,_free_pelem-_Arc(r14)
L018730:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_Arc(r14)
	mr	r11,r2
L018731:
	sts	r13,0(r11)
	cal	r2,4(r11)
	cal	r3,72(sp)	# P1
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,12(r11)
	cal	r3,64(sp)	# P2
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,20(r11)
	cal	r3,56(sp)	# P3
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	ls	r2,0(r12)
	cal	r2,36(r2)
	cal	r3,56(sp)	# P3
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r11,0
	jne	L018733
	cal	r2,L8-_Arc(r14)
	balix	r15,_.Panic
	l	r0,LF2-_Arc(r14)
L018733:
	sts	r8,28(r11)
	ls	r2,32(r8)
	sts	r2,32(r11)
	ls	r2,32(r8)
	sts	r11,28(r2)
	sts	r11,32(r8)
	lis	r2,1
	cal	r15,L128-_Arc(r14)
	balr	r15,r15	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,80(sp)
	lm	r8,152(sp)
	brx	r15
	cal	sp,220(sp)
L018737:
	bx	L018717
	l	r10,_free_pelem-_Arc(r14)
	.byte	0xdf,0x07,0xdf	#Trace table for Arc
	.byte	0x8a	# First nonvolatile register saved is r8
	.byte	0xa1	# npars=10 words
	.byte	0x3f	# rtfl regs saved
	.byte	0x37	# lcl_offset=220
	.align	1
	.data
	.align	2
L47:	# STOREM	<fr2/fr3/fr4/fr5/fr6/fr7>,80(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1d,2
	.short	LSI47
	.byte	92
	.byte	LSF47
	.short	0xff3f	# opd
	.short	0xff01	# opd
	.long	80
	.space	72
	.align	2
L48:	# MOVE	F:20(sp),D:220(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI48
	.byte	80
	.byte	LSF48
	.short	0x3201	# opd
	.short	0x3301	# opd
	.long	20
	.long	220
	.space	56
	.align	2
L49:	# MOVE	F:fr2,D:228(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI49
	.byte	96
	.byte	LSF49
	.short	0x1202	# opd
	.short	0x3301	# opd
	.long	228
	.space	76
	.align	2
L50:	# MOVE	F:fr0,D:236(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI50
	.byte	96
	.byte	LSF50
	.short	0x1200	# opd
	.short	0x3301	# opd
	.long	236
	.space	76
	.align	2
L51:	# SUB	F:fr6,F:fr0,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI51
	.byte	48
	.byte	LSF51
	.short	0x1206	# opd
	.short	0x1200	# opd
	.short	0x1202	# opd
	.align	2
	.space	28
	.align	2
L52:	# SUB	F:fr6,F:fr2,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI52
	.byte	48
	.byte	LSF52
	.short	0x1206	# opd
	.short	0x1202	# opd
	.short	0x1200	# opd
	.align	2
	.space	28
	.align	2
L53:	# MOVE	F:12(sp),F:0.000000e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI53
	.byte	92
	.byte	LSF53
	.short	0x3201	# opd
	.short	0x2200	# opd
	.long	12
	.long	0x00000000	# 0
	.space	68
	.align	2
L54:	# CMPT	F:fr6,F:12(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI54
	.byte	156
	.byte	LSF54
	.short	0x1206	# opd
	.short	0x3201	# opd
	.long	12
	.space	136
	.align	2
L55:	# MOVE	F:fr0,F:6.283185e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI55
	.byte	108
	.byte	LSF55
	.short	0x1200	# opd
	.short	0x2200	# opd
	.long	0x40c90fdb	# 6.28319
	.space	88
	.align	2
L56:	# ADD	F:fr6,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI56
	.byte	36
	.byte	LSF56
	.short	0x1206	# opd
	.short	0x1200	# opd
	.space	20
	.align	2
L57:	# MOVE	F:fr1,F:212(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI57
	.byte	108
	.byte	LSF57
	.short	0x1201	# opd
	.short	0x3201	# opd
	.long	212
	.space	88
	.align	2
L58:	# MOVE	F:fr5,F:32(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI58
	.byte	108
	.byte	LSF58
	.short	0x1205	# opd
	.short	0x3201	# opd
	.long	32
	.space	88
	.align	2
L59:	# MUL	F:fr4,F:fr5,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI59
	.byte	48
	.byte	LSF59
	.short	0x1204	# opd
	.short	0x1205	# opd
	.short	0x1201	# opd
	.align	2
	.space	28
	.align	2
L60:	# MOVE	F:fr0,F:216(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI60
	.byte	108
	.byte	LSF60
	.short	0x1200	# opd
	.short	0x3201	# opd
	.long	216
	.space	88
	.align	2
L61:	# MOVE	F:16(sp),F:40(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI61
	.byte	92
	.byte	LSF61
	.short	0x3201	# opd
	.short	0x3201	# opd
	.long	16
	.long	40
	.space	68
	.align	2
L62:	# MUL	F:fr3,F:16(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI62
	.byte	76
	.byte	LSF62
	.short	0x1203	# opd
	.short	0x3201	# opd
	.short	0x1200	# opd
	.align	2
	.long	16
	.space	52
	.align	2
L63:	# ADD	F:fr3,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI63
	.byte	36
	.byte	LSF63
	.short	0x1203	# opd
	.short	0x1204	# opd
	.space	20
	.align	2
L64:	# ADD	F:28(sp),F:48(sp),F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI64
	.byte	104
	.byte	LSF64
	.short	0x3201	# opd
	.short	0x3201	# opd
	.short	0x1203	# opd
	.align	2
	.long	28
	.long	48
	.space	76
	.align	2
L65:	# MOVE	F:fr4,F:36(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI65
	.byte	108
	.byte	LSF65
	.short	0x1204	# opd
	.short	0x3201	# opd
	.long	36
	.space	88
	.align	2
L66:	# MUL	F:fr1,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI66
	.byte	36
	.byte	LSF66
	.short	0x1201	# opd
	.short	0x1204	# opd
	.space	20
	.align	2
L67:	# MOVE	F:fr3,F:44(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI67
	.byte	108
	.byte	LSF67
	.short	0x1203	# opd
	.short	0x3201	# opd
	.long	44
	.space	88
	.align	2
L68:	# MUL	F:fr0,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI68
	.byte	36
	.byte	LSF68
	.short	0x1200	# opd
	.short	0x1203	# opd
	.space	20
	.align	2
L69:	# ADD	F:fr0,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI69
	.byte	36
	.byte	LSF69
	.short	0x1200	# opd
	.short	0x1201	# opd
	.space	20
	.align	2
L70:	# ADD	F:24(sp),F:52(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI70
	.byte	104
	.byte	LSF70
	.short	0x3201	# opd
	.short	0x3201	# opd
	.short	0x1200	# opd
	.align	2
	.long	24
	.long	52
	.space	76
	.align	2
L71:	# CMP	F:fr2,F:12(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x07,2
	.short	LSI71
	.byte	132
	.byte	LSF71
	.short	0x1202	# opd
	.short	0x3201	# opd
	.long	12
	.space	112
	.align	2
L72:	# MOVE	D:fr7,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI72
	.byte	52
	.byte	LSF72
	.short	0x1307	# opd
	.short	0x1202	# opd
	.space	36
	.align	2
L73:	# MOVE	D:<r2,r3>,D:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI73
	.byte	68
	.byte	LSF73
	.short	0x0323	# opd
	.short	0x1307	# opd
	.space	52
	.align	2
L74:	# MOVE	D:fr0,D:<r2,r3>
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI74
	.byte	84
	.byte	LSF74
	.short	0x1300	# opd
	.short	0x0323	# opd
	.space	68
	.align	2
L75:	# MOVE	F:fr2,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI75
	.byte	24
	.byte	LSF75
	.short	0x1202	# opd
	.short	0x1300	# opd
	.space	8
	.align	2
L76:	# MOVE	D:<r2,r3>,D:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI76
	.byte	68
	.byte	LSF76
	.short	0x0323	# opd
	.short	0x1307	# opd
	.space	52
	.align	2
L77:	# MOVE	D:fr0,D:<r2,r3>
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI77
	.byte	84
	.byte	LSF77
	.short	0x1300	# opd
	.short	0x0323	# opd
	.space	68
	.align	2
L78:	# MOVE	F:fr7,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI78
	.byte	52
	.byte	LSF78
	.short	0x1207	# opd
	.short	0x1300	# opd
	.space	36
	.align	2
L79:	# MOVE	F:fr1,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI79
	.byte	36
	.byte	LSF79
	.short	0x1201	# opd
	.short	0x1205	# opd
	.space	20
	.align	2
L80:	# MOVE	F:8(sp),F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI80
	.byte	64
	.byte	LSF80
	.short	0x3201	# opd
	.short	0x1204	# opd
	.long	8
	.space	44
	.align	2
L81:	# MUL	F:fr5,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI81
	.byte	36
	.byte	LSF81
	.short	0x1205	# opd
	.short	0x1202	# opd
	.space	20
	.align	2
L82:	# MUL	F:fr0,F:16(sp),F:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI82
	.byte	104
	.byte	LSF82
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x1207	# opd
	.align	2
	.long	16
	.space	80
	.align	2
L83:	# ADD	F:fr5,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI83
	.byte	36
	.byte	LSF83
	.short	0x1205	# opd
	.short	0x1200	# opd
	.space	20
	.align	2
L84:	# MUL	F:fr4,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI84
	.byte	36
	.byte	LSF84
	.short	0x1204	# opd
	.short	0x1202	# opd
	.space	20
	.align	2
L85:	# MUL	F:fr0,F:fr3,F:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI85
	.byte	76
	.byte	LSF85
	.short	0x1200	# opd
	.short	0x1203	# opd
	.short	0x1207	# opd
	.align	2
	.space	56
	.align	2
L86:	# ADD	F:fr4,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI86
	.byte	36
	.byte	LSF86
	.short	0x1204	# opd
	.short	0x1200	# opd
	.space	20
	.align	2
L87:	# MUL	F:fr0,F:16(sp),F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI87
	.byte	76
	.byte	LSF87
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x1202	# opd
	.align	2
	.long	16
	.space	52
	.align	2
L88:	# MUL	F:fr1,F:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI88
	.byte	108
	.byte	LSF88
	.short	0x1201	# opd
	.short	0x1207	# opd
	.space	92
	.align	2
L89:	# SUB	F:16(sp),F:fr0,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI89
	.byte	76
	.byte	LSF89
	.short	0x3201	# opd
	.short	0x1200	# opd
	.short	0x1201	# opd
	.align	2
	.long	16
	.space	52
	.align	2
L90:	# MUL	F:fr0,F:fr3,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI90
	.byte	48
	.byte	LSF90
	.short	0x1200	# opd
	.short	0x1203	# opd
	.short	0x1202	# opd
	.align	2
	.space	28
	.align	2
L91:	# MUL	F:fr1,F:8(sp),F:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI91
	.byte	104
	.byte	LSF91
	.short	0x1201	# opd
	.short	0x3201	# opd
	.short	0x1207	# opd
	.align	2
	.long	8
	.space	80
	.align	2
L92:	# SUB	F:fr3,F:fr0,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI92
	.byte	48
	.byte	LSF92
	.short	0x1203	# opd
	.short	0x1200	# opd
	.short	0x1201	# opd
	.align	2
	.space	28
	.align	2
L93:	# DIV	F:fr0,F:fr6,F:1.570786e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI93
	.byte	76
	.byte	LSF93
	.short	0x1200	# opd
	.short	0x1206	# opd
	.short	0x2200	# opd
	.align	2
	.long	0x3fc90f87	# 1.57079
	.space	52
	.align	2
L94:	# MOVE	I:r11,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI94
	.byte	64
	.byte	LSF94
	.short	0x000b	# opd
	.short	0x1200	# opd
	.space	48
	.align	2
L95:	# MUL	F:fr0,I:r11,F:1.570796e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI95
	.byte	104
	.byte	LSF95
	.short	0x1200	# opd
	.short	0x000b	# opd
	.short	0x2200	# opd
	.align	2
	.long	0x3fc90fdb	# 1.5708
	.space	80
	.align	2
L96:	# SUB	F:fr6,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,2
	.short	LSI96
	.byte	36
	.byte	LSF96
	.short	0x1206	# opd
	.short	0x1200	# opd
	.space	20
	.align	2
L97:	# MUL	F:fr2,F:5.522847e-01,F:20(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI97
	.byte	104
	.byte	LSF97
	.short	0x1202	# opd
	.short	0x2200	# opd
	.short	0x3201	# opd
	.align	2
	.long	0x3f0d6288	# 0.552285
	.long	20
	.space	76
	.align	2
L98:	# MUL	F:fr0,F:fr5,F:20(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI98
	.byte	76
	.byte	LSF98
	.short	0x1200	# opd
	.short	0x1205	# opd
	.short	0x3201	# opd
	.align	2
	.long	20
	.space	52
	.align	2
L99:	# ADD	F:fr1,F:fr0,F:28(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI99
	.byte	76
	.byte	LSF99
	.short	0x1201	# opd
	.short	0x1200	# opd
	.short	0x3201	# opd
	.align	2
	.long	28
	.space	52
	.align	2
L100:	# MOVE	F:72(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI100
	.byte	64
	.byte	LSF100
	.short	0x3201	# opd
	.short	0x1200	# opd
	.long	72
	.space	44
	.align	2
L101:	# MUL	F:fr0,F:fr4,F:20(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI101
	.byte	76
	.byte	LSF101
	.short	0x1200	# opd
	.short	0x1204	# opd
	.short	0x3201	# opd
	.align	2
	.long	20
	.space	52
	.align	2
L102:	# ADD	F:fr1,F:fr0,F:24(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI102
	.byte	76
	.byte	LSF102
	.short	0x1201	# opd
	.short	0x1200	# opd
	.short	0x3201	# opd
	.align	2
	.long	24
	.space	52
	.align	2
L103:	# MOVE	F:76(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI103
	.byte	64
	.byte	LSF103
	.short	0x3201	# opd
	.short	0x1200	# opd
	.long	76
	.space	44
	.align	2
L104:	# MUL	F:fr0,F:16(sp),F:20(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI104
	.byte	104
	.byte	LSF104
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x3201	# opd
	.align	2
	.long	16
	.long	20
	.space	76
	.align	2
L105:	# MOVE	F:56(sp),F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI105
	.byte	64
	.byte	LSF105
	.short	0x3201	# opd
	.short	0x1201	# opd
	.long	56
	.space	44
	.align	2
L106:	# MUL	F:fr0,F:fr3,F:20(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI106
	.byte	76
	.byte	LSF106
	.short	0x1200	# opd
	.short	0x1203	# opd
	.short	0x3201	# opd
	.align	2
	.long	20
	.space	52
	.align	2
L107:	# ADD	F:fr0,F:24(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI107
	.byte	108
	.byte	LSF107
	.short	0x1200	# opd
	.short	0x3201	# opd
	.long	24
	.space	88
	.align	2
L108:	# MOVE	F:60(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI108
	.byte	64
	.byte	LSF108
	.short	0x3201	# opd
	.short	0x1200	# opd
	.long	60
	.space	44
	.align	2
L109:	# MUL	F:fr7,F:fr5,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI109
	.byte	76
	.byte	LSF109
	.short	0x1207	# opd
	.short	0x1205	# opd
	.short	0x1202	# opd
	.align	2
	.space	56
	.align	2
L110:	# ADD	F:fr1,F:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI110
	.byte	108
	.byte	LSF110
	.short	0x1201	# opd
	.short	0x1207	# opd
	.space	92
	.align	2
L111:	# MOVE	F:64(sp),F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI111
	.byte	64
	.byte	LSF111
	.short	0x3201	# opd
	.short	0x1201	# opd
	.long	64
	.space	44
	.align	2
L112:	# MUL	F:fr1,F:fr4,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI112
	.byte	48
	.byte	LSF112
	.short	0x1201	# opd
	.short	0x1204	# opd
	.short	0x1202	# opd
	.align	2
	.space	28
	.align	2
L113:	# MOVE	F:68(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI113
	.byte	64
	.byte	LSF113
	.short	0x3201	# opd
	.short	0x1200	# opd
	.long	68
	.space	44
	.align	2
L114:	# MOVE	F:fr0,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI114
	.byte	36
	.byte	LSF114
	.short	0x1200	# opd
	.short	0x1205	# opd
	.space	20
	.align	2
L115:	# MOVE	F:fr5,F:16(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI115
	.byte	108
	.byte	LSF115
	.short	0x1205	# opd
	.short	0x3201	# opd
	.long	16
	.space	88
	.align	2
L116:	# NEG	F:fr0,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x16,2
	.short	LSI116
	.byte	36
	.byte	LSF116
	.short	0x1200	# opd
	.short	0x1200	# opd
	.space	20
	.align	2
L117:	# MOVE	F:16(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI117
	.byte	64
	.byte	LSF117
	.short	0x3201	# opd
	.short	0x1200	# opd
	.long	16
	.space	44
	.align	2
L118:	# MOVE	F:fr0,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI118
	.byte	36
	.byte	LSF118
	.short	0x1200	# opd
	.short	0x1204	# opd
	.space	20
	.align	2
L119:	# MOVE	F:fr4,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI119
	.byte	36
	.byte	LSF119
	.short	0x1204	# opd
	.short	0x1203	# opd
	.space	20
	.align	2
L120:	# NEG	F:fr3,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x16,2
	.short	LSI120
	.byte	36
	.byte	LSF120
	.short	0x1203	# opd
	.short	0x1200	# opd
	.space	20
	.align	2
L121:	# SUB	F:fr0,F:fr1,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI121
	.byte	48
	.byte	LSF121
	.short	0x1200	# opd
	.short	0x1201	# opd
	.short	0x1200	# opd
	.align	2
	.space	28
	.align	2
L122:	# SUB	F:fr1,F:28(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI122
	.byte	76
	.byte	LSF122
	.short	0x1201	# opd
	.short	0x3201	# opd
	.short	0x1200	# opd
	.align	2
	.long	28
	.space	52
	.align	2
L123:	# SUB	F:fr0,F:24(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI123
	.byte	76
	.byte	LSF123
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x1200	# opd
	.align	2
	.long	24
	.space	52
	.align	2
L124:	# NEG	F:fr5,F:16(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x16,2
	.short	LSI124
	.byte	108
	.byte	LSF124
	.short	0x1205	# opd
	.short	0x3201	# opd
	.long	16
	.space	88
	.align	2
L125:	# NEG	F:fr4,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x16,2
	.short	LSI125
	.byte	36
	.byte	LSF125
	.short	0x1204	# opd
	.short	0x1203	# opd
	.space	20
	.align	2
L126:	# MOVE	F:fr3,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI126
	.byte	36
	.byte	LSF126
	.short	0x1203	# opd
	.short	0x1200	# opd
	.space	20
	.align	2
L127:	# CMPT	F:fr6,F:12(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI127
	.byte	156
	.byte	LSF127
	.short	0x1206	# opd
	.short	0x3201	# opd
	.long	12
	.space	136
	.align	2
L128:	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,80(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0f,2
	.short	LSI128
	.byte	116
	.byte	LSF128
	.short	0xff3f	# opd
	.short	0xff01	# opd
	.long	80
	.space	96
	.align	2
L129:	# DIV	F:fr0,F:fr6,F:2.000000e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI129
	.byte	76
	.byte	LSF129
	.short	0x1200	# opd
	.short	0x1206	# opd
	.short	0x2200	# opd
	.align	2
	.long	0x40000000	# 2
	.space	52
	.align	2
L130:	# MOVE	D:fr6,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI130
	.byte	24
	.byte	LSF130
	.short	0x1306	# opd
	.short	0x1200	# opd
	.space	8
	.align	2
L131:	# MOVE	D:<r2,r3>,D:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI131
	.byte	40
	.byte	LSF131
	.short	0x0323	# opd
	.short	0x1306	# opd
	.space	24
	.align	2
L132:	# MOVE	D:<r2,r3>,D:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI132
	.byte	40
	.byte	LSF132
	.short	0x0323	# opd
	.short	0x1306	# opd
	.space	24
	.align	2
L133:	# MOVE	F:fr1,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI133
	.byte	24
	.byte	LSF133
	.short	0x1201	# opd
	.short	0x1300	# opd
	.space	8
	.align	2
L134:	# MOVE	F:fr6,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI134
	.byte	36
	.byte	LSF134
	.short	0x1206	# opd
	.short	0x1205	# opd
	.space	20
	.align	2
L135:	# MOVE	F:fr0,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI135
	.byte	36
	.byte	LSF135
	.short	0x1200	# opd
	.short	0x1204	# opd
	.space	20
	.align	2
L136:	# MUL	F:fr5,F:16(sp),F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI136
	.byte	76
	.byte	LSF136
	.short	0x1205	# opd
	.short	0x3201	# opd
	.short	0x1201	# opd
	.align	2
	.long	16
	.space	52
	.align	2
L137:	# SUB	F:fr5,F:fr7,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI137
	.byte	76
	.byte	LSF137
	.short	0x1205	# opd
	.short	0x1207	# opd
	.short	0x1205	# opd
	.align	2
	.space	56
	.align	2
L138:	# MUL	F:fr7,F:fr4,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI138
	.byte	76
	.byte	LSF138
	.short	0x1207	# opd
	.short	0x1204	# opd
	.short	0x1202	# opd
	.align	2
	.space	56
	.align	2
L139:	# MUL	F:fr4,F:fr3,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI139
	.byte	48
	.byte	LSF139
	.short	0x1204	# opd
	.short	0x1203	# opd
	.short	0x1201	# opd
	.align	2
	.space	28
	.align	2
L140:	# SUB	F:fr4,F:fr7,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI140
	.byte	76
	.byte	LSF140
	.short	0x1204	# opd
	.short	0x1207	# opd
	.short	0x1204	# opd
	.align	2
	.space	56
	.align	2
L141:	# NEG	F:fr7,F:16(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x16,2
	.short	LSI141
	.byte	92
	.byte	LSF141
	.short	0x1207	# opd
	.short	0x3201	# opd
	.long	16
	.space	72
	.align	2
L142:	# MUL	F:fr7,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI142
	.byte	64
	.byte	LSF142
	.short	0x1207	# opd
	.short	0x1202	# opd
	.space	48
	.align	2
L143:	# MUL	F:fr6,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI143
	.byte	36
	.byte	LSF143
	.short	0x1206	# opd
	.short	0x1201	# opd
	.space	20
	.align	2
L144:	# SUB	F:16(sp),F:fr7,F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI144
	.byte	104
	.byte	LSF144
	.short	0x3201	# opd
	.short	0x1207	# opd
	.short	0x1206	# opd
	.align	2
	.long	16
	.space	80
	.align	2
L145:	# NEG	F:fr3,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x16,2
	.short	LSI145
	.byte	36
	.byte	LSF145
	.short	0x1203	# opd
	.short	0x1203	# opd
	.space	20
	.align	2
L146:	# MUL	F:fr3,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI146
	.byte	36
	.byte	LSF146
	.short	0x1203	# opd
	.short	0x1202	# opd
	.space	20
	.align	2
L147:	# MUL	F:fr0,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI147
	.byte	36
	.byte	LSF147
	.short	0x1200	# opd
	.short	0x1201	# opd
	.space	20
	.align	2
L148:	# MUL	F:fr0,F:fr2,F:20(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI148
	.byte	76
	.byte	LSF148
	.short	0x1200	# opd
	.short	0x1202	# opd
	.short	0x3201	# opd
	.align	2
	.long	20
	.space	52
	.align	2
L149:	# NEG	F:fr6,F:20(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x16,2
	.short	LSI149
	.byte	108
	.byte	LSF149
	.short	0x1206	# opd
	.short	0x3201	# opd
	.long	20
	.space	88
	.align	2
L150:	# MUL	F:fr7,F:fr6,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI150
	.byte	76
	.byte	LSF150
	.short	0x1207	# opd
	.short	0x1206	# opd
	.short	0x1201	# opd
	.align	2
	.space	56
	.align	2
L151:	# MUL	F:0(sp),F:fr5,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI151
	.byte	76
	.byte	LSF151
	.short	0x3201	# opd
	.short	0x1205	# opd
	.short	0x1200	# opd
	.align	2
	.long	0
	.space	52
	.align	2
L152:	# MUL	F:fr6,F:16(sp),F:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI152
	.byte	104
	.byte	LSF152
	.short	0x1206	# opd
	.short	0x3201	# opd
	.short	0x1207	# opd
	.align	2
	.long	16
	.space	80
	.align	2
L153:	# SUB	F:fr6,F:0(sp),F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI153
	.byte	76
	.byte	LSF153
	.short	0x1206	# opd
	.short	0x3201	# opd
	.short	0x1206	# opd
	.align	2
	.long	0
	.space	52
	.align	2
L154:	# ADD	F:fr6,F:28(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI154
	.byte	108
	.byte	LSF154
	.short	0x1206	# opd
	.short	0x3201	# opd
	.long	28
	.space	88
	.align	2
L155:	# MOVE	F:56(sp),F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI155
	.byte	64
	.byte	LSF155
	.short	0x3201	# opd
	.short	0x1206	# opd
	.long	56
	.space	44
	.align	2
L156:	# MUL	F:fr6,F:fr4,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI156
	.byte	48
	.byte	LSF156
	.short	0x1206	# opd
	.short	0x1204	# opd
	.short	0x1200	# opd
	.align	2
	.space	28
	.align	2
L157:	# MUL	F:fr7,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI157
	.byte	64
	.byte	LSF157
	.short	0x1207	# opd
	.short	0x1203	# opd
	.space	48
	.align	2
L158:	# SUB	F:fr6,F:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,2
	.short	LSI158
	.byte	108
	.byte	LSF158
	.short	0x1206	# opd
	.short	0x1207	# opd
	.space	92
	.align	2
L159:	# ADD	F:fr6,F:24(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI159
	.byte	108
	.byte	LSF159
	.short	0x1206	# opd
	.short	0x3201	# opd
	.long	24
	.space	88
	.align	2
L160:	# MOVE	F:60(sp),F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI160
	.byte	64
	.byte	LSF160
	.short	0x3201	# opd
	.short	0x1206	# opd
	.long	60
	.space	44
	.align	2
L161:	# MOVE	F:fr6,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI161
	.byte	36
	.byte	LSF161
	.short	0x1206	# opd
	.short	0x1205	# opd
	.space	20
	.align	2
L162:	# MOVE	F:fr7,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI162
	.byte	64
	.byte	LSF162
	.short	0x1207	# opd
	.short	0x1204	# opd
	.space	48
	.align	2
L163:	# MUL	F:fr0,F:16(sp),F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI163
	.byte	76
	.byte	LSF163
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x1201	# opd
	.align	2
	.long	16
	.space	52
	.align	2
L164:	# MUL	F:fr0,F:fr3,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI164
	.byte	48
	.byte	LSF164
	.short	0x1200	# opd
	.short	0x1203	# opd
	.short	0x1201	# opd
	.align	2
	.space	28
	.align	2
L165:	# SUB	F:16(sp),F:fr0,F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI165
	.byte	76
	.byte	LSF165
	.short	0x3201	# opd
	.short	0x1200	# opd
	.short	0x1206	# opd
	.align	2
	.long	16
	.space	52
	.align	2
L166:	# MUL	F:fr0,F:fr7,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI166
	.byte	76
	.byte	LSF166
	.short	0x1200	# opd
	.short	0x1207	# opd
	.short	0x1201	# opd
	.align	2
	.space	56
	.align	2
L167:	# MUL	F:fr7,F:fr1,F:20(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI167
	.byte	104
	.byte	LSF167
	.short	0x1207	# opd
	.short	0x1201	# opd
	.short	0x3201	# opd
	.align	2
	.long	20
	.space	80
	.align	2
L168:	# MUL	F:4(sp),F:fr5,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI168
	.byte	76
	.byte	LSF168
	.short	0x3201	# opd
	.short	0x1205	# opd
	.short	0x1200	# opd
	.align	2
	.long	4
	.space	52
	.align	2
L169:	# ADD	F:fr6,F:4(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI169
	.byte	108
	.byte	LSF169
	.short	0x1206	# opd
	.short	0x3201	# opd
	.long	4
	.space	88
	.align	2
L170:	# ADD	F:fr6,F:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI170
	.byte	108
	.byte	LSF170
	.short	0x1206	# opd
	.short	0x1207	# opd
	.space	92
	.align	2
L171:	# MUL	F:fr6,F:4.000000e+00,F:20(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI171
	.byte	104
	.byte	LSF171
	.short	0x1206	# opd
	.short	0x2200	# opd
	.short	0x3201	# opd
	.align	2
	.long	0x40800000	# 4
	.long	20
	.space	76
	.align	2
L172:	# SUB	F:fr0,F:fr6,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI172
	.byte	48
	.byte	LSF172
	.short	0x1200	# opd
	.short	0x1206	# opd
	.short	0x1200	# opd
	.align	2
	.space	28
	.align	2
L173:	# DIV	F:fr0,F:3.000000e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,2
	.short	LSI173
	.byte	108
	.byte	LSF173
	.short	0x1200	# opd
	.short	0x2200	# opd
	.long	0x40400000	# 3
	.space	88
	.align	2
L174:	# DIV	F:fr6,F:20(sp),F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI174
	.byte	76
	.byte	LSF174
	.short	0x1206	# opd
	.short	0x3201	# opd
	.short	0x1202	# opd
	.align	2
	.long	20
	.space	52
	.align	2
L175:	# MUL	F:fr1,F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI175
	.byte	36
	.byte	LSF175
	.short	0x1201	# opd
	.short	0x1206	# opd
	.space	20
	.align	2
L176:	# DIV	F:fr6,F:fr1,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI176
	.byte	48
	.byte	LSF176
	.short	0x1206	# opd
	.short	0x1201	# opd
	.short	0x1202	# opd
	.align	2
	.space	28
	.align	2
L177:	# MUL	F:fr2,F:fr5,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI177
	.byte	48
	.byte	LSF177
	.short	0x1202	# opd
	.short	0x1205	# opd
	.short	0x1200	# opd
	.align	2
	.space	28
	.align	2
L178:	# MUL	F:fr1,F:16(sp),F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI178
	.byte	76
	.byte	LSF178
	.short	0x1201	# opd
	.short	0x3201	# opd
	.short	0x1206	# opd
	.align	2
	.long	16
	.space	52
	.align	2
L179:	# SUB	F:fr5,F:fr2,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI179
	.byte	48
	.byte	LSF179
	.short	0x1205	# opd
	.short	0x1202	# opd
	.short	0x1201	# opd
	.align	2
	.space	28
	.align	2
L180:	# ADD	F:fr5,F:28(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI180
	.byte	108
	.byte	LSF180
	.short	0x1205	# opd
	.short	0x3201	# opd
	.long	28
	.space	88
	.align	2
L181:	# MOVE	F:72(sp),F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI181
	.byte	64
	.byte	LSF181
	.short	0x3201	# opd
	.short	0x1205	# opd
	.long	72
	.space	44
	.align	2
L182:	# MUL	F:fr0,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI182
	.byte	36
	.byte	LSF182
	.short	0x1200	# opd
	.short	0x1204	# opd
	.space	20
	.align	2
L183:	# MUL	F:fr4,F:fr3,F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI183
	.byte	48
	.byte	LSF183
	.short	0x1204	# opd
	.short	0x1203	# opd
	.short	0x1206	# opd
	.align	2
	.space	28
	.align	2
L184:	# SUB	F:fr3,F:fr0,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI184
	.byte	48
	.byte	LSF184
	.short	0x1203	# opd
	.short	0x1200	# opd
	.short	0x1204	# opd
	.align	2
	.space	28
	.align	2
L185:	# ADD	F:fr3,F:24(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI185
	.byte	108
	.byte	LSF185
	.short	0x1203	# opd
	.short	0x3201	# opd
	.long	24
	.space	88
	.align	2
L186:	# MOVE	F:76(sp),F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI186
	.byte	64
	.byte	LSF186
	.short	0x3201	# opd
	.short	0x1203	# opd
	.long	76
	.space	44
	.align	2
L187:	# ADD	F:fr1,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI187
	.byte	36
	.byte	LSF187
	.short	0x1201	# opd
	.short	0x1202	# opd
	.space	20
	.align	2
L188:	# ADD	F:fr1,F:28(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI188
	.byte	108
	.byte	LSF188
	.short	0x1201	# opd
	.short	0x3201	# opd
	.long	28
	.space	88
	.align	2
L189:	# ADD	F:fr0,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI189
	.byte	36
	.byte	LSF189
	.short	0x1200	# opd
	.short	0x1204	# opd
	.space	20
	.text
	.data
	.align	2
_ArcTo:	.long	_.ArcTo
	.text
	.globl	_.ArcTo
	.globl	_ArcTo
	.text
#--------------| ArcTo |-----------------------#
_.ArcTo:
	.using	_ArcTo,r14
	stm	r8,-68(sp)
	mr	r14,r0
	cal	sp,-248(sp)
	cal	r15,L190-_ArcTo(r14)
	balr	r15,r15	# STOREM	<fr2/fr3/fr4/fr5/fr6/fr7>,108(sp)
	st	r5,244(sp)	# x1
	mr	r8,r2
	cal	r15,L191-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr3,D:244(sp)
	cal	r15,L192-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:24(sp),D:252(sp)
	cal	r15,L193-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr5,D:260(sp)
	cal	r15,L194-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr4,D:268(sp)
	cal	r15,L195-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:76(sp),D:276(sp)
	cal	r2,100(sp)	# p0
	.data
	.align	2
LF5:	.long	_IntToExt
	.text
	balix	r15,_.IntToExt
	l	r0,LF5-_ArcTo(r14)
	cal	r15,L196-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr2,F:100(sp)
	cal	r15,L197-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:56(sp),F:104(sp)
	cal	r15,L198-_ArcTo(r14)
	balr	r15,r15	# SUB	F:fr1,F:fr4,F:24(sp)
	cal	r15,L199-_ArcTo(r14)
	balr	r15,r15	# SUB	F:fr0,F:fr5,F:fr3
	cal	r15,L200-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,F:fr1
	cal	r15,L201-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r4,r5>,F:fr0
	.data
	.align	2
LF6:	.long	_atan2
	.text
	balix	r15,_.atan2
	l	r0,LF6-_ArcTo(r14)
	cal	r15,L77-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L202-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr4,D:fr0
	cal	r15,L203-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr5,F:0.000000e+00
	cal	r15,L204-_ArcTo(r14)
	balr	r15,r15	# CMPT	F:fr4,F:fr5
	bh	L018819
	cal	r15,L205-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr6,F:6.283185e+00
	cal	r15,L206-_ArcTo(r14)
	balr	r15,r15	# ADD	F:fr4,F:fr6
L01883:
	cal	r15,L207-_ArcTo(r14)
	balr	r15,r15	# SUB	F:fr1,F:56(sp),F:24(sp)
	cal	r15,L208-_ArcTo(r14)
	balr	r15,r15	# SUB	F:fr0,F:fr2,F:fr3
	cal	r15,L200-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,F:fr1
	cal	r15,L201-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r4,r5>,F:fr0
	balix	r15,_.atan2
	l	r0,LF6-_ArcTo(r14)
	cal	r15,L74-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L75-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr2,D:fr0
	cal	r15,L209-_ArcTo(r14)
	balr	r15,r15	# CMPT	F:fr2,F:fr5
	jh	L01885
	cal	r15,L210-_ArcTo(r14)
	balr	r15,r15	# ADD	F:fr2,F:fr6
L01885:
	cal	r15,L211-_ArcTo(r14)
	balr	r15,r15	# SUB	F:fr7,F:fr4,F:fr2
	cal	r15,L212-_ArcTo(r14)
	balr	r15,r15	# CMPT	F:fr7,F:fr5
	jh	L01887
	cal	r15,L213-_ArcTo(r14)
	balr	r15,r15	# ADD	F:fr7,F:fr6
L01887:
	cal	r15,L214-_ArcTo(r14)
	balr	r15,r15	# CMPT	F:fr7,F:fr6
	jl	L01889
	cal	r15,L215-_ArcTo(r14)
	balr	r15,r15	# SUB	F:fr7,F:fr6
L01889:
	cal	r15,L216-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr4,F:3.141593e+00
	cal	r15,L217-_ArcTo(r14)
	balr	r15,r15	# CMPT	F:fr7,F:fr4
	jl	L018811
	cal	r15,L215-_ArcTo(r14)
	balr	r15,r15	# SUB	F:fr7,F:fr6
L018811:
	cal	r15,L212-_ArcTo(r14)
	balr	r15,r15	# CMPT	F:fr7,F:fr5
	jh	L018813
	lis	r10,1
	bx	L018814
	mr	r9,r10
L018813:
	cal	r9,-1(r0)
	lis	r10,1
L018814:
	cal	r15,L218-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:80(sp),F:2.000000e+00
	cal	r15,L219-_ArcTo(r14)
	balr	r15,r15	# DIV	F:64(sp),F:fr7,F:80(sp)
	cal	r15,L220-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,F:64(sp)
	balix	r15,_.sin
	l	r0,LF4-_ArcTo(r14)
	cal	r15,L74-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L221-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr6,F:76(sp)
	cal	r15,L222-_ArcTo(r14)
	balr	r15,r15	# DIV	D:fr0,D:fr6,D:fr0
	cal	r15,L223-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:fr0
	.data
	.align	2
LF7:	.long	_fabs
	.text
	balix	r15,_.fabs
	l	r0,LF7-_ArcTo(r14)
	cal	r15,L77-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L224-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:52(sp),D:fr0
	cal	r15,L225-_ArcTo(r14)
	balr	r15,r15	# ADD	F:fr2,F:64(sp)
	cal	r15,L226-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:68(sp),F:52(sp)
	cal	r15,L227-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:28(sp),F:fr2
	cal	r15,L228-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:28(sp)
	balix	r15,_.cos
	l	r0,LF3-_ArcTo(r14)
	sts	r2,36(sp)
	sts	r3,40(sp)
	cal	r15,L229-_ArcTo(r14)
	balr	r15,r15	# MUL	D:44(sp),D:68(sp),D:36(sp)
	cal	r15,L230-_ArcTo(r14)
	balr	r15,r15	# ADD	D:fr0,F:fr3,D:44(sp)
	cal	r15,L231-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:60(sp),D:fr0
	cal	r15,L232-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:28(sp)
	balix	r15,_.sin
	l	r0,LF4-_ArcTo(r14)
	cal	r15,L77-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L233-_ArcTo(r14)
	balr	r15,r15	# MUL	D:fr0,D:68(sp)
	cal	r15,L234-_ArcTo(r14)
	balr	r15,r15	# ADD	D:fr0,F:24(sp)
	cal	r15,L235-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr3,D:fr0
	cal	r15,L236-_ArcTo(r14)
	balr	r15,r15	# ADD	F:fr2,F:fr4
	cal	r15,L212-_ArcTo(r14)
	balr	r15,r15	# CMPT	F:fr7,F:fr5
	jle	L018816
	cal	r15,L237-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr4,F:-3.141593e+00
	j	L018817
L018816:
L018817:
	cal	r15,L238-_ArcTo(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr4,F:fr7
	cal	r15,L239-_ArcTo(r14)
	balr	r15,r15	# DIV	F:fr7,F:fr0,F:80(sp)
	cal	r15,L240-_ArcTo(r14)
	balr	r15,r15	# SUB	F:fr0,F:fr2,F:fr7
	cal	r15,L241-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr5,F:fr0
	cal	r15,L242-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:fr5
	balix	r15,_.cos
	l	r0,LF3-_ArcTo(r14)
	cal	r15,L74-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L243-_ArcTo(r14)
	balr	r15,r15	# MUL	D:fr0,D:fr6
	cal	r15,L244-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr4,F:60(sp)
	cal	r15,L245-_ArcTo(r14)
	balr	r15,r15	# ADD	D:fr0,D:fr4
	cal	r15,L246-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr0,D:fr0
	l	r11,284(sp)	# xt1
	cal	r15,L247-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:0(r11),F:fr0
	cal	r15,L248-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:fr5
	balix	r15,_.sin
	l	r0,LF4-_ArcTo(r14)
	cal	r15,L249-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L243-_ArcTo(r14)
	balr	r15,r15	# MUL	D:fr0,D:fr6
	cal	r15,L250-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr3,F:fr3
	cal	r15,L251-_ArcTo(r14)
	balr	r15,r15	# ADD	D:fr0,D:fr3
	cal	r15,L246-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr0,D:fr0
	l	r12,288(sp)	# yt1
	cal	r15,L252-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:0(r12),F:fr0
	cal	r15,L253-_ArcTo(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr7,F:fr2
	cal	r15,L254-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr2,F:fr0
	cal	r15,L255-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:fr2
	balix	r15,_.cos
	l	r0,LF3-_ArcTo(r14)
	cal	r15,L249-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L243-_ArcTo(r14)
	balr	r15,r15	# MUL	D:fr0,D:fr6
	cal	r15,L245-_ArcTo(r14)
	balr	r15,r15	# ADD	D:fr0,D:fr4
	cal	r15,L246-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr0,D:fr0
	l	r2,292(sp)	# xt2
	cal	r15,L256-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:0(r2),F:fr0
	cal	r15,L257-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,D:fr2
	balix	r15,_.sin
	l	r0,LF4-_ArcTo(r14)
	cal	r15,L249-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L243-_ArcTo(r14)
	balr	r15,r15	# MUL	D:fr0,D:fr6
	cal	r15,L251-_ArcTo(r14)
	balr	r15,r15	# ADD	D:fr0,D:fr3
	cal	r15,L246-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr0,D:fr0
	l	r2,296(sp)	# yt2
	cal	r15,L256-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:0(r2),F:fr0
	cal	r13,92(sp)
	mr	r2,r13
	cal	r15,L258-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r3,r4>,F:0(r11)
	cal	r15,L259-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:-4(sp),F:0(r12)
	l	r5,-4(sp)
	.data
	.align	2
LF8:	.long	_NewUserPoint
	.text
	balix	r15,_.NewUserPoint
	l	r0,LF8-_ArcTo(r14)
	l	r5,96(sp)
	l	r4,92(sp)
	cal	r2,84(sp)
	mr	r3,r4
	mr	r4,r5
	.data
	.align	2
LF9:	.long	_ExtToInt
	.text
	balix	r15,_.ExtToInt
	l	r0,LF9-_ArcTo(r14)
	l	r4,88(sp)
	l	r3,84(sp)
	mr	r2,r8
	.data
	.align	2
LF10:	.long	_LineTo
	.text
	balix	r15,_.LineTo
	l	r0,LF10-_ArcTo(r14)
	mr	r2,r13
	cal	r15,L260-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:<r3,r4>,D:fr4
	cal	r15,L261-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:-4(sp),D:fr3
	l	r5,-4(sp)
	balix	r15,_.NewUserPoint
	l	r0,LF8-_ArcTo(r14)
	l	r5,96(sp)
	l	r4,92(sp)
	cal	r15,L262-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:0(sp),D:fr6
	cal	r15,L263-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:8(sp),D:fr5
	cal	r15,L264-_ArcTo(r14)
	balr	r15,r15	# MOVE	D:16(sp),D:fr2
	mr	r2,r8
	mr	r3,r9
	.data
	.align	2
LF11:	.long	_Arc
	.text
	balix	r15,_.Arc
	l	r0,LF11-_ArcTo(r14)
	mr	r2,r10
	cal	r15,L265-_ArcTo(r14)
	balr	r15,r15	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,108(sp)
	lm	r8,180(sp)
	brx	r15
	cal	sp,248(sp)
L018819:
	cal	r15,L266-_ArcTo(r14)
	balr	r15,r15	# MOVE	F:fr6,F:6.283185e+00
	b	L01883
	.byte	0xdf,0x07,0xdf	#Trace table for ArcTo
	.byte	0x8a	# First nonvolatile register saved is r8
	.byte	0x111	# npars=17 words
	.byte	0x3f	# rtfl regs saved
	.byte	0x3e	# lcl_offset=248
	.align	1
	.data
	.align	2
L190:	# STOREM	<fr2/fr3/fr4/fr5/fr6/fr7>,108(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1d,2
	.short	LSI190
	.byte	92
	.byte	LSF190
	.short	0xff3f	# opd
	.short	0xff01	# opd
	.long	108
	.space	72
	.align	2
L191:	# MOVE	F:fr3,D:244(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI191
	.byte	96
	.byte	LSF191
	.short	0x1203	# opd
	.short	0x3301	# opd
	.long	244
	.space	76
	.align	2
L192:	# MOVE	F:24(sp),D:252(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI192
	.byte	80
	.byte	LSF192
	.short	0x3201	# opd
	.short	0x3301	# opd
	.long	24
	.long	252
	.space	56
	.align	2
L193:	# MOVE	F:fr5,D:260(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI193
	.byte	96
	.byte	LSF193
	.short	0x1205	# opd
	.short	0x3301	# opd
	.long	260
	.space	76
	.align	2
L194:	# MOVE	F:fr4,D:268(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI194
	.byte	96
	.byte	LSF194
	.short	0x1204	# opd
	.short	0x3301	# opd
	.long	268
	.space	76
	.align	2
L195:	# MOVE	F:76(sp),D:276(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI195
	.byte	80
	.byte	LSF195
	.short	0x3201	# opd
	.short	0x3301	# opd
	.long	76
	.long	276
	.space	56
	.align	2
L196:	# MOVE	F:fr2,F:100(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI196
	.byte	108
	.byte	LSF196
	.short	0x1202	# opd
	.short	0x3201	# opd
	.long	100
	.space	88
	.align	2
L197:	# MOVE	F:56(sp),F:104(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI197
	.byte	92
	.byte	LSF197
	.short	0x3201	# opd
	.short	0x3201	# opd
	.long	56
	.long	104
	.space	68
	.align	2
L198:	# SUB	F:fr1,F:fr4,F:24(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI198
	.byte	76
	.byte	LSF198
	.short	0x1201	# opd
	.short	0x1204	# opd
	.short	0x3201	# opd
	.align	2
	.long	24
	.space	52
	.align	2
L199:	# SUB	F:fr0,F:fr5,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI199
	.byte	48
	.byte	LSF199
	.short	0x1200	# opd
	.short	0x1205	# opd
	.short	0x1203	# opd
	.align	2
	.space	28
	.align	2
L200:	# MOVE	D:<r2,r3>,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI200
	.byte	52
	.byte	LSF200
	.short	0x0323	# opd
	.short	0x1201	# opd
	.space	36
	.align	2
L201:	# MOVE	D:<r4,r5>,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI201
	.byte	52
	.byte	LSF201
	.short	0x0345	# opd
	.short	0x1200	# opd
	.space	36
	.align	2
L202:	# MOVE	F:fr4,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI202
	.byte	24
	.byte	LSF202
	.short	0x1204	# opd
	.short	0x1300	# opd
	.space	8
	.align	2
L203:	# MOVE	F:fr5,F:0.000000e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI203
	.byte	108
	.byte	LSF203
	.short	0x1205	# opd
	.short	0x2200	# opd
	.long	0x00000000	# 0
	.space	88
	.align	2
L204:	# CMPT	F:fr4,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI204
	.byte	84
	.byte	LSF204
	.short	0x1204	# opd
	.short	0x1205	# opd
	.space	68
	.align	2
L205:	# MOVE	F:fr6,F:6.283185e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI205
	.byte	108
	.byte	LSF205
	.short	0x1206	# opd
	.short	0x2200	# opd
	.long	0x40c90fdb	# 6.28319
	.space	88
	.align	2
L206:	# ADD	F:fr4,F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI206
	.byte	36
	.byte	LSF206
	.short	0x1204	# opd
	.short	0x1206	# opd
	.space	20
	.align	2
L207:	# SUB	F:fr1,F:56(sp),F:24(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI207
	.byte	104
	.byte	LSF207
	.short	0x1201	# opd
	.short	0x3201	# opd
	.short	0x3201	# opd
	.align	2
	.long	56
	.long	24
	.space	76
	.align	2
L208:	# SUB	F:fr0,F:fr2,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI208
	.byte	48
	.byte	LSF208
	.short	0x1200	# opd
	.short	0x1202	# opd
	.short	0x1203	# opd
	.align	2
	.space	28
	.align	2
L209:	# CMPT	F:fr2,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI209
	.byte	84
	.byte	LSF209
	.short	0x1202	# opd
	.short	0x1205	# opd
	.space	68
	.align	2
L210:	# ADD	F:fr2,F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI210
	.byte	36
	.byte	LSF210
	.short	0x1202	# opd
	.short	0x1206	# opd
	.space	20
	.align	2
L211:	# SUB	F:fr7,F:fr4,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI211
	.byte	76
	.byte	LSF211
	.short	0x1207	# opd
	.short	0x1204	# opd
	.short	0x1202	# opd
	.align	2
	.space	56
	.align	2
L212:	# CMPT	F:fr7,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI212
	.byte	112
	.byte	LSF212
	.short	0x1207	# opd
	.short	0x1205	# opd
	.space	96
	.align	2
L213:	# ADD	F:fr7,F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI213
	.byte	64
	.byte	LSF213
	.short	0x1207	# opd
	.short	0x1206	# opd
	.space	48
	.align	2
L214:	# CMPT	F:fr7,F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI214
	.byte	112
	.byte	LSF214
	.short	0x1207	# opd
	.short	0x1206	# opd
	.space	96
	.align	2
L215:	# SUB	F:fr7,F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,2
	.short	LSI215
	.byte	64
	.byte	LSF215
	.short	0x1207	# opd
	.short	0x1206	# opd
	.space	48
	.align	2
L216:	# MOVE	F:fr4,F:3.141593e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI216
	.byte	108
	.byte	LSF216
	.short	0x1204	# opd
	.short	0x2200	# opd
	.long	0x40490fdb	# 3.14159
	.space	88
	.align	2
L217:	# CMPT	F:fr7,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI217
	.byte	112
	.byte	LSF217
	.short	0x1207	# opd
	.short	0x1204	# opd
	.space	96
	.align	2
L218:	# MOVE	F:80(sp),F:2.000000e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI218
	.byte	92
	.byte	LSF218
	.short	0x3201	# opd
	.short	0x2200	# opd
	.long	80
	.long	0x40000000	# 2
	.space	68
	.align	2
L219:	# DIV	F:64(sp),F:fr7,F:80(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI219
	.byte	132
	.byte	LSF219
	.short	0x3201	# opd
	.short	0x1207	# opd
	.short	0x3201	# opd
	.align	2
	.long	64
	.long	80
	.space	104
	.align	2
L220:	# MOVE	D:<r2,r3>,F:64(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI220
	.byte	80
	.byte	LSF220
	.short	0x0323	# opd
	.short	0x3201	# opd
	.long	64
	.space	60
	.align	2
L221:	# MOVE	D:fr6,F:76(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI221
	.byte	96
	.byte	LSF221
	.short	0x1306	# opd
	.short	0x3201	# opd
	.long	76
	.space	76
	.align	2
L222:	# DIV	D:fr0,D:fr6,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI222
	.byte	20
	.byte	LSF222
	.short	0x1300	# opd
	.short	0x1306	# opd
	.short	0x1300	# opd
	.align	2
	.align	2
L223:	# MOVE	D:<r2,r3>,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI223
	.byte	40
	.byte	LSF223
	.short	0x0323	# opd
	.short	0x1300	# opd
	.space	24
	.align	2
L224:	# MOVE	F:52(sp),D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI224
	.byte	52
	.byte	LSF224
	.short	0x3201	# opd
	.short	0x1300	# opd
	.long	52
	.space	32
	.align	2
L225:	# ADD	F:fr2,F:64(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI225
	.byte	108
	.byte	LSF225
	.short	0x1202	# opd
	.short	0x3201	# opd
	.long	64
	.space	88
	.align	2
L226:	# MOVE	D:68(sp),F:52(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI226
	.byte	80
	.byte	LSF226
	.short	0x3301	# opd
	.short	0x3201	# opd
	.long	68
	.long	52
	.space	56
	.align	2
L227:	# MOVE	D:28(sp),F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI227
	.byte	52
	.byte	LSF227
	.short	0x3301	# opd
	.short	0x1202	# opd
	.long	28
	.space	32
	.align	2
L228:	# MOVE	D:<r2,r3>,D:28(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI228
	.byte	68
	.byte	LSF228
	.short	0x0323	# opd
	.short	0x3301	# opd
	.long	28
	.space	48
	.align	2
L229:	# MUL	D:44(sp),D:68(sp),D:36(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI229
	.byte	96
	.byte	LSF229
	.short	0x3301	# opd
	.short	0x3301	# opd
	.short	0x3301	# opd
	.align	2
	.long	44
	.long	68
	.long	36
	.space	64
	.align	2
L230:	# ADD	D:fr0,F:fr3,D:44(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI230
	.byte	52
	.byte	LSF230
	.short	0x1300	# opd
	.short	0x1203	# opd
	.short	0x3301	# opd
	.align	2
	.long	44
	.space	28
	.align	2
L231:	# MOVE	F:60(sp),D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI231
	.byte	52
	.byte	LSF231
	.short	0x3201	# opd
	.short	0x1300	# opd
	.long	60
	.space	32
	.align	2
L232:	# MOVE	D:<r2,r3>,D:28(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI232
	.byte	68
	.byte	LSF232
	.short	0x0323	# opd
	.short	0x3301	# opd
	.long	28
	.space	48
	.align	2
L233:	# MUL	D:fr0,D:68(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI233
	.byte	84
	.byte	LSF233
	.short	0x1300	# opd
	.short	0x3301	# opd
	.long	68
	.space	64
	.align	2
L234:	# ADD	D:fr0,F:24(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI234
	.byte	96
	.byte	LSF234
	.short	0x1300	# opd
	.short	0x3201	# opd
	.long	24
	.space	76
	.align	2
L235:	# MOVE	F:fr3,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI235
	.byte	24
	.byte	LSF235
	.short	0x1203	# opd
	.short	0x1300	# opd
	.space	8
	.align	2
L236:	# ADD	F:fr2,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI236
	.byte	36
	.byte	LSF236
	.short	0x1202	# opd
	.short	0x1204	# opd
	.space	20
	.align	2
L237:	# MOVE	F:fr4,F:-3.141593e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI237
	.byte	108
	.byte	LSF237
	.short	0x1204	# opd
	.short	0x2200	# opd
	.long	0xc0490fdb	# -3.14159
	.space	88
	.align	2
L238:	# ADD	F:fr0,F:fr4,F:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI238
	.byte	76
	.byte	LSF238
	.short	0x1200	# opd
	.short	0x1204	# opd
	.short	0x1207	# opd
	.align	2
	.space	56
	.align	2
L239:	# DIV	F:fr7,F:fr0,F:80(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI239
	.byte	104
	.byte	LSF239
	.short	0x1207	# opd
	.short	0x1200	# opd
	.short	0x3201	# opd
	.align	2
	.long	80
	.space	80
	.align	2
L240:	# SUB	F:fr0,F:fr2,F:fr7
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI240
	.byte	76
	.byte	LSF240
	.short	0x1200	# opd
	.short	0x1202	# opd
	.short	0x1207	# opd
	.align	2
	.space	56
	.align	2
L241:	# MOVE	D:fr5,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI241
	.byte	24
	.byte	LSF241
	.short	0x1305	# opd
	.short	0x1200	# opd
	.space	8
	.align	2
L242:	# MOVE	D:<r2,r3>,D:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI242
	.byte	40
	.byte	LSF242
	.short	0x0323	# opd
	.short	0x1305	# opd
	.space	24
	.align	2
L243:	# MUL	D:fr0,D:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI243
	.byte	16
	.byte	LSF243
	.short	0x1300	# opd
	.short	0x1306	# opd
	.align	2
L244:	# MOVE	D:fr4,F:60(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI244
	.byte	96
	.byte	LSF244
	.short	0x1304	# opd
	.short	0x3201	# opd
	.long	60
	.space	76
	.align	2
L245:	# ADD	D:fr0,D:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI245
	.byte	16
	.byte	LSF245
	.short	0x1300	# opd
	.short	0x1304	# opd
	.align	2
L246:	# MOVE	F:fr0,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI246
	.byte	24
	.byte	LSF246
	.short	0x1200	# opd
	.short	0x1300	# opd
	.space	8
	.align	2
L247:	# MOVE	F:0(r11),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI247
	.byte	64
	.byte	LSF247
	.short	0x320b	# opd
	.short	0x1200	# opd
	.long	0
	.space	44
	.align	2
L248:	# MOVE	D:<r2,r3>,D:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI248
	.byte	40
	.byte	LSF248
	.short	0x0323	# opd
	.short	0x1305	# opd
	.space	24
	.align	2
L249:	# MOVE	D:fr0,D:<r2,r3>
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI249
	.byte	84
	.byte	LSF249
	.short	0x1300	# opd
	.short	0x0323	# opd
	.space	68
	.align	2
L250:	# MOVE	D:fr3,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI250
	.byte	24
	.byte	LSF250
	.short	0x1303	# opd
	.short	0x1203	# opd
	.space	8
	.align	2
L251:	# ADD	D:fr0,D:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI251
	.byte	16
	.byte	LSF251
	.short	0x1300	# opd
	.short	0x1303	# opd
	.align	2
L252:	# MOVE	F:0(r12),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI252
	.byte	64
	.byte	LSF252
	.short	0x320c	# opd
	.short	0x1200	# opd
	.long	0
	.space	44
	.align	2
L253:	# ADD	F:fr0,F:fr7,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI253
	.byte	76
	.byte	LSF253
	.short	0x1200	# opd
	.short	0x1207	# opd
	.short	0x1202	# opd
	.align	2
	.space	56
	.align	2
L254:	# MOVE	D:fr2,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI254
	.byte	24
	.byte	LSF254
	.short	0x1302	# opd
	.short	0x1200	# opd
	.space	8
	.align	2
L255:	# MOVE	D:<r2,r3>,D:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI255
	.byte	40
	.byte	LSF255
	.short	0x0323	# opd
	.short	0x1302	# opd
	.space	24
	.align	2
L256:	# MOVE	F:0(r2),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI256
	.byte	64
	.byte	LSF256
	.short	0x3202	# opd
	.short	0x1200	# opd
	.long	0
	.space	44
	.align	2
L257:	# MOVE	D:<r2,r3>,D:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI257
	.byte	40
	.byte	LSF257
	.short	0x0323	# opd
	.short	0x1302	# opd
	.space	24
	.align	2
L258:	# MOVE	D:<r3,r4>,F:0(r11)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI258
	.byte	80
	.byte	LSF258
	.short	0x0334	# opd
	.short	0x320b	# opd
	.long	0
	.space	60
	.align	2
L259:	# MOVE	D:-4(sp),F:0(r12)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI259
	.byte	80
	.byte	LSF259
	.short	0x3301	# opd
	.short	0x320c	# opd
	.long	-4
	.long	0
	.space	56
	.align	2
L260:	# MOVE	D:<r3,r4>,D:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI260
	.byte	40
	.byte	LSF260
	.short	0x0334	# opd
	.short	0x1304	# opd
	.space	24
	.align	2
L261:	# MOVE	D:-4(sp),D:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI261
	.byte	40
	.byte	LSF261
	.short	0x3301	# opd
	.short	0x1303	# opd
	.long	-4
	.space	20
	.align	2
L262:	# MOVE	D:0(sp),D:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI262
	.byte	40
	.byte	LSF262
	.short	0x3301	# opd
	.short	0x1306	# opd
	.long	0
	.space	20
	.align	2
L263:	# MOVE	D:8(sp),D:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI263
	.byte	40
	.byte	LSF263
	.short	0x3301	# opd
	.short	0x1305	# opd
	.long	8
	.space	20
	.align	2
L264:	# MOVE	D:16(sp),D:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI264
	.byte	40
	.byte	LSF264
	.short	0x3301	# opd
	.short	0x1302	# opd
	.long	16
	.space	20
	.align	2
L265:	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,108(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0f,2
	.short	LSI265
	.byte	116
	.byte	LSF265
	.short	0xff3f	# opd
	.short	0xff01	# opd
	.long	108
	.space	96
	.align	2
L266:	# MOVE	F:fr6,F:6.283185e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI266
	.byte	108
	.byte	LSF266
	.short	0x1206	# opd
	.short	0x2200	# opd
	.long	0x40c90fdb	# 6.28319
	.space	88
	.text
	.data
	.align	2
_ClosePath:	.long	_.ClosePath
	.text
	.data
	.align	2
L9:	.ascii	"PathInsert given NULL\0"
	.align	2
L10:	.ascii	"PathInsert given NULL\0"
	.text
	.globl	_.ClosePath
	.globl	_ClosePath
	.text
#--------------| ClosePath |-----------------------#
_.ClosePath:
	stm	r10,-60(sp)
	mr	r14,r0
	cal	sp,-60(sp)
	mr	r10,r2
	lda	r13,_gstate
	ls	r2,0(r13)
	ls	r0,44(r2)
	cis	r0,0
	jne	L01893
	lis	r2,1
	lm	r10,0(sp)
	brx	r15
	cal	sp,60(sp)
L01893:
	l	r12,_free_pelem-_ClosePath(r14)
	cis	r12,0
	jeq	L01895
	ls	r0,28(r12)
	bx	L01896
	st	r0,_free_pelem-_ClosePath(r14)
L01895:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_ClosePath(r14)
	mr	r12,r2
	ls	r2,0(r13)
L01896:
	lis	r0,5
	sts	r0,0(r12)
	ls	r0,16(r2)
	sts	r0,4(r12)
	ls	r0,20(r2)
	sts	r0,8(r12)
	mr	r11,r10
	ls	r0,0(r11)
	cis	r0,1
	jeq	L01898
L01897:
	ls	r11,32(r11)
	ls	r0,0(r11)
	cis	r0,1
	jne	L01897
	mr	r0,r0	# no op to fix APC bug
	mr	r0,r0	# no op to fix APC bug
	mr	r0,r0	# no op to fix APC bug
L01898:
	cis	r12,0
	jne	L018910
	cal	r2,L9-_ClosePath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_ClosePath(r14)
L018910:
	l	r0,_free_pelem-_ClosePath(r14)
	sts	r10,28(r12)
	ls	r2,32(r10)
	sts	r2,32(r12)
	ls	r2,32(r10)
	sts	r12,28(r2)
	sts	r12,32(r10)
	mr	r12,r0
	cis	r12,0
	jeq	L018912
	ls	r0,28(r12)
	bx	L018913
	st	r0,_free_pelem-_ClosePath(r14)
L018912:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_ClosePath(r14)
	mr	r12,r2
L018913:
	ls	r4,0(r13)
	lis	r13,1
	sts	r13,0(r12)
	cal	r2,4(r12)
	cal	r3,4(r11)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,36(r4)
	cal	r3,4(r12)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r12,0
	jne	L018915
	cal	r2,L10-_ClosePath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_ClosePath(r14)
L018915:
	sts	r10,28(r12)
	ls	r2,32(r10)
	sts	r2,32(r12)
	ls	r2,32(r10)
	sts	r12,28(r2)
	sts	r12,32(r10)
	mr	r2,r13
	lm	r10,0(sp)
	brx	r15
	cal	sp,60(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for ClosePath
	.byte	0xa8	# First nonvolatile register saved is r10
	.byte	0x11	# npars=1 words
	.byte	0xf	# lcl_offset=60
	.align	1
	.data
	.align	2
_NewUserPoint:	.long	_.NewUserPoint
	.text
	.globl	_.NewUserPoint
	.globl	_NewUserPoint
	.text
#--------------| NewUserPoint |-----------------------#
_.NewUserPoint:
	.using	_NewUserPoint,r14
	stm	r14,-44(sp)
	mr	r14,r0
	cal	sp,-52(sp)
	sts	r5,48(sp)	# y
	cal	r15,L267-_NewUserPoint(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r3,r4>
	cal	r15,L268-_NewUserPoint(r14)
	balr	r15,r15	# MOVE	F:fr1,D:fr0
	cal	r15,L269-_NewUserPoint(r14)
	balr	r15,r15	# MOVE	F:fr0,D:48(sp)
	cal	r15,L270-_NewUserPoint(r14)
	balr	r15,r15	# MOVE	F:0(sp),F:fr1
	cal	r15,L271-_NewUserPoint(r14)
	balr	r15,r15	# MOVE	F:4(sp),F:fr0
	mr	r3,sp
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	lm	r14,8(sp)
	brx	r15
	cal	sp,52(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for NewUserPoint
	.byte	0xe8	# First nonvolatile register saved is r14
	.byte	0x51	# npars=5 words
	.byte	0xd	# lcl_offset=52
	.align	1
	.data
	.align	2
L267:	# MOVE	D:fr0,D:<r3,r4>
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI267
	.byte	84
	.byte	LSF267
	.short	0x1300	# opd
	.short	0x0334	# opd
	.space	68
	.align	2
L268:	# MOVE	F:fr1,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI268
	.byte	24
	.byte	LSF268
	.short	0x1201	# opd
	.short	0x1300	# opd
	.space	8
	.align	2
L269:	# MOVE	F:fr0,D:48(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI269
	.byte	96
	.byte	LSF269
	.short	0x1200	# opd
	.short	0x3301	# opd
	.long	48
	.space	76
	.align	2
L270:	# MOVE	F:0(sp),F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI270
	.byte	64
	.byte	LSF270
	.short	0x3201	# opd
	.short	0x1201	# opd
	.long	0
	.space	44
	.align	2
L271:	# MOVE	F:4(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI271
	.byte	64
	.byte	LSF271
	.short	0x3201	# opd
	.short	0x1200	# opd
	.long	4
	.space	44
	.text
	.data
	.align	2
_NewHardPoint:	.long	_.NewHardPoint
	.text
	.globl	_.NewHardPoint
	.globl	_NewHardPoint
	.text
#--------------| NewHardPoint |-----------------------#
_.NewHardPoint:
	.using	_NewHardPoint,r14
	stm	r14,-44(sp)
	mr	r14,r0
	cal	sp,-52(sp)
	sts	r5,48(sp)	# y
	cal	r15,L267-_NewHardPoint(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r3,r4>
	cal	r15,L268-_NewHardPoint(r14)
	balr	r15,r15	# MOVE	F:fr1,D:fr0
	cal	r15,L269-_NewHardPoint(r14)
	balr	r15,r15	# MOVE	F:fr0,D:48+LUA0178(sp)
	cal	r15,L270-_NewHardPoint(r14)
	balr	r15,r15	# MOVE	F:0+LUF0178(sp),F:fr1
	cal	r15,L271-_NewHardPoint(r14)
	balr	r15,r15	# MOVE	F:4+LUF0178(sp),F:fr0
	mr	r3,sp
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	lm	r14,8(sp)
	brx	r15
	cal	sp,52(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for NewHardPoint
	.byte	0xe8	# First nonvolatile register saved is r14
	.byte	0x51	# npars=5 words
	.byte	0xd	# lcl_offset=52
	.align	1
	.data
	.align	2
_MoveHardPoint:	.long	_.MoveHardPoint
	.text
	.globl	_.MoveHardPoint
	.globl	_MoveHardPoint
	.text
#--------------| MoveHardPoint |-----------------------#
_.MoveHardPoint:
	.using	_MoveHardPoint,r14
	stm	r13,-48(sp)
	mr	r14,r0
	cal	sp,-60(sp)
	mr	r13,r2
	cal	r2,4(sp)
	cal	r15,L272-_MoveHardPoint(r14)
	balr	r15,r15	# MOVE	F:fr1,F:r3
	cal	r15,L273-_MoveHardPoint(r14)
	balr	r15,r15	# MOVE	F:fr0,F:r5
	cal	r15,L274-_MoveHardPoint(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr0
	cal	r15,L275-_MoveHardPoint(r14)
	balr	r15,r15	# MOVE	F:fr0,F:r4
	cal	r15,L276-_MoveHardPoint(r14)
	balr	r15,r15	# ADD	F:fr0,F:60(sp)
	cal	r15,L277-_MoveHardPoint(r14)
	balr	r15,r15	# MOVE	D:<r3,r4>,F:fr1
	cal	r15,L278-_MoveHardPoint(r14)
	balr	r15,r15	# MOVE	D:-4(sp),F:fr0
	l	r5,-4(sp)
	.data
	.align	2
LF12:	.long	_NewHardPoint
	.text
	balix	r15,_.NewHardPoint
	l	r0,LF12-_MoveHardPoint(r14)
	mr	r2,r13
	cal	r3,4(sp)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	lm	r13,12(sp)
	brx	r15
	cal	sp,60(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for MoveHardPoint
	.byte	0xd8	# First nonvolatile register saved is r13
	.byte	0x51	# npars=5 words
	.byte	0xf	# lcl_offset=60
	.align	1
	.data
	.align	2
L272:	# MOVE	F:fr1,F:r3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI272
	.byte	116
	.byte	LSF272
	.short	0x1201	# opd
	.short	0x0203	# opd
	.space	100
	.align	2
L273:	# MOVE	F:fr0,F:r5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI273
	.byte	112
	.byte	LSF273
	.short	0x1200	# opd
	.short	0x0205	# opd
	.space	96
	.align	2
L274:	# ADD	F:fr1,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI274
	.byte	40
	.byte	LSF274
	.short	0x1201	# opd
	.short	0x1200	# opd
	.space	24
	.align	2
L275:	# MOVE	F:fr0,F:r4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI275
	.byte	108
	.byte	LSF275
	.short	0x1200	# opd
	.short	0x0204	# opd
	.space	92
	.align	2
L276:	# ADD	F:fr0,F:60(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI276
	.byte	108
	.byte	LSF276
	.short	0x1200	# opd
	.short	0x3201	# opd
	.long	60
	.space	88
	.align	2
L277:	# MOVE	D:<r3,r4>,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI277
	.byte	52
	.byte	LSF277
	.short	0x0334	# opd
	.short	0x1201	# opd
	.space	36
	.align	2
L278:	# MOVE	D:-4(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI278
	.byte	60
	.byte	LSF278
	.short	0x3301	# opd
	.short	0x1200	# opd
	.long	-4
	.space	40
	.text
	.data
	.align	2
_MoveUserPoint:	.long	_.MoveUserPoint
	.text
	.globl	_.MoveUserPoint
	.globl	_MoveUserPoint
	.text
#--------------| MoveUserPoint |-----------------------#
_.MoveUserPoint:
	.using	_MoveUserPoint,r14
	stm	r13,-48(sp)
	mr	r14,r0
	cal	sp,-60(sp)
	mr	r13,r2
	cal	r2,4(sp)
	cal	r15,L272-_MoveUserPoint(r14)
	balr	r15,r15	# MOVE	F:fr1,F:r3
	cal	r15,L273-_MoveUserPoint(r14)
	balr	r15,r15	# MOVE	F:fr0,F:r5
	cal	r15,L274-_MoveUserPoint(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr0
	cal	r15,L275-_MoveUserPoint(r14)
	balr	r15,r15	# MOVE	F:fr0,F:r4
	cal	r15,L276-_MoveUserPoint(r14)
	balr	r15,r15	# ADD	F:fr0,F:60+LUA0133(sp)
	cal	r15,L277-_MoveUserPoint(r14)
	balr	r15,r15	# MOVE	D:<r3,r4>,F:fr1
	cal	r15,L278-_MoveUserPoint(r14)
	balr	r15,r15	# MOVE	D:-4(sp),F:fr0
	l	r5,-4(sp)
	balix	r15,_.NewUserPoint
	l	r0,LF8-_MoveUserPoint(r14)
	mr	r2,r13
	cal	r3,4(sp)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	lm	r13,12(sp)
	brx	r15
	cal	sp,60(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for MoveUserPoint
	.byte	0xd8	# First nonvolatile register saved is r13
	.byte	0x51	# npars=5 words
	.byte	0xf	# lcl_offset=60
	.align	1
	.data
	.align	2
_PointX:	.long	_.PointX
	.text
	.globl	_.PointX
	.globl	_PointX
	.text
#--------------| PointX |-----------------------#
_.PointX:
	.using	_PointX,r14
	stm	r14,-44(sp)
	mr	r14,r0
	cal	sp,-44(sp)
	cal	r15,L279-_PointX(r14)
	balr	r15,r15	# MOVE	F:fr0,F:r2
	cal	r15,L280-_PointX(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,F:fr0
	lm	r14,0(sp)
	brx	r15
	cal	sp,44(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for PointX
	.byte	0xe8	# First nonvolatile register saved is r14
	.byte	0x21	# npars=2 words
	.byte	0xb	# lcl_offset=44
	.align	1
	.data
	.align	2
L279:	# MOVE	F:fr0,F:r2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI279
	.byte	108
	.byte	LSF279
	.short	0x1200	# opd
	.short	0x0202	# opd
	.space	92
	.align	2
L280:	# MOVE	D:<r2,r3>,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI280
	.byte	52
	.byte	LSF280
	.short	0x0323	# opd
	.short	0x1200	# opd
	.space	36
	.text
	.data
	.align	2
_PointY:	.long	_.PointY
	.text
	.globl	_.PointY
	.globl	_PointY
	.text
#--------------| PointY |-----------------------#
_.PointY:
	.using	_PointY,r14
	stm	r14,-44(sp)
	mr	r14,r0
	cal	sp,-44(sp)
	cal	r15,L281-_PointY(r14)
	balr	r15,r15	# MOVE	F:fr0,F:r3
	cal	r15,L280-_PointY(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,F:fr0
	lm	r14,0(sp)
	brx	r15
	cal	sp,44(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for PointY
	.byte	0xe8	# First nonvolatile register saved is r14
	.byte	0x21	# npars=2 words
	.byte	0xb	# lcl_offset=44
	.align	1
	.data
	.align	2
L281:	# MOVE	F:fr0,F:r3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI281
	.byte	108
	.byte	LSF281
	.short	0x1200	# opd
	.short	0x0203	# opd
	.space	92
	.text
	.lcomm	L00BSS,0
	.lcomm	_path,4
	.data
	.align	2
_Bezier:	.long	_.Bezier
	.text
	.data
	.align	2
L11:	.ascii	"PathInsert given NULL\0"
	.align	2
L12:	.ascii	"PathInsert given NULL\0"
	.text
	.globl	_.Bezier
	.globl	_Bezier
	.text
#--------------| Bezier |-----------------------#
_.Bezier:
	.using	_Bezier,r14
	stm	r11,-56(sp)
	mr	r14,r0
	cal	sp,-196(sp)
	cal	r15,L282-_Bezier(r14)
	balr	r15,r15	# STOREM	<fr2/fr3/fr4/fr5/fr6/fr7>,68(sp)
	cal	r15,L283-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr3,F:r2
	cal	r15,L284-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr2,F:r3
	sts	r4,52(sp)
	sts	r5,56(sp)
	cal	r15,L285-_Bezier(r14)
	balr	r15,r15	# MOVE	F:40(sp),F:208(sp)
	cal	r15,L286-_Bezier(r14)
	balr	r15,r15	# SUB	F:fr5,F:40(sp),F:fr2
	cal	r15,L287-_Bezier(r14)
	balr	r15,r15	# MOVE	F:44(sp),F:204(sp)
	cal	r15,L288-_Bezier(r14)
	balr	r15,r15	# SUB	F:fr4,F:44(sp),F:fr3
	cal	r15,L289-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr0,F:0.000000e+00
	cal	r15,L290-_Bezier(r14)
	balr	r15,r15	# CMP	F:fr5,F:fr0
	jne	L01849
	cal	r15,L291-_Bezier(r14)
	balr	r15,r15	# CMP	F:fr4,F:fr0
	jne	L01849
	cal	r15,L292-_Bezier(r14)
	balr	r15,r15	# MOVE	F:60(sp),F:44(sp)
	cal	r15,L293-_Bezier(r14)
	balr	r15,r15	# MOVE	F:64(sp),F:40(sp)
	l	r13,_free_pelem-_Bezier(r14)
	cis	r13,0
	jeq	L01845
	ls	r0,28(r13)
	bx	L01846
	st	r0,_free_pelem-_Bezier(r14)
L01845:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_Bezier(r14)
	mr	r13,r2
L01846:
	lis	r0,2
	sts	r0,0(r13)
	cal	r2,4(r13)
	cal	r3,60(sp)	# P3
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	load	r2,_gstate
	cal	r2,36(r2)
	cal	r3,4(r13)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r13,0
	jne	L01848
	cal	r2,L11-_Bezier(r14)
	balix	r15,_.Panic
	l	r0,LF2-_Bezier(r14)
L01848:
	load	r3,_path
	sts	r3,28(r13)
	ls	r2,32(r3)
	sts	r2,32(r13)
	ls	r2,32(r3)
	sts	r13,28(r2)
	sts	r13,32(r3)
	lis	r2,1
	cal	r15,L294-_Bezier(r14)
	balr	r15,r15	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,68(sp)
	lm	r11,140(sp)
	brx	r15
	cal	sp,196(sp)
L01849:
	cal	r15,L295-_Bezier(r14)
	balr	r15,r15	# MUL	F:fr1,F:fr5,F:fr5
	cal	r15,L296-_Bezier(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr4,F:fr4
	cal	r15,L69-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr1
	cal	r15,L297-_Bezier(r14)
	balr	r15,r15	# MOVE	D:<r2,r3>,F:fr0
	.data
	.align	2
LF13:	.long	_sqrt
	.text
	balix	r15,_.sqrt
	l	r0,LF13-_Bezier(r14)
	cal	r15,L74-_Bezier(r14)
	balr	r15,r15	# MOVE	D:fr0,D:<r2,r3>
	cal	r15,L298-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr1,D:fr0
	lda	r12,_gstate
	ls	r4,0(r12)
	l	r2,140(r4)
	cal	r15,L299-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr0,I:r2
	cal	r15,L147-_Bezier(r14)
	balr	r15,r15	# MUL	F:fr0,F:fr1
	cal	r15,L300-_Bezier(r14)
	balr	r15,r15	# SUB	F:fr1,F:52(sp),F:fr3
	cal	r15,L301-_Bezier(r14)
	balr	r15,r15	# NEG	F:fr6,F:fr5
	cal	r15,L302-_Bezier(r14)
	balr	r15,r15	# MUL	F:fr5,F:fr6,F:fr1
	cal	r15,L303-_Bezier(r14)
	balr	r15,r15	# SUB	F:fr1,F:56(sp),F:fr2
	cal	r15,L66-_Bezier(r14)
	balr	r15,r15	# MUL	F:fr1,F:fr4
	cal	r15,L304-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr5
	cal	r15,L305-_Bezier(r14)
	balr	r15,r15	# NEG	F:fr5,F:fr0
	cal	r15,L306-_Bezier(r14)
	balr	r15,r15	# CMPT	F:fr1,F:fr5
	bl	L018423
	cal	r15,L307-_Bezier(r14)
	balr	r15,r15	# CMPT	F:fr1,F:fr0
	bh	L018424
	cal	r15,L308-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr7,F:196(sp)
	cal	r15,L309-_Bezier(r14)
	balr	r15,r15	# SUB	F:fr1,F:fr7,F:fr3
	cal	r15,L310-_Bezier(r14)
	balr	r15,r15	# MUL	F:28(sp),F:fr1,F:fr6
	cal	r15,L311-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr6,F:200(sp)
	cal	r15,L312-_Bezier(r14)
	balr	r15,r15	# SUB	F:fr1,F:fr6,F:fr2
	cal	r15,L66-_Bezier(r14)
	balr	r15,r15	# MUL	F:fr1,F:fr4
	cal	r15,L188-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr1,F:28(sp)
	cal	r15,L306-_Bezier(r14)
	balr	r15,r15	# CMPT	F:fr1,F:fr5
	jl	L018419
	cal	r15,L307-_Bezier(r14)
	balr	r15,r15	# CMPT	F:fr1,F:fr0
	jh	L018419
	cal	r15,L292-_Bezier(r14)
	balr	r15,r15	# MOVE	F:60(sp),F:44(sp)
	cal	r15,L293-_Bezier(r14)
	balr	r15,r15	# MOVE	F:64(sp),F:40(sp)
	l	r13,_free_pelem-_Bezier(r14)
	cis	r13,0
	jeq	L018415
	ls	r0,28(r13)
	bx	L018416
	st	r0,_free_pelem-_Bezier(r14)
L018415:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_Bezier(r14)
	mr	r13,r2
	ls	r4,0(r12)
L018416:
	lis	r0,2
	sts	r0,0(r13)
	cal	r2,4(r13)
	cal	r3,60(sp)	# P3
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,36(r4)
	cal	r3,4(r13)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r13,0
	jne	L018418
	cal	r2,L12-_Bezier(r14)
	balix	r15,_.Panic
	l	r0,LF2-_Bezier(r14)
L018418:
	load	r3,_path
	sts	r3,28(r13)
	ls	r2,32(r3)
	sts	r2,32(r13)
	ls	r2,32(r3)
	sts	r13,28(r2)
	sts	r13,32(r3)
	lis	r2,1
	cal	r15,L313-_Bezier(r14)
	balr	r15,r15	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,68(sp)
	lm	r11,140(sp)
	brx	r15
	cal	sp,196(sp)
L018419:
	cal	r15,L314-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:44(sp),F:fr3
	cal	r15,L315-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr4,F:8.000000e+00
	cal	r15,L316-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr0,F:fr4
	cal	r15,L317-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr1,F:3.000000e+00
	cal	r15,L318-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr5,F:fr7,F:52(sp)
	cal	r15,L319-_Bezier(r14)
	balr	r15,r15	# MUL	F:fr5,F:fr1
	cal	r15,L320-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr5,F:fr4
	cal	r15,L321-_Bezier(r14)
	balr	r15,r15	# ADD	F:48(sp),F:fr5,F:fr0
	cal	r15,L322-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:40(sp),F:fr2
	cal	r15,L316-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr0,F:fr4
	cal	r15,L323-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr5,F:fr6,F:56(sp)
	cal	r15,L324-_Bezier(r14)
	balr	r15,r15	# MUL	F:fr1,F:fr5
	cal	r15,L325-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr1,F:fr4
	cal	r15,L326-_Bezier(r14)
	balr	r15,r15	# ADD	F:16(sp),F:fr1,F:fr0
	cal	r15,L327-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:52(sp),F:fr3
	cal	r15,L328-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr5,F:2.000000e+00
	cal	r15,L329-_Bezier(r14)
	balr	r15,r15	# DIV	F:32(sp),F:fr0,F:fr5
	cal	r15,L330-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:56(sp),F:fr2
	cal	r15,L331-_Bezier(r14)
	balr	r15,r15	# DIV	F:36(sp),F:fr0,F:fr5
	cal	r15,L332-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr7,F:fr3
	cal	r15,L333-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr4,F:4.000000e+00
	cal	r15,L334-_Bezier(r14)
	balr	r15,r15	# DIV	F:24(sp),F:fr0,F:fr4
	cal	r15,L335-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr0,F:52(sp),F:fr5
	cal	r15,L107-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:24(sp)
	cal	r15,L336-_Bezier(r14)
	balr	r15,r15	# MOVE	F:0(sp),F:fr0
	cal	r15,L337-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr6,F:fr2
	cal	r15,L338-_Bezier(r14)
	balr	r15,r15	# DIV	F:20(sp),F:fr0,F:fr4
	cal	r15,L339-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr0,F:56(sp),F:fr5
	cal	r15,L340-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:20(sp)
	cal	r15,L341-_Bezier(r14)
	balr	r15,r15	# MOVE	F:4(sp),F:fr0
	cal	r15,L342-_Bezier(r14)
	balr	r15,r15	# MOVE	F:8(sp),F:48(sp)
	cal	r15,L343-_Bezier(r14)
	balr	r15,r15	# MOVE	F:12(sp),F:16(sp)
	cal	r15,L344-_Bezier(r14)
	balr	r15,r15	# MOVE	F:r2,F:fr3
	cal	r15,L345-_Bezier(r14)
	balr	r15,r15	# MOVE	F:r3,F:fr2
	ls	r5,36(sp)
	ls	r4,32(sp)
	.data
	.align	2
LF14:	.long	_Bezier
	.text
	balix	r15,_.Bezier
	l	r0,LF14-_Bezier(r14)
	cis	r2,0
	jne	L018421
	lis	r2,0
	cal	r15,L313-_Bezier(r14)
	balr	r15,r15	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,68(sp)
	lm	r11,140(sp)
	brx	r15
	cal	sp,196(sp)
L018421:
	cal	r15,L346-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:44(sp),F:52(sp)
	cal	r15,L347-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr1,F:fr0,F:fr4
	cal	r15,L348-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr0,F:fr7,F:fr5
	cal	r15,L274-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr1,F:fr0
	cal	r15,L349-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:40(sp),F:56(sp)
	cal	r15,L350-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr2,F:fr0,F:fr4
	cal	r15,L351-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr0,F:fr6,F:fr5
	cal	r15,L352-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr0,F:fr2
	cal	r15,L353-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr2,F:fr7,F:44(sp)
	cal	r15,L354-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr2,F:fr5
	cal	r15,L355-_Bezier(r14)
	balr	r15,r15	# MOVE	F:0(sp),F:fr2
	cal	r15,L356-_Bezier(r14)
	balr	r15,r15	# ADD	F:fr2,F:fr6,F:40(sp)
	cal	r15,L354-_Bezier(r14)
	balr	r15,r15	# DIV	F:fr2,F:fr5
	cal	r15,L357-_Bezier(r14)
	balr	r15,r15	# MOVE	F:4(sp),F:fr2
	cal	r15,L358-_Bezier(r14)
	balr	r15,r15	# MOVE	F:8(sp),F:44(sp)
	cal	r15,L359-_Bezier(r14)
	balr	r15,r15	# MOVE	F:12(sp),F:40(sp)
	ls	r3,16(sp)
	ls	r2,48(sp)
	cal	r15,L360-_Bezier(r14)
	balr	r15,r15	# MOVE	F:r4,F:fr1
	cal	r15,L361-_Bezier(r14)
	balr	r15,r15	# MOVE	F:r5,F:fr0
	balix	r15,_.Bezier
	l	r0,LF14-_Bezier(r14)
	cal	r15,L362-_Bezier(r14)
	balr	r15,r15	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,68(sp)
	lm	r11,140(sp)
	brx	r15
	cal	sp,196(sp)
L018423:
	cal	r15,L363-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr7,F:196(sp)
	cal	r15,L364-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr6,F:200(sp)
	b	L018419
L018424:
	cal	r15,L363-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr7,F:196(sp)
	cal	r15,L364-_Bezier(r14)
	balr	r15,r15	# MOVE	F:fr6,F:200(sp)
	b	L018419
	.byte	0xdf,0x07,0xdf	#Trace table for Bezier
	.byte	0xba	# First nonvolatile register saved is r11
	.byte	0x81	# npars=8 words
	.byte	0x3f	# rtfl regs saved
	.byte	0x31	# lcl_offset=196
	.align	1
	.data
	.align	2
L282:	# STOREM	<fr2/fr3/fr4/fr5/fr6/fr7>,68(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1d,2
	.short	LSI282
	.byte	92
	.byte	LSF282
	.short	0xff3f	# opd
	.short	0xff01	# opd
	.long	68
	.space	72
	.align	2
L283:	# MOVE	F:fr3,F:r2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI283
	.byte	108
	.byte	LSF283
	.short	0x1203	# opd
	.short	0x0202	# opd
	.space	92
	.align	2
L284:	# MOVE	F:fr2,F:r3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI284
	.byte	108
	.byte	LSF284
	.short	0x1202	# opd
	.short	0x0203	# opd
	.space	92
	.align	2
L285:	# MOVE	F:40(sp),F:208(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI285
	.byte	92
	.byte	LSF285
	.short	0x3201	# opd
	.short	0x3201	# opd
	.long	40
	.long	208
	.space	68
	.align	2
L286:	# SUB	F:fr5,F:40(sp),F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI286
	.byte	76
	.byte	LSF286
	.short	0x1205	# opd
	.short	0x3201	# opd
	.short	0x1202	# opd
	.align	2
	.long	40
	.space	52
	.align	2
L287:	# MOVE	F:44(sp),F:204(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI287
	.byte	92
	.byte	LSF287
	.short	0x3201	# opd
	.short	0x3201	# opd
	.long	44
	.long	204
	.space	68
	.align	2
L288:	# SUB	F:fr4,F:44(sp),F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI288
	.byte	76
	.byte	LSF288
	.short	0x1204	# opd
	.short	0x3201	# opd
	.short	0x1203	# opd
	.align	2
	.long	44
	.space	52
	.align	2
L289:	# MOVE	F:fr0,F:0.000000e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI289
	.byte	108
	.byte	LSF289
	.short	0x1200	# opd
	.short	0x2200	# opd
	.long	0x00000000	# 0
	.space	88
	.align	2
L290:	# CMP	F:fr5,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x07,2
	.short	LSI290
	.byte	60
	.byte	LSF290
	.short	0x1205	# opd
	.short	0x1200	# opd
	.space	44
	.align	2
L291:	# CMP	F:fr4,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x07,2
	.short	LSI291
	.byte	60
	.byte	LSF291
	.short	0x1204	# opd
	.short	0x1200	# opd
	.space	44
	.align	2
L292:	# MOVE	F:60(sp),F:44(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI292
	.byte	92
	.byte	LSF292
	.short	0x3201	# opd
	.short	0x3201	# opd
	.long	60
	.long	44
	.space	68
	.align	2
L293:	# MOVE	F:64(sp),F:40(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI293
	.byte	92
	.byte	LSF293
	.short	0x3201	# opd
	.short	0x3201	# opd
	.long	64
	.long	40
	.space	68
	.align	2
L294:	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,68(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0f,2
	.short	LSI294
	.byte	116
	.byte	LSF294
	.short	0xff3f	# opd
	.short	0xff01	# opd
	.long	68
	.space	96
	.align	2
L295:	# MUL	F:fr1,F:fr5,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI295
	.byte	48
	.byte	LSF295
	.short	0x1201	# opd
	.short	0x1205	# opd
	.short	0x1205	# opd
	.align	2
	.space	28
	.align	2
L296:	# MUL	F:fr0,F:fr4,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI296
	.byte	48
	.byte	LSF296
	.short	0x1200	# opd
	.short	0x1204	# opd
	.short	0x1204	# opd
	.align	2
	.space	28
	.align	2
L297:	# MOVE	D:<r2,r3>,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI297
	.byte	52
	.byte	LSF297
	.short	0x0323	# opd
	.short	0x1200	# opd
	.space	36
	.align	2
L298:	# MOVE	F:fr1,D:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI298
	.byte	24
	.byte	LSF298
	.short	0x1201	# opd
	.short	0x1300	# opd
	.space	8
	.align	2
L299:	# MOVE	F:fr0,I:r2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI299
	.byte	108
	.byte	LSF299
	.short	0x1200	# opd
	.short	0x0002	# opd
	.space	92
	.align	2
L300:	# SUB	F:fr1,F:52(sp),F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI300
	.byte	76
	.byte	LSF300
	.short	0x1201	# opd
	.short	0x3201	# opd
	.short	0x1203	# opd
	.align	2
	.long	52
	.space	52
	.align	2
L301:	# NEG	F:fr6,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x16,2
	.short	LSI301
	.byte	36
	.byte	LSF301
	.short	0x1206	# opd
	.short	0x1205	# opd
	.space	20
	.align	2
L302:	# MUL	F:fr5,F:fr6,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI302
	.byte	48
	.byte	LSF302
	.short	0x1205	# opd
	.short	0x1206	# opd
	.short	0x1201	# opd
	.align	2
	.space	28
	.align	2
L303:	# SUB	F:fr1,F:56(sp),F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI303
	.byte	76
	.byte	LSF303
	.short	0x1201	# opd
	.short	0x3201	# opd
	.short	0x1202	# opd
	.align	2
	.long	56
	.space	52
	.align	2
L304:	# ADD	F:fr1,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI304
	.byte	36
	.byte	LSF304
	.short	0x1201	# opd
	.short	0x1205	# opd
	.space	20
	.align	2
L305:	# NEG	F:fr5,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x16,2
	.short	LSI305
	.byte	36
	.byte	LSF305
	.short	0x1205	# opd
	.short	0x1200	# opd
	.space	20
	.align	2
L306:	# CMPT	F:fr1,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI306
	.byte	84
	.byte	LSF306
	.short	0x1201	# opd
	.short	0x1205	# opd
	.space	68
	.align	2
L307:	# CMPT	F:fr1,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI307
	.byte	84
	.byte	LSF307
	.short	0x1201	# opd
	.short	0x1200	# opd
	.space	68
	.align	2
L308:	# MOVE	F:fr7,F:196(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI308
	.byte	92
	.byte	LSF308
	.short	0x1207	# opd
	.short	0x3201	# opd
	.long	196
	.space	72
	.align	2
L309:	# SUB	F:fr1,F:fr7,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI309
	.byte	76
	.byte	LSF309
	.short	0x1201	# opd
	.short	0x1207	# opd
	.short	0x1203	# opd
	.align	2
	.space	56
	.align	2
L310:	# MUL	F:28(sp),F:fr1,F:fr6
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,3
	.short	LSI310
	.byte	76
	.byte	LSF310
	.short	0x3201	# opd
	.short	0x1201	# opd
	.short	0x1206	# opd
	.align	2
	.long	28
	.space	52
	.align	2
L311:	# MOVE	F:fr6,F:200(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI311
	.byte	108
	.byte	LSF311
	.short	0x1206	# opd
	.short	0x3201	# opd
	.long	200
	.space	88
	.align	2
L312:	# SUB	F:fr1,F:fr6,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x1e,3
	.short	LSI312
	.byte	48
	.byte	LSF312
	.short	0x1201	# opd
	.short	0x1206	# opd
	.short	0x1202	# opd
	.align	2
	.space	28
	.align	2
L313:	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,68(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0f,2
	.short	LSI313
	.byte	116
	.byte	LSF313
	.short	0xff3f	# opd
	.short	0xff01	# opd
	.long	68
	.space	96
	.align	2
L314:	# ADD	F:fr0,F:44(sp),F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI314
	.byte	76
	.byte	LSF314
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x1203	# opd
	.align	2
	.long	44
	.space	52
	.align	2
L315:	# MOVE	F:fr4,F:8.000000e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI315
	.byte	108
	.byte	LSF315
	.short	0x1204	# opd
	.short	0x2200	# opd
	.long	0x41000000	# 8
	.space	88
	.align	2
L316:	# DIV	F:fr0,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,2
	.short	LSI316
	.byte	36
	.byte	LSF316
	.short	0x1200	# opd
	.short	0x1204	# opd
	.space	20
	.align	2
L317:	# MOVE	F:fr1,F:3.000000e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI317
	.byte	108
	.byte	LSF317
	.short	0x1201	# opd
	.short	0x2200	# opd
	.long	0x40400000	# 3
	.space	88
	.align	2
L318:	# ADD	F:fr5,F:fr7,F:52(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI318
	.byte	104
	.byte	LSF318
	.short	0x1205	# opd
	.short	0x1207	# opd
	.short	0x3201	# opd
	.align	2
	.long	52
	.space	80
	.align	2
L319:	# MUL	F:fr5,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI319
	.byte	36
	.byte	LSF319
	.short	0x1205	# opd
	.short	0x1201	# opd
	.space	20
	.align	2
L320:	# DIV	F:fr5,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,2
	.short	LSI320
	.byte	36
	.byte	LSF320
	.short	0x1205	# opd
	.short	0x1204	# opd
	.space	20
	.align	2
L321:	# ADD	F:48(sp),F:fr5,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI321
	.byte	76
	.byte	LSF321
	.short	0x3201	# opd
	.short	0x1205	# opd
	.short	0x1200	# opd
	.align	2
	.long	48
	.space	52
	.align	2
L322:	# ADD	F:fr0,F:40(sp),F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI322
	.byte	76
	.byte	LSF322
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x1202	# opd
	.align	2
	.long	40
	.space	52
	.align	2
L323:	# ADD	F:fr5,F:fr6,F:56(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI323
	.byte	76
	.byte	LSF323
	.short	0x1205	# opd
	.short	0x1206	# opd
	.short	0x3201	# opd
	.align	2
	.long	56
	.space	52
	.align	2
L324:	# MUL	F:fr1,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x15,2
	.short	LSI324
	.byte	36
	.byte	LSF324
	.short	0x1201	# opd
	.short	0x1205	# opd
	.space	20
	.align	2
L325:	# DIV	F:fr1,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,2
	.short	LSI325
	.byte	36
	.byte	LSF325
	.short	0x1201	# opd
	.short	0x1204	# opd
	.space	20
	.align	2
L326:	# ADD	F:16(sp),F:fr1,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI326
	.byte	76
	.byte	LSF326
	.short	0x3201	# opd
	.short	0x1201	# opd
	.short	0x1200	# opd
	.align	2
	.long	16
	.space	52
	.align	2
L327:	# ADD	F:fr0,F:52(sp),F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI327
	.byte	76
	.byte	LSF327
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x1203	# opd
	.align	2
	.long	52
	.space	52
	.align	2
L328:	# MOVE	F:fr5,F:2.000000e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI328
	.byte	108
	.byte	LSF328
	.short	0x1205	# opd
	.short	0x2200	# opd
	.long	0x40000000	# 2
	.space	88
	.align	2
L329:	# DIV	F:32(sp),F:fr0,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI329
	.byte	76
	.byte	LSF329
	.short	0x3201	# opd
	.short	0x1200	# opd
	.short	0x1205	# opd
	.align	2
	.long	32
	.space	52
	.align	2
L330:	# ADD	F:fr0,F:56(sp),F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI330
	.byte	76
	.byte	LSF330
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x1202	# opd
	.align	2
	.long	56
	.space	52
	.align	2
L331:	# DIV	F:36(sp),F:fr0,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI331
	.byte	76
	.byte	LSF331
	.short	0x3201	# opd
	.short	0x1200	# opd
	.short	0x1205	# opd
	.align	2
	.long	36
	.space	52
	.align	2
L332:	# ADD	F:fr0,F:fr7,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI332
	.byte	76
	.byte	LSF332
	.short	0x1200	# opd
	.short	0x1207	# opd
	.short	0x1203	# opd
	.align	2
	.space	56
	.align	2
L333:	# MOVE	F:fr4,F:4.000000e+00
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI333
	.byte	108
	.byte	LSF333
	.short	0x1204	# opd
	.short	0x2200	# opd
	.long	0x40800000	# 4
	.space	88
	.align	2
L334:	# DIV	F:24(sp),F:fr0,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI334
	.byte	76
	.byte	LSF334
	.short	0x3201	# opd
	.short	0x1200	# opd
	.short	0x1204	# opd
	.align	2
	.long	24
	.space	52
	.align	2
L335:	# DIV	F:fr0,F:52(sp),F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI335
	.byte	76
	.byte	LSF335
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x1205	# opd
	.align	2
	.long	52
	.space	52
	.align	2
L336:	# MOVE	F:0(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI336
	.byte	64
	.byte	LSF336
	.short	0x3201	# opd
	.short	0x1200	# opd
	.long	0
	.space	44
	.align	2
L337:	# ADD	F:fr0,F:fr6,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI337
	.byte	48
	.byte	LSF337
	.short	0x1200	# opd
	.short	0x1206	# opd
	.short	0x1202	# opd
	.align	2
	.space	28
	.align	2
L338:	# DIV	F:20(sp),F:fr0,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI338
	.byte	76
	.byte	LSF338
	.short	0x3201	# opd
	.short	0x1200	# opd
	.short	0x1204	# opd
	.align	2
	.long	20
	.space	52
	.align	2
L339:	# DIV	F:fr0,F:56(sp),F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI339
	.byte	76
	.byte	LSF339
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x1205	# opd
	.align	2
	.long	56
	.space	52
	.align	2
L340:	# ADD	F:fr0,F:20(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI340
	.byte	108
	.byte	LSF340
	.short	0x1200	# opd
	.short	0x3201	# opd
	.long	20
	.space	88
	.align	2
L341:	# MOVE	F:4(sp),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI341
	.byte	64
	.byte	LSF341
	.short	0x3201	# opd
	.short	0x1200	# opd
	.long	4
	.space	44
	.align	2
L342:	# MOVE	F:8(sp),F:48(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI342
	.byte	92
	.byte	LSF342
	.short	0x3201	# opd
	.short	0x3201	# opd
	.long	8
	.long	48
	.space	68
	.align	2
L343:	# MOVE	F:12(sp),F:16(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI343
	.byte	92
	.byte	LSF343
	.short	0x3201	# opd
	.short	0x3201	# opd
	.long	12
	.long	16
	.space	68
	.align	2
L344:	# MOVE	F:r2,F:fr3
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI344
	.byte	64
	.byte	LSF344
	.short	0x0202	# opd
	.short	0x1203	# opd
	.space	48
	.align	2
L345:	# MOVE	F:r3,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI345
	.byte	64
	.byte	LSF345
	.short	0x0203	# opd
	.short	0x1202	# opd
	.space	48
	.align	2
L346:	# ADD	F:fr0,F:44(sp),F:52(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI346
	.byte	104
	.byte	LSF346
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x3201	# opd
	.align	2
	.long	44
	.long	52
	.space	76
	.align	2
L347:	# DIV	F:fr1,F:fr0,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI347
	.byte	48
	.byte	LSF347
	.short	0x1201	# opd
	.short	0x1200	# opd
	.short	0x1204	# opd
	.align	2
	.space	28
	.align	2
L348:	# DIV	F:fr0,F:fr7,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI348
	.byte	76
	.byte	LSF348
	.short	0x1200	# opd
	.short	0x1207	# opd
	.short	0x1205	# opd
	.align	2
	.space	56
	.align	2
L349:	# ADD	F:fr0,F:40(sp),F:56(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI349
	.byte	104
	.byte	LSF349
	.short	0x1200	# opd
	.short	0x3201	# opd
	.short	0x3201	# opd
	.align	2
	.long	40
	.long	56
	.space	76
	.align	2
L350:	# DIV	F:fr2,F:fr0,F:fr4
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI350
	.byte	48
	.byte	LSF350
	.short	0x1202	# opd
	.short	0x1200	# opd
	.short	0x1204	# opd
	.align	2
	.space	28
	.align	2
L351:	# DIV	F:fr0,F:fr6,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,3
	.short	LSI351
	.byte	48
	.byte	LSF351
	.short	0x1200	# opd
	.short	0x1206	# opd
	.short	0x1205	# opd
	.align	2
	.space	28
	.align	2
L352:	# ADD	F:fr0,F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,2
	.short	LSI352
	.byte	36
	.byte	LSF352
	.short	0x1200	# opd
	.short	0x1202	# opd
	.space	20
	.align	2
L353:	# ADD	F:fr2,F:fr7,F:44(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI353
	.byte	104
	.byte	LSF353
	.short	0x1202	# opd
	.short	0x1207	# opd
	.short	0x3201	# opd
	.align	2
	.long	44
	.space	80
	.align	2
L354:	# DIV	F:fr2,F:fr5
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0c,2
	.short	LSI354
	.byte	36
	.byte	LSF354
	.short	0x1202	# opd
	.short	0x1205	# opd
	.space	20
	.align	2
L355:	# MOVE	F:0(sp),F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI355
	.byte	64
	.byte	LSF355
	.short	0x3201	# opd
	.short	0x1202	# opd
	.long	0
	.space	44
	.align	2
L356:	# ADD	F:fr2,F:fr6,F:40(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x02,3
	.short	LSI356
	.byte	76
	.byte	LSF356
	.short	0x1202	# opd
	.short	0x1206	# opd
	.short	0x3201	# opd
	.align	2
	.long	40
	.space	52
	.align	2
L357:	# MOVE	F:4(sp),F:fr2
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI357
	.byte	64
	.byte	LSF357
	.short	0x3201	# opd
	.short	0x1202	# opd
	.long	4
	.space	44
	.align	2
L358:	# MOVE	F:8(sp),F:44(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI358
	.byte	92
	.byte	LSF358
	.short	0x3201	# opd
	.short	0x3201	# opd
	.long	8
	.long	44
	.space	68
	.align	2
L359:	# MOVE	F:12(sp),F:40(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI359
	.byte	92
	.byte	LSF359
	.short	0x3201	# opd
	.short	0x3201	# opd
	.long	12
	.long	40
	.space	68
	.align	2
L360:	# MOVE	F:r4,F:fr1
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI360
	.byte	64
	.byte	LSF360
	.short	0x0204	# opd
	.short	0x1201	# opd
	.space	48
	.align	2
L361:	# MOVE	F:r5,F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI361
	.byte	64
	.byte	LSF361
	.short	0x0205	# opd
	.short	0x1200	# opd
	.space	48
	.align	2
L362:	# LOADM	<fr2/fr3/fr4/fr5/fr6/fr7>,68(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x0f,2
	.short	LSI362
	.byte	116
	.byte	LSF362
	.short	0xff3f	# opd
	.short	0xff01	# opd
	.long	68
	.space	96
	.align	2
L363:	# MOVE	F:fr7,F:196(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI363
	.byte	92
	.byte	LSF363
	.short	0x1207	# opd
	.short	0x3201	# opd
	.long	196
	.space	72
	.align	2
L364:	# MOVE	F:fr6,F:200(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI364
	.byte	108
	.byte	LSF364
	.short	0x1206	# opd
	.short	0x3201	# opd
	.long	200
	.space	88
	.text
	.data
	.align	2
_RevSeg:	.long	_.RevSeg
	.text
	.data
	.align	2
L13:	.ascii	"RevSeg: unknown pe type\0"
	.text
	.text
#--------------| RevSeg |-----------------------#
_.RevSeg:
	.using	_RevSeg,r14
	stm	r13,-48(sp)
	mr	r14,r0
	cal	sp,-60(sp)
	sts	r3,48(sp)	# p
	sts	r4,52(sp)	# p
	mr	r13,r5
	ls	r15,0(r13)
	cli	r15,5
	jh	L01907
	a	r15,r15
	lda	r5,LL1909(r15)
	lhas	r15,0(r5)
	a	r15,r5
	br	r15
LL1909:
	.short	L01902-LL1909
	.short	L01902-LL1909-2
	.short	L01903-LL1909-4
	.short	L01907-LL1909-6
	.short	L01904-LL1909-8
	.short	L01902-LL1909-10
L01902:
	.data
	.align	2
LF15:	.long	_MoveTo
	.text
	balix	r15,_.MoveTo
	l	r0,LF15-_RevSeg(r14)
	lm	r13,12(sp)
	brx	r15
	cal	sp,60(sp)
L01903:
	balix	r15,_.LineTo
	l	r0,LF10-_RevSeg(r14)
	lm	r13,12(sp)
	brx	r15
	cal	sp,60(sp)
L01904:
	ls	r0,8(r13)
	ls	r5,4(r13)
	ls	r4,16(r13)
	ls	r3,12(r13)
	sts	r0,0(sp)
	ls	r0,52(sp)	# p
	ls	r15,48(sp)	# p
	sts	r15,4(sp)
	sts	r0,8(sp)
	.data
	.align	2
LF16:	.long	_CurveTo
	.text
	balix	r15,_.CurveTo
	l	r0,LF16-_RevSeg(r14)
	lm	r13,12(sp)
	brx	r15
	cal	sp,60(sp)
L01907:
	cal	r2,L13-_RevSeg(r14)
	balix	r15,_.Panic
	l	r0,LF2-_RevSeg(r14)
	lis	r2,1
	lm	r13,12(sp)
	brx	r15
	cal	sp,60(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for RevSeg
	.byte	0xd8	# First nonvolatile register saved is r13
	.byte	0x41	# npars=4 words
	.byte	0xf	# lcl_offset=60
	.align	1
	.data
	.align	2
_ReversePath:	.long	_.ReversePath
	.text
	.data
	.align	2
L14:	.ascii	"PathFree given NULL\0"
	.align	2
L15:	.ascii	"PathFree given NULL\0"
	.align	2
L16:	.ascii	"PathFree given NULL\0"
	.align	2
L17:	.ascii	"PathFree given NULL\0"
	.align	2
L18:	.ascii	"ReversePath: unknown element type\0"
	.align	2
L19:	.ascii	"PathFree given NULL\0"
	.text
	.globl	_.ReversePath
	.globl	_ReversePath
	.text
#--------------| ReversePath |-----------------------#
_.ReversePath:
	stm	r8,-68(sp)
	mr	r14,r0
	cal	sp,-68(sp)
	mr	r8,r2
	lis	r12,0
	l	r10,_free_pelem-_ReversePath(r14)
	cis	r10,0
	jeq	L01413
	ls	r0,28(r10)
	bx	L01414
	st	r0,_free_pelem-_ReversePath(r14)
L01413:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_ReversePath(r14)
	mr	r10,r2
L01414:
	lis	r13,0
	sts	r13,0(r10)
	sts	r10,32(r10)
	sts	r10,28(r10)
	mr	r11,r10
	ls	r9,32(r8)
	c	r9,r8
	beq	L014131
	b	L014128
L01416:
	bx	L014130
	lis	r12,1
L01417:
	ls	r5,28(r9)
	ls	r4,8(r9)
	ls	r3,4(r9)
	mr	r2,r10
	.data
	.align	2
LF17:	.long	_RevSeg
	.text
	balix	r15,_.RevSeg
	l	r0,LF17-_ReversePath(r14)
	cis	r2,0
	jne	L014111
	cis	r10,0
	jne	L014110
	cal	r2,L14-_ReversePath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_ReversePath(r14)
L014110:
	ls	r2,32(r10)
	l	r0,_free_pelem-_ReversePath(r14)
	sts	r0,28(r2)
	sts	r10,32(r10)
	st	r10,_free_pelem-_ReversePath(r14)
	mr	r2,r13
	lm	r8,0(sp)
	brx	r15
	cal	sp,68(sp)
L014111:
	cis	r12,0
	jeq	L014116
	mr	r2,r10
	.data
	.align	2
LF18:	.long	_ClosePath
	.text
	balix	r15,_.ClosePath
	l	r0,LF18-_ReversePath(r14)
	cis	r2,0
	jne	L014116
	cis	r10,0
	jne	L014115
	cal	r2,L15-_ReversePath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_ReversePath(r14)
L014115:
	ls	r2,32(r10)
	l	r0,_free_pelem-_ReversePath(r14)
	sts	r0,28(r2)
	sts	r10,32(r10)
	st	r10,_free_pelem-_ReversePath(r14)
	mr	r2,r13
	lm	r8,0(sp)
	brx	r15
	cal	sp,68(sp)
L014116:
	ls	r10,28(r10)
	bx	L014130
	lis	r12,0
L014117:
	ls	r5,28(r9)
	ls	r4,8(r9)
	ls	r3,4(r9)
	mr	r2,r10
	balix	r15,_.RevSeg
	l	r0,LF17-_ReversePath(r14)
	cis	r2,0
	jne	L014130
	cis	r10,0
	jne	L014120
	cal	r2,L16-_ReversePath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_ReversePath(r14)
L014120:
	ls	r2,32(r10)
	l	r0,_free_pelem-_ReversePath(r14)
	sts	r0,28(r2)
	sts	r10,32(r10)
	st	r10,_free_pelem-_ReversePath(r14)
	mr	r2,r13
	lm	r8,0(sp)
	brx	r15
	cal	sp,68(sp)
L014122:
	ls	r5,28(r9)
	ls	r4,24(r9)
	ls	r3,20(r9)
	mr	r2,r10
	balix	r15,_.RevSeg
	l	r0,LF17-_ReversePath(r14)
	cis	r2,0
	jne	L014130
	cis	r10,0
	jne	L014125
	cal	r2,L17-_ReversePath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_ReversePath(r14)
L014125:
	ls	r2,32(r10)
	l	r0,_free_pelem-_ReversePath(r14)
	sts	r0,28(r2)
	sts	r10,32(r10)
	st	r10,_free_pelem-_ReversePath(r14)
	mr	r2,r13
	lm	r8,0(sp)
	brx	r15
	cal	sp,68(sp)
L014127:
	cal	r2,L18-_ReversePath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_ReversePath(r14)
L014130:
	ls	r9,32(r9)
	c	r9,r8
	jeq	L014131
L014128:
	ls	r0,0(r9)
	ai	r2,r0,-1
	cli	r2,4
	jh	L014127
	a	r2,r2
	lda	r5,LL14136(r2)
	lhas	r2,0(r5)
	a	r2,r5
	br	r2
LL14136:
	.short	L01417-LL14136
	.short	L014117-LL14136-2
	.short	L014127-LL14136-4
	.short	L014122-LL14136-6
	.short	L01416-LL14136-8
L014131:
	cis	r9,0
	jne	L014133
	cal	r2,L19-_ReversePath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_ReversePath(r14)
L014133:
	ls	r2,32(r9)
	l	r0,_free_pelem-_ReversePath(r14)
	sts	r0,28(r2)
	sts	r9,32(r9)
	st	r9,_free_pelem-_ReversePath(r14)
	mr	r2,r11
	lm	r8,0(sp)
	brx	r15
	cal	sp,68(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for ReversePath
	.byte	0x88	# First nonvolatile register saved is r8
	.byte	0x11	# npars=1 words
	.byte	0x11	# lcl_offset=68
	.align	1
	.data
	.align	2
_FlattenPath:	.long	_.FlattenPath
	.text
	.data
	.align	2
L20:	.ascii	"delete from empty path\0"
	.align	2
L21:	.ascii	"PathInsert given NULL\0"
	.align	2
L22:	.ascii	"PathInsert given NULL\0"
	.align	2
L23:	.ascii	"PathInsert given NULL\0"
	.align	2
L24:	.ascii	"PathInsert given NULL\0"
	.align	2
L25:	.ascii	"PathFree given NULL\0"
	.align	2
L26:	.ascii	"Flattenpath discovers unknown path element type\0"
	.text
	.globl	_.FlattenPath
	.globl	_FlattenPath
	.text
#--------------| FlattenPath |-----------------------#
_.FlattenPath:
	.using	_FlattenPath,r14
	stm	r6,-76(sp)
	mr	r14,r0
	cal	sp,-100(sp)
	mr	r12,r2
	l	r6,_free_pelem-_FlattenPath(r14)
	cis	r6,0
	jeq	L01423
	ls	r0,28(r6)
	bx	L01424
	st	r0,_free_pelem-_FlattenPath(r14)
L01423:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_FlattenPath(r14)
	mr	r6,r2
L01424:
	lis	r0,0
	lda	r8,L00BSS
	sts	r0,0(r6)
	sts	r6,32(r6)
	sts	r6,28(r6)
	st	r6,_path-L00BSS(r8)
	ls	r13,28(r12)
	c	r13,r12
	beq	L014249
	b	L014246
L01426:
	lis	r9,1
	lda	r10,_gstate
	cal	r2,16(sp)	# lastp
	cal	r3,4(r13)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	l	r7,_free_pelem-_FlattenPath(r14)
	cis	r7,0
	jeq	L01428
	ls	r0,28(r7)
	bx	L01429
	st	r0,_free_pelem-_FlattenPath(r14)
L01428:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_FlattenPath(r14)
	mr	r7,r2
L01429:
	sts	r9,0(r7)
	cal	r2,4(r7)
	cal	r3,16(sp)	# lastp
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r6,0
	jne	L014211
	bx	L014212
	mr	r0,r9
L014211:
	ls	r0,28(r6)
	c	r0,r6
	beqx	LL14256
	lis	r0,1
	lis	r0,0
LL14256:
L014212:
	cis	r0,0
	jne	L014217
	ls	r3,32(r6)
	ls	r0,0(r3)
	cis	r0,1
	jne	L014217
	ls	r0,28(r6)
	c	r0,r6
	jne	L014216
	cal	r2,L20-_FlattenPath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_FlattenPath(r14)
	ls	r3,32(r6)
L014216:
	ls	r2,32(r3)
	l	r0,_free_pelem-_FlattenPath(r14)
	sts	r6,28(r2)
	ls	r2,32(r3)
	sts	r2,32(r6)
	sts	r0,28(r3)
	st	r3,_free_pelem-_FlattenPath(r14)
L014217:
	ls	r2,0(r10)
	sts	r9,44(r2)
	cal	r2,36(r2)
	cal	r3,16(sp)	# lastp
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r7,0
	jne	L014219
	cal	r2,L21-_FlattenPath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_FlattenPath(r14)
L014219:
	sts	r6,28(r7)
	ls	r2,32(r6)
	sts	r2,32(r7)
	ls	r2,32(r6)
	sts	r7,28(r2)
	bx	L014248
	sts	r7,32(r6)
L014220:
	lda	r10,_gstate
	lis	r11,2
	cal	r2,16(sp)	# lastp
	cal	r3,4(r13)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	l	r7,_free_pelem-_FlattenPath(r14)
	cis	r7,0
	jeq	L014222
	ls	r0,28(r7)
	bx	L014223
	st	r0,_free_pelem-_FlattenPath(r14)
L014222:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_FlattenPath(r14)
	mr	r7,r2
L014223:
	ls	r4,0(r10)
	sts	r11,0(r7)
	cal	r2,4(r7)
	cal	r3,16(sp)	# lastp
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,36(r4)
	cal	r3,16(sp)	# lastp
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r7,0
	jne	L014225
	cal	r2,L22-_FlattenPath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_FlattenPath(r14)
L014225:
	sts	r6,28(r7)
	ls	r2,32(r6)
	sts	r2,32(r7)
	ls	r2,32(r6)
	sts	r7,28(r2)
	bx	L014248
	sts	r7,32(r6)
L014226:
	lda	r10,_gstate
	ls	r2,0(r10)
	ls	r0,44(r2)
	cis	r0,0
	beq	L014248
	lis	r9,1
	lis	r11,5
	l	r7,_free_pelem-_FlattenPath(r14)
	cis	r7,0
	jeq	L014229
	ls	r0,28(r7)
	bx	L014230
	st	r0,_free_pelem-_FlattenPath(r14)
L014229:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_FlattenPath(r14)
	mr	r7,r2
	ls	r2,0(r10)
L014230:
	sts	r11,0(r7)
	ls	r0,16(r2)
	sts	r0,4(r7)
	ls	r0,20(r2)
	sts	r0,8(r7)
	mr	r11,r6
	ls	r0,0(r11)
	cis	r0,1
	jeq	L014232
L014231:
	ls	r11,32(r11)
	ls	r0,0(r11)
	cis	r0,1
	jne	L014231
	mr	r0,r0	# no op to fix APC bug
	mr	r0,r0	# no op to fix APC bug
	mr	r0,r0	# no op to fix APC bug
L014232:
	cis	r7,0
	jne	L014234
	cal	r2,L23-_FlattenPath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_FlattenPath(r14)
L014234:
	l	r0,_free_pelem-_FlattenPath(r14)
	sts	r6,28(r7)
	ls	r2,32(r6)
	sts	r2,32(r7)
	ls	r2,32(r6)
	sts	r7,28(r2)
	sts	r7,32(r6)
	mr	r7,r0
	cis	r7,0
	jeq	L014236
	ls	r0,28(r7)
	bx	L014237
	st	r0,_free_pelem-_FlattenPath(r14)
L014236:
	cal	r2,36(r0)
	balix	r15,_.malloc
	l	r0,LF1-_FlattenPath(r14)
	mr	r7,r2
L014237:
	ls	r4,0(r10)
	sts	r9,0(r7)
	cal	r2,4(r7)
	cal	r3,4(r11)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cal	r2,36(r4)
	cal	r3,4(r7)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	cis	r7,0
	jne	L014239
	cal	r2,L24-_FlattenPath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_FlattenPath(r14)
L014239:
	sts	r6,28(r7)
	ls	r2,32(r6)
	sts	r2,32(r7)
	ls	r2,32(r6)
	sts	r7,28(r2)
	bx	L014248
	sts	r7,32(r6)
L014240:
	ls	r0,12(r13)
	ls	r5,8(r13)
	ls	r4,4(r13)
	ls	r3,20(sp)	# lastp
	ls	r2,16(sp)	# lastp
	sts	r0,0(sp)
	ls	r0,16(r13)
	sts	r0,4(sp)
	ls	r0,20(r13)
	sts	r0,8(sp)
	ls	r0,24(r13)
	sts	r0,12(sp)
	balix	r15,_.Bezier
	l	r0,LF14-_FlattenPath(r14)
	cis	r2,0
	jne	L014244
	cis	r6,0
	jne	L014243
	cal	r2,L25-_FlattenPath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_FlattenPath(r14)
L014243:
	ls	r2,32(r6)
	l	r0,_free_pelem-_FlattenPath(r14)
	sts	r0,28(r2)
	sts	r6,32(r6)
	st	r6,_free_pelem-_FlattenPath(r14)
	lis	r2,0
	lm	r6,24(sp)
	brx	r15
	cal	sp,100(sp)
L014244:
	cal	r2,16(sp)	# lastp
	cal	r3,20(r13)
	ls	r0,0(r3)
	sts	r0,0(r2)
	ls	r0,4(r3)
	bx	L014248
	sts	r0,4(r2)
L014245:
	cal	r2,L26-_FlattenPath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_FlattenPath(r14)
L014248:
	ls	r13,28(r13)
	c	r13,r12
	jeq	L014255
L014246:
	ls	r0,0(r13)
	ai	r2,r0,-1
	cli	r2,4
	jh	L014245
	a	r2,r2
	lda	r5,LL14257(r2)
	lhas	r2,0(r5)
	a	r2,r5
	br	r2
LL14257:
	.short	L01426-LL14257
	.short	L014220-LL14257-2
	.short	L014245-LL14257-4
	.short	L014240-LL14257-6
	.short	L014226-LL14257-8
L014249:
	mr	r2,r6
	lm	r6,24(sp)
	brx	r15
	cal	sp,100(sp)
L014255:
	l	r6,_path-L00BSS(r8)
	j	L014249
	.byte	0xdf,0x07,0xdf	#Trace table for FlattenPath
	.byte	0x68	# First nonvolatile register saved is r6
	.byte	0x11	# npars=1 words
	.byte	0x19	# lcl_offset=100
	.align	1
	.data
	.align	2
_CloseAll:	.long	_.CloseAll
	.text
	.data
	.align	2
L27:	.ascii	"delete from empty path\0"
	.text
	.globl	_.CloseAll
	.globl	_CloseAll
	.text
#--------------| CloseAll |-----------------------#
_.CloseAll:
	stm	r12,-52(sp)
	mr	r14,r0
	cal	sp,-52(sp)
	mr	r12,r2
	lis	r2,0
	ls	r13,28(r12)
	c	r13,r12
	jeq	L01918
L01912:
	ls	r0,0(r13)
	cis	r0,1
	jne	L01917
	cis	r2,5
	jeq	L01917
	cis	r2,0
	jeq	L01917
	mr	r2,r13
	balix	r15,_.ClosePath
	l	r0,LF18-_CloseAll(r14)
	cis	r2,0
	jne	L019117
	lda	r3,_PLimitCheck
	ls	r0,0(r3)
	lda	r2,_error_name
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	ls	r0,8(r3)
	sts	r0,8(r2)
	ls	r0,12(r3)
	sts	r0,12(r2)
	lis	r2,0
	lm	r12,0(sp)
	brx	r15
	cal	sp,52(sp)
L01917:
	mr	r2,r0
	ls	r13,28(r13)
	c	r13,r12
	jne	L01912
L01918:
	cis	r2,1
	jne	L019112
	ls	r0,28(r13)
	c	r0,r13
	jne	L019111
	cal	r2,L27-_CloseAll(r14)
	balix	r15,_.Panic
	l	r0,LF2-_CloseAll(r14)
L019111:
	ls	r3,32(r13)
	ls	r2,32(r3)
	sts	r13,28(r2)
	ls	r2,32(r3)
	sts	r2,32(r13)
	l	r0,_free_pelem-_CloseAll(r14)
	sts	r0,28(r3)
	st	r3,_free_pelem-_CloseAll(r14)
	lis	r2,1
	lm	r12,0(sp)
	brx	r15
	cal	sp,52(sp)
L019112:
	cis	r2,0
	jeq	L019114
	cis	r2,5
	jne	L019115
L019114:
	lis	r2,1
	lm	r12,0(sp)
	brx	r15
	cal	sp,52(sp)
L019115:
	mr	r2,r13
	balix	r15,_.ClosePath
	l	r0,LF18-_CloseAll(r14)
	lm	r12,0(sp)
	brx	r15
	cal	sp,52(sp)
L019117:
	bx	L01917
	ls	r0,0(r13)
	.byte	0xdf,0x07,0xdf	#Trace table for CloseAll
	.byte	0xc8	# First nonvolatile register saved is r12
	.byte	0x11	# npars=1 words
	.byte	0xd	# lcl_offset=52
	.align	1
	.data
	.align	2
_SetPath:	.long	_.SetPath
	.text
	.data
	.align	2
L28:	.ascii	"PathFree given NULL\0"
	.text
	.globl	_.SetPath
	.globl	_SetPath
	.text
#--------------| SetPath |-----------------------#
_.SetPath:
	stm	r12,-52(sp)
	mr	r14,r0
	cal	sp,-52(sp)
	mr	r12,r2
	mr	r13,r3
	ls	r2,0(r12)
	cis	r2,0
	jne	L01923
	cal	r2,L28-_SetPath(r14)
	balix	r15,_.Panic
	l	r0,LF2-_SetPath(r14)
	ls	r2,0(r12)
L01923:
	ls	r2,32(r2)
	l	r0,_free_pelem-_SetPath(r14)
	sts	r0,28(r2)
	ls	r2,0(r12)
	sts	r2,32(r2)
	ls	r2,0(r12)
	st	r2,_free_pelem-_SetPath(r14)
	sts	r13,0(r12)
	lm	r12,0(sp)
	brx	r15
	cal	sp,52(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for SetPath
	.byte	0xc8	# First nonvolatile register saved is r12
	.byte	0x21	# npars=2 words
	.byte	0xd	# lcl_offset=52
	.align	1
	.data
	.align	2
_Bound:	.long	_.Bound
	.text
	.globl	_.Bound
	.globl	_Bound
	.text
#--------------| Bound |-----------------------#
_.Bound:
	.using	_Bound,r14
	stm	r14,-44(sp)
	mr	r14,r0
	cal	sp,-44(sp)
	cal	r15,L365-_Bound(r14)
	balr	r15,r15	# MOVE	F:fr0,F:44(sp)
	cal	r15,L366-_Bound(r14)
	balr	r15,r15	# CMPT	F:fr0,F:0(r2)
	jhe	L01933
	cal	r15,L367-_Bound(r14)
	balr	r15,r15	# MOVE	F:0(r2),F:fr0
L01933:
	cal	r15,L368-_Bound(r14)
	balr	r15,r15	# CMPT	F:fr0,F:0(r3)
	jle	L01935
	cal	r15,L369-_Bound(r14)
	balr	r15,r15	# MOVE	F:0(r3),F:fr0
L01935:
	cal	r15,L370-_Bound(r14)
	balr	r15,r15	# MOVE	F:fr0,F:48(sp)
	cal	r15,L371-_Bound(r14)
	balr	r15,r15	# CMPT	F:fr0,F:0(r5)
	jhe	L01937
	cal	r15,L372-_Bound(r14)
	balr	r15,r15	# MOVE	F:0(r5),F:fr0
L01937:
	cal	r15,L373-_Bound(r14)
	balr	r15,r15	# CMPT	F:fr0,F:0(r4)
	jle	L01939
	cal	r15,L374-_Bound(r14)
	balr	r15,r15	# MOVE	F:0(r4),F:fr0
	lm	r14,0(sp)
	brx	r15
	cal	sp,44(sp)
L01939:
	lm	r14,0(sp)
	brx	r15
	cal	sp,44(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for Bound
	.byte	0xe8	# First nonvolatile register saved is r14
	.byte	0x61	# npars=6 words
	.byte	0xb	# lcl_offset=44
	.align	1
	.data
	.align	2
L365:	# MOVE	F:fr0,F:44(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI365
	.byte	124
	.byte	LSF365
	.short	0x1200	# opd
	.short	0x3201	# opd
	.long	44
	.space	104
	.align	2
L366:	# CMPT	F:fr0,F:0(r2)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI366
	.byte	172
	.byte	LSF366
	.short	0x1200	# opd
	.short	0x3202	# opd
	.long	0
	.space	152
	.align	2
L367:	# MOVE	F:0(r2),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI367
	.byte	80
	.byte	LSF367
	.short	0x3202	# opd
	.short	0x1200	# opd
	.long	0
	.space	60
	.align	2
L368:	# CMPT	F:fr0,F:0(r3)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI368
	.byte	164
	.byte	LSF368
	.short	0x1200	# opd
	.short	0x3203	# opd
	.long	0
	.space	144
	.align	2
L369:	# MOVE	F:0(r3),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI369
	.byte	72
	.byte	LSF369
	.short	0x3203	# opd
	.short	0x1200	# opd
	.long	0
	.space	52
	.align	2
L370:	# MOVE	F:fr0,F:48(sp)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI370
	.byte	112
	.byte	LSF370
	.short	0x1200	# opd
	.short	0x3201	# opd
	.long	48
	.space	92
	.align	2
L371:	# CMPT	F:fr0,F:0(r5)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI371
	.byte	160
	.byte	LSF371
	.short	0x1200	# opd
	.short	0x3205	# opd
	.long	0
	.space	140
	.align	2
L372:	# MOVE	F:0(r5),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI372
	.byte	68
	.byte	LSF372
	.short	0x3205	# opd
	.short	0x1200	# opd
	.long	0
	.space	48
	.align	2
L373:	# CMPT	F:fr0,F:0(r4)
	mr	r0,r15
	.long	FPGLUE
	.byte	0x08,2
	.short	LSI373
	.byte	156
	.byte	LSF373
	.short	0x1200	# opd
	.short	0x3204	# opd
	.long	0
	.space	136
	.align	2
L374:	# MOVE	F:0(r4),F:fr0
	mr	r0,r15
	.long	FPGLUE
	.byte	0x14,2
	.short	LSI374
	.byte	64
	.byte	LSF374
	.short	0x3204	# opd
	.short	0x1200	# opd
	.long	0
	.space	44
	.text
	.data
	.align	2
_PathBBox:	.long	_.PathBBox
	.text
	.data
	.align	2
L29:	.ascii	"PathBBox, header found.\0"
	.align	2
L30:	.ascii	"PathBBox, unknown path element type.\0"
	.text
	.globl	_.PathBBox
	.globl	_PathBBox
	.text
#--------------| PathBBox |-----------------------#
_.PathBBox:
	.using	_PathBBox,r14
	stm	r7,-72(sp)
	mr	r14,r0
	cal	sp,-80(sp)
	mr	r7,r2
	mr	r8,r3
	mr	r9,r4
	mr	r10,r5
	lda	r12,_gstate
	ls	r3,0(r12)
	ls	r0,44(r3)
	cis	r0,0
	jeq	L01946
	ls	r2,48(r3)
	cis	r2,0
	jne	L01944
	bx	L01945
	lis	r0,1
L01944:
	ls	r0,28(r2)
	c	r0,r2
	beqx	LL19419
	lis	r0,1
	lis	r0,0
LL19419:
L01945:
	cis	r0,0
	jeq	L01947
L01946:
	lda	r3,_PNoCurrentPoint
	ls	r0,0(r3)
	lda	r2,_error_name
	sts	r0,0(r2)
	ls	r0,4(r3)
	sts	r0,4(r2)
	ls	r0,8(r3)
	sts	r0,8(r2)
	ls	r0,12(r3)
	sts	r0,12(r2)
	lis	r2,0
	lm	r7,8(sp)
	brx	r15
	cal	sp,80(sp)
L01947:
	lis	r13,1
	ls	r0,36(r3)
	sts	r0,0(r8)
	sts	r0,0(r7)
	ls	r3,0(r12)
	ls	r0,40(r3)
	sts	r0,0(r10)
	sts	r0,0(r9)
	ls	r3,0(r12)
	ls	r3,48(r3)
	ls	r11,28(r3)
	c	r11,r3
	jeq	L019417
	j	L019414
L01949:
	ls	r0,8(r11)
	ls	r2,4(r11)
	sts	r2,0(sp)
	sts	r0,4(sp)
	mr	r2,r7
	mr	r3,r8
	mr	r4,r9
	mr	r5,r10
	.data
	.align	2
LF19:	.long	_Bound
	.text
	balix	r15,_.Bound
	l	r0,LF19-_PathBBox(r14)
	ls	r2,0(r12)
	bx	L019416
	ls	r3,48(r2)
L019411:
	ls	r0,8(r11)
	ls	r2,4(r11)
	sts	r2,0(sp)
	sts	r0,4(sp)
	mr	r2,r7
	mr	r3,r8
	mr	r4,r9
	mr	r5,r10
	balix	r15,_.Bound
	l	r0,LF19-_PathBBox(r14)
	ls	r0,16(r11)
	ls	r2,12(r11)
	sts	r2,0(sp)
	sts	r0,4(sp)
	mr	r2,r7
	mr	r3,r8
	mr	r4,r9
	mr	r5,r10
	balix	r15,_.Bound
	l	r0,LF19-_PathBBox(r14)
	ls	r0,24(r11)
	ls	r2,20(r11)
	sts	r2,0(sp)
	sts	r0,4(sp)
	mr	r2,r7
	mr	r3,r8
	mr	r4,r9
	mr	r5,r10
	balix	r15,_.Bound
	l	r0,LF19-_PathBBox(r14)
	ls	r2,0(r12)
	bx	L019416
	ls	r3,48(r2)
L019412:
	cal	r2,L29-_PathBBox(r14)
	balix	r15,_.Panic
	l	r0,LF2-_PathBBox(r14)
	ls	r2,0(r12)
	bx	L019416
	ls	r3,48(r2)
L019413:
	cal	r2,L30-_PathBBox(r14)
	balix	r15,_.Panic
	l	r0,LF2-_PathBBox(r14)
	ls	r2,0(r12)
	bx	L019416
	ls	r3,48(r2)
L019414:
	ls	r2,0(r11)
	cli	r2,5
	jh	L019413
	a	r2,r2
	lda	r5,LL19420(r2)
	lhas	r2,0(r5)
	a	r2,r5
	br	r2
LL19420:
	.short	L019412-LL19420
	.short	L01949-LL19420-2
	.short	L01949-LL19420-4
	.short	L019413-LL19420-6
	.short	L019411-LL19420-8
	.short	L019416-LL19420-10
L019416:
	ls	r11,28(r11)
	c	r11,r3
	jne	L019414
L019417:
	mr	r2,r13
	lm	r7,8(sp)
	brx	r15
	cal	sp,80(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for PathBBox
	.byte	0x78	# First nonvolatile register saved is r7
	.byte	0x41	# npars=4 words
	.byte	0x14	# lcl_offset=80
	.align	1
	.data
	.align	2
_UserBound:	.long	_.UserBound
	.text
	.globl	_.UserBound
	.globl	_UserBound
	.text
#--------------| UserBound |-----------------------#
_.UserBound:
	.using	_UserBound,r14
	stm	r14,-44(sp)
	mr	r14,r0
	cal	sp,-44(sp)
	cal	r15,L365-_UserBound(r14)
	balr	r15,r15	# MOVE	F:fr0,F:44+LUA0193(sp)
	cal	r15,L366-_UserBound(r14)
	balr	r15,r15	# CMPT	F:fr0,F:0(r2)
	jhe	L01953
	cal	r15,L367-_UserBound(r14)
	balr	r15,r15	# MOVE	F:0(r2),F:fr0
L01953:
	cal	r15,L368-_UserBound(r14)
	balr	r15,r15	# CMPT	F:fr0,F:0(r3)
	jle	L01955
	cal	r15,L369-_UserBound(r14)
	balr	r15,r15	# MOVE	F:0(r3),F:fr0
L01955:
	cal	r15,L370-_UserBound(r14)
	balr	r15,r15	# MOVE	F:fr0,F:48+LUA0193(sp)
	cal	r15,L371-_UserBound(r14)
	balr	r15,r15	# CMPT	F:fr0,F:0(r5)
	jhe	L01957
	cal	r15,L372-_UserBound(r14)
	balr	r15,r15	# MOVE	F:0(r5),F:fr0
L01957:
	cal	r15,L373-_UserBound(r14)
	balr	r15,r15	# CMPT	F:fr0,F:0(r4)
	jle	L01959
	cal	r15,L374-_UserBound(r14)
	balr	r15,r15	# MOVE	F:0(r4),F:fr0
	lm	r14,0(sp)
	brx	r15
	cal	sp,44(sp)
L01959:
	lm	r14,0(sp)
	brx	r15
	cal	sp,44(sp)
	.byte	0xdf,0x07,0xdf	#Trace table for UserBound
	.byte	0xe8	# First nonvolatile register saved is r14
	.byte	0x61	# npars=6 words
	.byte	0xb	# lcl_offset=44
	.align	1
# RTFL scratch register masks:
	.set	LSI31,0x0018
	.set	LSF31,0xc0
	.set	LSI32,0x3c19
	.set	LSF32,0xf0
	.set	LSI33,0x3c19
	.set	LSF33,0xf0
	.set	LSI34,0x3c19
	.set	LSF34,0xe0
	.set	LSI35,0x3c19
	.set	LSF35,0xc0
	.set	LSI36,0x3c19
	.set	LSF36,0xc0
	.set	LSI37,0x3c19
	.set	LSF37,0x00
	.set	LSI38,0x3c19
	.set	LSF38,0x80
	.set	LSI39,0x3c19
	.set	LSF39,0x10
	.set	LSI40,0x3c11
	.set	LSF40,0x10
	.set	LSI41,0x3c19
	.set	LSF41,0x00
	.set	LSI42,0x3c19
	.set	LSF42,0x00
	.set	LSI43,0x3c19
	.set	LSF43,0xf0
	.set	LSI44,0x0018
	.set	LSF44,0xc0
	.set	LSI45,0x3c19
	.set	LSF45,0xc0
	.set	LSI46,0x3c19
	.set	LSF46,0x80
	.set	LSI47,0x0018
	.set	LSF47,0xc0
	.set	LSI48,0x3c3d
	.set	LSF48,0xff
	.set	LSI49,0x3c3d
	.set	LSF49,0xff
	.set	LSI50,0x3c3d
	.set	LSF50,0xdf
	.set	LSI51,0x3c3d
	.set	LSF51,0xdf
	.set	LSI52,0x3c3d
	.set	LSF52,0xdf
	.set	LSI53,0x3c3d
	.set	LSF53,0xdd
	.set	LSI54,0x3c3d
	.set	LSF54,0x5d
	.set	LSI55,0x3c3d
	.set	LSF55,0xdd
	.set	LSI56,0x3c3d
	.set	LSF56,0x5f
	.set	LSI57,0x3c35
	.set	LSF57,0xdd
	.set	LSI58,0x3c35
	.set	LSF58,0x9d
	.set	LSI59,0x3c35
	.set	LSF59,0x99
	.set	LSI60,0x3c35
	.set	LSF60,0x91
	.set	LSI61,0x3c35
	.set	LSF61,0x11
	.set	LSI62,0x3c35
	.set	LSF62,0x11
	.set	LSI63,0x3c35
	.set	LSF63,0x19
	.set	LSI64,0x3c35
	.set	LSF64,0x19
	.set	LSI65,0x3c35
	.set	LSF65,0x19
	.set	LSI66,0x3415
	.set	LSF66,0x40
	.set	LSI67,0x3c35
	.set	LSF67,0x11
	.set	LSI68,0x3c35
	.set	LSF68,0x81
	.set	LSI69,0x3c01
	.set	LSF69,0xc1
	.set	LSI70,0x3c35
	.set	LSF70,0xc1
	.set	LSI71,0x3c35
	.set	LSF71,0xc1
	.set	LSI72,0x3c35
	.set	LSF72,0xe1
	.set	LSI73,0x3c35
	.set	LSF73,0xe0
	.set	LSI74,0x3011
	.set	LSF74,0xc0
	.set	LSI75,0x3031
	.set	LSF75,0xe0
	.set	LSI76,0x3c35
	.set	LSF76,0xc1
	.set	LSI77,0x3011
	.set	LSF77,0xc0
	.set	LSI78,0x3c35
	.set	LSF78,0xc1
	.set	LSI79,0x3c35
	.set	LSF79,0xc0
	.set	LSI80,0x3c35
	.set	LSF80,0x80
	.set	LSI81,0x3c31
	.set	LSF81,0x84
	.set	LSI82,0x3c35
	.set	LSF82,0x80
	.set	LSI83,0x3c31
	.set	LSF83,0x84
	.set	LSI84,0x3c31
	.set	LSF84,0x88
	.set	LSI85,0x3c35
	.set	LSF85,0x80
	.set	LSI86,0x3c31
	.set	LSF86,0x88
	.set	LSI87,0x3c01
	.set	LSF87,0x80
	.set	LSI88,0x3c35
	.set	LSF88,0x40
	.set	LSI89,0x3c35
	.set	LSF89,0xc0
	.set	LSI90,0x3c01
	.set	LSF90,0x80
	.set	LSI91,0x3c35
	.set	LSF91,0x71
	.set	LSI92,0x3c35
	.set	LSF92,0xf1
	.set	LSI93,0x3c35
	.set	LSF93,0xe1
	.set	LSI94,0x3c35
	.set	LSF94,0xe1
	.set	LSI95,0x3c25
	.set	LSF95,0xe1
	.set	LSI96,0x3c21
	.set	LSF96,0x03
	.set	LSI97,0x3c25
	.set	LSF97,0xe1
	.set	LSI98,0x3c01
	.set	LSF98,0xc1
	.set	LSI99,0x3c01
	.set	LSF99,0xc1
	.set	LSI100,0x3c01
	.set	LSF100,0xc1
	.set	LSI101,0x3c01
	.set	LSF101,0xc1
	.set	LSI102,0x3c01
	.set	LSF102,0xc1
	.set	LSI103,0x3c01
	.set	LSF103,0xc1
	.set	LSI104,0x3c01
	.set	LSF104,0xc1
	.set	LSI105,0x3c01
	.set	LSF105,0x81
	.set	LSI106,0x3c01
	.set	LSF106,0x81
	.set	LSI107,0x3c01
	.set	LSF107,0x80
	.set	LSI108,0x3c01
	.set	LSF108,0x01
	.set	LSI109,0x3c01
	.set	LSF109,0x01
	.set	LSI110,0x3c01
	.set	LSF110,0x41
	.set	LSI111,0x3c01
	.set	LSF111,0x41
	.set	LSI112,0x3c01
	.set	LSF112,0x41
	.set	LSI113,0x3c01
	.set	LSF113,0xc1
	.set	LSI114,0x1c21
	.set	LSF114,0xc5
	.set	LSI115,0x1c21
	.set	LSF115,0x45
	.set	LSI116,0x1c21
	.set	LSF116,0xc1
	.set	LSI117,0x1c21
	.set	LSF117,0xc1
	.set	LSI118,0x1c21
	.set	LSF118,0xc9
	.set	LSI119,0x1c21
	.set	LSF119,0x59
	.set	LSI120,0x1c21
	.set	LSF120,0xd1
	.set	LSI121,0x3c01
	.set	LSF121,0xc1
	.set	LSI122,0x3c01
	.set	LSF122,0xc1
	.set	LSI123,0x3c01
	.set	LSF123,0x81
	.set	LSI124,0x1c21
	.set	LSF124,0x45
	.set	LSI125,0x1c21
	.set	LSF125,0x59
	.set	LSI126,0x1c21
	.set	LSF126,0xd1
	.set	LSI127,0x3c35
	.set	LSF127,0xe1
	.set	LSI128,0x0018
	.set	LSF128,0xc0
	.set	LSI129,0x3c31
	.set	LSF129,0xe3
	.set	LSI130,0x3c31
	.set	LSF130,0xe3
	.set	LSI131,0x3c31
	.set	LSF131,0xe1
	.set	LSI132,0x3c31
	.set	LSF132,0xc3
	.set	LSI133,0x3c31
	.set	LSF133,0xc3
	.set	LSI134,0x3c71
	.set	LSF134,0x83
	.set	LSI135,0x3c71
	.set	LSF135,0x81
	.set	LSI136,0x3c71
	.set	LSF136,0x04
	.set	LSI137,0x3c71
	.set	LSF137,0x05
	.set	LSI138,0x3c71
	.set	LSF138,0x09
	.set	LSI139,0x3c71
	.set	LSF139,0x08
	.set	LSI140,0x3c71
	.set	LSF140,0x09
	.set	LSI141,0x3c71
	.set	LSF141,0x01
	.set	LSI142,0x3c71
	.set	LSF142,0x01
	.set	LSI143,0x3c71
	.set	LSF143,0x02
	.set	LSI144,0x3c71
	.set	LSF144,0x03
	.set	LSI145,0x3c71
	.set	LSF145,0x13
	.set	LSI146,0x3c71
	.set	LSF146,0x12
	.set	LSI147,0x3411
	.set	LSF147,0x83
	.set	LSI148,0x3c71
	.set	LSF148,0x83
	.set	LSI149,0x3c71
	.set	LSF149,0x03
	.set	LSI150,0x3c71
	.set	LSF150,0x03
	.set	LSI151,0x3c71
	.set	LSF151,0x02
	.set	LSI152,0x3c71
	.set	LSF152,0x02
	.set	LSI153,0x3c71
	.set	LSF153,0x02
	.set	LSI154,0x3c71
	.set	LSF154,0x02
	.set	LSI155,0x3c71
	.set	LSF155,0x02
	.set	LSI156,0x3c71
	.set	LSF156,0x02
	.set	LSI157,0x3c71
	.set	LSF157,0x01
	.set	LSI158,0x3c71
	.set	LSF158,0x03
	.set	LSI159,0x3c71
	.set	LSF159,0x03
	.set	LSI160,0x3c71
	.set	LSF160,0x03
	.set	LSI161,0x3c71
	.set	LSF161,0x83
	.set	LSI162,0x3c71
	.set	LSF162,0x81
	.set	LSI163,0x3c71
	.set	LSF163,0x80
	.set	LSI164,0x3c71
	.set	LSF164,0x80
	.set	LSI165,0x3c71
	.set	LSF165,0x82
	.set	LSI166,0x3c71
	.set	LSF166,0x83
	.set	LSI167,0x3c71
	.set	LSF167,0x03
	.set	LSI168,0x3c71
	.set	LSF168,0x02
	.set	LSI169,0x3c71
	.set	LSF169,0x02
	.set	LSI170,0x3c71
	.set	LSF170,0x03
	.set	LSI171,0x3c71
	.set	LSF171,0x03
	.set	LSI172,0x3c71
	.set	LSF172,0x83
	.set	LSI173,0x3c71
	.set	LSF173,0x83
	.set	LSI174,0x3c71
	.set	LSF174,0x03
	.set	LSI175,0x3c71
	.set	LSF175,0x43
	.set	LSI176,0x3c71
	.set	LSF176,0x63
	.set	LSI177,0x3c71
	.set	LSF177,0x65
	.set	LSI178,0x3c71
	.set	LSF178,0x45
	.set	LSI179,0x3c71
	.set	LSF179,0x05
	.set	LSI180,0x3c71
	.set	LSF180,0x05
	.set	LSI181,0x3c71
	.set	LSF181,0x05
	.set	LSI182,0x3c71
	.set	LSF182,0x8d
	.set	LSI183,0x3c71
	.set	LSF183,0x1f
	.set	LSI184,0x3c71
	.set	LSF184,0x17
	.set	LSI185,0x3c71
	.set	LSF185,0x17
	.set	LSI186,0x3c71
	.set	LSF186,0x17
	.set	LSI187,0x3c71
	.set	LSF187,0x77
	.set	LSI188,0x3411
	.set	LSF188,0x40
	.set	LSI189,0x3c71
	.set	LSF189,0xff
	.set	LSI190,0x0018
	.set	LSF190,0xc0
	.set	LSI191,0x247d
	.set	LSF191,0xff
	.set	LSI192,0x247d
	.set	LSF192,0xef
	.set	LSI193,0x247d
	.set	LSF193,0xef
	.set	LSI194,0x247d
	.set	LSF194,0xeb
	.set	LSI195,0x247d
	.set	LSF195,0xe3
	.set	LSI196,0x047d
	.set	LSF196,0xe3
	.set	LSI197,0x047d
	.set	LSF197,0xc3
	.set	LSI198,0x047d
	.set	LSF198,0xcb
	.set	LSI199,0x047d
	.set	LSF199,0x8f
	.set	LSI200,0x047d
	.set	LSF200,0x41
	.set	LSI201,0x047d
	.set	LSF201,0xc1
	.set	LSI202,0x307d
	.set	LSF202,0xcf
	.set	LSI203,0x307d
	.set	LSF203,0xc7
	.set	LSI204,0x307d
	.set	LSF204,0xc3
	.set	LSI205,0x3c7d
	.set	LSF205,0xc3
	.set	LSI206,0x3c7d
	.set	LSF206,0xc9
	.set	LSI207,0x3c7d
	.set	LSF207,0xc1
	.set	LSI208,0x3c7d
	.set	LSF208,0xa1
	.set	LSI209,0x307d
	.set	LSF209,0xc1
	.set	LSI210,0x3c7d
	.set	LSF210,0xe1
	.set	LSI211,0x3c7d
	.set	LSF211,0xc9
	.set	LSI212,0x3c1d
	.set	LSF212,0xc0
	.set	LSI213,0x3c7d
	.set	LSF213,0xc9
	.set	LSI214,0x3c7d
	.set	LSF214,0xc8
	.set	LSI215,0x3c7d
	.set	LSF215,0xc1
	.set	LSI216,0x3c7d
	.set	LSF216,0xc8
	.set	LSI217,0x3c7d
	.set	LSF217,0xc0
	.set	LSI218,0x3c1d
	.set	LSF218,0xc2
	.set	LSI219,0x3c1d
	.set	LSF219,0xc2
	.set	LSI220,0x3c1d
	.set	LSF220,0xc2
	.set	LSI221,0x3c1d
	.set	LSF221,0x42
	.set	LSI222,0x3c1d
	.set	LSF222,0xc0
	.set	LSI223,0x3c1d
	.set	LSF223,0xc0
	.set	LSI224,0x3c1d
	.set	LSF224,0xc0
	.set	LSI225,0x3c1d
	.set	LSF225,0xe0
	.set	LSI226,0x3c1d
	.set	LSF226,0xc0
	.set	LSI227,0x3c1d
	.set	LSF227,0xc0
	.set	LSI228,0x3c1d
	.set	LSF228,0xc0
	.set	LSI229,0x3c1d
	.set	LSF229,0xc0
	.set	LSI230,0x3c1d
	.set	LSF230,0xd0
	.set	LSI231,0x3c1d
	.set	LSF231,0xd0
	.set	LSI232,0x3c1d
	.set	LSF232,0xd0
	.set	LSI233,0x3c1d
	.set	LSF233,0xd0
	.set	LSI234,0x3c1d
	.set	LSF234,0xd0
	.set	LSI235,0x3c1d
	.set	LSF235,0xd0
	.set	LSI236,0x3c1d
	.set	LSF236,0xe0
	.set	LSI237,0x3c1d
	.set	LSF237,0xcc
	.set	LSI238,0x3c1d
	.set	LSF238,0xcd
	.set	LSI239,0x3c1d
	.set	LSF239,0xcd
	.set	LSI240,0x3c1d
	.set	LSF240,0xcc
	.set	LSI241,0x3c1d
	.set	LSF241,0xcc
	.set	LSI242,0x3c1d
	.set	LSF242,0xc8
	.set	LSI243,0x3c05
	.set	LSF243,0xc0
	.set	LSI244,0x3c1d
	.set	LSF244,0x48
	.set	LSI245,0x3c05
	.set	LSF245,0xc0
	.set	LSI246,0x3c05
	.set	LSF246,0xc0
	.set	LSI247,0x3c0d
	.set	LSF247,0xc0
	.set	LSI248,0x3c0d
	.set	LSF248,0xc0
	.set	LSI249,0x3c05
	.set	LSF249,0xc0
	.set	LSI250,0x3c0d
	.set	LSF250,0x50
	.set	LSI251,0x3c05
	.set	LSF251,0xc0
	.set	LSI252,0x3c05
	.set	LSF252,0xc0
	.set	LSI253,0x3c05
	.set	LSF253,0xe1
	.set	LSI254,0x3c05
	.set	LSF254,0xe1
	.set	LSI255,0x3c05
	.set	LSF255,0xc1
	.set	LSI256,0x1c05
	.set	LSF256,0xc1
	.set	LSI257,0x1c05
	.set	LSF257,0xc1
	.set	LSI258,0x1c11
	.set	LSF258,0xc1
	.set	LSI259,0x0419
	.set	LSF259,0xc1
	.set	LSI260,0x041d
	.set	LSF260,0xc9
	.set	LSI261,0x041d
	.set	LSF261,0xd9
	.set	LSI262,0x001d
	.set	LSF262,0xdb
	.set	LSI263,0x001d
	.set	LSF263,0xdf
	.set	LSI264,0x001d
	.set	LSF264,0xff
	.set	LSI265,0x0018
	.set	LSF265,0xc0
	.set	LSI266,0x3c7d
	.set	LSF266,0xc3
	.set	LSI267,0x1c01
	.set	LSF267,0xc0
	.set	LSI268,0x1c01
	.set	LSF268,0xc0
	.set	LSI269,0x1c01
	.set	LSF269,0x80
	.set	LSI270,0x1c01
	.set	LSF270,0x40
	.set	LSI271,0x1c01
	.set	LSF271,0xc0
	.set	LSI272,0x1001
	.set	LSF272,0xc0
	.set	LSI273,0x1401
	.set	LSF273,0x80
	.set	LSI274,0x1401
	.set	LSF274,0xc0
	.set	LSI275,0x1c01
	.set	LSF275,0x80
	.set	LSI276,0x1c01
	.set	LSF276,0x80
	.set	LSI277,0x1c01
	.set	LSF277,0x40
	.set	LSI278,0x0401
	.set	LSF278,0xc0
	.set	LSI279,0x3c01
	.set	LSF279,0xc0
	.set	LSI280,0x3c01
	.set	LSF280,0xc0
	.set	LSI281,0x3c01
	.set	LSF281,0xc0
	.set	LSI282,0x0018
	.set	LSF282,0xc0
	.set	LSI283,0x201d
	.set	LSF283,0xff
	.set	LSI284,0x301d
	.set	LSF284,0xef
	.set	LSI285,0x3c1d
	.set	LSF285,0xcf
	.set	LSI286,0x3c1d
	.set	LSF286,0xcf
	.set	LSI287,0x3c1d
	.set	LSF287,0xcb
	.set	LSI288,0x3c1d
	.set	LSF288,0xcb
	.set	LSI289,0x3c1d
	.set	LSF289,0xc3
	.set	LSI290,0x3c1d
	.set	LSF290,0x43
	.set	LSI291,0x3c1d
	.set	LSF291,0xc3
	.set	LSI292,0x3415
	.set	LSF292,0xff
	.set	LSI293,0x3415
	.set	LSF293,0xff
	.set	LSI294,0x0018
	.set	LSF294,0xc0
	.set	LSI295,0x3c1d
	.set	LSF295,0xc3
	.set	LSI296,0x3c1d
	.set	LSF296,0x83
	.set	LSI297,0x3c1d
	.set	LSF297,0xc3
	.set	LSI298,0x3c1d
	.set	LSF298,0xc3
	.set	LSI299,0x3415
	.set	LSF299,0x83
	.set	LSI300,0x3415
	.set	LSF300,0x43
	.set	LSI301,0x3415
	.set	LSF301,0x07
	.set	LSI302,0x3415
	.set	LSF302,0x45
	.set	LSI303,0x3415
	.set	LSF303,0x41
	.set	LSI304,0x3415
	.set	LSF304,0x45
	.set	LSI305,0x3415
	.set	LSF305,0x05
	.set	LSI306,0x3415
	.set	LSF306,0x00
	.set	LSI307,0x3415
	.set	LSF307,0x40
	.set	LSI308,0x3415
	.set	LSF308,0x41
	.set	LSI309,0x3415
	.set	LSF309,0x40
	.set	LSI310,0x3415
	.set	LSF310,0x42
	.set	LSI311,0x3415
	.set	LSF311,0x42
	.set	LSI312,0x3415
	.set	LSF312,0x40
	.set	LSI313,0x0018
	.set	LSF313,0xc0
	.set	LSI314,0x3c1d
	.set	LSF314,0xcc
	.set	LSI315,0x3c1d
	.set	LSF315,0x4c
	.set	LSI316,0x3c1d
	.set	LSF316,0x84
	.set	LSI317,0x3c1d
	.set	LSF317,0x44
	.set	LSI318,0x3c1d
	.set	LSF318,0x04
	.set	LSI319,0x3c1d
	.set	LSF319,0x04
	.set	LSI320,0x3c1d
	.set	LSF320,0x04
	.set	LSI321,0x3c1d
	.set	LSF321,0x84
	.set	LSI322,0x3c1d
	.set	LSF322,0x84
	.set	LSI323,0x3c1d
	.set	LSF323,0x04
	.set	LSI324,0x3c1d
	.set	LSF324,0x44
	.set	LSI325,0x3c1d
	.set	LSF325,0x4c
	.set	LSI326,0x3c1d
	.set	LSF326,0xcc
	.set	LSI327,0x3c1d
	.set	LSF327,0xcc
	.set	LSI328,0x3c1d
	.set	LSF328,0x4c
	.set	LSI329,0x3c1d
	.set	LSF329,0xc8
	.set	LSI330,0x3c1d
	.set	LSF330,0xc8
	.set	LSI331,0x3c1d
	.set	LSF331,0xc8
	.set	LSI332,0x3c1d
	.set	LSF332,0xc8
	.set	LSI333,0x3c1d
	.set	LSF333,0x48
	.set	LSI334,0x3c1d
	.set	LSF334,0xc0
	.set	LSI335,0x3c1d
	.set	LSF335,0xc0
	.set	LSI336,0x3c1d
	.set	LSF336,0xc0
	.set	LSI337,0x3c1d
	.set	LSF337,0xc0
	.set	LSI338,0x3c1d
	.set	LSF338,0xc0
	.set	LSI339,0x3c1d
	.set	LSF339,0xc0
	.set	LSI340,0x3c1d
	.set	LSF340,0xc0
	.set	LSI341,0x3c1d
	.set	LSF341,0xc0
	.set	LSI342,0x3c1d
	.set	LSF342,0xc0
	.set	LSI343,0x3c1d
	.set	LSF343,0xc0
	.set	LSI344,0x3c1d
	.set	LSF344,0xd0
	.set	LSI345,0x1c1d
	.set	LSF345,0xf0
	.set	LSI346,0x3c1d
	.set	LSF346,0xf0
	.set	LSI347,0x3c1d
	.set	LSF347,0xf0
	.set	LSI348,0x3c1d
	.set	LSF348,0xb0
	.set	LSI349,0x3c1d
	.set	LSF349,0xb0
	.set	LSI350,0x3c1d
	.set	LSF350,0xb8
	.set	LSI351,0x3c1d
	.set	LSF351,0x98
	.set	LSI352,0x3c1d
	.set	LSF352,0xb8
	.set	LSI353,0x3c1d
	.set	LSF353,0x39
	.set	LSI354,0x3c1d
	.set	LSF354,0x39
	.set	LSI355,0x3c1d
	.set	LSF355,0x39
	.set	LSI356,0x3c1d
	.set	LSF356,0x3b
	.set	LSI357,0x3c1d
	.set	LSF357,0x3f
	.set	LSI358,0x3c1d
	.set	LSF358,0x3f
	.set	LSI359,0x3c1d
	.set	LSF359,0x3f
	.set	LSI360,0x0c1d
	.set	LSF360,0x7f
	.set	LSI361,0x041d
	.set	LSF361,0xff
	.set	LSI362,0x0018
	.set	LSF362,0xc0
	.set	LSI363,0x3c1d
	.set	LSF363,0xcf
	.set	LSI364,0x3c1d
	.set	LSF364,0xce
	.set	LSI365,0x0001
	.set	LSF365,0xc0
	.set	LSI366,0x0001
	.set	LSF366,0x40
	.set	LSI367,0x0001
	.set	LSF367,0x40
	.set	LSI368,0x2001
	.set	LSF368,0x40
	.set	LSI369,0x2001
	.set	LSF369,0xc0
	.set	LSI370,0x3001
	.set	LSF370,0xc0
	.set	LSI371,0x3001
	.set	LSF371,0x40
	.set	LSI372,0x3001
	.set	LSF372,0x40
	.set	LSI373,0x3401
	.set	LSF373,0x40
	.set	LSI374,0x3401
	.set	LSF374,0xc0
