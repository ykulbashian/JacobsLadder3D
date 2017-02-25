#include "SurfaceRenderer/surfacehandler.h"
#include "wallpaper.h"

SurfaceRenderer * SurfaceHandler::getNewSurfaceRenderer(GLuint ident){
	return new Wallpaper(ident);
}
