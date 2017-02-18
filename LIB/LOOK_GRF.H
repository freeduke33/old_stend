/*  <<<<  LOOK_GRF.H  >>>> */

#ifndef __LOOK_GRF__
#define __LOOK_GRF__

/*       ОПИСАНИЕ ПРОЦЕДУР ПРОСМОТРА ГРАФИКОВ

 * вывести произвольное число графиков
 *
 * int far LookGRF(int num,          число графиков
		   triadGRF p[]);    массив описаний каждого графика

 * вывести один график для просмотра
 *
 * int far LookGRF1(float *y,        массив значений
		    char *name,      заголовок графика
		    int num,         число элементов в массиве
		    float delt);     интервал дискретизации

 * просмотр 2 графиков в 2 окнах
 *
 * int far LookGRF2(float *y1,       массив значений 1 графика
		    char *name1,     заголовок графика
		    int num1,        число элементов в массиве
		    float delt1,     интервал дискретизации
		    float *y2,       массив значений 2 графика
		    char *name2,     заголовок графика
		    int num2,        число элементов в массиве
		    float delt2);    интервал дискретизации

 * просмотр 3 графиков в 2 окнах (1 - в верхнем, 2 - в нижнем окнах)
 *
 * int far LookGRF3(float *y1,       массив значений 1 графика
		    char *name1,     заголовок графика
		    int num1,        число элементов в массиве
		    float delt1,     интервал дискретизации
		    float *y2,       массив значений 2 графика
		    char *name2,     заголовок графика
		    int num2,        число элементов в массиве
		    float delt2,     интервал дискретизации
		    float *y3);      массив значений 3 графика

 *  Изменить цвета графиков при использовавнии функций Look_GRF??
 *
 * int far SetLook_Col(unsigned int num,         номер графика
		       struct _scr_params *col)  структура цветов

 *  Описать функцию очистки экрана при использовании LookGRF
 *
 * int far SetLook_Clr(int far (*Fun)(int mode));

*/



#include "_grf.h"

#define grfUNSELECT   0x01       /* окно графика без маркера */
#define grfUNMARK     0x02       /* окно графика без возможности лупы */
#define grfDOUBLE     0x04       /* в окно выводится график, определенный */
				 /* в следующем элементе массива описания */
#define grfSCALE_FIL  0x08       /* масштабирование полного графика */
#define grfSCALE_FRM  0x10       /* масштабирование лупы */
#define grfSCALE_ALL  grfSCALE_FIL|grfSCALE_FRM
#define grfPARAM_ALL  0x20       /* вывод текущих значений курсора */
				 /* в полном графике */

/*  Параметр при вызове ClrFun(int) */
#define grfCLR_FULL   1          /* показываются все графики */
#define grfCLR_FRM    2          /* показывается лупа одного графика */

typedef struct _triadGRF_ triadGRF;

struct _scr_params
{
 int X1,Y1,X2,Y2;    /* положение графика */
 unsigned char axis, /* Цвета графика ... */
	       box_wind,
	       box_grf,
	       box_back,
	       title_fore,
	       title_back,
	       act_fore,
	       act_back,
	       grf_fore,
	       grf_back,
	       num,msg,
	       marker;
};

struct _triadGRF_
{
 struct _scr_params scr;      /* параметры окна на экране */
 float Xmin,Ymin,Xmax,Ymax;   /* масштаб графика */
 void far (*GetPoint)(float *X,float *Y,triadGRF *p,int num);
			      /* функция выдачи  X и Y по номеру отсчета */
			      /* GetPoint(float *X,float *Y,triadGRF *p,int num) */
 void far (*MaxMin)(triadGRF *p);
			      /* функция подсчета масштаба по осям */
			      /* MaxMin(triadGRF *p) */
 float *Array1,*Array2;       /* указатели на массивы данных */
 float dX,dY;                 /* приращения для расчетов монотонных X и Y */
 int  beg,end;                /* границы отсчетов */
 char *title;                 /* заголовок */
 char *argument;              /* описание аргумента */
 char *StrParam1;             /* строка для вывода текущих значений */
 char *StrParam2;	      /* строка для вывода инфо о маркере */
 int  ctrl;		      /* слово состояния */
 int  open_mode;	      /* режим открытия окна */
 /*-------------[for internal use]------------*/
 int pos,step,cur;
 float Xpos,first;
 int H,use,mark;
};

int far LookGRF(int num,triadGRF p[]);
int far LookGRF1(float *y,char *name,int num,float delt);
int far LookGRF2(float *y1,char *name1,int num1,float delt1,
		 float *y2,char *name2,int num2,float delt2);
int far LookGRF3(float *y1,char *name1,int num1,float delt1,
		 float *y2,char *name2,int num2,float delt2,
		 float *y3);

int far SetLook_Col(unsigned int num,struct _scr_params *col);
int far SetLook_Clr(int far (*Fun)(int mode));

#define AXIS_MESSAGE    0x01
#define TITLE_MESSAGE   0x02
#define PARAMS_MESSAGE  0x04
#define MARKER_MESSAGE  0x08
#define ALL_MESSAGE     0x07


extern struct _scr_params FULLscr;


#endif

