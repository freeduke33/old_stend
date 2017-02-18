.model large

public AR_SS_2

.data

extrn _ax:qword
extrn _ay:qword
extrn _axy:qword

PMAX	equ	6

_XX		equ	_ax
_YY		equ	_ay
_XY		equ	_axy

_00_		equ	0
_01_		equ	8
_02_		equ	(8*2)
_03_		equ	(8*3)
_04_		equ	(8*4)
_05_		equ	(8*5)
_06_		equ	(8*6)
_07_		equ	(8*7)
_08_		equ	(8*8)
_09_		equ	(8*9)
_010_		equ	(8*10)
_011_		equ	(8*11)
_012_		equ	(8*12)
_013_		equ	(8*13)
_014_		equ	(8*14)
_015_		equ	(8*15)

_10_		equ	(PMAX*8)
_11_		equ	((PMAX*8)+8)
_12_		equ	((PMAX*8)+16)
_20_		equ	((PMAX*2*8))
_21_		equ	((PMAX*2*8)+8)
_22_		equ	((PMAX*2*8)+16)


XX_00		equ	[qword ptr ds:offset _00_+_XX]
XX_01		equ	[qword ptr ds:offset _01_+_XX]
XX_02		equ	[qword ptr ds:offset _02_+_XX]
XX_10		equ	[qword ptr ds:offset _10_+_XX]
XX_11		equ	[qword ptr ds:offset _11_+_XX]
XX_12		equ	[qword ptr ds:offset _12_+_XX]
XX_20		equ	[qword ptr ds:offset _20_+_XX]
XX_21		equ	[qword ptr ds:offset _21_+_XX]
XX_22		equ	[qword ptr ds:offset _22_+_XX]


XY_00		equ	[qword ptr ds:offset _00_+_XY]
XY_01		equ	[qword ptr ds:offset _01_+_XY]
XY_02		equ	[qword ptr ds:offset _02_+_XY]
XY_10		equ	[qword ptr ds:offset _10_+_XY]
XY_11		equ	[qword ptr ds:offset _11_+_XY]
XY_12		equ	[qword ptr ds:offset _12_+_XY]
XY_20		equ	[qword ptr ds:offset _20_+_XY]
XY_21		equ	[qword ptr ds:offset _21_+_XY]
XY_22		equ	[qword ptr ds:offset _22_+_XY]

YY_00		equ	[qword ptr ds:offset _00_+_YY]
YY_01		equ	[qword ptr ds:offset _01_+_YY]
YY_02		equ	[qword ptr ds:offset _02_+_YY]
YY_10		equ	[qword ptr ds:offset _10_+_YY]
YY_11		equ	[qword ptr ds:offset _11_+_YY]
YY_12		equ	[qword ptr ds:offset _12_+_YY]
YY_20		equ	[qword ptr ds:offset _20_+_YY]
YY_21		equ	[qword ptr ds:offset _21_+_YY]
YY_22		equ	[qword ptr ds:offset _22_+_YY]

extrn _c:qword
extrn _lam:qword

C0		equ	[qword ptr ds:offset _00_+_c]
C1		equ	[qword ptr ds:offset _01_+_c]
C2		equ	[qword ptr ds:offset _02_+_c]
Lam1		equ	[qword ptr ds:offset _01_+_lam]
Lam2		equ	[qword ptr ds:offset _02_+_lam]

Fi		dq	10 dup(0)
Psi		dq	20 dup(0)
q		dq	10 dup(0)


Q0		equ	[qword ptr ds:offset _00_+q]
Q1		equ	[qword ptr ds:offset _01_+q]
Q2		equ	[qword ptr ds:offset _02_+q]
Q3		equ	[qword ptr ds:offset _03_+q]
Q4		equ	[qword ptr ds:offset _04_+q]
Q5		equ	[qword ptr ds:offset _05_+q]
Q6		equ	[qword ptr ds:offset _06_+q]


FI0		equ	[qword ptr ds:offset _00_+Fi]
FI1		equ	[qword ptr ds:offset _01_+Fi]
FI2		equ	[qword ptr ds:offset _02_+Fi]
FI3		equ	[qword ptr ds:offset _03_+Fi]
FI4		equ	[qword ptr ds:offset _04_+Fi]
FI5		equ	[qword ptr ds:offset _05_+Fi]

PSI0		equ	[qword ptr ds:offset _01_+Psi]
PSI1		equ	[qword ptr ds:offset _02_+Psi]
PSI2		equ	[qword ptr ds:offset _03_+Psi]
PSI3		equ	[qword ptr ds:offset _04_+Psi]
PSI4		equ	[qword ptr ds:offset _05_+Psi]
PSI5		equ	[qword ptr ds:offset _06_+Psi]
PSI6		equ	[qword ptr ds:offset _07_+Psi]
PSI7		equ	[qword ptr ds:offset _08_+Psi]
PSI8		equ	[qword ptr ds:offset _09_+Psi]
PSI9		equ	[qword ptr ds:offset _010_+Psi]
PSI10		equ	[qword ptr ds:offset _011_+Psi]
PSI11		equ	[qword ptr ds:offset _012_+Psi]
PSI12		equ	[qword ptr ds:offset _013_+Psi]
PSI13		equ	[qword ptr ds:offset _014_+Psi]
PSI14		equ	[qword ptr ds:offset _015_+Psi]



.code
IDEAL

proc pascal AR_SS_2
	fld	XX_11
	fld	XX_21
	fdiv	st(0),st(1)
	fchs
; E0,XX_11
	fld	XY_10
	fdiv	st(0),st(2)
	fchs
; D0,E0,XX_11
	fld	XY_11
	fdiv	st(0),st(3)
	fchs
; D1,D0,E0,XX_11
	fxch	st(3)
	fld	XY_12
	fxch	st(1)
	fdiv
	fchs
; D2,D0,E0,D1
	fld	XX_10
	fld	st(2)
	fmul	st(0),st(1)
	fadd	XY_00
	fstp	FI0
; XX_10,D2,D0,E0,D1
	fld	st(4)
	fmul	st(0),st(1)
	fadd	XY_01
	fstp	FI1

	fmul	st(0),st(1)
	fadd	XY_02
	fstp	FI2
; D2,D0,E0,D1
	fld	XX_21
	fld	st(2)
	fmul	st(0),st(1)
	fadd	XY_20
	fstp	FI3
; XX_21,D2,D0,E0,D1
	fld	st(4)
	fmul	st(0),st(1)
	fadd	XY_21
	fstp	FI4

	fmul	st(0),st(1)
	fadd	XY_22
	fstp	FI5
; D2,D0,E0,D1
	fld	XY_12
	fxch	st(1)
	fmul	st(0),st(1)
	fadd	YY_22
	fstp	PSI5
; XY_12,D0,E0,D1
	fld	st(3)
	fmul	st(0),st(1)
	fadd	YY_21
	fstp	PSI4

	fmul	st(0),st(1)
	fadd	YY_20
	fstp	PSI3
; D0,E0,D1
	fld	XY_11
	fld	st(3)
	fmul	st(0),st(1)
	fadd	YY_11
	fstp	PSI2
; XY_11,D0,E0,D1
	fmul	st(0),st(1)
	fadd	YY_10
	fstp	PSI1

	fmul	XY_10
	fadd	YY_00
	fstp	PSI0
	ffree	st(1)
; E0
	fld	st(0)
	fmul	XX_10
	fadd	XX_20
	fst	Q0
; Q0,E0
	fxch	st(1)
	fmul	XX_21
	fadd	XX_22
	fst	Q1
; Q1,Q0
	fxch	st(1)
	fld	FI3
	fdiv	st(0),st(2)
	fchs
; D3,Q0,Q1
	fld	FI4
	fdiv	st(0),st(3)
	fchs
; D4,D3,Q0,Q1
	fld	FI5
	fdiv	st(0),st(4)
	fchs
	ffree	st(4)
; D5,D4,D3,Q0
	fld	st(2)
	fmul	FI3
	fadd	PSI0
	fst	PSI6
; PSI6,D5,D4,D3,Q0
	fld	st(3)
	fmul	FI4
	fadd	PSI1
	fst	PSI7
; PSI7,PSI6,D5,D4,D3,Q0
	fld	st(4)
	fmul	FI5
	fadd	PSI3
	fst	PSI8
; PSI8,PSI7,PSI6,D5,D4,D3,Q0
	fld	st(4)
	fmul	FI4
	fadd	PSI2
	fstp	PSI9

	fld	st(4)
	fmul	FI5
	fadd	PSI4
	fstp	PSI10

	fld	st(3)
	fmul	FI5
	fadd	PSI5
	fstp	PSI11

	fxch	st(2)
	fchs
	fdiv	st(2),st(0)
	fdiv
; D6,D7,D5,D4,D3,Q0
	fld	st(5)
	fmul	st(0),st(5)
	fadd	FI0
; Q2,D6,D7,D5,D4,D3,Q0
	fld	st(6)
	ffree	st(7)
	ffree	st(6)
; Q0,Q2,D6,D7,D5,D4
	fxch	st(5)
	fmul	st(0),st(5)
	fadd	FI1
; Q3,Q2,D6,D7,D5,Q0
	fld	st(5)
	fmul	st(0),st(5)
	fadd	FI2
	ffree	st(6)
	ffree	st(5)
; Q4,Q3,Q2,D6,D7
	fld	st(2)
	fmul	st(0),st(5)
	fadd
; Q6,Q3,Q2,D6,D7
	fxch	st(1)
; Q3,Q6,Q2,D6,D7
	fld	st(2)
	fmul	st(0),st(4)
	fadd
; Q5,Q6,Q2,D6,D7
	fld	st(3)
	fmul	PSI7
	fadd	PSI9
	fst	PSI12
; PSI12,Q5,Q6,Q2,D6,D7
	fld	st(4)
	fmul	PSI8
	fadd	PSI10
	fst	PSI13
; PSI13,PSI12,Q5,Q6,Q2,D6,D7
	fld	st(6)
	fmul	PSI8
	fadd	PSI11
	fstp	PSI14
	ffree	st(6)
	ffree	st(5)
; PSI13,PSI12,Q5,Q6,Q2
	fxch	st(1)
	fdiv
; D8,Q5,Q6,Q2
	fxch	st(2)
	fxch	st(1)
	fst	Q5
; Q5,Q6,D8,Q2
	fmul	st(0),st(2)
	fsub
; Q6-Q5*D8,D8,Q2
	fxch	st(1)
	fmul	PSI13
	fchs
	fadd	PSI14
	fdiv
; C2,Q2
	fld	st(0)
	fmul	PSI13
	fchs
	fadd	Q5
	fdiv	PSI12
; C1,C2,Q2
	fxch	st(2)
; Q2,C2,C1
	fld	st(2)
	fmul	PSI7
	fld	st(2)
	fmul	PSI8
	fadd
	fsub
	fdiv	PSI6
; C0,C2,C1
	fld	st(2)
	fmul	FI4
	fld	st(2)
	fmul	FI5
	fld	st(2)
	fmul	FI3
	fadd
	fadd
	fchs
	fadd	Q0
	fdiv	Q1
; Lam2,C0,C2,C1
	fld	st(3)
	fmul	XY_11
	fld	st(3)
	fmul	XY_12
	fadd
	fld	st(2)
	fmul	XY_10
	fld	st(2)
	fmul	XX_21
	fadd
	fadd
	fchs
	fadd	XX_10
	fdiv	XX_11
; Lam1,Lam2,C0,C2,C1
	fstp	Lam1
	fstp	Lam2
	fstp	C0
	fstp	C2
	fstp	C1
;
	ret
endp

end

