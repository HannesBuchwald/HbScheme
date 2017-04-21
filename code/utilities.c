/*
 * hbscheme.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.2
 * Last edit:    20.04.2017
 *
 */



/**************** includes *********************/

#include "../header/hbscheme.h"
#include "../header/utilities.h"
#include "../header/printer.h"
#include <sys/time.h>



/**************** global functions *********************/

long millisecondTime() {
    long millis;

#ifdef USE_GETTIMEOFDAY
    struct timeval tv;
    struct timezone tz;

    if (gettimeofday(&tv, &tz) < 0) {
        FATAL("gettimeofday failed");
    }
    millis = (tv.tv_sec * 1000)+(tv.tv_usec / 1000);
#else
    struct timeb timebuffer;

    ftime(&timebuffer);
    millis = (timebuffer.time*1000)+timebuffer.millitm;
#endif
    return millis;
}
