#ifndef __din_zw__
#define __din_zw__

#define TIMER
#define VIRTUAL

#include "pcl-71#.h"

#define  din_zw1(num) ((((float)(UsePCL((int)((num)*pcl_RANGE+0.5))))-y_n)/(y_w-y_n))
#define  din_zw2(num) dsys(num) /*((dsys(num)*pcl_RANGE-y_n)/(y_w-y_n))*/

#define convertDAC(X)  (X*pcl_RANGE+0.5)
#define convertADC(Y)  ((Y-y_n)/(y_w-y_n))

#ifndef TIMER
# define disablePCL()
#endif


#define CheckKeyboard()\
      if((keyboard[0]!=keyboard[1]) && bioskey(1)==0x11b)\
	 {\
	 DisablePCL();\
	 Error(" Управление прервано пользователем ");\
	 return;\
	 }\

#ifdef VIRTUAL
extern float *x,*y,*yi;
#else
extern float x[],y[],yi[];
#endif

#endif
