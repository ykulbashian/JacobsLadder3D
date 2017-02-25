#include "SurfaceRenderer/surfacehandler.h"
#include "wallpaper.h"

SurfaceRenderer * SurfaceHandler::getNewSurfaceRenderer(GLuint id){
	return new Wallpaper(id);
}
