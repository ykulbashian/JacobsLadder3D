#ifndef LIGHT_H_
#define LIGHT_H_

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>
#include <Utils/utils.h>
#include "SurfaceRenderer/logger.h"
#include "Drawing/ColorSets/colorset.h"
#include "Drawing/ColorSets/utils.h"
#include "SurfaceRenderer/surfaceutils.h"



#define MAX_LIGHT_STRENGTH 1600.0f

struct Light
{
	const char* lightName;
	GLfloat x_pos_fraction, y_pos_fraction, z_pos_fraction;
	GLubyte colors[3];
	GLfloat maxReach;

	void assignFromColor(GLuint color){

		color_array cols = {0, 0, 0, 0};

		getColorByteArrayFromInt(color, false, cols);
		assignFromColorArray(cols);
	}

	void assignFromColorArray(GLubyte *cols){

		colors[0] = cols[0];
		colors[1] = cols[1];
		colors[2] = cols[2];

		maxReach = MAX_LIGHT_STRENGTH*cols[3]/255.0f;
	}
};


#endif /* LIGHT_H_ */
