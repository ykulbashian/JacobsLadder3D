#include "Template/templatewallpaper.h"


void TemplateWallpaper::setupCustomOpenGLInThread(GLuint width, GLuint height)
{

}

void TemplateWallpaper::renderFrameInThread(GLfloat fractionOfPreferredFrame)
{
		glClearColor(0.5f, fractionOfPreferredFrame, 0.1f, 1.0f);
}

void TemplateWallpaper::iterateFrameInThread(GLfloat fractionOfPreferredFrame)
{

}

void TemplateWallpaper::destroyingCustomGLContextInThread()
{

}


