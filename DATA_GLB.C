#include "immak.h"
#include "stend.h"
#include "data_glb.h"
#include "din_zw.h"
#include "mod_pods.h"

//================================================================
// описание идеального отклика обьекта управления
//
int IdealType=0;                        // тип идеального отклика
//
char filename[100]="NONAME.STD";        // из файла
double af=1.00, uf=0.00;
//
double a0=0.10, u0=0.50, f0d=13.14;     // синусоида
//
double a1=0.24, u1=0.50, f1d=3.0,       // полигармоника
       a2=0.08, u2=0.00, f2d=6.0;
//
double ash=0.1, ush=0.4;		// совсем не белый шум
//================================================================


//================================================================
//  Описание тракта приема/передачи сигналов (ЦАП/АЦП)
float y_w=pcl_RANGE,y_n=0;


//================================================================
// Описание управляемого обьекта
//
int CtrlType=0;			        // режим управления
int OBJType=0;				// тип обьекта

double cz=1.0,cn=1.0,beta=0.5,frr=27.7;
double delt,dtds;
double fdi=100.0,fdi;


// ===============================================================
// Описание управляющего обьекта
//

int m,p=1,q=10,nti,shift=1,_USE_=0;
int qp=Q_P,GlbType,IsTrend;

int ModType,qq[3];
Word ncikl=1,nr=200,ep=0;
float Reload_Val=0.5;

#ifndef VIRTUAL
 float x[NMAX+1],yi[NMAX+1],y[NMAX+1];
#else
 float *x,*y,*yi;
#endif

double z[3][MI+1],pr[3][11];
float yp[52],yst[102];
double fdds=1000.0,temax=5,temin=-5;
double koef[EMAX+1],dk[QMAX+EMAX+10];
double beta_kx[EMAX+1][PMAX+1],beta_ky[EMAX+1][PMAX+1];
double beta_x[PMAX+1],beta_y[PMAX+1];

double pkm[EMAX+1][QMAX+10],pk[EMAX+1];
double c[PMAX+1],lam[PMAX+1];
double xs[PMAX+1],ys[PMAX+1];


struct __Pods_Model Pods_Model;


