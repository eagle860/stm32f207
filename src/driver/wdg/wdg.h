#ifndef _WDG_H
#define _WDG_H


#include "public.h"


#define IWDG_RELOAD_TIM       2000                 // 2000 * 4ms = 8s
#define IWDG_PRESCALER        IWDG_Prescaler_128   // 看门狗计数器4ms减1


void WDG_init(void);


#endif //~ _WDG_H
