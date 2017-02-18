/* Дополнительные функции для работы со стендом в штатном режиме */
/* или имитации работы стенда */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "immak.h"
#include "Look_def.h"
#include "string.h"
#include "signal.h"
#include "dos.h"
#include "bios.h"
#include "setjmp.h"
#include "alloc.h"
#include "float.h"
#include "math.h"

#include "din_zw.h"
#include "stend.h"
#include "data_glb.h"
#include "mod_pods.h"

#define FPE_DENORMAL    130

#define GRF_1_COL        MAGENTA,WHITE,WHITE,BLUE,\
			 CYAN,BLUE,\
			 MAGENTA,WHITE,\
			 BLUE,CYAN,\
			 YELLOW,GREEN,BLACK

#define GRF_2_COL        MAGENTA,WHITE,WHITE,BLUE,\
                         CYAN,BLUE,\
			 MAGENTA,WHITE,\
			 BLUE,CYAN,\
			 YELLOW,GREEN,BLACK

#define GRF_3_COL        MAGENTA,WHITE,WHITE,BLUE,\
                         CYAN,BLUE,\
			 MAGENTA,WHITE,\
			 BROWN,CYAN,\
			 YELLOW,GREEN,YELLOW



unsigned int far *keyboard=MK_FP(0,0x41a);
jmp_buf FPE_ERR;


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//


int Init_Loop(int mode)
{
 int i;
 double fr;

/*-----------БЛОК ФОРМИРОВАНИЯ ЗАДАННОГО ВЫХОДНОГО СИГНАЛА----------*/
#ifdef VIRTUAL
 if(CreateBuf(&x ,nr+Q_P)) return(1);
 if(CreateBuf(&y ,nr+Q_P)) return(1);
 if(CreateBuf(&yi,nr+Q_P)) return(1);
#endif

 Pods_Model.x=x;
 Pods_Model.y=y;
 Pods_Model.yi=yi;

 delt=1/fdi;
 dtds=1.0/fdds;
 nti=(int)(delt*fdds);

  /*--БЛОК ВЫЧИСЛЕНИЯ ВСПОМОГАТЕЛЬНЫХ ПАРАМЕТРОВ ДИНАМ.СИСТЕМЫ.--*/


for(i=0;i<3;i++)
  {
  fr=KolbModel[i].Frez/sqrt(1.0-2.0*KolbModel[i].Betta*KolbModel[i].Betta);
  pr[i][9]=2.0*M_PI*fr;
  pr[i][3]=dtds*pr[i][9];
  pr[i][4]=pr[i][3]*pr[i][3];
  pr[i][5]=4.0+4.0*KolbModel[i].Betta*pr[i][3]+pr[i][4];
  pr[i][6]=1.0+2.0*KolbModel[i].Betta*pr[i][3]+pr[i][4];
  pr[i][0]=1.0*pr[i][4]/pr[i][6];
  pr[i][1]=2.0*(1.0+KolbModel[i].Betta*pr[i][3])/pr[i][6];
  pr[i][2]=1.0/pr[i][6];
/*  qq[i]=((int)(5.0/(KolebModel[i].Betta*2.0*M_PI*fr*dtds)))+1;   */
  z[i][0]=z[i][1]=z[i][2]=NU;
  }

 if(!mode) return(0);

/* ---------- ФОРМИРОВАНИЕ ЭТАЛОННОГО СИГНАЛА ------------*/

 for(i=0;i<=nr;i++)
   y[i]=x[i]=yi[i]=NU;

  if(GenIdeal(IdealType)) return(1);

 if(ncikl==0) ncikl=1;

 return(0);
}

void far get_my(float *x,float *y,triadGRF *p,int pos)
{
 *x=(float)pos*delt;
 *y=10*((float *)p->Array2)[pos]-5;
 return;
}

void far get_my1(float *x,float *y,triadGRF *p,int pos)
{
 *x=(float)pos*(temax-temin)/50.0+temin;
 *y=10*((float *)p->Array2)[pos]-5;
 return;
}


triadGRF trdP1[3]={{{5,510,995,995,GRF_1_COL},
	  0,-5.0,50,5.0,get_my,NULL,NULL,(void *)yp,
	  0,0,0,50,"Пеpеходная хаp-ка","Время [с]","y[%6.3f]=%+7.3f",NULL,
	  grfPARAM_ALL,grfRANGE|grfBLABEL},
		  {{5,5,995,490,GRF_2_COL},
	  -5,-5,5,5,get_my1,NULL,NULL,(void *)yst,
	  0,0,0,50,"Статическая хаp-ка","Вход [В]",
	  "y[%+7.3f]=%+7.3f(%+7.3f)",NULL,
	  grfPARAM_ALL|grfDOUBLE,grfRANGE|grfBLABEL},
		  {{0,0,0,0,GRF_3_COL},
	  -5,-5,5,5,get_my1,NULL,NULL,(void *)&yst[51],
	  0,0,0,50}};




void far get_M0(float *x,float *y,triadGRF *p,int pos)
{
 *x=(float)(pos-p->beg)*delt+p->Xmin;
 *y=10*((float *)p->Array2)[pos]-5;
 return;
}

VOID CTYP Loop_Graph(NOARG)
{
 void *sav=NULL;
 triadGRF trdP2[3]={{{5,510,995,995,GRF_1_COL},
       0,-5,10,5,get_M0,NULL,NULL,NULL,
	   0,0,0,50,"Вход","Время [с]"," x[%6.3f]=%+7.4f",
	   " Время=%6.3f ",grfPARAM_ALL,grfRANGE|grfBLABEL},
		   {{5,5,995,490,GRF_2_COL},
       0,-5,10,5,get_M0,NULL,NULL,NULL,
	   0,0,0,50,"Выход","Время [с]"," y[%6.3f]=%+7.4f(%+7.4f)",
	   " Время=%6.3f ",grfDOUBLE|grfPARAM_ALL,grfRANGE|grfBLABEL},
		   {{0,0,0,0,GRF_3_COL},
       0,-5,10,5,get_M0,NULL,NULL,NULL,
	   0,0,0,50,"","","",""}};

  switch(_USE_)
   {
   case 0:
     Error("Нет данных об управлении с такими параметрами");
     return;
   case 2:
     Esc(" Управление было прервано, конечная область графиков не определена");
   }
  {
  int i,beg,end;
  MathBeg_End(&beg,&end);
  for(i=0;i<3;i++)
    {
    trdP2[i].Xmax=end*delt;
    trdP2[i].Xmin=beg*delt;
    trdP2[i].end=nr+Q_P-1;
	trdP2[i].beg=beg-nr*(ncikl-1);
    }
  trdP2[0].Array2=x;
  trdP2[1].Array2=y;
  trdP2[2].Array2=yi;
  }

  sav=SaveSCR(0,0,24,79);
  if(sav)
    {
    if(InitGRF(grfGRAPHICS)==okGRF)
       {
       signal(SIGFPE,MyFPE);
       if(!setjmp(FPE_ERR)) LookGRF(3,trdP2);
       DeinitGRF(grfTEXT);
       signal(SIGFPE,GlobalFPE);
       }
    else Error("Не могу включить графический режим");
    RestoreSCR(sav);
    }
  else Error("Нет свободной памяти");
 return;
}

VOID CTYP Test_Graph(NOARG)
{
 void *sav=NULL;

 sav=SaveSCR(0,0,24,79);
 if(sav)
   {
   if(InitGRF(grfGRAPHICS)==okGRF)
     {
     trdP1[0].Xmax=50*delt;
     signal(SIGFPE,MyFPE);
     if(!setjmp(FPE_ERR)) LookGRF(3,trdP1);
     DeinitGRF(grfTEXT);
     signal(SIGFPE,GlobalFPE);
     }
   else Error("Не могу показать графики");
   RestoreSCR(sav);
   }
 else Error(" Нет свободной памяти ");
 return;
}


/*=========================================================================*/

void MsgFPE(int sig)
{
 static char str[200];

 strcpy(str," Возникла ошибка при вычислениях : ");
 switch(sig)
   {
   case FPE_INTOVFLOW:      strcat(str, "INT overflow" );               break;
   case FPE_INTDIV0:        strcat(str, "INT divide by 0" );            break;
   case FPE_INVALID:		strcat(str, "Domain" );                     break;
   case FPE_DENORMAL:		strcat(str, "Denormal" );                   break;
   case FPE_ZERODIVIDE:     strcat(str, "Divide by 0" );                break;
   case FPE_OVERFLOW:		strcat(str, "Overflow" );                   break;
   case FPE_UNDERFLOW:      strcat(str, "Underflow" );                  break;
   case FPE_INEXACT:		strcat(str, "Partial loss of precision" );  break;
   case FPE_STACKFAULT: 	strcat(str, "Stack fault" );                break;
   default:                 strcat(str, "Unknown" );                    break;
   }
 Error(str);
 _fpreset();
 return;
}

void MyFPE(int sig,int code)
{
#ifdef TIMER
 DisablePCL();
#endif
if(sig==SIGFPE)
  {
  signal(SIGFPE,MyFPE);
  MsgFPE(code);
  longjmp(FPE_ERR,1);
  }
 return;
}

jmp_buf GFPE_ERR;
void GlobalFPE(int sig,int code)
{
 if(sig==SIGFPE)
   {
   signal(SIGFPE,GlobalFPE);
   MsgFPE(code);
   longjmp(GFPE_ERR,1);
   }
 return;
}

void LoopExit(char *str)
{
#ifdef TIMER
 DisablePCL();
#endif
 Error(str);
 return;
}

float din_zw(float x)
{
 if(OBJType) return(din_zw1(x));
 else return(din_zw2(x));
}

int CreateBuf(float **buf,unsigned long size)
{
#ifdef VIRTUAL
	if(*buf) farfree(*buf); *buf=NULL;
	if((*buf=farcalloc(sizeof(float),(size+1)))==NULL)
	  {
	  Error(" Нет памяти для размещения данных");
	  return(1);
	  }
    else return(0);
#else
 return(1);
#endif
}

int MathBeg_End(int *beg,int *end)
{
 int _beg,_end;
 _end=nr*ncikl+Q_P-1;
 _beg=nr*(ncikl-1);
 if(ncikl>1) _beg+=Q_P-q-2*p+2;
 *beg=_beg;
 *end=_end;
 return(_end-_beg);
}


int CTYP GenIdeal(int code)
{
 FILE *in;
 int i,re1;
 char str[160];
 float prr1,prr0;

  if(yi==NULL)
	{
	Error(" Недостаток памяти при формировании эталона");
	return(1);
	}

  if(code==3)
    if((in=fopen(filename,"r"))==NULL)
    {
     Error("Файл эталонного сигнала не найден");
     return(1);
    }
    else
    {
     for(i=0;i<8;i++) fgets(str,120,in);
    }

  prr0=2.0*3.14159*delt;
  for(i=0;i<=nr;i++)
    {
	if(bioskey(1)==0x11b)
	  {
	  bioskey(0);
	  Error("Формирование эталонного сигнала прервано пользователем");
	  return(1);
	  }
    prr1=prr0*(float)i;
    switch(code+1)
      {
      case 1:
	yi[i+Q_P]=u0+a0*sin(prr1*f0d);
      break;
      case 2:
	yi[i+Q_P]=u1+a1*sin(prr1*f1d)+a2*sin(prr1*f2d);
      break;
      case 3:
	yi[i+Q_P]=ush+(((float)(random(20000))-10000.0)/10000.0)*ash;
      break;
      case 4:
	fscanf(in," %d",&re1);
	yi[i+Q_P]=uf+(re1/(float)pcl_RANGE)*af;
      break;
      default:
	Error("Неизвестный тип эталонного сигнала");
	return(1);
      }
    }

 if(code==4) fclose(in);
 return(0);
}



