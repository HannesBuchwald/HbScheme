#include "hbscheme.h"

#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>

#define USE_GETTIMEOFDAY

long
millisecondTime() {
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
