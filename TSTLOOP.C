/*-------------------------------------------*/
/*--------[ Тестовые характеристики ]--------*/
/*-------------------------------------------*/
#include "immak.h"
#include "stend.h"
#include "data_glb.h"
#include "din_zw.h"



VOID CTYP Test_Fun(int MODE)
{
 int i,j,k;
 float re,_temax,_temin;

 if(Init_Loop(0)) return;

/*----------------------- ИНИЦИАЛИЗАЦИЯ ТАЙМЕРА --------------------*/
 Ini_Time();

/*------------------- ВЫЧИСЛЕНИЕ ТЕСТОВЫХ ХАРАКТЕРИСТИК -------------------*/

_temax=(temax+5.0)/10.0;
_temin=(temin+5.0)/10.0;


/*-- БЛОК ВЫЧИСЛЕНИЯ СТАТИЧЕСКОЙ ХАР-КИ ДИН.СИСТЕМЫ--*/
if(MODE & STAT_TST)
   {
      y_n=0;      /* -5B */
      y_w=pcl_RANGE;  /* +5B */
      switch(GlbType)
	{
	case 0:
	   for(i=0;i<=50;i++)
	     {
             for(j=0;j<=20;j++)
	       re=din_zw((((_temax-_temin)/50.0)*(double)i)+_temin);
	     yst[i]=re;
	     }
	   yst[101]=re;
	   for(k=100,i=49;i>=0;i--,k--)
             {
             for(j=0;j<=20;j++)
	       re=din_zw(((_temax-_temin)/50.0)*((double)i)+_temin);
	     yst[k]=re;
             }

	   for(i=0;i<=20;i++) re=din_zw(Reload_Val);
	   break;

/*
       case 2:
	   d2=xw*2.0/((float)(q*(q+1)));
           for(k=0;k<=30;k++) re=din_zw(0.5);
           for(k=1;k<=nh;k++)
             {
             dtds=din_zw(x[(2*k-2)*q%3]=NU);
             dtds=din_zw(x[((2*k-2)*q+1)%3]=((float)k)*d2/(float)nh);
             for(m=(2*k-2)*q+2;m<=(2*k-1)*q;m++)
               dtds=din_zw(x[m%3]=2.0*x[(m-1)%3]-x[(m-2)%3]+((float)k)*d2/(float)nh);
             y[k]=din_zw(x[((2*k-1)*q+1)%3]=x[((2*k-1)*q-1)%3]);
             for(m=(2*k-1)*q+2;m<=2*k*q;m++)
               dtds=din_zw(x[m%3]=2.0*x[(m-1)%3]-x[(m-2)%3]-((float)k)*d2/(float)nh);
             }

         for(k=0;k<=30;k++) dtds=din_zw(0.5);
	 break;
	 */
	}
#define _MAX y_w
#define _MIN y_n
    for(_MAX=_MIN=yst[0],i=0;i<=50;i++)
      {
      if(_MAX<yst[i]) _MAX=yst[i];
      if(_MIN>yst[i]) _MIN=yst[i];
      }
    _MAX*=pcl_RANGE;
    _MIN*=pcl_RANGE;
  }

/*-- БЛОК ВЫЧИСЛЕНИЯ ПЕРЕХОДНОЙ ХАР-КИ ДИН.СИСТЕМЫ--*/
if(MODE & DIN_TST)
  {
  for(i=50;i>=0;i--)
	   re=din_zw(((Reload_Val-_temin)/50)*(double)i);
  re=din_zw(((Reload_Val-_temin)/50.0)*((double)i)+_temin);
  for(i=0;i<=10;i++) yp[i]=din_zw(_temin);
  for(   ;i<=50;i++) yp[i]=din_zw(_temax);
  for(i=50;i>=0;i--)
	  re=din_zw(((_temax-Reload_Val)/50.0)*((double)i)+Reload_Val);
  for(i=0;i<=20;i++)
	   re=din_zw(Reload_Val);
  }
/*------------------------- ВЫКЛЮЧЕНИЕ ТАЙМЕРА --------------------------*/
#ifdef TIMER
 DisablePCL();
#endif
 return;
}

