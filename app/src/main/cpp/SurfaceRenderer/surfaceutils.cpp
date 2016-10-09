#include "SurfaceRenderer/surfaceutils.h"

#include <time.h>
#include <math.h>

bool Constants::DEBUG_DETAIL_MODE = true;
bool Constants::RENDER_OPENGL = true;



long long SurfaceTimer::checkCurrentTimestamp(){

    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);
    long long secs = ((long long) spec.tv_sec)*1000ll;
    long long xx = (long long)round(spec.tv_nsec / 1.0e6);

    return secs + xx;

}
