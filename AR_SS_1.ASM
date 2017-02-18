.model large

public AR_SS_1

.data

extrn _ax:qword
extrn _ay:qword
extrn _axy:qword

PMAX	equ	6

_XX		equ	_ax
_YY		equ	_ay
_XY		equ	_axy

_00_		equ	0
_10_		equ	(PMAX*8)
_01_		equ	8
_11_		equ	((PMAX*8)+8)

XX_00		equ	[qword ptr ds:offset _00_+_XX]
XX_10		equ	[qword ptr ds:offset _10_+_XX]
XX_11		equ	[qword ptr ds:offset _11_+_XX]

YY_00		equ	[qword ptr ds:offset _00_+_YY]
YY_10		equ	[qword ptr ds:offset _10_+_YY]
YY_11		equ	[qword ptr ds:offset _11_+_YY]

XY_00		equ	[qword ptr ds:offset _00_+_XY]
XY_01		equ	[qword ptr ds:offset _01_+_XY]
XY_10		equ	[qword ptr ds:offset _10_+_XY]
XY_11		equ	[qword ptr ds:offset _11_+_XY]

extrn _lam:qword
extrn _c:qword

C0		equ	[qword ptr ds:offset   _c]
C1		equ	[qword ptr ds:offset 8+_c]
Lambda		equ	[qword ptr ds:offset 8+_lam]

.code
IDEAL

proc pascal AR_SS_1
	fld	XX_11
	fld	XY_10
	fld	st(0)
	fdiv	st(0),st(2)
; D0,XY_10,XX_11
	fld	XY_11
	fld	st(0)
	fdiv	st(0),st(4)
	ffree	st(4)
; D1,XY_11,D0,XY_10
	fld	st(3)
	fmul	st(0),st(3)
	fchs
	fadd	YY_00
	ffree	st(4)
; FI0,D1,XY_11,D0
	fld	st(2)
	fmul	st(0),st(4)
	fchs
	fadd	YY_10
;FI1,FI0,D1,XY_11,D0
	fld	st(3)
	fmul	st,st(3)
	fchs
	fadd	YY_11
;FI2,FI1,FI0,D1,XY_11,D0
	ffree	st(4)
	fld	XX_10
	fst	st(5)
;XX_10,FI2,FI1,FI0,D1,XX_10,D0
	fmul	st,st(6)
	fchs
	fadd	XY_00
; PSI0,FI2,FI1,FI0,D1,XY_10,D0
	fld	st(5)
	fmul	st,st(5)
	fchs
	fadd	XY_01
; PSI1,PSI0,FI2,FI1,FI0,D1,XY_10,D0
	ffree	st(7)
	ffree	st(6)
	ffree	st(5)
; PSI1,PSI0,FI2,FI1,FI0
	fxch	st(1)
	fdiv	st(0),st(4)
; B0,PSI1,FI2,FI1,FI0
	fld	st(3)
	fdiv	st(0),st(5)
	ffree	st(5)
; B1,B0,PSI1,FI2,FI1
	fld	st(0)
	fmul	st(0),st(5)
	fsubp	st(4),st(0)
	fxch	st(1)
; B0,B1,PSI1,FI2-B1*FI1,FI1
	fmul	st(4),st(0)
	fxch	st(2)
	fsub	st(0),st(4)
; PSI1-B0*FI1,B1,B0,FI2-B1*FI1,FI1*B0
	fdiv	st(0),st(3)
	ffree	st(4)
	ffree	st(3)
; C1,B1,B0
	fxch	st(1)
	fmul	st(0),st(1)
	fsubp	st(2),st(0)
; C1,C0
	fld	st(1)
	fchs
	fmul	XY_10
	fld	st(1)
	fchs
	fmul	XY_11
	fadd
	fadd	XX_10
	fdiv	XX_11
;
	fstp	Lambda
	fstp	C1
	fstp	C0
	ret
endp

end

