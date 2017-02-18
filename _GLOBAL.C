#include "immak.h"
#include "tcxlmnu.h"
#include "tcxlent.h"
#include "tcxlcod.h"
#include "tcxlinp.h"

#include "stend.h"
#include "data_glb.h"
#include "color.h"
#include "string.h"
#include "pcl-71#.h"
#include "stdlib.h"


LCL VOID CTYP GlbTypeFun(NOARG);

LCL char _GlbGMax[10]=" 1.000",_GlbGMin[10]=" 0.500";
LCL char GlbStr[5][20]={"ПЕРЕМЕЩЕНИЕ",
                    "УДАР-ПЕРЕМЕЩ",
		    "УСКОРЕНИЕ",
		    "УСИЛИЕ",
		    "УСИЛИЕ+ЛЮФТ"};

LCL char _GlbType[20],_GlbDelt[20],_GlbLen[20],_GlbCnt[20],
     _GlbMax[10],_GlbMin[10];




LCL VOID CTYP Num2Str(NOARG)
{
 strcpy(_GlbType,GlbStr[GlbType]);
 cvtfc(_GlbDelt,fdi,7,2);
 cvtfc(_GlbMax,y_w/(pcl_RANGE/10.0)-5,7,4);
 cvtfc(_GlbMin,y_n/(pcl_RANGE/10.0)-5,7,4);
 cvtfc(_GlbLen, nr,5,0);
 cvtfc(_GlbCnt, ncikl,5,0);
 return;
}

LCL VOID CTYP Str2Num(NOARG)
{
 fdi=cvtcf(_GlbDelt);
 y_w=(cvtcf(_GlbMax)+5)*(pcl_RANGE/10.0);
 y_n=(cvtcf(_GlbMin)+5)*(pcl_RANGE/10.0);
 nr=(unsigned int)min(cvtcf(_GlbLen),65530L-Q_P);
 ncikl=(unsigned int)min(cvtcf(_GlbCnt),65530L);
 return;
}



LCL FbldT GlbFldT[2]=
{{"Верхний предел",0,0, _GlbGMax,  0,25,"99.999",
  NULL,	     NULL,NULL,  0, 80,0,  FLD_TRM|FLD_DEC|FLD_RJS,0,FLD_CHG},
 {"Нижний  предел",1,0, _GlbGMin,  1,25,"99.999",
  NULL,	     NULL,NULL,  0, 81,0,  FLD_TRM|FLD_DEC|FLD_RJS,0,FLD_CHG}
};
LCL   EbldT   CDC   GlbFrmT =              /*- form-build object --------*/
{  &GlbFldT[0], 2,                         /* field-build list, count    */
   NULL, NULL,                            /* getkey(), key-buffer       */
   0, ENT_SAV,                            /* user, options              */
   C_WTXT,C_WSEL,C_WASEL,C_WNSEL
};

LCL VOID CTYP _GlbFun(NOARG)
{
 int val;
 Wopen(3,30,6,70,BOX_SNG,C_WBOX,C_WTXT);
 if(!MemAllocErr(0))
   {
   Wshadow(C_SHADOW);
   val=EntPush(); if(val) Wperror("- 1 -");
   if(EntBuild(&GlbFrmT)==ERR_MEM) MemAllocErr(0);
   else
	 {
     val=EntGet(GlbFldT[0].ftag);
     if(val!=ERR_ARG) _USE_=0;
     EntFree();
	 }
   EntPop();
   Wclose();
   }
 return;
}

LCL IbldT GlbItm[4]=
{{GlbStr[0],'',ITM_AFT, 0,0,26,0,0,0,NULL,_GlbFun},
 {GlbStr[1],'',ITM_AFT, 1,0,27,0,0,0,NULL,_GlbFun},
 {GlbStr[2],'',ITM_AFT, 2,0,28,0,0,0,NULL,_GlbFun},
 {GlbStr[3],'',ITM_AFT, 3,0,29,0,0,0,NULL,_GlbFun},
};

LCL MbldT GlbMnu=
{&GlbItm[0],4, 1,25,6,45,  MNU_SAV|MNU_PUL, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_WNSEL,C_WASEL,
   0, OpenW,  0, 0,  3, 0xFF, 0xFF, 0xFF
};


LCL FbldT GlbFld[6]=
{{"Вид физической величины", 2,3, _GlbType,  2,30, "************",
  GlbTypeFun,NULL,NULL,  0, 3,0,  FLD_NUL,0,FLD_CHG},
 {"Частота дискретизации",   4,3, _GlbDelt,  4,32, "9999.99",
  NULL,	     NULL,NULL,  0, 4,0,  FLD_DEC,0,FLD_CHG},
 {"Длина исходного ряда",   5,3, _GlbLen,   5,34,  "99999",
  NULL,	     NULL,NULL,  0, 5,0,  FLD_DEC,0,FLD_CHG},
 {"Число циклов повторения", 6,3, _GlbCnt,   6,34,  "99999",
  NULL,	     NULL,NULL,  0, 6,0,  FLD_DEC,0,FLD_CHG},
 {"Диапазон управления",     7,3, _GlbMin,   7,23, "99.9999",
  NULL,	     NULL,NULL,  0, 7,0,  FLD_DEC,0,FLD_CHG},
 {"", 7,1,                        _GlbMax,   7,33, "99.9999",
  NULL,	     NULL,NULL,  0, 8,0,  FLD_DEC,0,FLD_CHG}
};

LCL EbldT   CDC   GlbFrm =              /*- form-build object --------*/
{  &GlbFld[0], 6,                         /* field-build list, count    */
   NULL, NULL,                            /* getkey(), key-buffer       */
   0, ENT_SAV,                            /* user, options              */
   C_TXT,C_SEL,C_ASEL,C_SEL
};


LCL VOID CTYP GlbTypeFun(NOARG)
{
 int s;
 FdfP f;


 if(SubFunBeg()) return;
 if(MnuBuild(&GlbMnu)==ERR_MEM) MemAllocErr(0);
 else
   {
   s=MnuGet();
   f=FldCur();
   if(s)
     {
     GlbType=s-26;
     strcpy(FdfBfr(f), GlbItm[GlbType].istr);
     FldChg();
     }
   }
 SubFunEnd();
 return;
}

//--------------------------------------------------------------------------

VOID CTYP GlbFun(NOARG)
{
 int val;

 Num2Str();
 if(EntBuild(&GlbFrm)==ERR_MEM) MemAllocErr(0);
 else
   {
   val=EntGet(GlbFld[0].ftag);
   if(val!=ERR_ARG)
     {
     _USE_=0;
     Str2Num();
     }
   EntFree();
   }
 return;
}

VOID CTYP GlbView(NOARG)
{
 Num2Str();
 if(EntBuild(&GlbFrm)==ERR_MEM) MemAllocErr(1);
 EntDsp();
 EntFree();
 return;
}

char *CTYP GlbName(NOARG)
{
 Num2Str();
 return _GlbType;
}

