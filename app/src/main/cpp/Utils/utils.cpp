#include "SurfaceRenderer/logger.h"
#include "Drivers/Grid/gridposition.h"
#include "Utils/utils.h"
#include "SurfaceRenderer/surfaceutils.h"

#define LOG_TAG "Utils"

bool Utils::areOppositeDirections(GLubyte one, GLubyte two)
{
	GLubyte merge = one | two;

	return (merge == X_DIR || merge == Y_DIR || merge == Z_DIR);
}

//error printing utility method---------------------------------------
void Utils::checkGlError(const char* mode, GLint id){
	if(!DEBUG_OPENGL || !Constants::DEBUG_DETAIL_MODE) return;

	LOG_INFO_DEBUG("%s has error?", mode);
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		LOG_INFO_DEBUG("%s %d (%d)", mode, err, id);
	}
}
