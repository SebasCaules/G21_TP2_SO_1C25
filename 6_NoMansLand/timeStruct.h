#ifndef __TIME_STRUCT_H__
#define __TIME_STRUCT_H__

#include <stdint.h>

typedef struct time {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minutes;
    uint8_t seconds;

} time_struct;

#endif // __TIME_STRUCT_H__