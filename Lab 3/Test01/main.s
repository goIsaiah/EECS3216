;;;-----------------------------------------
;;; Start MC68HC12 gcc assembly output
;;; gcc compiler 3.3.6-m68hc1x-20060122
;;; Command:	C:\gcc-hcs12\usr\bin\..\lib\gcc-lib\m6811-elf\3.3.6-m68hc1x-20060122\cc1.exe -quiet -I./include -iprefix C:\gcc-hcs12\usr\bin\../lib/gcc-lib/m6811-elf\3.3.6-m68hc1x-20060122\ -D__GNUC__=3 -D__GNUC_MINOR__=3 -D__GNUC_PATCHLEVEL__=6 -D__mc68hc1x__ -D__mc68hc1x -D__HAVE_SHORT_INT__ -D__INT__=16 -Dmc6812 -DMC6812 -Dmc68hc12 main.c -quiet -dumpbase main.c -m68hc12 -mshort -auxbase main -O3 -std=c99 -fno-ident -fno-common -fomit-frame-pointer -o main.s
;;; Compiled:	Mon Mar 03 13:17:28 2025
;;; (META)compiled by GNU C version 3.3.2.
;;;-----------------------------------------
	.file	"main.c"
	.mode mshort
	.globl	segment_decoder
	.sect	.data
	.type	segment_decoder, @object
	.size	segment_decoder, 36
segment_decoder:
	.word	63
	.word	6
	.word	91
	.word	79
	.word	102
	.word	109
	.word	125
	.word	7
	.word	127
	.word	111
	.word	119
	.word	124
	.word	57
	.word	94
	.word	121
	.word	113
	.word	61
	.word	118
	.globl	keypad
	.section	.rodata
	.type	keypad, @object
	.size	keypad, 16
keypad:
	.byte	49
	.byte	50
	.byte	51
	.byte	65
	.byte	52
	.byte	53
	.byte	54
	.byte	66
	.byte	55
	.byte	56
	.byte	57
	.byte	67
	.byte	42
	.byte	48
	.byte	35
	.byte	68
	.globl	digit_decoder
	.sect	.data
	.type	digit_decoder, @object
	.size	digit_decoder, 8
digit_decoder:
	.word	7
	.word	11
	.word	13
	.word	14
	.globl	num1
	.type	num1, @object
	.size	num1, 2
num1:
	.word	0
	.globl	num2
	.type	num2, @object
	.size	num2, 2
num2:
	.word	0
	.globl	opCode
	.type	opCode, @object
	.size	opCode, 2
opCode:
	.word	0
	.globl	res
	.type	res, @object
	.size	res, 2
res:
	.word	3
	; extern	mSDelay
	.sect	.text
	.globl	main
	.type	main,@function
main:
	leas	-7,sp
	movb	#-1,3
	bset	618, #2
	bclr	616, #2
	bset	602, #15
	movb	#15,2
.L5:
	bset	0, #15
	ldab	0
	andb	#-16
	beq	.L5
	stab	row
.L63:
	ldd	#1
	bsr	mSDelay
	ldab	0
	andb	#-16
	stab	row
	beq	.L63
	ldd	#15
	bsr	mSDelay
	ldab	0
	andb	#-16
	stab	row
	beq	.L63
	bclr	0, #15
	bset	0, #1
	ldab	0
	andb	#-16
	stab	row
	bne	.L59
	bclr	0, #15
	bset	0, #2
	ldab	0
	andb	#-16
	stab	row
	bne	.L60
	bclr	0, #15
	bset	0, #4
	ldab	0
	andb	#-16
	stab	row
	bne	.L61
	bclr	0, #15
	bset	0, #8
	ldab	0
	andb	#-16
	stab	row
	beq	.L18
	movb	#3,column
.L18:
	clr	0,sp
	ldab	row
	cmpb	#16
	beq	.L65
	cmpb	#32
	beq	.L66
	cmpb	#64
	beq	.L67
	cmpb	#-128
	beq	.L68
.L26:
	ldy	num1
	cpy	#-1
	beq	.L32
	ldx	num2
	cpx	#-1
	beq	.L32
	ldab	0,sp
	addb	#-48
	cmpb	#9
	bhi	.L33
	ldd	#-48
	addb	0,sp
	adca	#0
	std	num1
	asld
	tfr	d,y
	ldab	segment_decoder+1,y
	stab	1
.L34:
	movb	digit_decoder+1,600
.L53:
	ldd	#15
	bsr	mSDelay
	bset	0, #15
	ldab	0
	andb	#-16
	stab	row
	bne	.L53
	bra	.L5
.L33:
	ldab	0,sp
	cmpb	#68
	beq	.L35
	cmpb	#65
	beq	.L69
	cmpb	#66
	beq	.L70
	cmpb	#67
	bne	.L34
	ldx	#0
	stx	num1
	stx	num2
	stx	opCode
	movw	#3,res
	clr	1
	bra	.L34
.L70:
	sty	num2
	clr	num1
	clr	num1+1
	movb	segment_decoder+23,1
	movw	#2,opCode
	bra	.L34
.L69:
	sty	num2
	clr	num1
	clr	num1+1
	movb	segment_decoder+21,1
	movw	#1,opCode
	bra	.L34
.L35:
	movb	segment_decoder+27,1
	ldd	opCode
	cpd	#1
	beq	.L71
	cpd	#2
	beq	.L72
.L43:
	movw	#-1,num1
	bra	.L34
.L72:
	tfr	y,d
	exg	x,y
	emul
	exg	x,y
	std	res
	bra	.L43
.L71:
	xgdy
	leax d,x
	xgdy
	stx	res
	bra	.L43
.L32:
	ldd	res
	bne	.L46
	movb	segment_decoder+1,1
	movb	digit_decoder+3,600
	bra	.L53
.L46:
	tfr	d,y
	clr	1,sp
	clr	2,sp
.L52:
	tfr	y,d
	ldx	#10
	idivs
	xgdx
	stx	5,sp
	std	3,sp
	tfr	x,d
	asld
	tfr	d,y
	ldab	segment_decoder+1,y
	stab	1
	ldd	1,sp
	asld
	ldy	1,sp
	iny
	sty	1,sp
	tfr	d,y
	ldab	digit_decoder+1,y
	stab	600
	ldd	#175
	bsr	mSDelay
	ldy	3,sp
	ldd	1,sp
	cpd	#4
	beq	.L53
	tbne	y,.L52
	bra	.L53
.L68:
	ldd	#keypad+12
.L64:
	addb	column
	adca	#0
	tfr	d,y
	ldab	0,y
	stab	0,sp
	bra	.L26
.L67:
	ldd	#keypad+8
	bra	.L64
.L66:
	ldd	#keypad+4
	bra	.L64
.L65:
	ldd	#keypad
	bra	.L64
.L61:
	movb	#2,column
	bra	.L18
.L60:
	movb	#1,column
	bra	.L18
.L59:
	clr	column
	bra	.L18
	.size	main, .-main
	.globl	mSDelay
	.type	mSDelay,@function
mSDelay:
	ldy	#0
	tbeq	d,.L85
.L83:
	ldx	#160
.L138:
	dbne	x,.L138
	iny
	pshd
	cpy	2,sp+
	blo	.L83
.L85:
	rts
	.size	mSDelay, .-mSDelay
	.globl	column
	.sect	.bss
	.type	column, @object
	.size	column, 1
column:
	.zero	1
	.globl	row
	.type	row, @object
	.size	row, 1
row:
	.zero	1
