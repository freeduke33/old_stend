/*--------ФУНКЦИЯ, РЕАЛИЗУЮЩАЯ МОДЕЛЬ ДИНАМИЧЕСКОЙ СИСТЕМЫ----*/
#include "immak.h"
#include <math.h>

#include "data_glb.h"



struct _FuncModel FuncModel[3]={{0,1.0},{0,1.0},{1,1.0}};
struct _AperModel AperModel[3]={{40,5,2},{40,5,2},{40,5,2}};
struct _KolbModel KolbModel[3]={{0.5,27.7},{0.5,27.7},{0.5,27.7}};
int Z_Type[4]={1,3,1,4};
float  imi_Const=0.0;


float dsys(float xf)
{
static double wy0,wy1=0.5;
static unsigned int iq=0;
int i,j;

  wy0=wy1;
  wy1=xf;

  for(i=0;i<3;i++)
    {
    switch(Z_Type[i])
	  {
	  case 1:  /* функциональное звено */
        switch(FuncModel[i].Type)
          {
          case 0: wy1=((double)(FuncModel[i].k*wy1));         break;
          case 1: wy1=((double)(FuncModel[i].k*wy1*wy1));     break;
          case 2: wy1=((double)(FuncModel[i].k*wy1*wy1*wy1)); break;
          case 3: wy1=((double)(FuncModel[i].k*wy1));         break;
          case 4: wy1=((double)(FuncModel[i].k*sqrt(wy1)));   break;
          }
	  break;
	  case 2:  /* апериодическое звено */
	  break;
      case 3:  /* колебательное звено */
        for(j=1;j<=nti;j++)
         {
         z[i][2]=pr[i][1]*z[i][1]-pr[i][2]*z[i][0]+pr[i][0]*wy1;
         z[i][0]=z[i][1];z[i][1]=z[i][2];
         }
		 wy1=z[i][2];
	  break;
      }
    }  /* end for */

  switch(Z_Type[3])
	{
	case 4: /* синусоида */
		wy1+=0.02*sin(2.0*M_PI*2.0*delt*(iq++)+1.33); break;
	case 5: /* полигармоника */
		break;
	case 6: /* полигармоника 3x5 */
		break;
	case 7: /* полигармоника 3x8 */
		break;
	}
  return(wy0);
}


