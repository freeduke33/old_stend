.AUTODEPEND

.PATH.obj = BIN

#		*Translator Definitions*
CC = bcc +STEND.CFG
TASM = TASM
TLINK = tlink


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
  connect.obj \
  display.obj \
  config.obj \
  archive.obj \
  loop_fun.obj \
  mainloop.obj \
  tstloop.obj \
  _ctrl.obj \
  _global.obj \
  _ideal.obj \
  _model.obj \
  _test.obj \
  _topmnu.obj \
  mod_pods.obj \
  imi_mod.obj \
  data_glb.obj \
  ar_ss_1.obj \
  ar_ss_2.obj \
  pcl-718.obj \
  rtrm.obj \
  trm.obj 

#		*Explicit Rules*
bin\stend.exe: stend.cfg $(EXE_dependencies)
  $(TLINK) /yx/v/s/c/d/LD:\BC\LIB;D:\BC\TCXL;LIB @&&|
c0l.obj+
bin\connect.obj+
bin\display.obj+
bin\config.obj+
bin\archive.obj+
bin\loop_fun.obj+
bin\mainloop.obj+
bin\tstloop.obj+
bin\_ctrl.obj+
bin\_global.obj+
bin\_ideal.obj+
bin\_model.obj+
bin\_test.obj+
bin\_topmnu.obj+
bin\mod_pods.obj+
bin\imi_mod.obj+
/v- bin\data_glb.obj+
/v+ bin\ar_ss_1.obj+
bin\ar_ss_2.obj+
bin\pcl-718.obj+
bin\rtrm.obj+
bin\trm.obj
bin\stend,bin\stend
/v- tcxlbcl.lib+
lib\_grf.lib+
fp87.lib+
mathl.lib+
cl.lib+
graphics.lib
|
#    bin\prote bin\stend.exe

#		*Individual File Dependencies*
connect.obj: connect.c 

display.obj: display.c 

config.obj: config.c 

archive.obj: archive.c 

loop_fun.obj: loop_fun.c 

mainloop.obj: mainloop.c 

tstloop.obj: tstloop.c 

_ctrl.obj: _ctrl.c 

_global.obj: _global.c 

_ideal.obj: _ideal.c 

_model.obj: _model.c 

_test.obj: _test.c 

_topmnu.obj: _topmnu.c 

mod_pods.obj: mod_pods.c 

imi_mod.obj: imi_mod.c 

data_glb.obj: data_glb.c 

ar_ss_1.obj: ar_ss_1.asm 
	$(TASM) /ML /O AR_SS_1.ASM,BIN\AR_SS_1.OBJ

ar_ss_2.obj: ar_ss_2.asm 
	$(TASM) /ML /O AR_SS_2.ASM,BIN\AR_SS_2.OBJ

pcl-718.obj: pcl-718.asm 
	$(TASM) /ML /ZI /O PCL-718.ASM,BIN\PCL-718.OBJ

rtrm.obj: moskow\rtrm.c 
	$(CC) -c moskow\rtrm.c

trm.obj: moskow\trm.asm 
	$(TASM) /ML /O MOSKOW\TRM.ASM,BIN\TRM.OBJ

#		*Compiler Configuration File*
stend.cfg: stend.mak
  copy &&|
-ml
-2
-f287
-N
-v
-y
-Qx
-G
-O
-Z
-d
-h
-H=STEND.SYM
-wpin
-wamb
-wamp
-wasm
-wpro
-wcln
-wdef
-wsig
-wnod
-wstv
-wucp
-wuse
-nBIN
-ID:\BC\INCLUDE;D:\BC\TCXL\INC;LIB
-LD:\BC\LIB;D:\BC\TCXL;LIB
-Ff
| stend.cfg


