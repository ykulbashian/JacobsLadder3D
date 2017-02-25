#include <Stats/statssnapshot.h>

#include "wallpaper.h"


//#define TAKE_LIVE_STATS true

#define LOG_TAG "Wallpaper"

Wallpaper::Wallpaper(GLuint id) : SurfaceRenderer(id)
{
	angleOfRotation = 0.0f;
	cubeMoveFraction = 0.0f;

	objectBufferVersion = 0;

	for(int i = 0; i < 20; i++)
		Timer::SavedTimes[i] = 0;
}

Wallpaper::~Wallpaper()
{
}

bool Wallpaper::isInitialized()
{
	ObjectBuffers *pBuffers = ObjectBuffers::getObjectBuffers();
	pBuffers->lock();
	bool isBufferReady = pBuffers->isBufferReady();
	pBuffers->unlock();

	return isBufferReady && cubeDriver.isDriverInitialized();
}

bool Wallpaper::resetSize(Dimension dimens)
{
	bool driverWasInit = cubeDriver.initializeDimensions(dimens);
	bool engineWasInit = ObjectBuffers::getObjectBuffers()->tryRederiveVertices(dimens);

	return driverWasInit || engineWasInit;
}

GLuint* Wallpaper::updateCurrentColorSet(GLint customColors[LENGTH_OF_CUSTOM_ARRAY])
{
	GLuint* adjustedArray = ColorSet::getCurrentColorSet()->updateCurrentColorSet(customColors);

	cubeDriver.shouldRefresh = true;

	return adjustedArray;
}

//END LIFE CYCLE======================================
void Wallpaper::setBlocks(Dimension dimens)
{
	pthread_mutex_lock(&_mutex_rendering);

	LOG_INFO_DEBUG("  Setting blocks to (%d, %d, %d).... (%d)", dimens.X_BLOCKS, dimens.Y_BLOCKS, dimens.Z_BLOCKS, surface_id);

	ObjectBuffers *pBuffers = ObjectBuffers::getObjectBuffers();
	pBuffers->lock();

	resetSize(dimens);
	int bufferVersion = pBuffers->getObjectBufferVersion();

	if(bufferVersion != objectBufferVersion) {
		_msg_renderer = MSG_RESET_SCENE_VBOS;
		objectBufferVersion = bufferVersion;
	}

	pBuffers->unlock();

	pthread_mutex_unlock(&_mutex_rendering);
}


void Wallpaper::setSpeed(GLint speed)
{
	pthread_mutex_lock(&_mutex_rendering);

	LOG_INFO_DEBUG("  Changing speed to %d.... (%d)", speed, surface_id);

	static GLuint cubeMoveIterations = 8;

	cubeMoveIterations = 11u-speed;

	cubeMoveFraction = 1.0f/(cubeMoveIterations);

	LOG_INFO_DEBUG("  Finished changing speed to %d. (%d)", speed, surface_id);

	pthread_mutex_unlock(&_mutex_rendering);
}

void Wallpaper::setAngle(GLfloat angle)
{
	pthread_mutex_lock(&_mutex_rendering);

	angleOfRotation = angle;

	pthread_mutex_unlock(&_mutex_rendering);
}

GLuint* Wallpaper::setCustomColorScheme(GLint *colorSet)
{
	START_TIMER

	pthread_mutex_lock(&_mutex_rendering);

	LOG_INFO_DEBUG("  Setting colors in engine to set: Custom Colors (%d)", surface_id);

	GLuint* adjustedArray = updateCurrentColorSet(colorSet);

	pthread_cond_signal(&_condition_renderer);

	pthread_mutex_unlock(&_mutex_rendering);

	END_TIMER(SurfaceTimer::SET_COLORS)

	return adjustedArray;
}

void Wallpaper::touchEvent(GLfloat x, GLfloat y, GLboolean isSingleTap)
{
	double offset = -1*tan(angleOfRotation) * _screenWidthHalf;
	x += offset;

	GLfloat xConst = ((x - _screenWidthHalf)/(_screenWidthHalf / _screenRatio))/NEAR_FIELD;
	GLfloat yConst = ((-y + _screenHeightHalf)/(_screenHeightHalf / ZOOM_FACTOR))/NEAR_FIELD;

	pthread_mutex_lock(&_mutex_driver);

	cubeDriver.onTouchEvent(int(x), int(y), xConst, yConst, drawing.cameraMovementShift, isSingleTap);

	pthread_mutex_unlock(&_mutex_driver);
}

void Wallpaper::setupCustomOpenGLInThread(GLuint width, GLuint height)
{
	START_TIMER

	if(!Constants::RENDER_OPENGL)
		return;

	if(width == 0 || height == 0) return;
	
	ObjectBuffers *pBuffers = ObjectBuffers::getObjectBuffers();
	
	pBuffers->lock();

	Utils::checkGlError("Setting up drawing... (%d)", surface_id);


    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);


	Utils::checkGlError("after setups", surface_id);


    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustumf(-_screenRatio, _screenRatio, -ZOOM_FACTOR, ZOOM_FACTOR, NEAR_FIELD, 2*(cubeDriver.Z_BLOCKS));
    Utils::checkGlError("after projection", surface_id);

	//SURFACING----------------------------
	glFrontFace(GL_CCW);
	glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	Utils::checkGlError("after surfacing", surface_id);


	//DEPTH--------------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glDepthRangef(0.0f, 1.0f);
	Utils::checkGlError("after depth", surface_id);


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//set up rendering-----------------------------
	glMatrixMode(GL_MODELVIEW);

	LOG_INFO_DEBUG("    GL set up : height - %d width - %d Zoom factor %f (%d)", height, width, ZOOM_FACTOR, surface_id);

	pBuffers->bindVerticesToBuffersInThread();
	LOG_INFO_DEBUG("   Finished creating custom GL context (%d)", surface_id);

	pBuffers->unlock();

	END_TIMER(SurfaceTimer::SET_CUSTOM_OPENGL)

	Timer::printTimes();
}


void Wallpaper::renderFrameInThread(GLfloat fractionOfPreferredFrame)
{
	static GLuint debugTimeIteration = 0;
	static long long lastFrameRenderTime = 0;
	static bool hasRendered = false;

	if(isInitialized())
	{

#ifdef DEBUG_MODE_TIMER
		if(!hasRendered){
			LOG_ERROR_DEBUG("First render loop %llu", Timer::stopTimer(Timer::LONG_TERM));
			Timer::startTimer(Timer::LONG_TERM);
			hasRendered = true;
		}
#endif

		if(DEBUG_OPENGL)
			Timer::startTimer();

		cubeDriver.resetIfNecessary();

		drawing.drawCubes(fractionOfPreferredFrame,
						   angleOfRotation,
						   &cubeDriver._grid,
						   cubeDriver.allArrayCollections.movingElements,
						   &cubeDriver.goalSet);

		if(DEBUG_OPENGL && Constants::DEBUG_DETAIL_MODE && ++debugTimeIteration > 30){
			lastFrameRenderTime = Timer::stopTimer();
			LOG_INFO_DEBUG("Last frame took %lld ms (%d)", lastFrameRenderTime, surface_id);
			debugTimeIteration = 0;
		}
	}
}

void Wallpaper::iterateFrameInThread(GLfloat fractionOfPreferredFrame)
{
	if(isInitialized())
	{

#ifdef TAKE_LIVE_STATS
		static int iteration = 0;
		//TODO the path should use getExternalStorageDirectory()
		if ((iteration % 30) == 0) {
			AggregateStats stats = cubeDriver.getAggregateStats();

			//statsJson += stats.getAsJson();
		}
		iteration++;
#endif

		GLuint moveDistance = static_cast<GLuint>(cubeMoveFraction*fractionOfPreferredFrame*FLOATING_POINT_PRECISION);
		cubeDriver.iterateOneFrame(moveDistance);
	}
}


void Wallpaper::destroyingCustomGLContextInThread()
{
	LOG_INFO_DEBUG(" Destroying custom GL context (%d)", surface_id);
	//TODO this should unbind if there are no wallpapers
	//ObjectBuffers::getObjectBuffers()->unbindVertexBuffers();
}

