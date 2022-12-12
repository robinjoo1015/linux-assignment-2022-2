#ifndef __CALCLOCK_H
#define __CALCLOCK_H

#include <linux/time.h>

#define BILLION 1000000000UL

unsigned long long calclock(struct timespec *myclock);

#endif
