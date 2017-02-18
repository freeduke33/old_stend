#include "stdlib.h"
#include "limits.h"

#define mod(a,b)   ((a)%(b)) /* ((a)-floor((a)/(b))*(b)) */
#define Lrand(a,b)   ((2*(unsigned)rand()%(a-b))+b)

#define __MAX_p 0x0fffL
#define __MIN_p 0x01ffL
#define __LEN_p 12

