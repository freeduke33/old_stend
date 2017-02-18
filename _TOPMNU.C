#include <stdio.h>
#include <stdlib.h>

#include "immak.h"
#include <tcxlmnu.h>
#include <tcxlcur.h>
#include <tcxlent.h>
#include <tcxlcod.h>
#include <tcxlinp.h>
#include "string.h"

#include "stend.h"
#include "data_glb.h"
#include "color.h"
#include "din_zw.h"
#include "version.h"


LCL VOID CTYP CreateLst(NOARG);
LCL VOID CTYP SetGoLst(NOARG);


/*------------------------------------------------------------------------*/

char OptStr1[]="ОБЩИЕ ХАРАКТЕРИСТИКИ СИГНАЛОВ",
	 OptStr2[]="МОДЕЛЬ ПОДСТРОЙКИ",
	 OptStr3[]="ИМИТАЦИОННАЯ МОДЕЛЬ",
	 OptStr4[]="ИДЕАЛЬНЫЙ СИГНАЛ ОТКЛИКА",
	 OptStr5[]="ОБЬЕКТ УПРАВЛЕНИЯ";

LCL IbldT OptItm[]=
{
 {OptStr1,'',ITM_NOR, 0,  7, 13,0,0,0,NULL,GlbFun},
 {OptStr2,'',ITM_NOR, 0, 54, 12,0,0,0,NULL,ModFun},
 {OptStr4,'',ITM_NOR, 9, 50, 14,0,0,0,NULL,IdealFun},
 {OptStr5,'',ITM_NOR, 15,54, 15,0,0,0,NULL,ObjFun},
 {OptStr3,'',ITM_NOR, 9, 13, 16,0,0,0,NULL,DinFun}
};



LCL MbldT OptMnu=
{ &OptItm[0],5,  16,20,21,54,  MNU_OMN|MNU_USE|MNU_SAV|MNU_PUL,BOX_SNG,
 C_BOX,C_TXT,C_MSEL, C_CHR,C_NSEL,C_MASEL,
   12, NULL,  0, 10,  0, 0xFF, 0xFF, 0xFF
};

LCL VOID CTYP options(NOARG)
{
static void *_TOP_save;

 _TOP_save=Wsave(22,0,24,79);
 if(_TOP_save==NULL) {TcxlErr=W_ALLOCERR;MemAllocErr(0);}
 else
   {
   WfilBox(22,0,24,79,' ',C_TXT);
   WsetEsc(TRUE);
   if(MnuBuild(&OptMnu)==ERR_MEM) MemAllocErr(0);
   else
     {
#ifdef COPYRIGHT
 int cc;
# define PASSWORD psw
# define PROTECTION cc
# include "protect.cmp"
  calloc(cc*45,1000);
#endif
     MnuGet();
     }
   Wrestore(_TOP_save);
   }
 return;
}



/*-------------------------------------------------------------------------*/

static int _Col[2][3]={{_LGREY|WHITE,_WHITE|BLACK,_LGREY|WHITE},
                       {_RED|WHITE,_LRED|WHITE,_LGREY|RED}};


int CTYP PassWin(char *Title,char *Msg)
{
 int val;
  WpopUp(CNT_CNT,0,0,2,3+_Wputl(Msg),BOX_DBL,
        _Col[OBJType][0],_Col[OBJType][1]);

  val=MemAllocErr(0);
  if(!val)
	{
    	Wtitle(Title,TTL_CNT,_Col[OBJType][2]);
    	Wputs(" ");Wputs(Msg);CurHide();
    	Wshadow(C_SHADOW);
	}
  return(!val);
}

VOID CTYP PassEnd(NOARG)
{
  Wclose();
  return;
}

VOID CTYP PassWrite(Word num)
{
 Wputf("\b\b\b\b\b%05u",num);
 return;
}

VOID CTYP PassText(char *str)
{
 Wputf("\b\b\b\b\b%s",str);
 return;
}

// =======================================================================

VOID CTYP UpdateGlbFrm(NOARG)
{
 WndT w;

 w=Whandle();
 Whide();
 if(!MemAllocErr(0))
   {
   GlbView();
   Wunhide(w);
   }
 return;
}

/*---------------------------------------------------------------------------*/

VOID CTYP Go_Model(NOARG)
{
 char str[][40]={"ИДЕТ УПРАВЛЕНИЕ ИМИТАТОРОМ :  Frm",
                 "ИДЕТ УПРАВЛЕНИЕ СТЕНДОМ :   Frm"};


 if(PassWin("┤ВНИМАНИЕ├",str[OBJType]))
   {
   if(CtrlType) Write();
   else Control();
   PassEnd();
   ItmNxt(18);
   UpdateGlbFrm();
   }
 return;
}

LCL VOID CTYP Go_Reload(NOARG)
{
 if(PassWin("┤ВНИМАНИЕ├","ИДЕТ ПРОЦЕСС РАЗГРУЗКИ СТЕНДА"))
   {
   Reload();
   Beep();
   PassEnd();
   }
 return;
}

VOID CTYP CreateFname(char *name,char *ext)
{
 int i;
 strcpy(name,SysDate(4));
 for(i=0;name[i];i++)
   if(name[i]=='/') {name[i]='_';name[i+1]=0;}
 strcat(name,SysTime(1));
 for(i=strlen(name);i;i--)
   if(name[i]==':') {name[i]=0;break;}
 strcat(name,ext);
 for(i=0;name[i];i++) if(name[i]==':') name[i]='_';
 return;
}

/*----------------------------------*/

LCL char _LstGo[10]=" - ДА - ",_LstFile[15]="noname.std",_LstRem[40];
LCL int _LstGO=0;

LCL FbldT LstFldT[3]=
{
 {"Файл протокола"      ,1,0, _LstFile,  1,25,"WWWWWWWWWWWWW",
  NULL,	     NULL,NULL,  0, 181,0,  FLD_TRM,0,FLD_CHG},
 {"Примечание протокола",2,0, _LstRem,   3, 0,
  "????????????????????????????????????????????",
  NULL,	     NULL,NULL,  0, 182,0,  0,0,FLD_CHG},
 {"Создание протокола"  ,0,0, _LstGo,    0,25,"********",
  SetGoLst, NULL,NULL,  0, 180,0,   FLD_TRM,0,FLD_CHG}
};
LCL   EbldT   CDC   LstFrmT =              /*- form-build object --------*/
{  &LstFldT[0], 3,                         /* field-build list, count    */
   NULL, NULL,                            /* getkey(), key-buffer       */
   0, ENT_SAV|ENT_WRP,                    /* user, options              */
   C_TXT,C_SEL,C_ASEL,C_NSEL
};

LCL VOID CTYP LstFun(NOARG)
{

 _LstGO=0;
 CreateFname(_LstFile,".st");
 Wopen(13,25,18,70,BOX_SNG,C_BOX,C_TXT);
 if(!MemAllocErr(0))
   {
   Wshadow(C_SHADOW);
   if(EntBuild(&LstFrmT)==ERR_MEM) MemAllocErr(0);
   else
     {
     EntGet(LstFldT[0].ftag);
     EntFree();
     if(_LstGO) CreateLst();
     }
   Wclose();
   }
 return;
}

LCL VOID CTYP SetGoLst(NOARG)
{
 if(SubFunBeg()) return;
 _LstGO=1;
 KquePut(Key_C_Ent);
 return;
}

LCL VOID CTYP CreateLst(NOARG)
{
 FILE *f;
 unsigned int i;
 int re_nr;

 if(!_USE_)
  {
   Error("Нет данных об управлении с такими параметрами");
   return;
  }
 if(PassWin("╢ Ждите ╟","Идет запись протокола ..."))
   {
   f=fopen(_LstFile,"w+t");
   if(f)
     {
     fprintf(f,"Протокол работы стенда от %s, %s \n",SysDate(5),SysTime(3));
/*   if(_LstRem[0]==0) strcpy(_LstRem," --//-- "); */
     fprintf(f,"\n  Примечание :  %s\n\n",_LstRem);
     fprintf(f,"Длина ряда               %u\n",MathBeg_End(&re_nr,&re_nr));
     fprintf(f,"Число циклов повторения  %u\n",ncikl);
     fprintf(f,"Частота дискретизации    %lf\n",fdi);
     fprintf(f,"Вид физической величины  %s\n\n",GlbName());
     fprintf(f,"Модель подстройки        %s типа %s\n",TrnName(),ModName);
     fprintf(f,"Порядок модели           %d\n",p);
     fprintf(f,"Порядок тренда           %d\n",ep);
     fprintf(f,"Регуляризация            %d\n",q);
     fprintf(f,"Прогноз                  %d\n",shift);

     fprintf(f,"\nУправление  Эталонный   Реальный\n\n");
     for(i=0;i<nr+qp;i++)
       fprintf(f,"   %05u      %05u     %05u\n",
	             (int)(x[i] *pcl_RANGE),
	             (int)(yi[i]*pcl_RANGE),
	             (int)(y[i] *pcl_RANGE));
     fclose(f);
     }
   else Error(" НЕ МОГУ СОЗДАТЬ ФАЙЛ ПРОТОКОЛА ");
   PassEnd();
   }
 KquePut(Key_C_Ent);
 return;
}



LCL IbldT RunItm[4]=
{{"Начать  цикл управления"    , '',0,  1,0, 17,0,0,0,
 NULL,Go_Model,NULL,NULL,NULL},
 {"Просмотреть  результаты"    , '',0,  2,0, 18,0,0,0,
 NULL,Loop_Graph,NULL,NULL,NULL},
 {"Разгрузка объекта упр-я",     '',0,  3,0, 19,0,0,0,
 NULL,Go_Reload,NULL,NULL,NULL},
 {"Выдача  протокола упр-я",     '',0,  4,0, 19,0,0,0,
 NULL,LstFun,NULL,NULL,NULL}
};

LCL MbldT RunMnu=
{ &RunItm[0],4,  16,22,23,56,  MNU_PUL,BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_WNSEL,C_WASEL,
   17, OpenW,  0, 10,  5, 0xFF, 0xFF, 0xFF
};


/*-------------------------------------------------------------------------*/


VOID CTYP Test_Full(NOARG)
{
 if(PassWin("┤ТЕСТ├","ОПРЕДЕЛЕНИЕ ВСЕХ ХАРАКТЕРИСТИК ТРАКТА УПРАВЛЕНИЯ"))
   {
   Test_Fun(STAT_TST+DIN_TST);
   PassEnd();
   UpdateGlbFrm();
   ItmNxt(20);
   }
 return;
}

LCL VOID CTYP Test_Stat(NOARG)
{

 if(PassWin("┤ТЕСТ├","ОПРЕДЕЛЕНИЕ СТАТИЧЕСКОЙ ХАРАКТЕРИСТИКИ"))
   {
   Test_Fun(STAT_TST);
   PassEnd();
   UpdateGlbFrm();
   ItmNxt(20);
   }
 return;
}

LCL VOID CTYP Test_Din(NOARG)
{
 if(PassWin("┤ТЕСТ├","ОПРЕДЕЛЕНИЕ ДИНАМИЧЕСКОЙ ХАРАКТЕРИСТИКИ"))
   {
   Test_Fun(DIN_TST);
   PassEnd();
   ItmNxt(20);
   }
 return;
}


LCL IbldT TstItm[4]=
{{"Полный нобор  тестов"    , '',0,  1,0, 17,0,0,0,
 NULL,Test_Full,NULL,NULL,NULL},
 {"Статическая   хар-ка"    , '',0,  2,0, 18,0,0,0,
 NULL,Test_Stat,NULL,NULL,NULL},
 {"Динамическая  хар-ка",     '',0,  3,0, 19,0,0,0,
 NULL,Test_Din,NULL,NULL,NULL},
 {"Просмотр результатов",     '',0,  4,0, 20,0,0,0,
 NULL,Test_Graph,NULL,NULL,NULL}
};

LCL MbldT TstMnu=
{ &TstItm[0],4,  16,22,23,56,  MNU_PUL,BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_WNSEL,C_WASEL,
   17, OpenW,  0, 10,  5, 0xFF, 0xFF, 0xFF
};

/*---------------------------------------------------------------------------*/

LCL VOID CTYP off_ESC(NOARG)
{
 WsetEsc(FALSE);
 return;
}

LCL VOID CTYP About(NOARG)
{
 WpopUp(CNT_CNT,0,0,7,40,BOX_DBL|BOX_EXP,_LGREY|WHITE,_LGREY|BLACK);
 if(!MemAllocErr(0))
   {
   Wtitle(" About ",TTL_CNT,_LGREY|WHITE);
   Wshadow(_BLACK|DGREY);
   CurHide();
   WprtCen(1,_LGREY|BLACK,"Управление стендом v" STEND_VER);
   WprtCen(2,_LGREY|BLACK, STEND_CTRL );
   WprtCen(3,_LGREY|BLACK,"Copyright (C) НПП\"ТРЕНД\" 1992");
   WfilBox(5,0,5,38,' ',_BLUE|CYAN);
   WprtCen(5,_BLUE|CYAN,"<нажмите любую клавишу>");
   KeyWait();
   Wclose();
   }
 return;
}

LCL IbldT TopItm[7]=
{{" (C) ",       '',ITM_NOR, 23,3,  19,1,0,0,
 NULL,About,off_ESC,NULL,NULL},
 {"   Тест  ",   '',ITM_NOR, 23,10, 20,2,0,0,
 NULL,NULL,off_ESC,NULL,&TstMnu},
 {"Управление",  '',ITM_NOR, 23,22, 21,3,0,0,
 NULL,NULL,off_ESC,NULL,&RunMnu},
 {"Параметры",   '',ITM_PUL, 23,36, 22,4,0,0,
 NULL,options,off_ESC,NULL,NULL},
 {"Обработка",   '',ITM_NOR, 23,47, 23,4,0,0,
 NULL,Archive,off_ESC,NULL,NULL},
 {"Настройка",   '',ITM_NOR, 23,59, 24,4,0,0,
 NULL,Config,off_ESC,NULL,NULL},
 {"  Выход  ",   '',ITM_ALL, 23,70, 25,5,0,0,
 NULL,toDOS,off_ESC,NULL,NULL}};

MbldT TopMnu=
{  &TopItm[0], 7,  0,  0, 1, 1, MNU_HOR|MNU_SAV|MNU_USE, BOX_SNG,
 C_BOX,C_TXT,C_MSEL, C_CHR,C_NSEL,C_MASEL,
   22, off_ESC,  0, 10,  0, 0xFF, 0xFF, 0xFF
};

