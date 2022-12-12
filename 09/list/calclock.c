#include "calclock.h"

unsigned long long calclock(struct timespec *myclock)
{
	unsigned long long timedelay = 0, temp = 0, temp_n = 0;
	
	if (myclock[1].tv_nsec >= myclock[0].tv_nsec) {
		temp = myclock[1].tv_sec - myclock[0].tv_sec;
		temp_n = myclock[1].tv_nsec - myclock[0].tv_nsec;
		timedelay = BILLION * temp + temp_n;
	} else {
		temp = myclock[1].tv_sec - myclock[0].tv_sec - 1;
		temp_n = BILLION + myclock[1].tv_nsec - myclock[0].tv_nsec;
		timedelay = BILLION * temp + temp_n;
	}
	
	return timedelay;
}
