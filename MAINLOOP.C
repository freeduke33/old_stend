#include <stdio.h>
#include <stdlib.h>

#include "immak.h"
#include "signal.h"
#include "setjmp.h"
#include "math.h"
#include "bios.h"

#include "stend.h"
#include "data_glb.h"
#include "mod_pods.h"
#include "din_zw.h"


Word NEW_X,BEG_OF_LINE,END_OF_LINE,LOOP_CTRL;
int ABORT;



int pascal INT_loop(NOARG);


/* ------------------------------------------------------------ */
/* ----------[Главный цикл управления стендом/моделью]--------- */
/* ------------------------------------------------------------ */


VOID CTYP Control(NOARG)
{
 extern jmp_buf FPE_ERR;
 extern double koef[],dk[];
 int i,j,m,k,_k1;

 signal(SIGFPE,MyFPE);
 if(!setjmp(FPE_ERR))
   {
   if((Init_Loop(1))) return;


   if(ncikl==1) BEG_OF_LINE=END_OF_LINE=nr+Q_P;
   else
	 {
	 BEG_OF_LINE=Q_P-(q+p*2);
	 END_OF_LINE=nr+BEG_OF_LINE;
	 }
  NEW_X=BEG_OF_LINE;
  ABORT=0;
  LOOP_CTRL=0;

for(j=0;j<=20;j++)
  {
  for(i=0;i<=ep;i++)
    {
    switch(i)
       {
       case 0: pkm[i][j]=1.0;
       break;
       case 1: pkm[i][j]=1.0-2.0*j/((float)(q-1));
       break;
       default:
         pkm[i][j]=(pkm[1][j]*pkm[i-1][j]*(2*i-1)*(q-1)\
         /((float)(i*(q-i))))-pkm[i-2][j]*(i-1)*(q+i-1)\
         /((float)(i*(q-i)));
       break;
       }
    }
  }
for(pk[0]=(float)q,i=1;i<=ep;i++)
	pk[i]=pk[i-1]*(2*i-1)*(q+i)/((float)((2*i+1)*(q-i)));
for(k=0;k<=ep;k++)
  {
  _k1=((k%2)?-1:1);
  koef[k]=(float)(_k1*(2*k+1)*(q+1+k+k*k))/(float)(q*(q+1));
  dk[k]=pkm[k][q+shift]/pk[k];
  }

Reload();
/*----------- ПЕРЕХОДНЫЙ ПРОЦЕСС + ВКЛ. ТАЙМЕР ------- */

#include "first.c"

/*----------- ЗАМЫКАНИЕ ОБРАТНОЙ СВЯЗИ -------------*/

Pods_Model.m=i-1;
Math_Model(i);
PassWrite(1);

if(ModType)
  {
  SetFuncPCL((void far *)INT_loop);
  while(!ABORT)
      {
      CheckKeyboard();
      m=Pods_Model.m;
      Math_Model(m);                     /*????????????????????*/
      }
  SetFuncPCL(NULL);
  }
else
  {
  while(!ABORT)
       {
       CheckKeyboard();
       m=Pods_Model.m;
       _AX=convertDAC(Pods_Model.y[Pods_Model.m]);
       INT_loop();
       Pods_Model.y[Pods_Model.m]=din_zw(convertADC(_AX));
       Math_Model(m);
       }
  }
 PassText("STOP ");
 FinishMelody();
/*----------------------- ВЫКЛЮЧЕНИЕ ТАЙМЕРА -----------------------*/

#ifdef TIMER
 DisablePCL();
 {
  static char str[80];
  unsigned int  real;

  real=nr+qp;

  if(real<NumDigit/ncikl)
    {
    sprintf(str," Время вычислений превышено на %d тика(ов) ",NumDigit-real);
    Esc(str);
    }
 }
#endif
   Reload();
   _USE_=1;
   }
 signal(SIGFPE,GlobalFPE);
 return;
}

void Math_Model(int m)
{

    switch(ModType)
      {
      case 0:
          wsp_fun1(&Pods_Model,m,q);
          ap_trend(&Pods_Model,m,q);
      break;
      case 1:
          wsp_fun1(&Pods_Model,m,q);
	  ss_trend(&Pods_Model,m,q);
      break;
      }
  return;
}

/*---------------------------*/
/*---- Разгрузка стенда -----*/
/*---------------------------*/

VOID CTYP Reload(NOARG)
{
 int i;
 float nil;

 if(Ini_Time()) return;
 for(i=0;i<10;i++) nil=din_zw(Reload_Val);
 nil=nil;
 DisablePCL();
 return;
}



VOID CTYP Write(NOARG)
{
 int m,i,j;

 if(Init_Loop(1)) return;

 _USE_=2;

/*----------- ПЕРЕХОДНЫЙ ПРОЦЕСС + ВКЛ. ТАЙМЕР ------- */
#  include "first.c"

for(j=0;j<ncikl;j++)
 for(m=i;m<(nr+Q_P);m++)
   {
   CheckKeyboard();
   y[m]=din_zw((x[m]=yi[m]));
   }

 DisablePCL();
 _USE_=1;
 FinishMelody();
 return;
}

/*------------------ ЧТЕНИЕ СИГНАЛА СО СТЕНДА ------------------*/

int CTYP Read(NOARG)
{
 int i,val=1;

#ifdef VIRTUAL
 if(CreateBuf(&yi,nr)) return (0);
#endif

 PassWin("ВНИМАНИЕ","ПРИЕМ ЭТАЛОННОГО СИГНАЛА СО СТЕНДА");
 if(Ini_Time()==0)
   {
   for(i=0;i<nr;i++) yi[i]=din_zw(NU);
   DisablePCL();
   }
 else val=0;
 PassEnd();
 return val;
}

int CTYP Ini_Time(NOARG)
{
 unsigned int BufTim,DivCount;
 float TAKT;
 int val;
 char str[100];
 char msg[][40]={"PCL unactive",
                 "UNIPOLAR mode",
		 "DT error",
		 "Time Out",
		 "Reinstall"};

/*-- БЛОК ИНИЦИАЛИЗАЦИИ ТАЙМЕРА ---------------------------------*/

 if(OBJType) TAKT=ETAKT;
 else TAKT=FTAKT;

 if(fdi>20.0)
   {
   BufTim=(unsigned int)(TAKT/(fdi*1000));
   DivCount=1000;
   }
 else
   {
   BufTim=(unsigned int)(TAKT/(fdi*10000));
   DivCount=10000;
   }
 NumDigit=0;
 val=0;
#ifdef TIMER
 if(OBJType) val=EnablePCL(BufTim,DivCount);
 else val=EnablePCLw();
#endif
 sprintf(str,"Ошибка при инициализации платы УСО : %s",msg[val-1]);
 if(val) Error(str);
return(val);
}

/***************************/
/*  x -->             m    */
/*  y <--             m    */
/*  x=INT_loop(y)     m+1  */

int pascal INT_loop(NOARG)
{
  int m;
  float x;

  Pods_Model.y[m=Pods_Model.m]=convertADC(_AX);

  if(m>=END_OF_LINE)
	{
    Pods_Model.x [NEW_X]=Pods_Model.x [m];
    Pods_Model.y [NEW_X]=Pods_Model.y [m];
    Pods_Model.yi[NEW_X]=Pods_Model.yi[m];
	if(m==nr+Q_P)
	  {
	  Pods_Model.m=NEW_X;
	  NEW_X=BEG_OF_LINE;
	  if((++LOOP_CTRL)>=ncikl) ABORT=1;
	  PassWrite(LOOP_CTRL+1);
	  }
	else NEW_X++;
	}

  m=++Pods_Model.m;

  switch(ModType)
	{
	case 0:  x=ap_prognoz(&Pods_Model,m); break;
	case 1:  x=ss_prognoz(&Pods_Model,m); break;
	default: x=Reload_Val;
	}

  if(x<=NU) x= (m%2) ? 0.011111 : 0.033333; else
  if(x>1.0) x= (m%2) ? 0.999999 : 0.977777;

  Pods_Model.x[m]=x;
  return(convertDAC(x));
}


