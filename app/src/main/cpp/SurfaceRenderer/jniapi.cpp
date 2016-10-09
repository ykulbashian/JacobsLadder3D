
#include "SurfaceRenderer/jniapi.h"
#include "SurfaceRenderer/logger.h"
#include "SurfaceRenderer/surfacehandler.h"

#include "SurfaceRenderer/surfaceutils.h"

#include "SurfaceRenderer/native_handle.h"

#define LOG_TAG "JniApi"

void methodStart(jint id, const char *methodName)
{
	LOG_INFO_DEBUG("------------------------------");
	LOG_INFO_DEBUG("JNI %s (%d)", methodName, id);

#ifdef DEBUG_MODE_TIMER

#endif
}


//====================================================================
//JNI methods---------------------------------------------------------

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnCreate(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnCreate");

	SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnCreate(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnCreate(jenv, obj, id);
}

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStart(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnStart");

	SurfaceRenderer *surface = SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

	if(surface)
		surface->startThread();
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnStart(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStart(jenv, obj, id);
}


JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnPause(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnPause");

	SurfaceRenderer *surface = SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

	if(surface)
		surface->pause();
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnPause(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnPause(jenv, obj, id);
}

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnResume(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnResume");

	SurfaceRenderer *surface = SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

	if(surface)
		surface->resume();
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnResume(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnResume(jenv, obj, id);
}


JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStop(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnStop");

	SurfaceRenderer *surface = SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

	if(surface)
		surface->stopAndExitThread();
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnStop(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStop(jenv, obj, id);
}

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnFinish(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnFinish");

	SurfaceRenderer *surface = SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

	if(surface){
		surface->finish();
		delete surface;
		surface = 0;

		NativeHandle::setHandle(jenv, obj, surface);
	}
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnFinish(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnFinish(jenv, obj, id);
}

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeSetSurface(JNIEnv* jenv, jobject obj, jint id, jobject javaSurface)
{
	methodStart(id, "nativeSetSurface");

	SurfaceRenderer *surface = SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

    if (javaSurface != 0) {
	    ANativeWindow *newWindow = ANativeWindow_fromSurface(jenv, javaSurface);

	    LOG_INFO_DEBUG("  Retrieved window %p (%d)", newWindow, id);
		if(surface && newWindow)
			surface->setOrResetWindow(newWindow);
    } else {
		LOG_INFO_DEBUG("  Releasing window (%d)", id);

	    ANativeWindow *window = surface->_window;

		if(window){
			surface->pause();

			surface->disposeWindow();
		}
		else
			LOG_ERROR("No window (%d)", id);
    }

}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeSetSurface(JNIEnv* jenv, jobject obj, jint id, jobject javaSurface)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeSetSurface(jenv, obj, id, javaSurface);
}
