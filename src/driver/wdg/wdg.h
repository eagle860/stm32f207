#ifndef _WDG_H
#define _WDG_H


#include "public.h"


#define IWDG_RELOAD_TIM       2000                 // 2000 * 4ms = 8s
#define IWDG_PRESCALER        IWDG_Prescaler_128   // ���Ź�������4ms��1


void WDG_init(void);


#endif //~ _WDG_H
