#include "TestSuite/cubedrivertest.h"
#include "Drivers/cubedriver.h"
#include "Stats/statssnapshot.h"

#define LOG_TAG "tags"

static CubeDriver *driver;



JNIEXPORT jstring JNICALL Java_com_mycardboarddreams_jacobsladder_deluxe_drivers_CubeDriverTest_createDriverInstance(
		JNIEnv* jenv, jobject obj,
		jint xBlocks, jint yBlocks, jint zBlocks, jint iterations, jint iterationSpan, jfloat speed){

	driver = new CubeDriver();
	Dimension dimens = Dimension(xBlocks, yBlocks, zBlocks, 4000);
	driver->initializeDimensions(dimens);

	jstring result;

	std::string statsJson = StatsSnapshot::startRecording(driver, iterations, iterationSpan, speed);

	result = jenv->NewStringUTF(statsJson.c_str());
	return result;
}

