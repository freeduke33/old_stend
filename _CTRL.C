#include "immak.h"

#include "tcxlmnu.h"
#include "tcxlent.h"
#include "tcxlcod.h"
#include "tcxlinp.h"

#include "stend.h"
#include "data_glb.h"
#include "color.h"
#include "string.h"

LCL VOID CTYP _ModFun(NOARG);
LCL VOID CTYP ModTypeFun(NOARG);

LCL char TrnStr[2][10]={"Пpямая","Обpатная"};
LCL char ModStr[2][10]={"полная","неполная"};
LCL char _ModType[35],_ModNum[5],_ModReg[5],
         _ModShift[5],_ModTrnd[5];


LCL VOID CTYP Num2Str(NOARG)
{
 strcpy(_ModType,TrnStr[IsTrend]);
 strcat(_ModType," ");
 strcat(_ModType,ModStr[ModType]);
 cvtic(_ModReg, q,2);
 cvtic(_ModNum, p,2);
 cvtic(_ModShift,shift,2);
 cvtic(_ModTrnd,ep,2);
 return;
}

LCL VOID CTYP Str2Num(NOARG)
{
 q=cvtci(_ModReg);
 p=cvtci(_ModNum);
 ep=cvtci(_ModTrnd);
 shift=cvtci(_ModShift);
 return;
}

LCL IbldT  TrnItm[2]=
{{TrnStr[0],'',ITM_AFT, 0,0, 24,0,0,0, NULL, _ModFun},
 {TrnStr[1],'',ITM_AFT, 1,0, 25,0,0,0, NULL, _ModFun},
};

LCL MbldT  TrnMnu=
{&TrnItm[0],2, 4,53,7,63,  MNU_SAV|MNU_PUL, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_WNSEL,C_WASEL,
   24, OpenW,  0, 10,  1, 0xFF, 0xFF, 0xFF
};

LCL  IbldT ModItm[2]=
{{ModStr[0],'',ITM_AFT, 0,0, 7,0,0,0,NULL,NULL,NULL,NULL,&TrnMnu},
 {ModStr[1],'',ITM_AFT, 1,0, 8,0,0,0,NULL,NULL,NULL,NULL,&TrnMnu}
};

LCL  MbldT ModMnu=
{&ModItm[0],2, 2,61,5,75,  MNU_SAV|MNU_PUL, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_WNSEL,C_WASEL,
   7, OpenW,  0, 10,  2, 0xFF, 0xFF, 0xFF
};

LCL  FbldT ModFld[6]=
{{"Вид модели",           2,49, _ModType,  2,61, "**************",
  ModTypeFun,NULL,NULL, 0,9,0, FLD_SUB,0,FLD_CHG},
 {"Порядок модели",       4,49, _ModNum,   4,74, "99",
  NULL,	     NULL,NULL, 0,10,0, FLD_DEC,0,FLD_CHG},
 {"Порядок тренда",       5,49, _ModTrnd, 5,74, "99",
  NULL,      NULL,NULL, 0,11,0, FLD_DEC,0,FLD_CHG},
 {"Глубина регуляризации",6,49, _ModReg,   6,74, "99",
  NULL,      NULL,NULL, 0,11,0, FLD_DEC,0,FLD_CHG},
 {"Глубина прогноза",     7,49, _ModShift, 7,74, "99",
  NULL,      NULL,NULL, 0,11,0, FLD_DEC,0,FLD_CHG}
};

LCL EbldT ModFrm=
{&ModFld[0],5,
   NULL, NULL,                       /* getkey(), key-buffer       */
   0, ENT_SAV,                       /* user, options              */
   C_TXT,C_SEL,C_ASEL,C_NSEL
};

LCL VOID CTYP _ModFun(NOARG)
{
 MdfP m;

 m=MnuCur();
 IsTrend=IdfTag(MdfCur(m))-24;
 return;
}

LCL VOID CTYP ModTypeFun(NOARG)
{
 int s;
 FdfP f;

 if(SubFunBeg()) return;
 if(MnuBuild(&ModMnu)==ERR_MEM) MemAllocErr(0);
 else
   {
   s=MnuGet();
   f=FldCur();
   if(s)
     {
     ModType=s-7;
     strcpy(FdfBfr(f),TrnStr[IsTrend]);
     strcat(FdfBfr(f)," ");
     strcat(FdfBfr(f),ModStr[ModType]);
     FldChg();
     }
   }
 SubFunEnd();
 return;
}

//---------------------------------------------------------------------------

VOID CTYP ModFun(NOARG)
{
 int val;
 if(EntBuild(&ModFrm)==ERR_MEM) MemAllocErr(0);
 else
   {
   val=EntGet(ModFld[1].ftag);
   if(val!=ERR_ARG)
     {
     _USE_=0;
     Str2Num();
     }
   EntFree();
   }
 return;
}




VOID CTYP ModView(NOARG)
{
 Num2Str();
 if(EntBuild(&ModFrm)==ERR_MEM) MemAllocErr(1);
 EntDsp();
 EntFree();
 return;
}

char *CTYP TrnName(NOARG)
{
 Str2Num();
 return TrnStr[IsTrend];
}

char *CTYP ModName(NOARG)
{
 Str2Num();
 return ModStr[ModType];
}

