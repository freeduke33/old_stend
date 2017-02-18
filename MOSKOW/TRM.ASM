;        B_TRM.ASM                               17.3.92
;        Byte transmit
;
dseg     segment
BASE_ADR dw     0
bfr      db     129 dup (0)
bfo      db     129 dup (0)
flag     db     0
rc       db     0
tm_rsp   dw     0                 ; time out period
rep_err  dw     0                 ; repite transmit block
cnt_err  dw     0
speed    db     60H               ; 1200
         db     30H               ; 2400
         db     18H               ; 4800
         db     0CH               ; 9600
         db     06H               ; 19K2
         db     03H               ; 38K4
         db     03H               ; 38K4  57K6
         db     03H               ; 38K4  112K2
dseg     ends
;
cseg     segment
         dosseg
         .model large
         .code
_cinit   proc   far
         assume ds:dseg
         push   bp
         mov    bp,sp
         push   si
         push   ds
         cld
         lds    si,[bp+6]         ; load command
         lodsb                    ; al - command code
         push   ax                ; save
         lodsb                    ; time out period
         mov    ah,al
         lodsb                    ; repite transmit
         mov    bx,dseg
         mov    ds,bx
         mov    byte ptr tm_rsp,ah
         mov    byte ptr rep_err,al
         pop    cx
;
         MOV   AX,40H          ;ES указывает на область данных BIOS
         MOV   ES,AX
         test  cl,10H          ;COM1
         jz    c_l01           ;yes
         MOV   DX,ES:[2]       ;получаем базовый адрес COM2
         jmp   c_l02
c_l01:   MOV   DX,ES:[0]       ;получаем базовый адрес COM1
c_l02:   mov   BASE_ADR,dx     ;store base address
;                              инициализируем регистры делителя
;                              скорости обмена на 9600 бод
         ADD   DX,3            ;указываем на регистр контроля линии
         MOV   AL,10000000B    ;устанавливаем бит 7
         OUT   DX,AL           ;посылаем байт
         DEC   DX              ;указываем на старший байт делителя
         DEC   DX              ;скорости обмена
         MOV   AL,0            ;старший байт для 9600 бод
         OUT   DX,AL           ;посылаем старший байт для 9600 бод
         DEC   DX              ;указываем на младший байт делителя
         xor   ax,ax
         mov   al,cl
         mov   cx,5
         shr   al,cl
         xor   bx,bx
         mov   bl,al
         MOV   AL,byte ptr speed[bx] ;младший байт делителя
;        MOV   AL,60H          ;младший байт делителя для 1200 бод
         OUT   DX,AL           ;посылаем младший байт
;                              инициализируем регистр контроля линии
         MOV   AL,0            ;обнуляем AL
         OR    AL,11B          ;длина данных 8 битов
         ADD   DX,3            ;указывае на регистр контроля линии
         OUT   DX,AL           ;посылаем инициализационное значение
;                              инициализируем регистр разрешения
;                              прерывания
         DEC   DX              ;указываем на регистр разрешения
         DEC   DX              ;прерывания
         MOV   AL,0            ;запрещаем прерывания
         OUT   DX,AL           ;посылаем байт
;
         pop    ds
         pop    si
         pop    bp
         ret
_cinit   endp
;
_cnct    proc  far
         assume ds:dseg
         push  ds
         mov   bx,dseg
         mov   ds,bx
         mov   cx,128
rs:      mov   al,3
         call  btrm
         test  al,1
         jz    rs1                ; no
         loop  rs
rs0:     mov   ax,16              ; Unknow process
         jmp   rse
;
rs1:     mov   al,0
         mov   ah,'?'             ; send start code
         call  btrm
         mov   al,1
         call  btrm
         test  al,1
         jnz   rs4
         test  al,4H
         jnz   rs3
         jmp   rse
rs3:     mov   ax,4               ; error communication
         jmp   rse
rs4:     cmp   ah,'.'
         je    rs5                ; yes, connect
         cmp   ah,'?'             ; requst
         jne   rs0
         mov   ah,'.'
         mov   al,0
         call  btrm
rs5:     mov   ax,0
rse:     mov   flag,0
         pop    ds
         ret
_cnct    endp
;
_iobf    proc   far
;        Call from Turbo-C:
;        i = iobf(unsigned int type, char *ib, char *ob)
;        type: al = 0 - send/receine; = 1 - only receive
;              ah - length input buffer (if al=0)
         assume ds:dseg
         push  bp
         mov   bp,sp
         sub   sp,2
         push  si
         push  ds
         push  es
;
         xor   cx,cx
         mov   ax,word ptr [bp+6] ; type
         mov   cl,ah              ; save length buffer
         mov   bx,dseg
         mov   es,bx              ; extend data segment
         mov   ES:byte ptr rc,al
         test  al,1               ; only receiving buffer
         jnz   bt_a               ; yes
         lds   si,[bp+8]          ; point DS:SI to buffer
         lea   bx,bfr+2           ; mov buffer
         add   ah,2
         mov   ES:byte ptr bfr,ah ; length io buffer
mvb:     lodsb                    ; move buffer
         mov   ES:byte ptr [bx],al
         inc   bx
         loop  mvb
         mov   ES:byte ptr [bx],0    ; stop byte
         mov   bx,es
         mov   ds,bx
         lea   bx,bfr
         call  cks
         mov   byte ptr bfr+1,al  ; save cks
         jmp   bt00
;
bt_a:    mov   bx,es
         mov   ds,bx
;        mov   al,1
         call  btrm
         mov   rc,al
         test  al,1
         jnz   bt02
         jmp   xx
bt00:    mov   al,3               ; test chennal
         call  btrm
         test  al,1               ; data ready
         jz    bt01               ; no data, go to 'test send'
         test  al,4
         jz    bt02               ; no error
         mov   rc,al
         jmp   xx                 ; chennal error
bt02:    mov   byte ptr bfo,ah    ; save length
         mov   ah,'.'             ; response
         mov   al,0
         xor   cx,cx
         mov   cl,byte ptr bfo
         lea   bx,bfo+1
         call  btrm
bt04:    mov   al,1
         call  btrm       ; receive blok
         test  al,1
         jnz   bt05               ; data occured
         mov   rc,al
         jmp   b_exit
bt05:    mov   byte ptr [bx],ah
         inc   bx
         loop  bt04
         lea   bx,bfo
         call  cks                ; test of transmit block
         mov   ah,'.'
         mov   flag,8H            ; receiving buffer
         cmp   byte ptr bfo+1,al
         je    bt06               ; Ok
         mov   ah,'R'             ; error
         mov   flag,0
bt06:    mov   al,0
         call  btrm
;
         test  flag,2             ; buffer alredy sending
         jne   b_exit             ; yes
;
bt01:    test  rc,1               ; only receiving block
         jne   b_exit             ; yes
bt23:    mov   ah,byte ptr bfr    ; sending block
         mov   al,0
         call  btrm
         mov   al,1
         call  btrm       ; wait response
         test  al,1               ; data ready
         jne   bt07
         mov   rc,al              ;  error, time out
         jmp   b_exit
b_exit:  test  flag,8
         jnz   b_ret
         test  rc,6
         jnz   b_ret
;
         mov   al,0
         call  tm_out
t1:      mov   al,1
         call  tm_out
         sub   ax,1
;        jl    t1
;
         mov   al,3
         call  btrm
         test  al,1
         jz    b_ret
         jmp   bt02
b_ret:   mov   rc,0
         test  flag,8
         jz    x_ret
         les   bx,[bp+12]         ; output buffer
         xor   cx,cx
         mov   cl,byte ptr bfo
         dec   cl
         lea   si,bfo+2
sz:      mov   al,byte ptr [si]
         mov   ES:byte ptr [bx],al
         inc   bx
         inc   si
         loop  sz
         mov   rc,1
x_ret:   mov   flag,0
         jmp   xx
;
bt07:    cmp   ah,'.'             ; Test of response
         je    bt08
         mov   cx,128             ; inknow response
bt09:    mov   al,1
         call  btrm
         test  al,2
         jnz   b_exit
         loop  bt09
         mov   al,16        ; remote station send unknow data stream
         jmp   b_exit
bt08:    xor   cx,cx
         mov   cl,byte ptr bfr
         lea   bx,bfr+1
bt11:    mov   ah,byte ptr [bx]
         mov   al,0
         call  btrm
         mov   rc,al
         test  al,6               ; error
         jz    bt22               ; no
         jmp   b_exit             ; yes
bt22:    inc   bx
         loop  bt11
         mov   al,1
         call  btrm
         mov   rc,al
         test  al,6
         jz    bt21
         jmp   b_exit
;
bt21:    or    flag,2             ; flag 'sending buffer'
         cmp   ah,'.'             ; Ok
         jne   bt20
         jmp   b_exit             ; yes
bt20:    cmp   ah,'R'             ; error transmit
         jne   bt12
         mov   cx,cnt_err
         dec   cx
         mov   cnt_err,cx
         cmp   cx,0
         je    bt12               ; repeate transmit block
         and   flag,0FDH          ; reset flag
         jmp   bt23
;
bt12:    or    rc,8               ; unknow response
         jmp   b_exit
;
xx:      mov   al,byte ptr rc
         pop   es
         pop   ds
         pop   si
         mov   sp,bp
         pop   bp
         ret
_iobf    endp
;
btrm     proc  near
         assume ds:dseg
         push  bx
         push  cx
         push  dx
         mov   bx,ax              ; put/get or get only, char
         mov   cx,rep_err
         mov   cnt_err,cx
B_L01:   mov   al,0               ; start time
         call  tm_out
         xor   cx,cx
;
B_L02:   MOV   DX,BASE_ADR        ; базовый адрес
         ADD   DX,5               ; регистр статуса линии
         IN    AL,DX              ; получаем байт статуса
         TEST  AL,00011110B       ; проверяем на ошибку
         JNZ   err_rtn            ; есть, - на процедуру обработки
B_L03:   TEST  AL,00000001B       ; проверяем получены ли данные
         JNZ   RECEIVE            ; да, - на процедуру приема
         test  bl,1               ; get only
         jnz   _tm_out            ; yes
         TEST  AL,00100000B       ; проверяем готовность к передаче
         JNZ   SEND               ; yes
;
_tm_out: test  bl,2               ; time_out
         jnz   _rtn               ; check channel only
         mov   al,1
         call  tm_out
         cmp   ax,tm_rsp          ; время вышло
         jle   B_L02              ; no
         or    cl,2               ; rc: time out
_rtn:    mov   ax,cx
         pop   dx
         pop   cx
         pop   bx
         ret                      ; exit
;
SEND:    sub   dx,5               ; а регистра хранения передатчика
         mov   al,bh
         OUT   DX,AL              ; посылаем символ
         jmp   short _rtn
;
RECEIVE: MOV   DX,BASE_ADR        ; базовый адрес
         IN    Al,DX              ; читаем полученный символ
         or    cl,1
         mov   ch,al
         test  bl,1
         jnz   _rtn               ; get only
         jmp   B_L02              ; put simbol
;
err_rtn: mov   dx,cx
         push  ax
         shr   al,1
         mov   cx,4
         shl   al,cl
         mov   cx,dx
         and   cl,0FH
         or    cl,al
         or    cl,4               ; rc
         pop   ax
         test  cl,010H
         jnz   B_L03
         jmp   short _rtn
btrm     endp
;
tm_out   proc  near                    ;
         assume ds:dseg
         push  cx
         push  dx
         push  ax
         mov   ah,2CH             ; ch - часы
         int   21H                ; cl - минуты
         and   cx,0FFH
         mov   ax,cx
         mov   cx,dx
         mul   CS:word ptr d60
         mov   dx,cx
         mov   cx,8
         shr   dx,cl
         add   ax,dx              ; ax: - к-во сек. от начала часа
         pop   cx
         cmp   cl,0
         je    m2
         cmp   ax,CS:word ptr xstr
         jge   m1
         add   ax,3600
m1:      sub   ax,CS:word ptr xstr
         jmp   ext
m2:      mov   CS:word ptr xstr,ax
ext:     pop   dx
         pop   cx
         ret
xstr     dw    0
d60      dw    60
tm_out   endp
;
cks      proc  near
         push  cx
         xor   al,al
         mov   ch,al
         mov   cl,byte ptr [bx]
         add   bx,2
         sub   cx,2
c1:      ror   al,1
         jnc   _m1
         xor   al,byte ptr [bx]
         mov   ah,al
         and   ah,0c3H
         not   al
         and   al,3cH
         or    al,ah
         jmp   _m2
_m1:     xor   al,byte ptr [bx]
_m2:     inc   bx
         loop  c1
         mov   ah,al
         and   ah,14H
         not   al
         and   al,0ebH
         or    al,ah
         pop   cx
         ret
cks      endp
;
cseg     ends
         public _cinit, _cnct, _iobf
         end
