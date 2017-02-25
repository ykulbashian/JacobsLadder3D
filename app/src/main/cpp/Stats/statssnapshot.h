
#ifndef STATSSNAPSHOT_H_
#define STATSSNAPSHOT_H_

#include "Stats/aggregatestats.h"
#include "Drivers/cubedriver.h"

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>


class StatsSnapshot {

public:
	static std::string startRecording(CubeDriver *driver, GLuint iterations, GLuint iterationSpan, GLfloat speed);

};


#endif
