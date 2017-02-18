#include <tcxlwin.h>
#include <tcxlinp.h>
#include <tcxlcod.h>

#include "stend.h"
#include "color.h"

/* rtrm.c */
int trm_dt(char *inbuf,char *outbuf);
static char o_buf[129];

#define  SET_COM1 "\x40\x1\x2"
#define  SET_COM2 "\x50\x1\x2"
#define  INIT     "\x1"
#define  RECIVE   "\x2"
#define  SEND     "\x3"

int CTYP ConnectBeg(NOARG);
int CTYP WaitStart(NOARG);
int CTYP ConnectEnd(NOARG);



int CTYP ConnectBeg(NOARG)
{
	 int i,val;
/* ------------[ Инициализировать порт ]------------*/
 trm_dt(SET_COM1,o_buf);
 if(trm_dt(INIT,o_buf))
   {
	trm_dt(SET_COM2,o_buf);
	if(trm_dt(INIT,o_buf)) return(0);
   }
/* ------------[  Прочитать файлы с данными о датчиках,паспорт,нагрузки ]------------*/
/* DATCHIK.DBF */
 for(i=0;i<3;i++)
   {
   val=trm_dt(RECIVE,o_buf);
   if(val!=1 || o_buf[0]!=5) return(0);
   }

/* ------------[ Прочитать задание ]-----------*/
 val=trm_dt(RECIVE,o_buf);
 if(val!=1 || o_buf[0]!=4) return(0);

/* ------------[ Прочитать паспорт ]-----------*/
 val=trm_dt(RECIVE,o_buf);
 if(val!=1 || o_buf[0]!=4) return(0);

/* ------------[ Прочитать программу ]-----------*/
 val=trm_dt(RECIVE,o_buf);
 if(val!=1 || o_buf[0]!=4) return(0);

 return(1);
}

int CTYP WaitStart(NOARG)
{
 if(trm_dt(SEND,o_buf)) return(0); /* сигнал готовности начать управление */
 while((trm_dt(RECIVE,o_buf)!=1) && o_buf[0]!=6);  /* ожидание разрешения*/
 return(1);
}

int CTYP ConnectEnd(NOARG)
{
 return(!trm_dt(SEND,o_buf));
}



VOID CTYP UseMainIBM_PC(NOARG)
{

 while(KeyPeek()!=Key_Esc)
   {
   WprtCen(23,C_SEL," Ожидание данных с ЦЭВМ ... ");
   if(KeyTime(50)==Key_Esc) break;
   while(!ConnectBeg())
	 {
     if(KeyPeek()==Key_Esc) return;
	 }

   if(KeyPeek()==Key_Esc) break;

   WfilBox(23,2,23,78,' ',C_TXT);
   Test_Full();

   WprtCen(23,C_SEL," Ожидание сигнала запуска ... ");
   if(KeyTime(50)==Key_Esc) break;
   WaitStart();

   WfilBox(23,2,23,78,' ',C_TXT);
   Go_Model();

   WprtCen(23,C_SEL," Сигнал о конце управления ");
   ConnectEnd();
   }
 return;
}


