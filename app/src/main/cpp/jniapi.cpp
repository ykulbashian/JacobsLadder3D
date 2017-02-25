#include "jniapi.h"
#include "SurfaceRenderer/logger.h"
#include "SurfaceRenderer/surfaceutils.h"

#include "wallpaper.h"
#include "SurfaceRenderer/surfacehandler.h"

#define LOG_TAG "JniApi"

void methodStartOne(jint id, const char *methodName)
{
	LOG_INFO_DEBUG("------------------------------");
	LOG_INFO_DEBUG("JNI %s (%d)", methodName, id);

#ifdef DEBUG_MODE_TIMER
	LOG_ERROR_DEBUG("JNI %s %llu", methodName, Timer::stopTimer(Timer::LONG_TERM));
#endif
}

jintArray convertToIntArray(JNIEnv* jenv, GLuint* adjustedArray){

	jintArray adjusted = jenv->NewIntArray(LENGTH_OF_CUSTOM_ARRAY);

	if (adjusted == NULL || adjustedArray == NULL) {
		return NULL;
	}

	int* signedArray = new int[LENGTH_OF_CUSTOM_ARRAY];
	for(int i = 0; i < LENGTH_OF_CUSTOM_ARRAY; i++){
		signedArray[i] = static_cast<int>(adjustedArray[i]);
	}

	jenv->SetIntArrayRegion(adjusted, 0, LENGTH_OF_CUSTOM_ARRAY, signedArray);
	return adjusted;
}


//====================================================================
//JNI methods---------------------------------------------------------

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupSpeed(JNIEnv* jenv, jobject obj, jint id, jint speed)
{
	methodStartOne(id, "nativeSetupSpeed");

	Wallpaper *wallpaper = (Wallpaper*) SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

	if(wallpaper)
		wallpaper->setSpeed(speed);
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeSetupSpeed(JNIEnv* jenv, jobject obj, jint id, jint speed)
{
	Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupSpeed(jenv, obj, id, speed);
}

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupAngle(JNIEnv* jenv, jobject obj, jint id, jfloat angle)
{
	Wallpaper *wallpaper = (Wallpaper*) SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

	if(wallpaper)
		wallpaper->setAngle(angle);
}

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupBlocks(JNIEnv* jenv, jobject obj, jint id, jint x, jint y, jint z, jint total)
{
	methodStartOne(id, "nativeSetupBlocks");

    Wallpaper *wallpaper = (Wallpaper*) SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

	if(wallpaper){
		const Dimension dimens = Dimension(x, y, z, total);
		wallpaper->setBlocks(dimens);
	}
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeSetupBlocks(JNIEnv* jenv, jobject obj, jint id, jint x, jint y, jint z, jint total)
{
	Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupBlocks(jenv, obj, id, x, y, z, total);
}

JNIEXPORT jintArray JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupCustomColors(JNIEnv* jenv, jobject obj, jint id, jstring setName, jintArray colors)
{
	methodStartOne(id, "nativeSetupCustomColors");
	Wallpaper *wallpaper = (Wallpaper*) SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

	const char* colorString = jenv->GetStringUTFChars(setName, 0);
	jenv->ReleaseStringUTFChars(setName, colorString);

	int* colorArray = jenv->GetIntArrayElements(colors, 0);

	jenv->ReleaseIntArrayElements(colors, colorArray, 0);

	GLuint* adjustedArray = 0;
	if(wallpaper)
		adjustedArray = wallpaper->setCustomColorScheme(colorArray);

	return convertToIntArray(jenv, adjustedArray);
}
JNIEXPORT jintArray JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeSetupCustomColors(JNIEnv* jenv, jobject obj, jint id, jstring setName, jintArray colors)
{
	return Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupCustomColors(jenv, obj, id, setName, colors);
}


JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeTouchEvent(JNIEnv* jenv, jobject obj, jint id, jfloat x, jfloat y, jboolean isSingleTap)
{
    Wallpaper *wallpaper = (Wallpaper*) SurfaceHandler::getSurfaceRendererByHandle(id, obj, jenv);

	if(wallpaper)
		wallpaper->touchEvent(x, y, isSingleTap);
	else
		LOG_ERROR_DEBUG("No wallpaper (on touch) (%d)", id);
}


JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_nativeIsInDebugMode(JNIEnv* jenv, jobject obj, jboolean isDebug)
{
	Constants::DEBUG_DETAIL_MODE = isDebug;
}

JNIEXPORT jintArray JNICALL Java_net_margaritov_preference_colorpicker_ColorPickerPreference_nativeGetApparentCustomColors(JNIEnv* jenv, jobject obj)
{
	GLuint* adjustedArray = ColorSet::getCurrentColorSet()->getApparentColors();

	return convertToIntArray(jenv, adjustedArray);
}
