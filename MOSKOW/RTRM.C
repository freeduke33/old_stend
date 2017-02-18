/*       RTRM.C                                  23.4.92
 *       Data transmit system
 *                                                                   */
#include "dos.h"
#include "stdio.h"
#include "alloc.h"

void cinit(char *);
char cnct(void);
char iobf(unsigned int,char *,char *);
int rf(char *);
int g_set(char *, char *);

int trm_dt(char *i_bf,char *o_bf)
{
 union REGS r;
 int i,j,k;
 static int flgi=0, flgc=0;       /* flag comm install, connect      */
 unsigned char cc, rc;
 unsigned int typ_;
 union
 {
 unsigned char u[4];
 void *fm;
 } q;

 cc = *i_bf;
 cc &= 0x0f;
 if (!(cc))
 {
    cinit(i_bf);                  /*  init com function */
    flgi = 1;
    return(0);
 }
 if (flgi)
 {
   if (cc == 1)
   {
     rc = cnct();                 /*  connect file */
     if (!(rc))
     {
       flgc = 1;
       return(0);
     }
     else
     {
       i = -(int) rc;
       return(i);
     }
   }
   if (flgc)
   {
     if (cc == 2)
     {
       typ_ = 1;                       /* receiving only */
       cc = iobf(typ_,i_bf,o_bf);      /* transmit routine */
       if (cc == 1)
       {
         if (*(o_bf) == 5)
         {
           k = rf(o_bf); /* receiving file */
           *(o_bf) = 5;
           if (!k) k = 1;
           return(k);
         }
         if (*(o_bf) == 4)
         {
           if (!(*(o_bf+1)))
             return(g_set(i_bf,o_bf));
         }
       }
       return((int)cc);
     }
     else
       if (cc == 6)               /* freemain function */
       {
         i = 0;
         for(k=0;k<4;k++) q.u[k] = *(i_bf+k+4);
         free(q.fm);
         return(i);
       }
       else
       {
         rc = 1;                  /* cc = 3 */
         if (cc == 4)  rc = 7;
         if (cc == 5)  rc = 21;
         asm    mov   ah,rc;
         asm    mov   al,0;
         asm    mov   typ_,ax
         cc = iobf(typ_,i_bf,o_bf); /* transmit routine */
         if (cc == 1)
         {
           if (*(o_bf) == 4)
           {
             if (!(*(o_bf+1))) return(g_set(i_bf,o_bf));
           }
           if (*(o_bf) == 5)
           {
              k = rf(o_bf);  /* receiving file */
              *(o_bf) = 5;
           }
         }
         i = (int)cc;
         return(i);
       }
   }
   else
     return(-2);                  /* not connect                     */
 }
 else
   return(-1);                    /* com is not install              */
}

int rf(char *o_bf)
{
 int i,k;
 unsigned int snd=512;
 unsigned int typ_=1;
 unsigned char cc;
 char tb[8];
 FILE *fp;

 tb[1] = 0;
 if ((fp=fopen((o_bf+1),"wb"))==NULL)
 {
    tb[0] = 1;                       /* error open */
    cc = iobf(snd,tb,o_bf);          /* transmit routine */
    return(-4);
 }                                   /* ok */
 i = 124;
 tb[0] = 0;
 cc = iobf(snd,tb,o_bf);              /* transmit routine */
 /*cc = 0;*/
 while (i==124)
 {
   if (cc<1) cc = iobf(typ_,o_bf,o_bf); /* get block file */
   if (cc!=1) break;
   k = (int)*(o_bf);
   k--;
   i = fwrite((o_bf+1),sizeof(char),k,fp);
   if (i!=k)
   {
    tb[0] = 1;                       /* error write */
    cc = iobf(snd,tb,o_bf);          /* transmit routine */
    fclose(fp);
    return(-3);                      /* error write block to file */
   }
  tb[0] = '\0';
  cc = iobf(snd,tb,o_bf);            /* transmit routine */
  if (cc > 2) break;
 }
 close(fp);
 return((int)cc);
}

int g_set(char *i_bf, char *o_bf)
{
 unsigned int sm;
 int i,j,k;
 void *bsr;
 char *z;
 unsigned int snd=512;
 unsigned int typ_=1;
 unsigned char cc,x,p;
 char tb[6];
 union
 {
 unsigned char u[4];
 void *y;
 } rq;

 tb[1] = 0;
 cc = *(o_bf+2);
 x  = *(o_bf+3);
 asm    mov    al,cc;
 asm    mov    ah,x;
 asm    mov    sm,ax;             /* the size of memory */
 if ((bsr = malloc(sm))==NULL)    /* getmain     */
 {
    tb[0] = 1;                    /* main not found  */
    cc = iobf(snd,tb,o_bf);       /* transmit routine */
    return(-5);
 }
 tb[0] = 0;                       /* Ok              */
 cc = iobf(snd,tb,o_bf);
 p = cc;
 j = (sm / 124);
 if (sm % 124) j++;
 sm = 0;
 z = bsr;
 for (;j>0;j--)
 {
   if (!(cc)) p = iobf(typ_,i_bf,o_bf); /* get block data */
   if (p!=1) break;
   k = (int)*(o_bf);
   for (i=1;i<=k;i++) *(z+sm++)=*(o_bf+i);
   cc=0;
 }
 *(o_bf)=4;
 *(o_bf+1)=0;
 asm    mov    ax,sm;             /* the size of memory */
 asm    mov    x,al;
 *(o_bf+2) = x;
 asm    mov    x,ah;
 *(o_bf+3) = x;
 rq.y = bsr;
 for(i=0;i<4;i++) *(o_bf+i+4)=rq.u[i]; /* pointer */
 return((int)p);
}
