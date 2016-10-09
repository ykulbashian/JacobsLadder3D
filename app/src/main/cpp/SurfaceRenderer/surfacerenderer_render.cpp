#include "SurfaceRenderer/surfacerenderer.h"
#include "SurfaceRenderer/logger.h"
#include "SurfaceRenderer/surfaceutils.h"
#include <unistd.h>

#define LOG_TAG "SurfaceRenderer Render"

void SurfaceRenderer::renderLoopInThread()
{
    bool renderingEnabled = true;
    bool renderThisFrame = true;


    LOG_INFO_DEBUG("  Entering renderLoop() (%d)", surface_id);

    while (renderingEnabled)
    {
        pthread_mutex_lock(&_mutex_rendering);

        // process incoming messages---------------
        switch (_msg_renderer) {
			case MSG_NONE:
			case MSG_PAUSE:
				break;

    		case MSG_WINDOW_SET:
    			LOG_INFO_DEBUG("case SET WINDOW (%d)", surface_id);
            	setupOpenGlContextInThread();
            	LOG_INFO_DEBUG("finished case SET WINDOW (%d)", surface_id);
                break;

            case MSG_RESET_SCENE_VBOS:
            case MSG_WINDOW_RESET:
            	LOG_INFO_DEBUG((_msg_renderer == MSG_RESET_SCENE_VBOS) ? "case reset SCENE VBOs (%d)" : "case RESET WINDOW (%d)", surface_id);
            	setupOpenGlContextInThread(true);
            	LOG_INFO_DEBUG((_msg_renderer == MSG_RESET_SCENE_VBOS) ? "finished case reset SCENE VBOs (%d)" : "finished case RESET WINDOW (%d)", surface_id);
				break;

			case MSG_RENDER_LOOP_EXIT:
				LOG_INFO_DEBUG("case EXIT LOOP (%d)", surface_id);
            	destroyGLContextInThread();
                renderingEnabled = false;
            	renderThisFrame = false;
            	LOG_INFO_DEBUG("finished case EXIT LOOP (%d)", surface_id);
            	break;

        }

        _msg_renderer = MSG_NONE;

        if(renderingEnabled && isPaused){
        	renderThisFrame = false;
			pthread_cond_wait(&_condition_renderer, &_mutex_rendering);
			LOG_INFO_DEBUG("Resuming thread from pause condition.");
			setThisFrameDuration();
        }
        else if(renderThisFrame){
        	renderThread();
        }

        pthread_mutex_unlock(&_mutex_rendering);

        if(renderingEnabled)
        	usleep(20000);

        if(renderThisFrame)
        {
            pthread_cond_signal(&_condition_driver);
        }

		setThisFrameDuration();

		renderThisFrame = true;

    }

    LOG_INFO_DEBUG("Render loop exits (%d)", surface_id);

}

void SurfaceRenderer::iterateDriver(){

	while(!_msg_driver){
		pthread_mutex_lock(&_mutex_driver);
		pthread_cond_wait(&_condition_driver, &_mutex_driver);

		if(!_msg_driver){
			iterateFrameInThread(fractionOfPreferredFrame);
		}

		pthread_mutex_unlock(&_mutex_driver);
	}
}


void SurfaceRenderer::renderThread()
{

	if (!_display || !_surface)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderFrameInThread(fractionOfPreferredFrame);

	if (!eglSwapBuffers(_display, _surface)) {
		LOG_ERROR("eglSwapBuffers() returned error %d", eglGetError());
	}
}


void SurfaceRenderer::setThisFrameDuration()
{
	static long long lastFrameEnded = 0;

	long long thisFrameTimestamp = SurfaceTimer::checkCurrentTimestamp();
	long long timeSinceLastFrame = thisFrameTimestamp - lastFrameEnded;
	lastFrameEnded = thisFrameTimestamp;
	fractionOfPreferredFrame = (GLfloat)timeSinceLastFrame/(GLfloat)PREFERRED_FRAME_DURATION;
}

