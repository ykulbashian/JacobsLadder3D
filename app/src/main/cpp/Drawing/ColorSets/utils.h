
#ifndef COLORSETS_UTILS_H
#define COLORSETS_UTILS_H

typedef GLubyte color_array[4];

static void getColorBytesFromInt(GLuint color, color_array (&outColor))
{
    outColor[0] = (GLubyte)((color >> 16) & 0xFF);
    outColor[1] = (GLubyte)((color >> 8) & 0xFF);
    outColor[2] = (GLubyte)(color & 0xFF);
    outColor[3] = 255;
}

static void getColorByteArrayFromInt(GLuint color, bool opaque, color_array (&outColor))
{
    outColor[0] = (GLubyte)((color >> 16) & 0xFF);
    outColor[1] = (GLubyte)((color >> 8) & 0xFF);
    outColor[2] = (GLubyte)(color & 0xFF);
    outColor[3] = (GLubyte)(opaque ? 255 : ((color >> 24) & 0xFF));

}

static GLuint getColorIntFromByteArray(color_array outColor)
{
    return (outColor[3] << 24) | (outColor[0] << 16) | (outColor[1] << 8) | (outColor[2]);
}

#endif //COLORSETS_UTILS_H
