#ifndef CLOCK_INTERFACE_H
#define CLOCK_INTERFACE_H

void MCLOCK_voidDelayMS(u32 ms);

void MCLOCK_voidDelay2USAt16MHz(void);

void MCLOCK_voidDelay1USAt16MHz(void);

u32 MCLOCK_u32GetSystemClock(void);

#endif

