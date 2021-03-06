/*------------ ПОДПPОГPАММА <DISPLAY.C> ----------------------*/

#include "version.h"
#include "immak.h"

#include "tcxlmnu.h"
#include "tcxlwin.h"
#include "tcxlent.h"
#include "tcxlcod.h"
#include "tcxlinp.h"
#include "tcxlcur.h"
#include "tcxlsel.h"
#include "tcxlvid.h"
#include "tcxlnls.h"

#include "string.h"
#include "process.h"
#include "alloc.h"
#include "signal.h"
#include "stdio.h"
#include "dos.h"
#include "setjmp.h"

#include "din_zw.h"
#include "stend.h"
#include "data_glb.h"
#include "color.h"

#ifdef COPYRIGHT
# include "protect.h"
#endif


#ifdef COPYRIGHT
unsigned int psw[__LEN_p];
#endif

char RusTabl[0x100]="ЙйЦцУуКкЕеНнГгШшЩщЗзХхФфЫыВвАаПпРрОоЛлДдЖжЭэЯяЧчСсМмИиТтЬьБбЮю\00";
/********************************************************************/

IntT CTYP _Wputl(ChrP str) { return(strlen(str)); }

VOID CTYP toDOS(NOARG)
{
 WriteCfg();
 WcloseAll();
 exit(0);
 return;
}


VOID CTYP SubFunEnd(NOARG)
{
 KquePut(Key_Tab);
 KquePut(Key_S_Tab);
 return;
}

int CTYP SubFunBeg(NOARG)
{
 KeyT k;
 FdfP f;

 CurHide();
 MouPush(MOU_FULL);
 MouShow();
 k=KeyGetc();
 MouPop();
 CurShow();


 if(k==0x3920) return(0);
 else
 {
  if(!(k==Key_Up || k==Key_Dwn || k==Key_Esc || k==Key_Ent
    || k==Key_Tab || k==Key_S_Tab || k==Key_C_Home || k==Key_C_End
    || (k&0xff00)==0xd400))
  {
   f = FldCur();
   if(FdfPrv(f) || FdfUp(f)) k = Key_Up;
   else k=Key_Dwn;
  }
  KquePut(k);
 }
 return(1);
}


/*------------------------------------------------------------------------*/

void FinishMelody(NOARG)
{
 static struct {int Frez,delay;} melody[]={
#include "finish.inc"
	  {0,-1}};
 int i=0;

 while(melody[i].delay!=-1 && !(KeyPeek()))
    {
    if(!melody[i].Frez) nosound();
    else sound(melody[i].Frez);
    delay(melody[i++].delay);
    }
 nosound();
 KqueClr();
 return;
}



int CTYP Error(char *str)
{
 int val;
 int sav=VidFlg;
 VidParm(VP_BIO);
 val=Wperror(str);
 VidFlg=sav;
 return(val);
}

int CTYP Esc(char *str)
{
 int val=0;
 WpopUp(CNT_CNT,0,0,2,4+_Wputl(str),BOX_DBL,_RED+WHITE,_RED+WHITE);
 if(!MemAllocErr(0))
   {
   Wtitle("╡ ВНИМАНИЕ ╞",TTL_CNT,_RED+WHITE);
   Wshadow(C_SHADOW);
   Wputs(" ");Wputs(str);
   if(KeyWait()==Key_Esc) val++;
   Wclose();
   }
 return(val);
}

int  *SaveSCR(int x,int y,int x1,int y1)
{
 return((int *)Wsave(x,y,x1,y1));
}

VOID CTYP  RestoreSCR(void *buf)
{
 Wrestore(buf);
 CurHide();
 return;
}

LCL VposT DOS_cur_POS;

int CTYP MemAllocErr(int arg)
{
 static struct {int Frez,delay;} melody[]={
#include "alarm.inc"
	  {0,-1}};
 int i=0,val;

 if(TcxlErr==W_ALLOCERR)
   {
   if(Error("Мало свободной памяти")==W_ALLOCERR && arg==0)
      {
      while(melody[i].delay!=-1 && !(KeyPeek()))
        {
        if(!melody[i].Frez) nosound();
        else sound(melody[i].Frez);
        delay(melody[i++].delay);
        }
      nosound();
      }
   if(arg)
      {
      WcloseAll();
      VcurPut(DOS_cur_POS);
      fprintf(stderr,"Internal: Memory allocations failure\n");
      exit(-1);
      }
   val=1;
   }
 else  val=0;
 return(val);
}

VOID CTYP Help(NOARG)
{
 int sav=VidFlg;

/*  VidParm(VP_BIO); */
  WpopUp(CNT_CNT,1,1,21,45,BOX_DBL,C_HBOX,C_HTXT);
  if(!MemAllocErr(0))
    {
	Wshadow(C_SHADOW);
	Wtitle(" ПОМОЩЬ ",TTL_CNT,C_HTITLE);
	Wprts(1,3,C_HTXT,"F1         - вызов помощи по программе");
	Wprts(3,3,C_HTXT,"-- текстовый режим ---");
	Wprts(5,3,C_HTXT,"Array keys - перемещение по пунктам");
	Wprts(6,3,C_HTXT,"Enter      - выбор пункта меню");
	Wprts(7,3,C_HTXT,"<Space>    - сменить опцию в поле ввода");
	Wprts(8,3,C_HTXT,"Esc        - возврат без изменений ");
	Wprts(9,3,C_HTXT,"Ctrl+Enter - запомнить поля ввода");
	Wprts(11,3,C_HTXT,"-- графический режим ---");
	Wprts(13,3,C_HTXT,"Array keys - перемещение маркера");
	Wprts(14,3,C_HTXT,"Tab        - изменять активный график");
	Wprts(15,3,C_HTXT,"<Space>    - max/min шаг маркера");
	Wprts(16,3,C_HTXT,"Enter      - поставить/убрать маркер");
	Wprts(17,3,C_HTXT,"BackSpace  - вернутся из увеличителя");
	Wmessage(" нажмите любую клавишу ",BRD_BOT,10,C_HBOX);
	MouPush(MOU_FULL);
	CurHide();
	MouShow();
	KeyWait();
	MouPop();
	Wclose();
	}
  VidFlg=sav;
  return;
}



main(int argc,char *args[])
{
  int cc=1,PRG_MODE;
  extern jmp_buf GFPE_ERR;
  extern MbldT TopMnu;
  extern char OptStr1[],OptStr2[],OptStr3[],OptStr4[],OptStr5[];

 TcxlKeyOff();
 TcxlMouOff();
 TcxlInit();


 if(VidDep!=25) VsetLin(25);
 fprintf(stdout,"\n\rStend (investigate version " STEND_CTRL ") v"
	 STEND_VER " (C) Samara-\"Trend\" 1992.\n\r\n");


 PRG_MODE=0;
 if(argc>1)
  {
  switch(args[1][0])
   {
   case '@':
	PRG_MODE=1;
	fprintf(stdout," Batch mode starting ... ");
	break;
   default:
    fprintf(stderr,"\nUsage :\n > %s [@]\n"
	           "  @    - batch mode (auto-connecting"
	           " with main computer)\n",args[0]);
    exit(2);
	break;
   }
  }

#ifdef COPYRIGHT
  if(_argv[0]==NULL)
    {
    fprintf(stderr,"Sorry: Internal error detected\n");
    exit(1);
    }
# define FILENAME _argv[0]
# define PASSWORD psw
# include "protect.ini"
#endif

 ReadCfg();

 if(!_8087)
  {
  VposT v;

  VcurGet(v);
  Esc(" Математический сопроцессор не найден ...");
  VcurPut(v);
  }

#ifdef COPYRIGHT
# define PASSWORD psw
# define PROTECTION cc
# include "protect.cmp"
#endif

 if(!TcxlCty)
 {
  NlsUpl=RusTabl;
  TcxlCty=007;
 }
 if(setjmp(GFPE_ERR)) WcloseAll();
 signal(SIGFPE,GlobalFPE);
 if(cc==0)
   {
   fprintf(stderr,"\n\007Unregistered version of program.\n");
   exit(-1);
   }

 if(KbndSet(Key_F1,Help,0))
   {
   TcxlErr=W_ALLOCERR;
   MemAllocErr(1);
   }
 if(!cc) fprintf(stdout,"\n Я же говорю - НЕЗАРЕГИСТРИРОВАННАЯ я версия\n");
 VcurGet(DOS_cur_POS);

 Wopen(0,0,24,79,BOX_SPA,C_BOX,C_TXT);
 MemAllocErr(1);
 CurHide();

 WdrwBox(22, 0,24,79,BOX_DBL,C_BOX); /* главное меню */
 WdrwBox( 0, 1, 8,44,BOX_DBL,C_BOX); /* общие хар-ки */
 WdrwBox( 0,47, 8,78,BOX_DBL,C_BOX); /* модель подстройки */
 WdrwBox( 9,47,14,78,BOX_DBL,C_BOX); /* испыт. сигналы */
 WdrwBox(15,47,20,78,BOX_DBL,C_BOX); /* тестовая проверка */

 Wprts(24, 4,C_BOX,"[ Управление стендом v" STEND_VER
                   " (C) НПП\"Тренд\" (г.Самара) ]");
 Wprts( 0, 7,C_MSEL,OptStr1);
 Wprts( 0,54,C_MSEL,OptStr2);
 Wprts( 9,50,C_MSEL,OptStr4);
 Wprts(15,54,C_MSEL,OptStr5);

 Wprts(11,75,C_TXT,"Гц.");
 Wprts(12,66,C_TXT,",");
 Wprts(11,66,C_TXT,",");
 Wprts(12,75,C_TXT,"В");
 Wprts(13,75,C_TXT,"В");
 Wprts(18,75,C_TXT,"В");
 Wprts(19,75,C_TXT,"В");

#ifdef COPYRIGHT
{
# define PASSWORD psw
# define PROTECTION cc
# include "protect.cmp"
}
#endif

 IdealType=0;

 GlbView();
 ModView();
 ObjView();
 DinView();
 IdealView();

 cc=12/cc;

  switch(PRG_MODE)
   {
   case 0:
     while(1)
       {
       if(MnuBuild(&TopMnu)==ERR_MEM) MemAllocErr(1);
       else MnuGet();
       }
   case 1:
	 UseMainIBM_PC();
     WcloseAll();
     VcurPut(DOS_cur_POS);
     fprintf(stdout," Ok.\n");
   break;
   }
 toDOS();
 return(0);
}



VOID CTYP OpenW(NOARG)
{
 Wshadow(C_SHADOW);
 CurHide();
 WsetEsc(TRUE);
 return;
}


