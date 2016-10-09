#include "SurfaceRenderer/logger.h"
#include "SurfaceRenderer/surfacerenderer.h"
#include "SurfaceRenderer/surfaceutils.h"

#define LOG_TAG "SurfaceRenderer"

//LIFE CYCLE===============================================
SurfaceRenderer::SurfaceRenderer(GLuint id)
{
    LOG_INFO_DEBUG("  Creating Wallpaper instance (%d)", id);
    pthread_mutex_init(&_mutex_rendering, 0);
    pthread_mutex_init(&_mutex_driver, 0);

    pthread_cond_init(&_condition_renderer, 0);
    pthread_cond_init(&_condition_driver, 0);

    _msg_renderer = MSG_NONE;
    _msg_driver = 0;

    isPaused = false;

    _display = 0;
    _surface = 0;
    _context = 0;
    _width = 0;
    _height = 0;
    _window = 0;
    _renderThreadId = 0;
    _driverThreadId = 0;

	_screenWidthHalf = 0.0f;
	_screenHeightHalf = 0.0f;

    _screenRatio = 0;

    surface_id = id;

    LOG_INFO_DEBUG("  SurfaceRenderer instance created (%d)", surface_id);
    return;
}

SurfaceRenderer::~SurfaceRenderer()
{
    LOG_INFO_DEBUG("  Destroying SurfaceRenderer instance ...(%d)", surface_id);
    pthread_mutex_destroy(&_mutex_rendering);
    pthread_mutex_destroy(&_mutex_driver);

    pthread_cond_destroy(&_condition_renderer);
    pthread_cond_destroy(&_condition_driver);

    LOG_INFO_DEBUG("  SurfaceRenderer instance destroyed (%d)", surface_id);
}

void SurfaceRenderer::disposeWindow()
{
	if(_window){
		ANativeWindow_release(_window);
		_window = 0;
	}
}

bool SurfaceRenderer::setOrResetWindow(ANativeWindow *window)
{
	START_TIMER

	LOG_INFO_DEBUG("    Setting window (%d)", surface_id);

	bool wasSet = false;

    pthread_mutex_lock(&_mutex_rendering);

    if(window != _window){
    	_msg_renderer = MSG_WINDOW_SET;
    	_window = window;
    	LOG_INFO_DEBUG("    Window set (%d)", surface_id);

		wasSet = true;
    }
    else {
    	_msg_renderer = MSG_WINDOW_RESET;
        pthread_cond_signal(&_condition_renderer);
    	LOG_INFO_DEBUG("    This window already set (%d)", surface_id);

    }

	pthread_mutex_unlock(&_mutex_rendering);

	END_TIMER(SurfaceTimer::SET_WINDOW)

    return wasSet;

}


void SurfaceRenderer::startThread()
{
	START_TIMER

	LOG_INFO_DEBUG("Creating renderer thread (%d)", surface_id);
    pthread_create(&_renderThreadId, 0, renderThreadStartCallback, this);
    pthread_create(&_driverThreadId, 0, driverThreadStartCallback, this);

    END_TIMER(SurfaceTimer::START)
}

void* SurfaceRenderer::renderThreadStartCallback(void *myself)
{
	LOG_INFO_DEBUG("-----------------------------\nRender thread start callback");
    SurfaceRenderer *surface = (SurfaceRenderer*)myself;

    surface->renderLoopInThread();

    pthread_exit(0);

    return 0;
}

void* SurfaceRenderer::driverThreadStartCallback(void *myself)
{
	LOG_INFO_DEBUG("-----------------------------\nDriver thread start callback");
    SurfaceRenderer *surface = (SurfaceRenderer*)myself;

	surface->iterateDriver();

    pthread_exit(0);

    return 0;
}


void SurfaceRenderer::stopAndExitThread()
{
	START_TIMER

    LOG_INFO_DEBUG("Stopping renderer thread (%d)", surface_id);

    pthread_mutex_lock(&_mutex_rendering);
    _msg_renderer = MSG_RENDER_LOOP_EXIT;
    pthread_cond_signal(&_condition_renderer);
    pthread_mutex_unlock(&_mutex_rendering);

    pthread_mutex_lock(&_mutex_driver);
    _msg_driver = 1;
    pthread_cond_signal(&_condition_driver);
    pthread_mutex_unlock(&_mutex_driver);

    LOG_INFO_DEBUG("Joining thread (%d)", surface_id);
    pthread_join(_renderThreadId, 0);
    pthread_join(_driverThreadId, 0);

    LOG_INFO_DEBUG("Renderer thread stopped (%d)", surface_id);

    END_TIMER(SurfaceTimer::STOP)

    return;
}



void SurfaceRenderer::finish()
{
	LOG_INFO_DEBUG("  Finishing SurfaceRenderer (%d)", surface_id);

	LOG_INFO_DEBUG("  SurfaceRenderer finished (%d)", surface_id);

    return;
}

void SurfaceRenderer::pause()
{
	LOG_INFO_DEBUG("Pausing renderer thread (%d)", surface_id);

    pthread_mutex_lock(&_mutex_rendering);
    isPaused = true;
    pthread_mutex_unlock(&_mutex_rendering);
}


void SurfaceRenderer::resume()
{
	LOG_INFO_DEBUG("Resume renderer thread (%d)", surface_id);

    pthread_mutex_lock(&_mutex_rendering);

    isPaused = false;
    pthread_cond_signal(&_condition_renderer);

    pthread_mutex_unlock(&_mutex_rendering);

}


void SurfaceRenderer::setupOpenGlContextInThread(bool resettingSurface)
{
	initializeOpenGLContextInThread(resettingSurface);

	setupCustomOpenGLInThread(_width, _height);
}




bool SurfaceRenderer::initializeOpenGLContextInThread(bool reset)
{
	if(!Constants::RENDER_OPENGL)
		return false;

	START_TIMER

	if(!_window){
		LOG_ERROR("  No window. (%d)", surface_id);
		return false;
	}

    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_NONE
    };
    EGLDisplay display;
    EGLConfig frame_buffer_config;
    EGLint numConfigs;
    EGLint format;
    EGLSurface surface;
    EGLContext context;
    EGLint width;
    EGLint height;

    if(reset){
    	if(_display){
			eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			eglDestroyContext(_display, _context);
			eglDestroySurface(_display, _surface);
			eglTerminate(_display);
    	} else {
    		LOG_ERROR_DEBUG("Tried to reset without a display");
    	}
    }


    LOG_INFO_DEBUG("   Initializing OpenGL context (%d)", surface_id);

    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
        LOG_ERROR("eglGetDisplay() returned error %X", eglGetError());
        return false;
    }
    if (!eglInitialize(display, 0, 0)) {
        LOG_ERROR("eglInitialize() returned error %X", eglGetError());
        return false;
    }

    if (!eglChooseConfig(display, attribs, &frame_buffer_config, 1, &numConfigs)) {
        LOG_ERROR("eglChooseConfig() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

    if (!eglGetConfigAttrib(display, frame_buffer_config, EGL_NATIVE_VISUAL_ID, &format)) {
        LOG_ERROR("eglGetConfigAttrib() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

    ANativeWindow_setBuffersGeometry(_window, 0, 0, format);

    if (!(surface = eglCreateWindowSurface(display, frame_buffer_config, _window, 0))) {
        LOG_ERROR("eglCreateWindowSurface() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

	if (!(context = eglCreateContext(display, frame_buffer_config, 0, 0))) {
        LOG_ERROR("eglCreateContext() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

	if (!eglMakeCurrent(display, surface, surface, context)) {
        LOG_ERROR("eglMakeCurrent() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

    if (!eglQuerySurface(display, surface, EGL_WIDTH, &width) ||
        !eglQuerySurface(display, surface, EGL_HEIGHT, &height)) {
        LOG_ERROR("eglQuerySurface() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

    _display = display;
    _surface = surface;
    _context = context;
    _width = width;
    _height = height;

	_screenWidthHalf = (GLfloat)_width/2.0f;
	_screenHeightHalf = (GLfloat)_height/2.0f;

	_screenRatio = (GLfloat) _width / (GLfloat) _height;

    LOG_INFO_DEBUG("   Finished initializing surface. Width %d, height %d", _width, _height);


	END_TIMER(SurfaceTimer::INIT_OPENGL_CONTEXT)

    return true;
}


void SurfaceRenderer::destroyGLContextInThread()
{
	if(!Constants::RENDER_OPENGL)
		return;

	START_TIMER

	LOG_INFO_DEBUG("Destroying context (%d)", surface_id);

    destroyingCustomGLContextInThread();

    eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(_display, _context);
    eglDestroySurface(_display, _surface);
    eglTerminate(_display);

    _display = EGL_NO_DISPLAY;
    _surface = EGL_NO_SURFACE;
    _context = EGL_NO_CONTEXT;
    _width = 0;
    _height = 0;

    LOG_INFO_DEBUG("Finished destroying GL context. (%d)", surface_id);

	END_TIMER(SurfaceTimer::DESTROY_OPENGL_CONTEXT)

    return;
}





