#include "immak.h"

#include "tcxlmnu.h"
#include "tcxlent.h"
#include "tcxlwin.h"

#include "stend.h"
#include "data_glb.h"
#include "color.h"
#include "string.h"      /*
┌──────── Имитационная модель ─────────────┐
│ Частота дискретизации    ХХХХХХ.ХХХХ  Гц │
├1────функциональное───────┬──вид помехи───┤
│ Вид нелин. КВАДРАТИЧЕСКАЯ│               │
│ К. передачи    ХХХ.ХХХХ  │ ПОЛИГАРМОНИКА │
├2────апериодическое───────┤               │
│ Порядок │ Fв  XXXXX.XXXX │ Ампл. XXXX.XX │
│  ХХХХ   │ Fн  XXXXX.XXXX │ Част. XXXX.XX │
├3────колебательное────────┤ Фаза   XXX.XX │
│ Степень усп.  ХХ.ХХХХХХХ ├────статика────┤
│ Рез. частота  ХХХХХ.ХХХХ │ U= XXXXX.XXXX │
└──────────────────────────┴───────────────┘

*/
LCL VOID CTYP ActPIC(NOARG);
LCL VOID CTYP DeActPIC(NOARG);
LCL VOID CTYP ZvenoView(int num);


LCL char names[][20]={"отсутствует",
	              "функциональное",
		      "апериодическое",
		      "колебательное",
		      "СИНУСОИДА",
		      "ПОЛИГАРМОНИКА",
		      "ПОЛИГАРМ. 3х5",
		      "ПОЛИГАРМ. 3х8"};
LCL char FuncNames[][20]={"ЛИНЕЙНОЕ",
                          "КВАДРАТИЧЕСКОЕ",
			  "КУБИЧЕСКОЕ",
			  "ГИСТЕРЕЗИС"};

LCL char _Zveno[6][20];
extern char OptStr3[];


LCL char FldStr1[20],FldStr2[20],FldStr3[20];
LCL int FldType=0,Z_curr;

/*====================[ Func Mode ]=====================*/

LCL IbldT FuncItm[4]=
{{FuncNames[0],'',ITM_AFT, 0,0, 44},
 {FuncNames[1],'',ITM_AFT, 1,0, 45},
 {FuncNames[2],'',ITM_AFT, 2,0, 46},
 {FuncNames[3],'',ITM_AFT, 3,0, 47}
};

LCL  MbldT FuncMnu=
{&FuncItm[0],4, 7,20,12,36,  MNU_SAV|MNU_PUL, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_NSEL,C_WASEL,
   44, OpenW,  0, 10,  1, 0xFF, 0xFF, 0xFF
};

LCL VOID CTYP FuncRun(NOARG)
{
 int s;
 FdfP f;

 f=FldCur();
 if(SubFunBeg()) return;
 MbldTag(&FuncMnu)=FldType+44;
 if(MnuBuild(&FuncMnu)==ERR_MEM) MemAllocErr(0);
 else
   {
   s=MnuGet();
   if(s)
     {
     FldType=s-44;
     strcpy(FdfBfr(f), FuncNames[FldType]);
     FldChg();
     }
   }

 SubFunEnd();
 return;
}
/*====================[ Func ]=====================*/
LCL FbldT FuncFld[2]=
{{"Вид нелинейности"    ,0,0,  FldStr1, 0,18,"***************",
  FuncRun,NULL,NULL, 0,170,0, FLD_NUL,0,FLD_CHG},
 {"Коэффициент передачи",1,0,  FldStr2, 1,23,"999.9999",
  NULL,NULL,NULL, 0,171,0, FLD_DEC,0,FLD_CHG}
};

EbldT FuncFrm=
{FuncFld,2,NULL, NULL,0, ENT_SAV,
 C_WTXT,C_WSEL,C_WASEL,C_WNSEL};


LCL VOID CTYP FuncParm(NOARG)
{
 int val;

 Wopen(8,12,11,48,BOX_DBL,C_WBOX,C_WTXT);
 if(!MemAllocErr(0))
   {
   Wshadow(C_SHADOW);
   FldType=FuncModel[Z_curr].Type;
   strcpy(FldStr1,FuncNames[FldType]);
   cvtfc(FldStr2,FuncModel[Z_curr].k,8,4);
   EntPush();
   if(EntBuild(&FuncFrm)==ERR_MEM) MemAllocErr(0);
	 {
     val=EntGet(FuncFld[0].ftag);
     EntFree();
	 }
   EntPop();
   if(val!=ERR_ARG)
     {
     FuncModel[Z_curr].k=cvtcf(FldStr2);
     FuncModel[Z_curr].Type=FldType;
     }
   Wclose();
   }
 return;
}

/*======================[ Aperiod ]========================*/
LCL FbldT AperFld[3]=
{{"Верхняя граница"    ,0,0,  FldStr1, 0,20,"99999.9999",
  NULL,NULL,NULL, 0,170,0, FLD_DEC,0,FLD_CHG},
 {"Нижняя  граница"    ,1,0,  FldStr2, 1,20,"99999.9999",
  NULL,NULL,NULL, 0,170,0, FLD_DEC,0,FLD_CHG},
 {"Порядок фильтра"    ,2,0,  FldStr3, 2,26,"9999",
  NULL,NULL,NULL, 0,171,0, FLD_DEC,0,FLD_CHG}
};

EbldT AperFrm=
{AperFld,3,NULL, NULL,0, ENT_SAV,
 C_WTXT,C_WSEL,C_WASEL,C_WNSEL};

LCL VOID CTYP AperParm(NOARG)
{
 int val;

 Wopen(8,12,12,48,BOX_DBL,C_WBOX,C_WTXT);
 if(!MemAllocErr(0))
   {
   cvtfc(FldStr1,AperModel[Z_curr].FV,10,4);
   cvtfc(FldStr2,AperModel[Z_curr].FN,10,4);
   cvtic(FldStr3,AperModel[Z_curr].p,4);
   EntPush();
   if(EntBuild(&AperFrm)==ERR_MEM) MemAllocErr(0);
   else
	 {
     val=EntGet(AperFld[0].ftag);
     EntFree();
	 }
   EntPop();
   if(val!=ERR_ARG)
     {
     AperModel[Z_curr].FV=cvtcf(FldStr1);
     AperModel[Z_curr].FN=cvtcf(FldStr2);
     AperModel[Z_curr].p =cvtci(FldStr3);
     }
   Wclose();
   }
 return;
}

/*=====================[ Kolebatel ]=======================*/
LCL FbldT KolbFld[2]=
{{"Степень успокоения"    ,0,0,  FldStr1, 0,20,"99.9999999",
  NULL,NULL,NULL, 0,170,0, FLD_DEC,0,FLD_CHG},
 {"Резонансная частота"   ,1,0,  FldStr2, 1,20,"99999.9999",
  NULL,NULL,NULL, 0,171,0, FLD_DEC,0,FLD_CHG}
};

EbldT KolbFrm=
{KolbFld,2,NULL, NULL,0, ENT_SAV,
 C_WTXT,C_WSEL,C_WASEL,C_WNSEL};

LCL VOID CTYP KolbParm(NOARG)
{
 int val;

 Wopen(8,12,11,48,BOX_DBL,C_WBOX,C_WTXT);
 if(!MemAllocErr(0))
   {
   cvtfc(FldStr1,KolbModel[Z_curr].Betta,10,7);
   cvtfc(FldStr2,KolbModel[Z_curr].Frez ,10,4);
   EntPush();
   if(EntBuild(&KolbFrm)==ERR_MEM) MemAllocErr(0);
   else
	 {
     val=EntGet(KolbFld[0].ftag);
     EntFree();
	 }
   EntPop();
   if(val!=ERR_ARG)
     {
     KolbModel[Z_curr].Betta=cvtcf(FldStr1);
     KolbModel[Z_curr].Frez =cvtcf(FldStr2);
     }
   Wclose();
   }
 return;
}
/*=========================================================================*/

LCL IbldT UnkItm[5]=
{{names[0],'',ITM_AFT, 0,0, 44},
 {names[4],'',ITM_AFT, 1,0, 48},
 {names[5],'',ITM_AFT, 2,0, 49},
 {names[6],'',ITM_AFT, 3,0, 50},
 {names[7],'',ITM_AFT, 4,0, 51},
};

LCL  MbldT UnkMnu=
{&UnkItm[0],5, 10,15,16,31,  MNU_SAV|MNU_PUL, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_NSEL,C_WASEL,
   44, OpenW,  0, 10,  1, 0xFF, 0xFF, 0xFF
};

LCL IbldT DinItm[4]=
{{names[0],'',ITM_AFT, 0,0, 44},
 {names[1],'',ITM_AFT, 1,0, 45,0,0,0,NULL,FuncParm},
 {names[2],'',ITM_AFT, 2,0, 46,0,0,0,NULL,AperParm},
 {names[3],'',ITM_AFT, 3,0, 47,0,0,0,NULL,KolbParm},
};

LCL  MbldT DinMnu=
{&DinItm[0],4, 10,15,15,32,  MNU_SAV|MNU_PUL, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_NSEL,C_WASEL,
   44, OpenW,  0, 10,  1, 0xFF, 0xFF, 0xFF
};

VOID CTYP DinTypeFun(NOARG)
{
 int s;
 FdfP f;


 ActPIC();
 s=SubFunBeg();
 if(!s)
   {
   f=FldCur();
   Z_curr=FdfTag(f)-51;
   if(FdfTag(f)!=54)
     {
     MbldTag(&DinMnu)=44+Z_Type[Z_curr];
     if(MnuBuild(&DinMnu)==ERR_MEM) MemAllocErr(0);
     else s=MnuGet();
     }
   else
     {
     MbldTag(&UnkMnu)=44+Z_Type[Z_curr];
     if(MnuBuild(&UnkMnu)==ERR_MEM) MemAllocErr(0);
     else s=MnuGet();
     }
   if(s)
     {
     s=Z_Type[Z_curr]=s-44;
     ZvenoView(Z_curr);
     strcpy(FdfBfr(f), names[s]);
     FldChg();
     }

   SubFunEnd();
   }
 return;
}
/*==========================================================================*/


/*-----------------11-24-92 04:19pm-----------------*/


LCL char _ZvenoPIC[20]="!Aa!**************";

LCL VOID CTYP ActPIC(NOARG)
{
  _ZvenoPIC[2]=C_MASEL;
  _FldPut(FldCur(),FDF_MOV);
  return;
}


LCL VOID CTYP DeActPIC(NOARG)
{
  _ZvenoPIC[2]=C_MSEL;
  _FldPut(FldCur(),FDF_MOV);
  return;
}

LCL VOID CTYP SetPIC(NOARG)
{
	_ZvenoPIC[2]=C_MSEL;
}

LCL FbldT  DinFld[6]=
{{"Частота дискретизации",10,3, _Zveno[5],   10,28,"99999.999",
  NULL      ,NULL,NULL, 0,50,0, FLD_DEC,0,FLD_CHG},
 {"1",          11,2,  _Zveno[0], 11,7 ,_ZvenoPIC,
  DinTypeFun,	    NULL,DeActPIC, 0,51,0, FLD_SUB,0,FLD_CHG},
 {"2",          14,2,  _Zveno[1], 14,7 ,_ZvenoPIC,
  DinTypeFun,       NULL,DeActPIC, 0,52,0, FLD_SUB,0,FLD_CHG},
 {"3",          17,2,  _Zveno[2], 17,7 ,_ZvenoPIC,
  DinTypeFun,	    NULL,DeActPIC, 0,53,0, FLD_SUB,0,FLD_CHG},
 {"вид помехи", 11,31, _Zveno[3], 13,30,_ZvenoPIC,
  DinTypeFun,	    NULL,DeActPIC, 0,54,0, FLD_SUB,0,FLD_CHG},
 {"статика",    18,33, _Zveno[4], 19,33,"99999.9999",
  NULL,	     NULL,NULL, 0,54,0, FLD_DEC,0,FLD_CHG}
};

EbldT DinFrm=
{DinFld,6,NULL, NULL,0, ENT_SAV,
 C_TXT,C_SEL,C_ASEL,C_NSEL};

VOID CTYP DinFun(NOARG)
{
 int val;

 SetPIC();
 cvtfc(_Zveno[4],imi_Const,10,5);
 cvtfc(_Zveno[5],fdds,9,4);

 if(EntBuild(&DinFrm)==ERR_MEM) MemAllocErr(0);
 else
   {
   val=EntGet(DinFld[1].ftag);
   if(val!=ERR_ARG)
	 {
     _USE_=0;
	 imi_Const=cvtcf(_Zveno[4]);
	 fdds     =cvtcf(_Zveno[5]);
	 }
   EntFree();
   }
 return;
}

LCL VOID CTYP ZvenoView(int num)
{
 strcpy(_Zveno[num],names[Z_Type[num]]);
 if(num<3)
   {
   WfilBox(12+num*3,2,13+num*3,27,' ',C_TXT);
   switch(Z_Type[num])
      {
	  case 0:  /* none */
		break;
	  case 1:  /* функциональное */
		Wprts(12+num*3, 3,C_TXT,"Вид нелин.");
		Wprts(12+num*3,14,C_SEL,FuncNames[FuncModel[num].Type]);
		Wprts(13+num*3, 3,C_TXT,"Кф. перед.");
		Wprtf(13+num*3,18,C_SEL,"%7.4f",FuncModel[num].k);
		break;
	  case 2:  /* апериодическое */
		Wprts(12+num*3, 3,C_TXT,"Порядок");
		Wprtf(13+num*3, 4,C_SEL,"%04d",AperModel[num].p);
		Wprtf(12+num*3,11,C_TXT,"├ Fв");
		Wprtf(12+num*3,17,C_SEL,"%10.4f",AperModel[num].FV);
		Wprtf(13+num*3,11,C_TXT,"├ Fн");
		Wprtf(13+num*3,17,C_SEL,"%10.4f",AperModel[num].FN);
		break;
	  case 3:  /* колебательное */
		Wprtf(12+num*3, 3,C_TXT,"Степень усп.");
		Wprtf(12+num*3,17,C_SEL,"%10.7f",KolbModel[num].Betta);
		Wprtf(13+num*3, 3,C_TXT,"Рез. частота");
		Wprtf(13+num*3,17,C_SEL,"%10.5f",KolbModel[num].Frez);
		break;
	  case 4:  /* sin  */
		break;
	  case 5:  /* psin */
		break;
	  case 6:  /* psin 3x5 */
		break;
	  case 7:  /* psin 3x8 */
		break;
      }
   }
 else
   {
   }
 return;
}

VOID CTYP DinView(NOARG)
{
 int i;

 cvtfc(_Zveno[4],imi_Const,10,5);
 cvtfc(_Zveno[5],fdds,9,4);

 SetPIC();
 WdrwBox( 9, 1,20,44,BOX_DBL,C_BOX); /* имитац. модель */
 WdrwHor(11, 1,44,BOX_VER,C_BOX);
 WdrwVer(12,28, 9,BOX_DBL,C_BOX);
 WdrwVer(11,28, 2,BOX_VER,C_BOX);
 WdrwHor(14, 1,28,BOX_VER,C_BOX);
 WdrwHor(17, 1,28,BOX_VER,C_BOX);
 WdrwHor(18,28,16,BOX_VER,C_BOX);
 Wprts(9,13,C_MSEL,OptStr3);
 Wprts(10,41,C_TXT,"Гц.");
 Wprts(19,30,C_TXT,"U=");
 for(i=0;i<4;i++) ZvenoView(i);
 if(EntBuild(&DinFrm)==ERR_MEM) MemAllocErr(0);
 else
   {
   EntDsp();
   EntFree();
   }
 return;
}

