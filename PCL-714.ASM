
     .MODEL  LARGE
     MASM51

     PUBLIC  _enable714,_enable714w,_disable714,USE714,SETLOOPFUN
     PUBLIC  _BufTim,_NumDigit,_DivCount

HI_base = 12h
LO_base = 2870h
;****************************
;   Базовые  адреса  плат
;****************************
base714 EQU     0200h                  ;  PCL-714

     .DATA
_FlagIO	 	db	0
_BufTim	 	dw	0
_DivCount 	dw	1
_BufADC  	dw	100 dup(0)
_NumDigit	dw	0
_BufDAC	 	dw	100 dup(0)

sav2         	dd	0

BufDAC    DW    offset _BufDAC ,seg _BufDAC
BufADC    DW    offset _BufADC ,seg _BufADC

ABSOL   SEGMENT WORD AT 0
        ORG     70H
VEC     LABEL   WORD
ABSOL   ENDS

     .CODE

old_ds  MACRO
     mov        ax, DGROUP
     mov        ds, ax
     ASSUME     DS:DGROUP
ENDM

act_fl	db	0


proc pascal USE714 far
arg dac:word
	cmp	cs:act_fl,0
	je	exit
	mov	ax,[dac]
	mov	_BufDAC,ax
	mov	_FlagIO,0
wait_ok:
	cmp	_FlagIO,0
	je	wait_ok
exit:
	mov	ax,_BufADC
	ret
endp USE714


_disable714  PROC    FAR
	push	ds
	cmp	cs:act_fl,0
	je	exit1
	dec	cs:act_fl

;********************************************
;   Восстановление старой частоты таймера
;********************************************
	cli
        mov     al, 0FFH
        out     040H, al
        out     040H, al
	sti

	lds	dx,dword ptr sav2
	mov	ax,251ch
	int	21h

;*****************************************
;   Обнуление счетчика таймерных тиков
;*****************************************
   ;     xor  ax, ax
   ;     xor  dx, dx
   ;     mov    Word Ptr [FlIRQ], ax   ;  мл. слово
   ;     mov    Word Ptr [FlIRQ+2], dx   ;  ст. слово

exit1:
	pop	ds
	ret
_disable714  ENDP

_enable714     PROC FAR
	cmp	cs:act_fl,0
	je	cont1
	call	_disable714
cont1:
	inc	cs:act_fl
	mov	dx,offset Time_int
	call    SetTimeInrpt
	ret
_enable714     ENDP

_enable714w     PROC FAR
	cmp	cs:act_fl,0
	je	cont2
	call	_disable714
cont2:
	inc	cs:act_fl
	mov	dx,offset Time_wait
	call    SetTimeInrpt
	ret
_enable714w     ENDP

;
; cs:dx - timer procedure
;
SetTimeInrpt  PROC  NEAR
	push	ds
	mov	ax,351ch
	int	21h
	mov	word ptr sav2,bx
	mov	word ptr [sav2+2],es

	push	cs
	pop	ds
	mov	ah,25h
	int	21h
	pop	ds
;***************************************
;	Изменение частоты таймера.
;***************************************
	cli
	mov     ax, _BufTim
        out     040H, al
        mov     al, ah
        out     040H, al
	mov	ax,_DivCount
	mov	cs:count,ax
	sti
     	ret
SetTimeInrpt  ENDP

SETLOOPFUN proc pascal far
arg fun:dword
	push	dword ptr [fun]
	pop	dword ptr cs:[LoopFun]
	ret
SETLOOPFUN endp

None proc
	iret
None endp

;********************************************************
;   П/пр-ма обработки прерывания от системного таймера
;********************************************************

flag	db	0
count	dw	0
LoopFun	dw	offset None,seg None

Time_int  PROC
	cmp	cs:flag,1
	jne	continue
no_cont:
	iret
continue:
	dec	cs:count
	jnz	no_cont
	mov	cs:flag,1
     	push	ax
	push	bx
	push	cx
	push	dx
	push	di
	push	es
     	push    ds

	cli
	mov     al, 020H            ; сброс регистра запросов контроллера
	OUT     020H, AL            ;        прерываний 8259
	sti


;***************************
;          ЦАП
;***************************

	mov        ax, DGROUP
	mov        ds, ax
	ASSUME     DS:DGROUP


	mov     dx, base714 + 13
	mov     al, 1            ;  DigOut 0 = 1  - трассировка
;     	out     dx, al          ;

 ; *************************************
 ;        Вывод числа на ЦАП
 ; *************************************
	les	bx,Dword Ptr BufDAC
	les	di,Dword Ptr BufADC

	mov	ax,Word Ptr es:[bx]

 ; *************************************
 ;    Выводим на ЦАП
 ; *************************************
	mov	dx, base714+4   ; мл. байт ->на 0-канал ЦАП
	out     dx, al
	mov     al, ah
	mov     dx, base714+5   ; ст. байт ->на 0-канал ЦАП
	out     dx, al

 ; *************************************
 ;    Читаем с АЦП
 ; *************************************

	mov     al, 11
	mov     dx, base714+10   ; выбор канала АЦП
	out     dx, al

     mov        dx, base714+11
     mov        al, 1            ; старт с'ема АЦП
     out        dx, al

     mov        dx, base714+5
wait:                             ; ждем конца преобразования
     in         al, dx
     cmp        al, 40H
     jge        wait

     mov        ah, al          ; ст. байт АЦП

     mov        dx, base714+4
     in         al, dx		; мл. байт АЦП

     stosw

     mov     dx, base714 + 13
     mov     al, 0            ;  DigOut 0 = 0  - трассировка
     out     dx, al           ;

     mov	_FlagIO,1
     inc	_NumDigit
     mov	ax,_DivCount
     mov	cs:count,ax


     	pop     ds
     	pop     es
	pop	di
     	pop     dx
	pop	cx
	pop	bx
	pop	ax
	pushf
	call	dword ptr cs:[LoopFun]
	mov	cs:flag,0
        iret
Time_int  ENDP

Time_wait proc
	assume ds:DGROUP
	dec	cs:count
	jnz	no_cont1
	push	ds
	push	ax
	mov	ax,DGROUP
	mov	ds,ax
	mov	_FlagIO,1
	inc	_NumDigit
	mov	ax,_DivCount
	mov	cs:count,ax
	pushf
	call	dword ptr cs:[LoopFun]
	pop	ax
	pop	ds
no_cont1:
        iret
endp

     END






