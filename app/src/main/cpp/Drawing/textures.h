
#ifndef TEXTURES_H_
#define TEXTURES_H_


#define TEXTURE_WIDTH 64
#define TEXTURE_HALF_WIDTH 32

#define COMPONENTS 3
#define MIPMAP_LEVELS 1

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>

class Textures
{
private:
	static unsigned char **texturePixelData;
	static unsigned char const edgeColors[][4];


public:
	Textures();
	~Textures();

	static void generateTexture();
	unsigned char** getTextureData();

	void unbindTexture();

	void loadTextureRAW();

private:
	GLuint texture[2];

};
#endif /* TEXTURES_H_ */
