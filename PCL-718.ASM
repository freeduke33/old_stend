.model LARGE,C
IDEAL
LOCALS
P286N

public EnablePCL,DisablePCL,UsePCL,EnablePCLw,NumDigit
public SetFuncPCL

DT_ACTIVE	=	1

BASE		=	300h
ADC_Lo		=	BASE+0
ADC_Hi		=	BASE+1

MUX		=	BASE+2		; beg - end ADC channel

DAC_Lo		=	BASE+4		; 0 DAC channel
DAC_Hi		=	BASE+5

STATUS		=	BASE+8
_EOC		=	10000000b	; end of convertion
_UNI		=	01000000b	; unipolar mode
_MUX		=	00100000b	; differential mode indicator
_INT		=	00010000b	; interrupt status
_CHANNEL	=	00001111b	; channel mask
CONTROL		=	BASE+9
_INTE		=	10000000b
_DMAE		=	00000100b
_ST_soft	=	00000000b
_ST_extern	=	00000010b
_ST_pacer	=	00000011b
PACER		=	BASE+10

TIMER_0		=	BASE+12
TIMER_1		=	BASE+13
TIMER_2		=	BASE+14
TIMER_SET	=	BASE+15

DT_STATUS	=	225h
_IBF		=       00000010b
_F0		=	00000100b


ERR_MULTI	=	5
ERR_TIMEOUT	=	4
ERR_DT_TEST	=	3
ERR_UNIPOLAR	=	2
ERR_NOPCL	=	1
SUCCESS		=	0

CODESEG

Active		db	0
savSeg		dw	0
savOfs		dw	0
NumDigit	dw	0
count		equ	NumDigit
dac_word	dw	0
adc_word	dw	0
flag		db	0
use		db	0
LoopFunc	dd	0
EmulFunc	dd	0

; ================[ Инициализация платы ]===============
proc EnablePCL
arg BufTim:word,DivCount:word
	mov	ax,ERR_MULTI
	cmp	[cs:Active],0
	je	@@ok
	jmp	@@exit

IFDEF DT_ACTIVE

@@timeout:
	sti
	mov	ax,ERR_TIMEOUT
	jmp	@@exit
@@ok:
; ==================[ DT2815 ]==============
	cli
	mov	dx,DT_STATUS
	out	dx,al         ; reset
	mov	ax,100h
@@stop:
	dec	ax
	jnz	@@stop

	in	al,dx
	and	al,11100001b	; get result of test
	jz	@@ini
	mov	ax,ERR_DT_TEST
	jmp	@@exit

	mov	cx,-1
@@ini:
	dec	cx
	jz	@@timeout
	in	al,dx
	test	al,_F0	; ready to read control word
	jz	@@ini

	mov	cx,-1
@@wait:
	dec	cx
	jz	@@timeout
	in	al,dx
	test	al,_IBF	; ready to read data
	jnz	@@wait


	dec	dx
	mov	al,00001000b
;			---    last channel
;		      --       output mode
;		   ---         reserved
	out	dx,al         ; init
	sti
ELSE
@@ok:
ENDIF
; ==========================================
	mov	dx,CONTROL
	mov	al,70h
	out	dx,al
	call	near ptr WaitProc
	in	al,dx
	cmp	al,70h
	je	@@active
	mov	ax,ERR_NOPCL
	jmp     @@exit
@@active:
	mov	dx,STATUS
	in	al,dx
	test	al,_UNI
	jz	@@bipolar
	mov	ax,ERR_UNIPOLAR
	jmp	@@exit
@@bipolar:
	mov	dx,MUX
	mov	al,077h
	out	dx,al

	mov	dx,CONTROL
	xor	al,al
	out	dx,al
	call	near ptr WaitProc

	mov	dx,TIMER_1
	mov	ax,-1
	out	dx,al
	mov	al,ah
	out	dx,al

	mov	dx,TIMER_SET
	mov	al,10111100b
	out	dx,al

	mov	dx,TIMER_2
	mov	ax,-1
	out	dx,al
	mov	al,ah
	out	dx,al

	mov	dx,PACER
	mov	al,0
	out	dx,al
	call	near ptr WaitProc

	mov	dx,STATUS
	out	dx,al
	call	near ptr WaitProc

; --------------------[ активизировать плату PCL ]----------------------
	push	ds
	push	es

	mov	ax,350Fh
	int	21h
	mov	[cs:savSeg],es
	mov	[cs:savOfs],bx

	mov	ax,cs
	mov	ds,ax
	lea	dx,[RunProc]
	mov	ax,250Fh
	int	21h

	pop	es
	pop	ds


	mov	dx,TIMER_SET
	mov	al,01111100b
;			  -  binary
;		       ---   divide by N count
;		     --      Lo first, then Hi counter bytes
;		   --        counter
	out	dx,al

	mov	dx,TIMER_1
	mov	ax,[DivCount]
	out	dx,al
	mov	al,ah
	out	dx,al

	mov	dx,TIMER_SET
	mov	al,10111100b
	out	dx,al

	mov	dx,TIMER_2
	mov	ax,[BufTim]
	out	dx,al
	mov	al,ah
	out	dx,al

	mov	dx,CONTROL
	mov	al,_INTE+_ST_pacer+ 70h
	out	dx,al
	call	near ptr WaitProc

@@wait_act:
	mov	dx,STATUS
	in	al,dx
	test	al,_INT
	jz	@@wait_act

	mov	dx,PACER
 	mov	al,11b
	out	dx,al

	cli
	in	al,21h
	jmp	$+2
	and	al,07Fh
	out	21h,al
	mov	al,67h
	jmp	$+2
	out	20h,al
	sti

	mov	dx,STATUS
	mov	al,1
	out	dx,al

	mov	[cs:Active],1
	mov	ax,SUCCESS
@@exit:
	ret
endp EnablePCL

proc EnablePCLw
	mov	ax,SUCCESS
	ret
endp EnablePCLw

; ================[ Деинициализация платы ]==============
proc DisablePCL
	push	ds
	cmp	[cs:Active],0
	je	@@exit
	cli
	mov	dx,CONTROL
	mov	al,0
	out	dx,al

	in	al,21h
	jmp	$+2
	or	al,80h
	out	21h,al
	sti
	mov	ds,[cs:savSeg]
	mov	dx,[cs:savOfs]
	mov	ax,250Fh
	int	21h
	mov	[cs:Active],0
@@exit:
	pop	ds
	mov	ax,[cs:count]
	ret
endp DisablePCL

; ================[ Обмен данными по флагу ]================
proc UsePCL
arg dac:word
	cmp	[cs:Active],1
	jne	@@exit
	mov	ax,[dac]
	clc
	shl	ax,4
	and	al,00h			; канал ЦАП !!!!!!!!!!! N*2
	mov	[cs:dac_word],ax

	mov	[cs:flag],0
	mov	cx,-1
@@loop:
	dec	cx
	jz	@@exit
	cmp	[cs:flag],1
	jne	@@loop
	mov	ax,[cs:adc_word]
	clc
	shr	ax,4
@@exit:
	ret
endp UsePCL


proc SetFuncPCL
arg  func:dword

	push	[func]
	pop	[cs:LoopFunc]
	ret
endp SetFuncPCL


; ========================================================= ;
; ================[ Внутренние процедуры ]================= ;
; ========================================================= ;

proc RunProc
	inc	[cs:count]
	cmp	[cs:use],1
	jne	@@cont
	iret
@@cont:
	inc	[cs:use]
	pusha
	cli
; ========================== Data Translation ==============
IFDEF DT_ACTIVE

	mov	dx,DT_STATUS
@@wait:
	in	al,dx
	test	al,_IBF
	jnz	@@wait

	mov	ax,[cs:dac_word]
	dec	dx
	out	dx,al

	inc	dx
@@wait1:
	in	al,dx
	test	al,_IBF
	jnz	@@wait1

	dec	dx
	mov	al,ah
	out	dx,al
ENDIF
; ========================== PCL 718 ==============
	mov	ax,[cs:dac_word]
	mov	dx,DAC_Lo
	out	dx,al
	inc	dx
	mov	al,ah
	out	dx,al

	mov	dx,ADC_Hi
	in	al,dx
	mov	ah,al
	dec	dx
	in	al,dx
	mov	[cs:adc_word],ax

	mov	bx,[word ptr cs:LoopFunc]
	xor	bx,[word ptr cs:LoopFunc]
	jz	@@nofunc
	shr	ax,4
	call	[dword ptr cs:LoopFunc]
@@nofunc:

	shl	ax,4
	and	al,00h			; канал ЦАП !!!!!!!!!!! N*2
	mov	[cs:dac_word],ax

	mov	[cs:flag],1
	dec	[cs:use]

	mov	dx,STATUS
	mov	al,1
	out	dx,al
	mov	al,20h
	out	20h,al
	popa
	iret
endp RunProc

proc WaitProc near
	nop
	ret
endp WaitProc

end
