#ifndef __data_glb__
#define __data_glb__

#include "mod_pods.h"

#ifndef Word
#define Word unsigned int
#endif


// -------------------[ модель управления ]----------------------------

extern double fdi,fdds,frr,beta,temin,temax,cn,cz,delt;
extern float y_n,y_w;
extern int   p,q,IsTrend,GlbType,ModType,
	     _USE_,shift,qp,CtrlType,OBJType;

extern Word nr,ncikl,ep;
extern float Reload_Val;

extern struct __Pods_Model Pods_Model;

// ---------------[ идеальный сигнал отклика ]------------------------
//
extern int IdealType,CtrlType;
extern char filename[];
extern double f0d,a0,u0, f1d,a1,u1, f2d,a2,u2, af,uf, ash,ush;

// ---------------[ имитационная модель обьекта ]----------------------
//
struct _FuncModel
{
	int Type;
	float k;   /* Тип звена , коэффициент */
};

struct _AperModel
{
	float FV,FN;  /* Верхняя, нижняя граница */
	int p;        /* Порядок */
};

struct _KolbModel
{
	float Betta,Frez;  /* Степень успокоения, резонансная частота */
};


extern struct _FuncModel FuncModel[3];
extern struct _AperModel AperModel[3];
extern struct _KolbModel KolbModel[3];
extern int Z_Type[4];
extern float imi_Const;


/*==========================[ ?????????? ]======================*/

extern double dtds;
extern int nti;
extern double pr[3][11],z[3][MI+1];
extern float yst[],yp[];
extern double pkm[EMAX+1][QMAX+10],pk[EMAX+1];
extern double c[PMAX+1],lam[PMAX+1];
extern double xs[PMAX+1],ys[PMAX+1];
extern double beta_kx[EMAX+1][PMAX+1],
              beta_ky[EMAX+1][PMAX+1];
extern double koef[EMAX+1],
              dk[QMAX+EMAX+10];


#endif
