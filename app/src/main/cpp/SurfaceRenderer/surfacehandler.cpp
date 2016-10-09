#include "SurfaceRenderer/surfacerenderer.h"
#include "SurfaceRenderer/logger.h"

#include "SurfaceRenderer/surfacehandler.h"
#include "SurfaceRenderer/native_handle.h"
#include "SurfaceRenderer/surfaceutils.h"

#define LOG_TAG "SurfaceRenderer handler"

SurfaceRenderer* SurfaceHandler::getSurfaceRendererByHandle(jint id, jobject obj, JNIEnv *jenv)
{
	SurfaceRenderer *existingSurfaceRenderer = NativeHandle::getHandle<SurfaceRenderer>(jenv, obj);

	if(!existingSurfaceRenderer){
		LOG_ERROR_DEBUG("Creating new SurfaceRenderer (%p) and setting handle", existingSurfaceRenderer);
		existingSurfaceRenderer = getNewSurfaceRenderer(id);

		NativeHandle::setHandle(jenv, obj, existingSurfaceRenderer);
	}

	if(!existingSurfaceRenderer)
		LOG_ERROR("Unable to create SurfaceRenderer (%d)", id);

	return existingSurfaceRenderer;
}




