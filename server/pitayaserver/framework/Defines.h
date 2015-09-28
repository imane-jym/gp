
#ifndef _DEFINES_H
#define _DEFINES_H
#include <stdint.h>

typedef int64_t      int64;
typedef uint64_t 	uint64;


typedef int32_t      int32;
typedef int16_t      int16;
typedef int8_t       int8;
typedef uint32_t     uint32;
typedef uint16_t     uint16;
typedef uint8_t      uint8;

enum
{
	MAIN_THREAD,                                /* main */
	WORLD_THREAD                                /* logical */
};

enum
{
	TIMER_1S = 1,
	TIMER_3S = 3,
	TIMER_5S = 5,
	TIMER_15S = 15,
	TIMER_30S = 30,
	TIMER_1MIN = 60,
	TIMER_3MIN = 180,
	TIMER_5MIN = 300,
	TIMER_10MIN = 600,
	TIMER_30MIN = 1800,
};


#endif
