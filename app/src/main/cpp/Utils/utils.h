
#ifndef UTILS_H_
#define UTILS_H_

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>


class Utils
{

public:
	static bool areOppositeDirections(GLubyte one, GLubyte two);
	static void checkGlError(const char* mode = "", GLint id = 0);

};



#endif /* UTILS_H_ */
