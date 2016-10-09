#include "SurfaceRenderer/surfacehandler.h"
#include "templatewallpaper.h"

SurfaceRenderer * SurfaceHandler::getNewSurfaceRenderer(GLuint id){
	return new TemplateWallpaper(id);
}
