
#ifndef RENDERER_H
#define RENDERER_H

#include <pthread.h>
#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>
#include <Geometry/dimension.h>

#include "SurfaceRenderer/surfacerenderer.h"
#include "Drawing/drawing.h"
#include "Drivers/cubedriver.h"
#include "Drawing/objectbuffers.h"
#include "Drawing/textures.h"

class Wallpaper : public SurfaceRenderer {

public:

    Wallpaper(GLuint id);
	~Wallpaper();

    void setBlocks(Dimension dimens);
	GLuint* setCustomColorScheme(GLint *colorSet);
    void setSpeed(GLint speed);
    void setAngle(GLfloat angle);

    void touchEvent(GLfloat x, GLfloat y, GLboolean isSingleTap);

    void setupCustomOpenGLInThread(GLuint width, GLuint height);

	bool resetSize(Dimension dimens);

	bool isInitialized();

	GLuint* updateCurrentColorSet(GLint *customColors = 0);

private:
    
    void renderFrameInThread(GLfloat fractionOfPreferredFrame);
    void iterateFrameInThread(GLfloat fractionOfPreferredFrame);

    void destroyingCustomGLContextInThread();

	GLfloat angleOfRotation;
	GLfloat cubeMoveFraction;

	Drawing drawing;
	CubeDriver cubeDriver;

	int objectBufferVersion;

};

#endif // RENDERER_H
