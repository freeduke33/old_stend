#include <tcxldef.h>
#include "stend.h"

struct Cfg_type
{
 int OBJType;     // управляемый обьект (стенд, имитатор)
 int CtrlType;    // тип управления (с ОС, без ОС)

};



VOID CTYP Config(NOARG)
{
   Error(" Будет в будующих версиях ...");
   return;
}

VOID CTYP ReadConfig(NOARG)
{
  return;
}

int CTYP ReadCfg(NOARG)
{
 return 0;
}


int CTYP WriteCfg(NOARG)
{
 return 0;
}


