//
//  Описание идеального сигнала отклика
//   IdeaView(),IdeaFun()
//
//

#include "immak.h"
#include "Look_Def.h"
#include "tcxlmnu.h"
#include "tcxlent.h"
#include "tcxlsel.h"
#include "tcxlcur.h"
#include "tcxlcod.h"
#include "tcxlinp.h"
#include "string.h"
#include "stdio.h"

#include "stend.h"
#include "data_glb.h"
#include "color.h"
#include "din_zw.h"


LCL char _StdFile[15],_StdTitle[30],_StdRem[80],_StdGo[]=" - ДА - ";
LCL int _LstGO;


static char _filename[35],__filename[75];
static char IdealStr[5][25]={"СИНУСОИДАЛЬНЫЙ","ПОЛИГАРМОНИЧЕСКИЙ",
		              "ШУМОВОЙ","ИЗ ФАЙЛА","СО СТЕНДА"};

static char _IdealChas1[10],_IdealChas2[10],
            _IdealAmp1[10],_IdealAmp2[10],
	    _IdealConst[10];

static int  ACT,_type,_ENTRY_;


LCL VOID CTYP SelFileName(NOARG);
LCL VOID CTYP SetGoLst(NOARG);


LCL VOID CTYP Num2Str(int num)
{
 int i;
 ACT=0;
 switch(_type=num)
 {
	case 0: /* sin */
	 cvtfc(_IdealChas1,f0d,6,3);
	 cvtfc(_IdealAmp1,a0,  6,3);
	 cvtfc(_IdealConst,u0, 6,3);
	break;  /* psin */
	case 1:
	 cvtfc(_IdealChas1,f1d,6,3);
	 cvtfc(_IdealChas2,f2d,6,3);
	 cvtfc(_IdealAmp1,a1,  6,3);
	 cvtfc(_IdealAmp2,a2,  6,3);
	 cvtfc(_IdealConst,u1, 6,3);
	break;
	case 2: /* unknown */
	 cvtfc(_IdealAmp1,ash,7,5);
	 cvtfc(_IdealConst,ush,7,5);
	break;
	case 3: /* file */
         for(i=strlen(filename);i>=0;i--)
	       if(filename[i]=='\\') break; i++;
         strcpy(_filename,&filename[i]);
	 cvtfc(_IdealAmp1,af,7,5);
	 cvtfc(_IdealConst,uf,7,5);
	break;
 }
}

LCL VOID CTYP Str2Num(NOARG)
{
 switch(_type)
 {
  case 0: /* sin */
    f0d=cvtcf(_IdealChas1);
    a0=cvtcf(_IdealAmp1);
    u0=cvtcf(_IdealConst);
    break;  /* psin */
  case 1:
    f1d=cvtcf(_IdealChas1);
    f2d=cvtcf(_IdealChas2);
    a1=cvtcf(_IdealAmp1);
    a2=cvtcf(_IdealAmp2);
    u1=cvtcf(_IdealConst);
    break;
  case 2: /* unknown */
    ash=cvtcf(_IdealAmp1);
    ush=cvtcf(_IdealConst);
    break;
  case 3: /* file */
    strcpy(filename,__filename);
    af=cvtcf(_IdealAmp1);
    uf=cvtcf(_IdealConst);
    break;
 }
 return;
}



// ***********************************************************************

LCL FbldT SinFld[]=
{{"Амплитуда синусоиды",    0,5,_IdealAmp1, 0,30,"99.999",
 NULL,	     NULL,NULL, 0,100,0, FLD_DEC,0,FLD_CND},
 {"Частота синусоиды",      1,5,_IdealChas1,1,30,"99.999",
 NULL,	     NULL,NULL, 0,101,0, FLD_DEC,0,FLD_CND},
 {"Постоянная составляющая",2,5,_IdealConst,2,30,"99.999",
 NULL,	     NULL,NULL, 0,102,0, FLD_DEC,0,FLD_CND}
};

LCL EbldT SinFrm=
{&SinFld[0],3,
	 NULL, NULL,
	 0, ENT_SAV,
   C_TXT,C_SEL,C_ASEL,C_NSEL
};


LCL VOID CTYP SinFun(NOARG)
{
 int val;

 _ENTRY_=1;
 Num2Str(0);
 Wopen(9,26,13,65,BOX_SNG,C_BOX,C_TXT);
 if(!MemAllocErr(0))
   {
   Wshadow(C_SHADOW);
   if(EntBuild(&SinFrm)==ERR_MEM) MemAllocErr(0);
   else
     {
     val=EntGet(SinFld[0].ftag);
     if(val!=ERR_ARG)
       {
       _USE_=0;
       ACT=1;
       }
     EntFree();
     }
   Wclose();
   }
 _ENTRY_=0;
 return;
}

// =======================================================================

LCL FbldT PSinFld[]=
{{"Амплитуда синусоиды 1",    0,5,_IdealAmp1, 0,30,"99.999",
	NULL,	     NULL,NULL, 0,103,0, FLD_DEC,0,FLD_CND},
 {"Частота синусоиды 1",      1,5,_IdealChas1,1,30,"99.999",
	NULL,	     NULL,NULL, 0,104,0, FLD_DEC,0,FLD_CND},
 {"Амплитуда синусоиды 2",    2,5,_IdealAmp2, 2,30,"99.999",
	NULL,	     NULL,NULL, 0,105,0, FLD_DEC,0,FLD_CND},
 {"Частота синусоиды 2",      3,5,_IdealChas2,3,30,"99.999",
	NULL,	     NULL,NULL, 0,106,0, FLD_DEC,0,FLD_CND},
 {"Постоянная составляющая",  4,5,_IdealConst,4,30,"99.999",
	NULL,	     NULL,NULL, 0,107,0, FLD_DEC,0,FLD_CND}
};

LCL EbldT PSinFrm=
{&PSinFld[0],5,
	 NULL, NULL,
	 0, ENT_SAV,
   C_TXT,C_SEL,C_ASEL,C_NSEL
};

LCL VOID CTYP PSinFun(NOARG)
{
 int val;

 _ENTRY_=1;
 Num2Str(1);
 Wopen(10,26,16,65,BOX_SNG,C_BOX,C_TXT);
 if(!MemAllocErr(0))
   {
   Wshadow(C_SHADOW);
   if(EntBuild(&PSinFrm)==ERR_MEM) MemAllocErr(0);
   else
     {
     val=EntGet(PSinFld[0].ftag);
     if(val!=ERR_ARG)
       {
       _USE_=0;
       ACT=1;
       }
     EntFree();
     }
   Wclose();
   }
 _ENTRY_=0;
 return;
}

// =======================================================================

LCL FbldT UnkFld[2]=
{{"Коэффициент",              0,5,_IdealAmp1, 0,30,"9.99999",
	NULL,	     NULL,NULL, 0,108,0, FLD_DEC,0,FLD_CND},
 {"Постоянная составляющая",  1,5,_IdealConst,1,30,"9.99999",
	NULL,	     NULL,NULL, 0,109,0, FLD_DEC,0,FLD_CND}
};

EbldT UnkFrm=
{&UnkFld[0],2,
	 NULL, NULL,
	 0, ENT_SAV,
   C_TXT,C_SEL,C_ASEL,C_NSEL
};

LCL VOID CTYP UnkFun(NOARG)
{
 int val;

 _ENTRY_=1;
 Num2Str(2);
 Wopen(11,26,14,65,BOX_SNG,C_BOX,C_TXT);
 if(!MemAllocErr(0))
   {
   Wshadow(C_SHADOW);
   if(EntBuild(&UnkFrm)==ERR_MEM) MemAllocErr(0);
   else
     {
     val=EntGet(UnkFld[0].ftag);
     if(val!=ERR_ARG)
       {
       _USE_=0;
       ACT=1;
       }
     EntFree();
     }
   Wclose();
   }
 _ENTRY_=0;
 return;
}

// =======================================================================


LCL FbldT FilFld[3]=
{{"Имя файла ", 0,2,_filename, 0,15,"FFFFFFFFFFFFFFFFFFFF",
	SelFileName ,NULL,NULL, 0,111,0, FLD_TRM,0,FLD_CND},
 {"Коэффициент передачи",     1,2,_IdealAmp1, 1,29,"9.99999",
	NULL,	     NULL,NULL, 0,112,0, FLD_DEC,0,FLD_CND},
 {"Постоянная составляющая",  2,2,_IdealConst,2,30,"9.99999",
	NULL,	     NULL,NULL, 0,113,0, FLD_DEC,0,FLD_CND}
};

LCL EbldT FilFrm=
{&FilFld[0],3,
	 NULL, NULL,
	 0, ENT_SAV,
   C_TXT,C_SEL,C_ASEL,C_NSEL
};

LCL VOID CTYP FilFun(NOARG)
{
 int val;

 _ENTRY_=1;
 Num2Str(3);
 Wopen(11,26,15,65,BOX_SNG,C_BOX,C_TXT);
 if(!MemAllocErr(0))
   {
   Wshadow(C_SHADOW);
   if(EntBuild(&FilFrm)==ERR_MEM) MemAllocErr(0);
   else
     {
     val=EntGet(FilFld[0].ftag);
     if(val!=ERR_ARG)
       {
       _USE_=0;
       ACT=1;
       }
     EntFree();
     }
   Wclose();
   }
 _ENTRY_=0;
 return;
}


// =======================================================================

LCL VOID CTYP SelFileName(NOARG)
{
 ChrP str;
 FdfP  f;
 int i;

 if(SubFunBeg()) return;
 f=FldCur();
 SelBarOff();
 SelDirOn();
 SelParOn();

 strcpy(__filename,filename);
 for(i=strlen(__filename);i>=0;i--)
        if(filename[i]=='\\') break;
 __filename[++i]=0;
 strcat(__filename,"*.STD");

 str=SelFile(4,15,13,60,BOX_SNG,C_BOX,C_SEL,C_ASEL,
             1,__filename,_VFVCP(OpenW));
 if(str)
 {
  strcpy(__filename,str);
  for(i=strlen(str);i;i--) if(str[i]=='\\') break; i++;
  strcpy(FdfBfr(f),&str[i]);
  FldChg();
 }

 SelFree();
 SubFunEnd();
 return;
}

// =======================================================================


LCL FbldT StdFldT[3]=
{
 {"Файл данных"        ,1,0, _StdFile,  1,25,"WWWWWWWWWWWWW",
  NULL,	     NULL,NULL,  0, 51,0,  FLD_TRM,0,FLD_CHG},
 {"Примечание "        ,2,0, _StdRem,   3, 0,
  "????????????????????????????????????????????",
  NULL,	     NULL,NULL,  0, 52,0,  0,0,FLD_CHG},
 {"Начать считывание"  ,0,0, _StdGo,    0,25,"********",
  SetGoLst, NULL,NULL,  0,  50,0,   FLD_NUL,0,FLD_CHG}
};
LCL   EbldT   CDC   StdFrmT =              /*- form-build object --------*/
{  &StdFldT[0], 3,                         /* field-build list, count    */
   NULL, NULL,                            /* getkey(), key-buffer       */
   0, ENT_SAV,                            /* user, options              */
   C_TXT,C_SEL,C_ASEL,C_NSEL
};

LCL VOID CTYP FromStend(NOARG)
{
 FILE *ff;
 int i;

 if(Read())
   {
   PassWin("┤ Ждите ├"," Идет запись эталонного сигнала в файл ...");
   ff=fopen(_StdFile,"w+t");
   if(ff)
     {
     fprintf(ff,"Файл эталонного сигнала со стенда за %s\n",_StdTitle);
     fprintf(ff,"\n примечание : %s\n\n",_StdRem);
     fprintf(ff,"Длинна ряда           %u\n",nr);
     fprintf(ff,"Число циклов упр-я    %u\n",ncikl);
     fprintf(ff,"Частота дискретизации %lf\n\n",fdi);
     for(i=0;i<nr;i++)
        fprintf(ff," %05d\n",(int)(yi[i]*pcl_RANGE));
     fclose(ff);
     FinishMelody();
     }
   else Error("Не могу записать данные");
   PassEnd();
   }
 return;
}

VOID CTYP SetGoLst(NOARG)
{
 if(SubFunBeg()) return;
 _LstGO=1;
 KquePut(Key_C_Ent);
 return;
}

LCL VOID CTYP StendFun(NOARG)
{
 int val;

 _LstGO=0;
 CreateFname(_StdFile,".std");
 Wopen(13,25,18,70,BOX_SNG,C_BOX,C_TXT);
 if(!MemAllocErr(0))
   {
   Wshadow(C_SHADOW);
   if(EntBuild(&StdFrmT)==ERR_MEM) MemAllocErr(0);
   else
    {
    val=EntGet(StdFldT[0].ftag);
    if(val!=ERR_ARG)
      {
      sprintf(_StdTitle,"%s, %s",SysDate(5),SysTime(3));
      if(_LstGO) FromStend();
      }
    EntFree();
    }
   Wclose();
   }
 return;
}

// =======================================================================

LCL double _f0d,_a0,_u0, _f1d,_a1,_u1, _f2d,_a2, _ash,_ush, _af,_uf;
LCL char _file[80];

LCL VOID CTYP SetParams(int num)
{
 if(_ENTRY_==0) return;

 switch(num)
 {
  case 0: /* sin */
    _a0 =a0;
    _f0d=f0d;
    _u0 =u0;
    a0 =cvtcf(FdfBfr(FldLoc( 100 )));
    f0d=cvtcf(FdfBfr(FldLoc( 101 )));
    u0 =cvtcf(FdfBfr(FldLoc( 102 )));
    break;
  case 1: /* psin */
    _a1=a1;
    _f1d=f1d;
    _a2=a2;
    _f2d=f2d;
    _u1=u1;
    a1 =cvtcf(FdfBfr(FldLoc( 103 )));
    f1d=cvtcf(FdfBfr(FldLoc( 104 )));
    a2 =cvtcf(FdfBfr(FldLoc( 105 )));
    f2d=cvtcf(FdfBfr(FldLoc( 106 )));
    u1 =cvtcf(FdfBfr(FldLoc( 107 )));
    break;
  case 2: /* unknown */
    _ash=ash;
    _ush=ush;
    ash=cvtcf(FdfBfr(FldLoc( 108 )));
    ush=cvtcf(FdfBfr(FldLoc( 109 )));
    break;
  case 3: /* file */
    strcpy(_file,filename);
    _af=af;
    _uf=uf;
    strcpy(filename,FdfBfr(FldLoc( 111 )));
    af =cvtcf(FdfBfr(FldLoc( 112 )));
    uf =cvtcf(FdfBfr(FldLoc( 113 )));
    break;
 }
 return;
}

LCL VOID CTYP RestoreParams(int num)
{
 if(_ENTRY_==0) return;
 switch(num)
 {
  case 0: /* sin */
    a0 =_a0;
    f0d=_f0d;
    u0 =_u0;
    break;
  case 1: /* psin */
    a1 =_a1;
    f1d=_f1d;
    a2 =_a2;
    f2d=_f2d;
    u1 =_u1;
    break;
  case 2: /* unknown */
    ash=_ash;
    ush=_ush;
    break;
  case 3: /* file */
    strcpy(filename,_file);
    af=_af;
    uf=_uf;
    break;
 }
 return;
}


LCL VOID CTYP Show(NOARG)
{
 int *scr,val;

 if(IdfTag(ItmCur())==33)
   {
   Error(" Нет прямого доступа к данным для показа графика");
   return;
   }
 CreateBuf(&yi,nr+Q_P);
 if(yi==NULL)
   {
	Error("Нет свободного места для просмотра эталона");
	return;
   }
 delt=1/fdi;
 val=IdfTag(ItmCur())-29;

 SetParams(val);
 if(GenIdeal(val)==0)
   {
   scr=SaveSCR(0,0,24,79);
   if(scr!=NULL)
	 {
     if(LookGRF1(&yi[Q_P]," ЭТАЛОН ",nr,delt)!=okGRF)
       Error(" Ошибка при просмотре графика эталонного сигнала");
	 RestoreSCR(scr);
	 }
   else Error("Не могу сохранить текстовый экран");
   }
 else Error(" Ошибка формирования эталонного сигнала ");
 RestoreParams(val);

 return;
}

LCL VOID CTYP OpenI(NOARG)
{
 Wshadow(C_SHADOW);
 Wmessage("┤F3-просмотр├",BRD_BOT,2,C_WBOX);
 return;
}
/*-------------------------------------------------------------------------*/

LCL IbldT IdealItm[5]=
{{IdealStr[0],'',ITM_ALL, 0,0, 29,0,0,0,NULL,SinFun},
 {IdealStr[1],'',ITM_ALL, 1,0, 30,0,0,0,NULL,PSinFun},
 {IdealStr[2],'',ITM_ALL, 2,0, 31,0,0,0,NULL,UnkFun},
 {IdealStr[3],'',ITM_ALL, 3,0, 32,0,0,0,NULL,FilFun},
 {IdealStr[4],'',ITM_PUL, 4,0, 33,0,0,0,NULL,StendFun}
};

LCL  MbldT IdealMnu=
{&IdealItm[0],5, 7,59,13,79,  MNU_SAV|MNU_PUL, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_WNSEL,C_WASEL,
 29, OpenI,  0, 10,  1, 0xFF, 0xFF, 0xFF
};


VOID CTYP IdealView(NOARG)
{
 int i;
 char str[120];
 FILE *ff;

 Num2Str(IdealType);

 if(IdealType<4)
   {
   WfilBox(10,49,13,77,' ',C_TXT);
   Wgotoxy(10,49);Wputs("Вид сигнала");
   Wprts(10,61,C_SEL,IdealStr[IdealType]);
   }

 switch(IdealType)
 {
 case 0: /* sin */
	Wgotoxy(11,49);Wputf("Частота      \033A%c%7g \033A%cГц",
	               C_SEL,f0d,C_TXT);
	Wgotoxy(12,49);Wputf("Амплитуда    \033A%c%7g \033A%cВ",
	               C_SEL,a0,C_TXT);
	Wgotoxy(13,49);Wputf("Пост. составляющая \033A%c%g \033A%cВ",
	               C_SEL,u0,C_TXT);
 break;
 case 1: /* psin */
	Wgotoxy(11,49);Wputf("Частоты  \033A%c%7g %7g \033A%cГц",
	               C_SEL,f1d,f2d,C_TXT);
	Wgotoxy(12,49);Wputf("Амплитуды \033A%c%7g %7g \033A%cВ",
	               C_SEL,a1,a2,C_TXT);
	Wgotoxy(13,49);Wputf("Пост. составляющая \033A%c%g \033A%cВ",
	               C_SEL,u1,C_TXT);
 break;
 case 2: /* unknown */
	Wgotoxy(12,49);Wputf("Коэффициент   \033A%c%7g \033A%cВ",
	               C_SEL,ash,C_TXT);
	Wgotoxy(13,49);Wputf("Пост. составляющая \033A%c%g \033A%cВ",
	               C_SEL,ush,C_TXT);
 break;
 case 3: /* file */
	Wgotoxy(11,49);Wputf("Имя файла :  \033A%c%s\033A%c",
	               C_SEL,_filename,C_TXT);
	Wgotoxy(12,49);Wputf("Коэффициент передачи \033A%c%g\033A%c В",
	               C_SEL,af,C_TXT);
	Wgotoxy(13,49);Wputf("Пост. составляющая   \033A%c%g\033A%c В",
	               C_SEL,uf,C_TXT);
	if(PassWin("╢ Ждите ╟","Чтение параметров из файла ..."))
	  {
	  ff=fopen(filename,"r");
	  if(ff)
	    {
	    for(i=0;i<3;i++) fgets(str,120,ff);
	    fscanf(ff,"%s %s %d\n",str,str,&nr);
	    fscanf(ff,"%s %s %s %d\n",str,str,str,&ncikl);
	    fscanf(ff,"%s %s %lf",str,str,&fdi);
	    fclose(ff);
	    }
	  else Error("Не могу прочитать параметры упр-я из файла данных ");
	  PassEnd();
	  GlbView();
	  }
 break;
 }
 return;
}

VOID CTYP IdealFun(NOARG)
{
 int s;

 if(MnuBuild(&IdealMnu)) MemAllocErr(0);
 else
   {
   _ENTRY_=0;
   KbndSet(Key_F3,Show,0);
   s=MnuGet();
   KbndClr(Key_F3);
   if(ACT && s>=29)
      {
      IdealType=s-29;
      Str2Num();
      IdealView();
      }
   }
 return;
}



