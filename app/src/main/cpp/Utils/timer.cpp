#include "Utils/timer.h"
#include "SurfaceRenderer/surfaceutils.h"

long long Timer::currentTimestamp0 = 0;
long long Timer::currentTimestamp1 = 0;

unsigned long long *Timer::SavedTimes = new unsigned long long[20];

#define LOG_TAG "Timer"

void Timer::startTimer(char timerNumber)
{
#ifdef DEBUG_MODE_TIMER
	(timerNumber == AD_HOC ? currentTimestamp0 : currentTimestamp1) = SurfaceTimer::checkCurrentTimestamp();
#endif
}

long long Timer::stopTimer(char timerNumber)
{
#ifdef DEBUG_MODE_TIMER
	return SurfaceTimer::checkCurrentTimestamp() - (timerNumber == AD_HOC ? currentTimestamp0 : currentTimestamp1);
#else
	return 0;
#endif
}

void Timer::printTimes()
{
#ifdef DEBUG_MODE_TIMER

	if(!Constants::DEBUG_DETAIL_MODE)
		return;

    LOG_ERROR_DEBUG("Wallpaper::start() took %llu ms", SavedTimes[SurfaceTimer::START]);
    LOG_ERROR_DEBUG("Wallpaper::stop() took %llu ms", SavedTimes[SurfaceTimer::STOP]);
    LOG_ERROR_DEBUG("Wallpaper::setWindow() took %llu ms", SavedTimes[SurfaceTimer::SET_WINDOW]);
	LOG_ERROR_DEBUG("Wallpaper::setBlocks() took %llu ms", SavedTimes[SurfaceTimer::SET_BLOCKS]);
	LOG_ERROR_DEBUG("Wallpaper::setInitialCubePositions() took %llu ms", SavedTimes[SurfaceTimer::SET_CUBE_POSITIONS]);
	LOG_ERROR_DEBUG("Wallpaper::setColors() took %llu ms", SavedTimes[SurfaceTimer::SET_COLORS]);
	LOG_ERROR_DEBUG("Wallpaper::setOpenGlContext() took %llu ms", SavedTimes[SurfaceTimer::SET_CUSTOM_OPENGL]);
    LOG_ERROR_DEBUG("Wallpaper::destroyGLContext() took %llu ms", SavedTimes[SurfaceTimer::DESTROY_OPENGL_CONTEXT]);
    LOG_ERROR_DEBUG("Engine::unsetVertices() took %llu ms", SavedTimes[SurfaceTimer::UNSET_VERTICES]);
    LOG_ERROR_DEBUG("Engine::setVertices() took %llu ms", SavedTimes[SurfaceTimer::SET_VERTICES]);
    LOG_ERROR_DEBUG("FaceColors::buildColors() init took %llu ms", SavedTimes[SurfaceTimer::BUILD_COLORS_INIT]);
    LOG_ERROR_DEBUG("FaceColors::buildColors() derive took %llu ms", SavedTimes[SurfaceTimer::BUILD_COLORS_DERIVE]);
    LOG_ERROR_DEBUG("Engine::deriveVerticies() took %llu ms", SavedTimes[SurfaceTimer::DERIVE_VERTICES]);
    LOG_ERROR_DEBUG("Engine::deriveBackground() took %llu ms", SavedTimes[SurfaceTimer::DERIVE_BACKGROUND]);
    LOG_ERROR_DEBUG("Wallpaper::initializeOpenGLContext() took %llu ms", SavedTimes[SurfaceTimer::INIT_OPENGL_CONTEXT]);

#endif
}

