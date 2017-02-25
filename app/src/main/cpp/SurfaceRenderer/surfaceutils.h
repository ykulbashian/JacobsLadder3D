
#ifndef SURFACEUTILS_H_
#define SURFACEUTILS_H_


#include "Utils/timer.h"
#include "SurfaceRenderer/logger.h"
#include <assert.h>

class Constants{
public:
	static bool DEBUG_DETAIL_MODE;
	static bool RENDER_OPENGL;

};

class SurfaceTimer {

public:

	enum ModuleNames{
		START,
		STOP,
		SET_BLOCKS,
		SET_COLORS,
		SET_WINDOW,
		SET_CUSTOM_OPENGL,
		DESTROY_OPENGL_CONTEXT,
		INIT_OPENGL_CONTEXT,
		BUILD_COLORS_INIT,
		BUILD_COLORS_DERIVE,
		UNSET_VERTICES,
		SET_CUBE_POSITIONS,
		SET_VERTICES,
		DERIVE_VERTICES,
		DERIVE_BACKGROUND,
	};

	static long long checkCurrentTimestamp();
};

#define DEBUG_MODE_TIMER true
#define DEBUG_OPENGL true

#define FLOATING_POINT_PRECISION 65536

#define RANDOM(n) (rand()%(n))


#define LOG_INFO_DEBUG(...) if(Constants::DEBUG_DETAIL_MODE)LOG_INFO(__VA_ARGS__)
#define LOG_ERROR_DEBUG(...) if(Constants::DEBUG_DETAIL_MODE)LOG_ERROR(__VA_ARGS__)

#ifdef DEBUG_MODE_TIMER

#define START_TIMER Timer::startTimer();
#define END_TIMER(num) Timer::SavedTimes[num] = Timer::stopTimer();

#else

#define START_TIMER
#define END_TIMER(num)

#endif

#endif
