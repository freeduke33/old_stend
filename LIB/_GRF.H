/**************************************************************************


		       Список функций пакета GRF:
		    ________________________________


 * Инициализировать пакет GRF
 *
 * int InitGRF(int Control)

 * Отключить пакет GRF
 *
 * int DeinitGRF(int Control)

 * Открыть новое окно на экране
 *
 * int OpenGRF(int xlo,int ylo,            / окно на плоскости
	       int xhi,int yhi,            \ размером 1000x1000
	       float Xmin,float Ymin,    минимальные значения графика
	       float Xmax,float Ymax,    максимальные значения графика
	       char *Title,              заголовок окна
	       int Control);             начальные установки

 *  Присоединяет к открытому окну независимый график
 *
 * int AppendGRF(int wind,               главное окно
		 int col,		 цвет графика
		 int Control);		 начальные установки

 * Изменить параметры графика в окне
 *
 * int  RangeGRF(int wind,
		 float Xmin,
		 float Ymin,
		 float Xmax,
		 float Ymax);

 * Умалчиваемые цвета графика
 *
 * int  ColorGRF(int wind,
		 int graph,        линий графика
		 int box,          внутренний бордюр и метки на нем
		 int wbox,         внешний бордюр
		 int axis,         оси
		 int ttl_fore,     заголовок
		 int ttl_back,     фон заголовка
		 int act_fore,     заголовок активного окна
		 int act_back,     фон заголовка активного окна
		 int num,          оцифровка
		 int bkg,          фон графика
		 int wbkg);        фон между бордюрами

 * Изменить заголовок окна
 *
 * int TitleGRF(int wind,
		char *title);

 * Умалчиваемые режимы рисования
 *
 * int GSetGRF(int wind,int Mode,int width,int pattern); для графика
 * int BSetGRF(int wind,int Mode,int width,int pattern); для бордюра графика
 * int WSetGRF(int wind,int Mode,int width,int pattern); для бордюра окна
 * int ASetGRF(int wind,int Mode,int width,int pattern); для осей
 * int MSetGRF(int wind,int Font);                       для сообщений
 * int TSetGRF(int wind,int Font);                       для заголовка
 * int NSetGRF(int wind,int Font);	   	         для оцифровки


 * Способ заполнения области
 *
 *
 * int BfillGRF(int wind,int fill,char *pattern);      между бордюрами
 * int GfillGRF(int wind,int fill,char *pattern);      графика

 * Изменить начальные установки окна
 *
 * int ModeGRF(int wind,int Control);

 *      Закрыть окно и освободить занимаемое место
 *      оставить изображение на дисплее
 *
 * int CloseGRF(int wind);

 * Вывести очередную точку в окно wind
 *
 *  int PointGRF(int wind,
		 float x,
		 float y);

 * Вывести массив длинной num в окно wind
 *
 *  int Fbuf2GRF(int wind,
		 int num,
		 float *bufX,
		 float *bufY);
 *
 *  int Dbuf2GRF(int wind,
		 int num,
		 double *bufX,
		 double *bufY);

 * Перерисовать все открытые окна
 *
 * void RedrawGRF();

 * Разрешить вывод в окно wind
 *
 * int ShowGRF_on(int wind);

 * Запретить вывод в окно wind
 *
 * int ShowGRF_off(int wind);

 * Выводить следующую точку графика в окне wind
 * без соединения ее линией с предыдушей
 *
 * int BreakGRF(int wind);

 * Перерисовать заголовок окна активным цветом
 *
 * int ActiveGRF(int wind);

 * Перерисовать заголовок окна пассивным цветом
 *
 * int NactiveGRF(int wind);

 *
 *  Вывести текст в окно wind
 *
 * int WTextGRF(int wind,
		char *str,            строка текста
		float x,float y,      координаты в окне
		int col,              цвет текста
		int bkg,              цвет фона
		int dx,int dy,        высота/ширина символа
		int axis              направление
		int mode);	      режим вывода
 *
 * Вывести текст на экран
 *
 * void TextGRF(char *str,
		int x,int y,          координаты на экране 1000x1000
		int col,              цвет текста
		int bkg,              цвет фона
		int dx,int dy,        высота/ширина символа
		int axis,             направление
		int pattern,          заполнитель фона
		int mode);	      режим вывода

 *  Вывести текст на бордюр
 *
 * int BTextGRF(int wind,
		char *str,
		int ofs,	      смещение на бордюре
		int col,              цвет текста
		int bkg,              цвет фона
		int dx,int dy,        высота/ширина символа
		int axis              направление
		int mode);	      режим вывода

 */

#ifndef ___GRF___
#define ___GRF___

#include <graphics.h>



/* Состояния системы для InitGRF и CloseGRF */

#define grfGRAPHICS    0x8  /* инициализировать графический режим */
#define grfCOLOR       0x4  /* всегда цветной режим */
#define grfTEXT        0x1  /* восстановить текстовый режим */
#define grfNULL        0x0  /* не изменять видеорежима */


/* Состояния окна  для openGRF */

#define grfNoBACK   0x800     /*  не закрашивать область графика */
#define grfNoAXIS   0x400     /*  не рисовать оси графика */
#define grfNoWBOX   0x200     /*  не рисовать внешнего бордюра */
#define grfNoGBOX   0x100     /*  не рисовать внутреннего бордюра */
#define grfHIDDEN   0x80      /*  открыть без вывода на экран */
#define grfALABEL   0x40      /*  маркировать оси */
#define grfBLABEL   0x20      /*  маркировать бордюр */
#define grfRANGE    0x10      /*  выводить пределы графика */

#define grfMULTI    0x08      /* @ будут выводится несколько графиков */
#define grfACTIVE   0x04      /* @ сделать окно активным */
#define grfBREAK    0x02      /* @ прервать график */
#define grfCHANGE   0x01      /* @ пересчитать параметры окна */

/* Параметр wind */
#define grfDEFAULT  0         /*   установить параметры по умолчанию */

/* направление вывода текста */
#define grfAxisX  HORIZ_DIR
#define grfAxisY  VERT_DIR

/* режим вывода текста */
#define grfCENTER     0x01
#define grfBEGIN      0x02
#define grfEND        0x04
#define grfREAL       0x08
#define grfNOBKG      0x10
#define grfREVERSE    0x20

/* доп. режимы для вывода текста на бордюр окна */
#define grfTOP	      0x40
#define grfBOTTOM     0x00


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                 коды ошибок GRF                          */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define okGRF           0
#define errWindGRF     -1
#define errPtrGRF      -2
#define errNomemGRF    -3
#define errAdapterGRF  -4
#define errHiddenGRF   -5
#define errPositionGRF -6
#define errNoneGRF     -7


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                 Структуры данных                         */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


typedef struct _internal_GRF *windowGRF;
typedef struct _append_GRF *appendGRF;
typedef struct buf_0_GRF bufferGRF;
typedef struct buf_2_GRF *buffer2GRF;

extern unsigned int disp_width,disp_height;

struct _grf_col
{
  unsigned char graph,
		box,
		wbox,
		axis,
		title_back,
		title_fore,
		act_back,
		act_fore,
		msg,
		bkg,
		wbkg;
};

struct __grf_style
{
 int style,pattern,width;
};

struct __grf_fill
{
 int fill_mode;
 char pattern[8];
};

struct _grf_set
{
 struct __grf_style graph,box,wbox,axis,mark;
 struct __grf_fill grf,wind;
 int    msg_font,title_font,num_font;
};

struct _internal_GRF
{
  int H;	       /* описатель окна */
  int xlo,ylo,xhi,yhi; /* логическое окно */
  int x1,y1,x2,y2;     /* реальное окно вывода графика */
  int x1w,y1w,x2w,y2w; /* внешнее окно графика */
  float x,y;           /* последняя выведенная точка */
  float Xmax,Ymax,
	Xmin,Ymin;     /* границы вывода */
  float kx,ky;	       /* коэффициенты масштабирования */
  windowGRF next;      /* указатель на пред. окно */
  int state;	       /* состояние окна */
  int append;	       /* число аппендиксов */
  struct _grf_col col; /* цвета окна */
  struct _grf_set set; /* установки рисования */
  char Title[30];      /* заголовок графика */
};

struct _append_GRF
{
 int H;          	/* описатель окна */
 int state;		/* состояние аппендикса */
 int col;		/* цвет графика */
 float x,y;		/* последняя выведенная точка */
 appendGRF next;        /* указатель на след. аппендикс */
 windowGRF top;         /* указатель на головное окно */
};

struct buf_0_GRF
{
 float x,y;
};

struct buf_2_GRF
{
  int x,y,x0,y0;
  char Break;
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                 Глобальные переменные                    */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



#define Clip 1

extern int far BackgroundGRF;
extern int far MarkColorGRF;
extern char StrLeftRangeGRF[20];
extern char StrRightRangeGRF[20];

extern int far aPage,                   /* активная страница */
	   far vPage;                   /* визуальная страница */
extern unsigned far Vseg,                  /* видео сегмент */
		far Voff,                /* смещение 0 страницы */
		far Voff1;               /* смешение 1 страници */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                 Описание процедур                        */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define RoundGRF(x) (int)(((x)<0) ? ((x)-0.5) : ((x)+0.5))
#define MaxGRF(a,b) ((a)>(b)  ? (a):(b))
#define MinGRF(a,b) ((a)<(b)  ? (a):(b))


int  far InitGRF(char);
int  far PointGRF(int,float,float);
int  far Fbuf2GRF(int,int,float far *,float far *);
int  far Dbuf2GRF(int,int,double far *,double far *);
int  far OpenGRF(int,int,int,int,float,float,float,float,char *,int);
int  far AppendGRF(int,int,int);
int  far ColorGRF(int,int,int,int,int,int,int,int,int,int,int,int);
int  far GSetGRF(int,int,int,int);
int  far BSetGRF(int,int,int,int);
int  far WSetGRF(int,int,int,int);
int  far ASetGRF(int,int,int,int);
int  far MSetGRF(int,int);
int  far TSetGRF(int,int);
int  far NSetGRF(int,int);
int  far BfillGRF(int,int,char far *);
int  far GfillGRF(int,int,char far *);
int  far ModeGRF(int,int);
int  far CloseGRF(int);
int  far RedrawGRF(void);
int  far ShowGRF_on(int);
int  far ShowGRF_off(int);
int  far BreakGRF(int);
int  far RangeGRF(int,float,float,float,float);
int  far BTextGRF(int,char far *,int,int,int,int,int,int,int);
int  far WTextGRF(int,char far *,float,float,int,int,int,int,int,int);
int  far TextGRF(char far *,int,int,int,int,int,int,int,int,int);
int  far TitleGRF(int,char *);
int  far DeinitGRF(int);
int  far ActiveGRF(int);
int  far NactiveGRF(int);

/*-----[internal functions]----*/

int   far  getNumWindGRF(windowGRF w);
void  far  pointxGRF(windowGRF w,float x2,float y2);
void  far  writeGRF(int x1,int y1,int x2,int y2,windowGRF w);
void  near delend(float *x1,float *y1,float *x2,float *y2);
void  far  verifyCol(int *fore,int *back);
int   far  todisp(int x,int type);
void  near redrawxGRF(windowGRF w);
void  far  *allocGRF(unsigned int size);
void  far  lineGRF(int x1,int y1,int x2,int y2,int col);
void  far  to_GRF(void);
void  far  GRF_to(void);
int   far  freeGRF(void *);
windowGRF far getWindowGRF(int n);
void  far _putTitleGRF(windowGRF w);
void  far SetWindGRF(windowGRF w);

#endif

