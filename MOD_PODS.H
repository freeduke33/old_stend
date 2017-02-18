#ifndef __mod_pods__
#define __mod_pods__

typedef struct __Pods_Model *_Pods_Model_Ptr;

struct __Pods_Model
{
  float *x,*y,*yi;                         /* массивы данных */
  int m;

  /* параметры вспомогательной модели */
  double beta_x[PMAX+1],beta_y[PMAX+1];
  double lam[PMAX+1];
  double c[PMAX+1];
};

float ss_prognoz(_Pods_Model_Ptr,int);
float ap_prognoz(_Pods_Model_Ptr,int);

void ss_trend(_Pods_Model_Ptr,int,int);
void ap_trend(_Pods_Model_Ptr,int,int);
void wsp_fun1(_Pods_Model_Ptr,int,int);

#endif


