

#define FTAKT 1179630.0              /* тактовая частота системного таймера */
#define ETAKT 10000000.0			 /* тактовая частота таймера платы */

#define pcl_RANGE  4096             /* диапазон изменения ЦАП АЦП */

extern unsigned int  NumDigit,	    /* число тиков таймера */
		     DivCount,	    /* делитель частоты */
		     BufTim;        /* FTAKT / нов. частота  */

int  far EnablePCL(unsigned int Count1,unsigned int Count2);
int  far EnablePCLw(void);
int  far DisablePCL(void);
void far SetFuncPCL(void far *Fun);
unsigned int  far UsePCL(unsigned int put);


