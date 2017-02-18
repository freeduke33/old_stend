//
//  Описание обьекта:     ObjFun(),ObjView()
//
#include "immak.h"

#include "tcxlmnu.h"
#include "tcxlent.h"
#include "tcxlcod.h"
#include "tcxlinp.h"

#include "stend.h"
#include "data_glb.h"
#include "color.h"
#include "string.h"

LCL VOID CTYP ObjCtrlFun(NOARG);
LCL VOID CTYP ObjModeFun(NOARG);
LCL VOID CTYP Str2Num(NOARG);
LCL VOID CTYP Num2Str(NOARG);

static char OBJStr[2][25]={"Модель","Стенд"};
static char CtrlStr[2][20]={"ОС замкнута","ОС разомкнута"};

static char _ObjMin[10],_ObjMax[10],
            _OBJStr[20],_CtrlStr[20];


static int _OBJType,_CtrlType;

// ************************************************************************

LCL VOID CTYP Num2Str(NOARG)
{
 _OBJType =OBJType;
 _CtrlType=CtrlType;
 strcpy(_OBJStr, OBJStr [OBJType ]);
 strcpy(_CtrlStr,CtrlStr[CtrlType]);
 cvtfc(_ObjMin, temin,6,3);
 cvtfc(_ObjMax, temax,6,3);
 return;
}

LCL VOID CTYP Str2Num(NOARG)
{
 OBJType =_OBJType;
 CtrlType=_CtrlType;
 temin=cvtcf(_ObjMin);
 temax=cvtcf(_ObjMax);
 return;
}

// ==========================================================================


LCL IbldT ModeItm[2]=
{{OBJStr[0],'',0, 0,0, 308},
 {OBJStr[1],'',0, 1,0, 309},
};

LCL  MbldT ModeMnu=
{&ModeItm[0],2, 13,64,16,79,  MNU_SAV|MNU_PUL, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_WNSEL,C_WASEL,
 308, OpenW,  0, 10,  1, 0xFF, 0xFF, 0xFF
};


LCL VOID CTYP ObjModeFun(NOARG)
{
 int s;
 FdfP f;


 if(SubFunBeg()) return;
 if(MnuBuild(&ModeMnu)==ERR_MEM);
 else
   {
   s=MnuGet();
   f=FldCur();
   if(s)
     {
     _OBJType=s-308;
     strcpy(FdfBfr(f), ModeItm[_OBJType].istr);
     FldChg();
     }
   }
 SubFunEnd();
 return;
}

// ==========================================================================

LCL IbldT CtrlItm[2]=
{{CtrlStr[0],'',0, 0,0, 308},
 {CtrlStr[1],'',0, 1,0, 309},
};

LCL  MbldT CtrlMnu=
{&CtrlItm[0],2, 13,60,16,77,  MNU_SAV|MNU_PUL, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_WNSEL,C_WASEL,
 308, OpenW,  0, 10,  1, 0xFF, 0xFF, 0xFF
};

LCL VOID CTYP ObjCtrlFun(NOARG)
{
 int s;
 FdfP f;


 if(SubFunBeg()) return;
 if(MnuBuild(&CtrlMnu)==ERR_MEM) MemAllocErr(0);
 else
   {
   s=MnuGet();
   f=FldCur();
   if(s)
     {
     _CtrlType=s-308;
     strcpy(FdfBfr(f), CtrlItm[_CtrlType].istr);
     FldChg();
     }
   }
 SubFunEnd();
 return;
}

// ==========================================================================


LCL FbldT ObjFld[4]=
{{"Тип объекта",        16,49, _OBJStr,   16,65,"******",
  ObjModeFun,NULL,NULL, 0,41,0, FLD_NUL,0,FLD_CHG},
 {"Режим упр-я",        17,49, _CtrlStr,  17,63,"**************",
  ObjCtrlFun,NULL,NULL, 0,42,0, FLD_NUL,0,FLD_CHG},
 {"Верхняя граница",    18,49, _ObjMax,    18,68,"99.999",
  NULL,	     NULL,NULL, 0,43,0, FLD_DEC,0,FLD_CHG},
 {"Нижняяя граница",    19,49, _ObjMin,    19,68,"99.999",
  NULL,	     NULL,NULL, 0,42,0, FLD_DEC,0,FLD_CHG}
};

LCL EbldT ObjFrm=
{&ObjFld[0],4,
   NULL, NULL,                            /* getkey(), key-buffer       */
   0, ENT_SAV,                            /* user, options              */
   C_TXT,C_SEL,C_ASEL,C_NSEL
};


VOID CTYP ObjFun(NOARG)
{
 int val;

 Num2Str();
 if(EntBuild(&ObjFrm)==ERR_MEM) MemAllocErr(0);
 else
   {
   val=EntGet(ObjFld[0].ftag);
   if(val!=ERR_ARG)
     {
     _USE_=0;
     Str2Num();
     }
   EntFree();
   }
 return;
}


// ==========================================================================

VOID CTYP ObjView(NOARG)
{
 Num2Str();
 if(EntBuild(&ObjFrm)==ERR_MEM) MemAllocErr(1);
 EntDsp();
 EntFree();
 return;
}

