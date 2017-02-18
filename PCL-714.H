

#define FTAKT 1179630.0              /* тактовая частота системного таймера */
#define pcl_RANGE  16384             /* диапазон изменения ЦАП АЦП */

extern unsigned int  NumDigit,	    /* число тиков таймера */
		     DivCount,	    /* делитель частоты */
		     BufTim;        /* FTAKT / нов. частота  */

void far enable714(void);
void far enable714w(void);
void far disable714(void);
unsigned int far pascal use714(unsigned int put);
void far pascal SetLoopFun(void far (*Fun)(void));



