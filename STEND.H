#define COPYRIGHT

extern unsigned int psw[];
typedef unsigned int Word;

#ifndef KeyT
#define KeyT unsigned int
#endif

#ifndef CTYP
#define CTYP pascal
#endif

#ifndef NOARG
#define NOARG void
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef LCL
#define LCL static
#endif

int CTYP ReadCfg(NOARG);
int CTYP WriteCfg(NOARG);
VOID CTYP Archive(NOARG);
VOID CTYP Control(NOARG);
VOID CTYP Config(NOARG);

VOID CTYP CreateFname(char *name,char *ext);
VOID CTYP OpenW(NOARG);
VOID CTYP RestoreSCR(void *buf);

VOID CTYP GlbFun(NOARG);
VOID CTYP GlbView(NOARG);
char *CTYP GlbName(NOARG);

VOID CTYP ModFun(NOARG);
VOID CTYP ModView(NOARG);
char *CTYP ModName(NOARG);
char *CTYP TrnName(NOARG);

VOID CTYP IdealFun(NOARG);
VOID CTYP IdealView(NOARG);

VOID CTYP ObjFun(NOARG);
VOID CTYP ObjView(NOARG);

VOID CTYP DinFun(NOARG);
VOID CTYP DinView(NOARG);
//
VOID CTYP Go_Model(NOARG);
VOID CTYP Test_Full(NOARG);
VOID CTYP Test_Fun(int MODE);
VOID CTYP Reload(NOARG);
VOID CTYP Write(NOARG);
int  CTYP Read(NOARG);
//
VOID CTYP Loop_Graph(NOARG);
VOID CTYP Test_Graph(NOARG);

VOID CTYP toDOS(NOARG);
int  CTYP Ini_Time(NOARG);
//
int  CTYP PassWin(char *Title,char *Msg);
VOID CTYP PassEnd(NOARG);
VOID CTYP PassWrite(Word num);
VOID CTYP PassText(char *str);
//
int CTYP MemAllocErr(int mode);

int  CTYP SubFunBeg(NOARG);
VOID CTYP SubFunEnd(NOARG);

int CTYP Esc(char *str);
int CTYP Error(char *str);
int CTYP GenIdeal(int code);
VOID CTYP UseMainIBM_PC(NOARG);

int  *SaveSCR(int x,int y,int x1,int y1);
int Init_Loop(int mode);
void LoopExit(char *str);
void MyFPE(int sig,int code);
void MsgFPE(int sig);
void Math_Model(int m);
void GlobalFPE(int sig,int code);
int pascal INT_loop(NOARG);
void FinishMelody(NOARG);

float dsys(float);
float din_zw(float);

int MathBeg_End(int *beg,int *end);
int CreateBuf(float **buf,unsigned long size);

// --------------------------[ общие описания ]----------------------------
//

extern Word far *keyboard;

#define  STAT_TST  0x1
#define  DIN_TST   0x2


