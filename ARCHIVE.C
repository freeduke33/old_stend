#include "immak.h"
#include "look_grf.h"
#include "stdio.h"
#include "tcxlsel.h"
#include "tcxlcur.h"

#include "stend.h"
#include "data_glb.h"
#include "color.h"
#include "din_zw.h"

LCL VOID CTYP Open(NOARG)
{
 Wshadow(C_SHADOW);
 Wtitle("Выберите файл отчета",TTL_CNT,C_WTITLE);
 WsetEsc(TRUE);
 return;
}

VOID CTYP Archive(NOARG)
{
 unsigned int _nr,i,_ncikl;
 char buf[100],*str;
 FILE *f;
 double _delt;
 float  fdi;

 _nr=nr;
 _delt=delt;
 _ncikl=ncikl;

 SelBarOff();
 SelDirOn();
 SelParOn();
 str=SelFile(4,15,13,60,BOX_SNG,C_WBOX,C_WSEL,C_WASEL,0,"*.ST",_VFVCP(Open));
 if(!MemAllocErr(0))
 {
 if(PassWin(" Ждите "," Идет чтение протокола работы ..."))
   {
   if(str)
     {
     f=fopen(str,"r+t");
     if(f!=NULL)
       {
       for(i=0;i<4;i++) fgets(buf,100,f);
       fscanf(f,"%s %s %u",buf,buf,&nr);
	   fscanf(f,"%s %s %s %u",buf,buf,buf,&ncikl);
       fscanf(f,"%s %s %f",buf,buf,&fdi);
#ifdef VIRTUAL
	   if(CreateBuf(&x ,nr)) return;
	   if(CreateBuf(&y ,nr)) return;
	   if(CreateBuf(&yi,nr)) return;
#endif
       delt=1/fdi;
       for(i=0;i<10;i++) fgets(buf,100,f);
       for(i=0;i<nr;i++)
	     {
	     fscanf(f,"%f %f %f",&x[i],&yi[i],&y[i]);
	     x[i]/=pcl_RANGE;
	     y[i]/=pcl_RANGE;
	     yi[i]/=pcl_RANGE;
	     }
       fclose(f);
	   if(ncikl==1) nr-=Q_P;
       _USE_=1;
       Loop_Graph();
       CurHide();
       _USE_=0;
       }
     else Error("Не могу открыть файл протокола работы стенда");
     }
   PassEnd();
   }
 SelFree();
 }
 nr=_nr;
 delt=_delt;
 ncikl=_ncikl;
 return;
}

