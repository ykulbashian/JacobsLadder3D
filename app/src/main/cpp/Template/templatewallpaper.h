#ifndef TEMPLATEWALLPAPER_H_
#define TEMPLATEWALLPAPER_H_

#include "SurfaceRenderer/surfacerenderer.h"

class TemplateWallpaper : public SurfaceRenderer {

public:
	TemplateWallpaper(GLuint id) : SurfaceRenderer(id){}

protected:
    void setupCustomOpenGLInThread(GLuint width, GLuint height);

    void renderFrameInThread(GLfloat fractionOfPreferredFrame);
    void iterateFrameInThread(GLfloat fractionOfPreferredFrame);

    void destroyingCustomGLContextInThread();
};


#endif /* TEMPLATEWALLPAPER_H_ */
