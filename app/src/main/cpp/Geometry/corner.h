
#ifndef CORNER_H_
#define CORNER_H_

#include <Drawing/ColorSets/utils.h>

struct Vertex {
	GLfloat x, y, z;
};

struct Texture {
	GLfloat u, v, t;
};

struct corner {
	Vertex vertex;
	Texture texture;
	color_array color[4];
};



#endif /* CORNER_H_ */
