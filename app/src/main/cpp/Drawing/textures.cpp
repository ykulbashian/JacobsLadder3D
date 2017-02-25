#include "textures.h"
#include "Utils/utils.h"
#include "SurfaceRenderer/surfaceutils.h"

#define COMPONENTS 3
#define MIPMAP_LEVELS 1

#define LOG_TAG "Textures"

//											TOP, RIGHT, BOTTOM, LEFT
unsigned char const Textures::edgeColors[][4] = {	{0xFF, 0xEA, 0xE5, 0xFE}, 		//TOP (TL)
													{0xFF, 0xF4, 0xEC, 0xF2},		//LEFT, RIGHT  (TR)
													{0xFF, 0xF0, 0xF2, 0xF4},		//NEAR (BL)
													{0xEC, 0xF0, 0xFF, 0xFF},	};	//BOTTOM (BR)

unsigned char **Textures::texturePixelData = 0;

Textures::Textures(){
	generateTexture();
}

Textures::~Textures(){
}


unsigned char** Textures::getTextureData()
{
	return texturePixelData;
}


//=========================================================================
//creates the texture map for the cube edges-------------------------------
void Textures::loadTextureRAW() {

	if(!Constants::RENDER_OPENGL)
		return;

	if(!getTextureData()){
		LOG_ERROR(" Tried to load texture that has not been generated!");
		return;
	}

	LOG_INFO_DEBUG("Creating texture: side length %d, components %d", TEXTURE_WIDTH, COMPONENTS);

	//==================================================================
	//Set initial texture parameters------------------------------------
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Set up OpenGL texture properties ---------------------------------
    glEnable(GL_TEXTURE_2D);
	Utils::checkGlError("glEnable");

	// select our current texture
	Utils::checkGlError("starter");
	glGenTextures( 1, &texture[0] );
	Utils::checkGlError("glGenTextures");

//    glActiveTexture(GL_TEXTURE0);
//	Utils::checkGlError("glActiveTexture");
	glBindTexture( GL_TEXTURE_2D, texture[0] );
	Utils::checkGlError("glBindTexture");

	glActiveTexture(GL_TEXTURE0);

	// select modulate to mix texture with color for shading
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	Utils::checkGlError("glTexEnvf");

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	Utils::checkGlError("glGenMipMap");
//	glTexParameteri(GL_TEXTURE_2D, GL_MAX_TEXTURE_UNITS, MIPMAP_LEVELS);
//	Utils::checkGlError("glGenMipMapLevels");

	// when texture area is small, bilinear filter the closest mipmap
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	Utils::checkGlError("glTexParameteri0");

	// when texture area is large, bilinear filter the first mipmap
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	Utils::checkGlError("glTexParameteri1");

	// if wrap is true, the texture wraps over at the edges (repeat)
	//       ... false, the texture ends at the edges (clamp)
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	Utils::checkGlError("glTexParameteri2");
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	Utils::checkGlError("glTexParameteri3");


	GLuint width = TEXTURE_WIDTH*2;

	for(GLubyte mml = 0; mml < MIPMAP_LEVELS; mml++)
	{
		width /= 2;

		if(width < 1) continue;

		if(COMPONENTS == 1){
			glTexImage2D(GL_TEXTURE_2D, mml, GL_LUMINANCE, width, width, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, Textures::getTextureData()[mml]);
		}
		else if(COMPONENTS == 3){
			glTexImage2D(GL_TEXTURE_2D, mml, GL_RGB, width, width, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::getTextureData()[mml]);
		}

	}
	Utils::checkGlError("glTexImage2D");

	LOG_INFO_DEBUG("  Created texture - bound texture: %d", texture[0]);
}


void Textures::generateTexture()
{
	if(texturePixelData){
		LOG_INFO_DEBUG("  Generated texture pixels - size: %10.0f Kb", (MIPMAP_LEVELS*TEXTURE_WIDTH*4*TEXTURE_WIDTH*COMPONENTS*sizeof(unsigned char))/1000.0f);
		return;
	}

	GLuint width = TEXTURE_WIDTH*2, half_width = TEXTURE_HALF_WIDTH*2;

	texturePixelData = new unsigned char*[MIPMAP_LEVELS];

	//create individual texture units-----------------------------------------
	for(GLubyte mml = 0; mml < MIPMAP_LEVELS; mml++)
	{
		width /= 2;
		half_width /= 2;

		if(width < 1) continue;

		GLuint starters[][2] = {{0, 0}, {half_width, 0}, {0, half_width}, {half_width, half_width}, };

		GLuint border = 1, currPos;

		texturePixelData[mml] = new unsigned char[width*width*COMPONENTS];
		unsigned char* dataArray = texturePixelData[mml];

		LOG_INFO_DEBUG("    Generating mipmap: level %u, width %u, half_width %u", mml, width, half_width);

		if(width == 1) dataArray[0] = dataArray[1] = dataArray[2] = 0xFF;

		else {

			GLuint i, j;

			for(GLuint panel = 0; panel < 4; panel++)
			{
				GLuint const* st = starters[panel];
				unsigned char const* ch = edgeColors[panel];

				for(i = st[0]; i < st[0] + half_width; i++)
				{

					for(j = st[1]; j < st[1] + half_width; j++)
					{

						currPos = (COMPONENTS*(i + j*width));

						if(mml >= 4){
							dataArray[currPos] = dataArray[currPos + 1] = dataArray[currPos + 2] = 0xFF;
							continue;
						}

						//====================================
						//edges-------------------------------
						//top/far/right (purple)
						if(j < st[1] + border){
							dataArray[currPos] = ch[0];
							if(COMPONENTS == 3)
								dataArray[currPos+1] = dataArray[currPos+2] = ch[0];
						}
						//near (yellow)
						else if(i >= st[0] + half_width - border){
							dataArray[currPos] = ch[1];
							if(COMPONENTS == 3)
								dataArray[currPos+1] = dataArray[currPos+2] = ch[1];
						}
						//lower/near (cyan)
						else if(j >= st[1] + half_width - border){
							dataArray[currPos] = ch[2];
							if(COMPONENTS == 3)
								dataArray[currPos+2] = dataArray[currPos+1] = ch[2];
						}
						// (red)
						else if(i < st[0] + border){
							dataArray[currPos] = ch[3];
							if(COMPONENTS == 3)
								dataArray[currPos+1] = dataArray[currPos+2] = ch[3];
						}

						//face of panel-------------------------
						else{
							dataArray[currPos] = 0xFF;
							if(COMPONENTS == 3)
								dataArray[currPos+2] = dataArray[currPos+1] = 0xFF;
						}
					}
				}

			}

		}
		LOG_INFO_DEBUG("      Generated texture pixels - size: %10.0f Kb", (MIPMAP_LEVELS*TEXTURE_WIDTH*4*TEXTURE_WIDTH*COMPONENTS*sizeof(unsigned char))/1000.0f);

	}

}

void Textures::unbindTexture()
{
	glDeleteTextures( 1, &texture[0] );
}
