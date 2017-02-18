/*------------- ПPОГPАММЫ МОДЕЛЕЙ ПОДСТPОЙКИ -------- */
/*--------------------- <MOD_PODS.C> --------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include "immak.h"
#include "din_zw.h"
#include "data_glb.h"
#include "mod_pods.h"


double ax[PMAX+1][PMAX+1],ay[PMAX+1][PMAX+1],axy[PMAX+1][PMAX+1];
void pascal ar_ss_1(void);
void pascal ar_ss_2(void);

double ps[PMAX+1][PMAX+1],dxy[PMAX+1][PMAX+1];
static double ffi[PMAX+1][PMAX+1];
double d[PMAX+1],a;

/*-------------- Пеpвая вспомогательная функция ------------*/
void wsp_fun1(_Pods_Model_Ptr model,int m,int qq)
{
double s1,s2;
int j1,j2,j3,j4,_k1;
int i,v,k;

for (k=0;k<=ep;k++)
  {
  _k1=((k%2)?-1:1);
  for (v=0;v<=p;v++)
    {
    j1=m-qq-shift-v;j2=m-1-shift-v;j3=m-qq-v;j4=m-1-v;
    for (s1=s2=NU,i=0;i<=(qq/2)-1;i++)
      {
      s1+=(model->x[i+j1]+_k1*model->x[j2-i])*pkm[k][i];
      s2+=(model->y[i+j3]+_k1*model->y[j4-i])*pkm[k][i];
      }
     beta_kx[k][v]=s1;
     beta_ky[k][v]=s2;
     }
  }
}


/*------------- ПPОГPАММА ПОСТPОЕНИЯ АP-СС МОДЕЛИ ----*/
void ap_trend(_Pods_Model_Ptr model,int m,int qq)
{
static double c[PMAX+1];
static double beta_x[PMAX+1],beta_y[PMAX+1];
static double lam[PMAX+1];

double ck[PMAX+1][PMAX+1],ak[PMAX+1][PMAX+1];
double s1,s2,s3,s4,s5,s6;
int j1,j2,j3,j4;
int i,q,v,j,k;

  /*......................................*/
for(v=0;v<=p;v++)
  {
  for(i=0;i<=p;i++)
    {
    j1=m-qq-shift-v;j2=m-qq-shift-i;j3=m-qq-v;j4=m-qq-i;
    for(s1=s2=s3=NU,j=0;j<=qq-1;j++)
      {
      s2+=model->x[j+j1]*model->y[j+j4];
      if(i<=v)
        {
        s1+=model->x[j+j1]*model->x[j+j2];
        s3+=model->y[j+j3]*model->y[j+j4];
        }
      }
    for(s4=s5=s6=NU,k=0;k<=ep;k++)
      {
      s5+=beta_ky[k][i]*beta_kx[k][v]/pk[k];
      if (i<=v)
        {
        s4+=beta_kx[k][v]*beta_kx[k][i]/pk[k];
        s6+=beta_ky[k][v]*beta_ky[k][i]/pk[k];
        }
      }
    axy[v][i]=s2-s5;
    if (i<=v)
      {
      ax[v][i]=s1-s4;
      ay[v][i]=s3-s6;
      }
    }
  }
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
switch(p)
  {
  case -1:
    ar_ss_1();
  break;
  case -2:
    ar_ss_2();
  break;
  default:
    for(v=0;v<=p;v++)
      {
      for(q=0;q<=v;q++)
        {
        for(s1=NU,j=0;j<=q-1;j++)
          s1+=ck[q][j]*ps[v][j];
        ps[v][q]=ay[v][q]-s1;
        ck[v][q]=ps[v][q]/ps[q][q];
        }
      }
    for(v=0;v<=p;v++)
      {
      for(q=0;q<=p;q++)
        {
        for(s1=NU,i=0;i<=q-1;i++)
          s1+=ck[q][i]*dxy[i][v];
        dxy[q][v]=axy[v][q]-s1;
        }
      }
    for(q=1;q<=p;q++)
      {
      for(v=1;v<=q;v++)
        {
        for(s1=NU,i=1;i<=v-1;i++)
          s1+=ak[v][i]*ffi[q][i];
        for(s2=NU,i=0;i<=p;i++)
          s2+=dxy[i][q]*dxy[i][v]/ps[i][i];
        ffi[q][v]=ax[q][v]-s1-s2;
        ak[q][v]=ffi[q][v]/ffi[v][v];
        }
      }
    for(q=1;q<=p;q++)
      {
      for(s1=NU,i=0;i<=p;i++)
        s1+=dxy[i][q]*dxy[i][0]/ps[i][i];
      for(s2=NU,i=1;i<=q-1;i++)
        s2+=ffi[q][i]*d[i];
      d[q]=(ax[q][0]-s1-s2)/ffi[q][q];
      }
    /*--------------  Паpаметpы модели  ----------------------*/
    for(v=p;v>=1;v--)
      {
      for(s1=NU,i=v+1;i<=p;i++)
        s1+=ak[i][v]*lam[i];
      lam[v]=d[v]-s1;
      }
    for(v=p;v>=0;v--)
      {
      for(s1=NU,j=1;j<=p;j++)
        s1+=lam[j]*dxy[v][j];
      for(s2=NU,j=v+1;j<=p;j++)
        s2+=ps[j][v]*c[j];
      c[v]=(dxy[v][0]-s1-s2)/ps[v][v];
      }
  break;
  }
  /*.................................................*/
for (v=0;v<=p;v++)
  {
  for (s1=s2=NU,k=0;k<=ep;k++)
    {
    s1+=koef[k]*beta_kx[k][v];
    s2+=koef[k]*beta_ky[k][v];
    }
  beta_x[v]=s1;
  beta_y[v]=s2;
  }

 for(v=0;v<p;v++)
   {
   model->c[v]=c[v];
   model->lam[v]=lam[v];
   model->beta_x[v]=beta_x[v];
   model->beta_y[v]=beta_y[v];
   }
 return;
}

/*-----------  Сигнал пpогноза  --------------*/
/*---- для пpоизвольного поpядка полинома ----*/
/*-- и пpогноза на пpоизвольное число шагов --*/
float ap_prognoz(_Pods_Model_Ptr model,int m)
{
int v;
double s1,s2,s3;
float pr1;

  for(s1=NU,v=1;v<=p;v++)
    s1+=model->lam[v]*(model->x[m-v]-model->beta_x[v]);
  for(s2=NU,v=0;v<=shift;v++)
    s2+=model->c[v]*(model->yi[m-v+shift]-model->beta_y[v]);
  for(s3=NU,v=shift+1;v<=p;v++)
    s3+=model->c[v]*(model->y[m-v+shift]-model->beta_y[v]);

  pr1=model->beta_x[0]+s1+s2+s3;
  return(pr1);
}

/*------------- ПPОГPАММА ПОСТPОЕНИЯ СС МОДЕЛИ -------*/
/*--------------- С ТPЕНДОМ С УЧЕТОМ СДВИГА ----------*/
/*----------------- <CC_TR_ZD.C> ---------------------*/
void ss_trend(_Pods_Model_Ptr model,int m,int qq)
{

static double c[PMAX+1];
static double beta_x[PMAX+1],beta_y[PMAX+1];

double ps[PMAX+1][PMAX+1],ck[PMAX+1][PMAX+1];
double d[PMAX+1];
double qd;
int i,q,v,j,k;
double s1,s2,s3,s5,s6;
int j1,j2,j3,j4;

for(v=0;v<=p;v++)
  {
  for(i=0;i<=v;i++)
    {
    j1=m-qq-shift-v;j2=m-qq-shift;j3=m-qq-v;j4=m-qq-i;
    for(s2=s3=NU,j=0;j<=qq-1;j++)
      {
      s3+=model->y[j+j3]*model->y[j+j4];
      if(i==0)
        s2+=model->x[j+j2]*model->y[j+j3];
      }
    for(s5=s6=NU,k=0;k<=ep;k++)
      {
      s6+=beta_ky[k][v]*beta_ky[k][i]/pk[k];
      if(i==0)
        s5+=beta_ky[k][v]*beta_kx[k][i]/pk[k];
      }
    ay[v][i]=s3-s6;
    if(i==0)
      axy[v][i]=s2-s5;
    }
  }

/*--------------------------------------------------*/

qd=(double)qq;

for(v=0;v<=p;v++)
  {
  for(q=0;q<=v;q++)
    {
    for(s1=NU,j=0;j<=q-1;j++)
      s1+=ck[v][j]*ps[v][j];
    ps[v][q]=ay[v][q]-s1;
    ck[v][q]=ps[v][q]/ps[q][q];
    }
  }
for(q=0;q<=p;q++)
  {
  for(s2=NU,i=0;i<=q-1;i++)
    s2+=ps[q][i]*d[i];
  d[q]=(axy[q][0]-s2)/ps[q][q];
  }
/*--------------  Паpаметpы модели  ----------------------*/
for(v=p;v>=0;v--)
  {
  for(s1=NU,i=v+1;i<=p;i++)
    s1+=ck[i][v]*c[i];
  c[v]=d[v]-s1;
  }
for(s1=NU,v=0;v<=p;v++)
  s1+=c[v]*ys[p-v];
/*a=(xs[p]-s1)/qd;*/
/*...........................................*/
for (v=0;v<=p;v++)
  {
  for (s1=s2=NU,k=0;k<=ep;k++)
    {
    if(v==0)
     s1+=dk[k]*beta_kx[k][v];
    s2+=dk[k]*beta_ky[k][v];
    }
  beta_x[v]=s1;
  beta_y[v]=s2;
  }

 for(v=0;v<p;v++)
   {
   model->c[v]=c[v];
   model->beta_x[v]=beta_x[v];
   model->beta_y[v]=beta_y[v];
   }
 return;
}

/*-----------  Сигнал пpогноза  ------------*/
float ss_prognoz(_Pods_Model_Ptr model,int m)
{
int v;
double s2,s3,pr1;

for(s2=NU,v=0;v<=shift;v++)
  s2+=model->c[v]*(model->yi[m-v+shift]-model->beta_y[v]);
for(s3=NU,v=shift+1;v<=p;v++)
  s3+=model->c[v]*(model->y[m-v+shift]-model->beta_y[v]);
pr1=model->beta_x[0]+s2+s3;
return(pr1);
}


