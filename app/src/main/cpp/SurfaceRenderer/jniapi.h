#ifndef JNIAPI_H
#define JNIAPI_H

#include <jni.h>

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>

extern "C" {
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnCreate(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStart(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnPause(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnResume(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStop(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnFinish(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeSetSurface(JNIEnv* jenv, jobject obj, jint id, jobject javaSurface);


    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnCreate(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnStart(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnPause(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnResume(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnStop(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnFinish(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeSetSurface(JNIEnv* jenv, jobject obj, jint id, jobject surface);

};

#endif // JNIAPI_H
