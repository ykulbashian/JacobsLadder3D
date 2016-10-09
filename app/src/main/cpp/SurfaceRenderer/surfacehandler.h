
#ifndef SURFACEGENERATOR_H_
#define SURFACEGENERATOR_H_

#include "SurfaceRenderer/surfacerenderer.h"


class SurfaceHandler
{
public:
	static SurfaceRenderer * getSurfaceRendererByHandle(jint id, jobject obj, JNIEnv *jenv);

	static SurfaceRenderer * getNewSurfaceRenderer(GLuint id);

};



#endif /* SURFACEGENERATOR_H_ */
