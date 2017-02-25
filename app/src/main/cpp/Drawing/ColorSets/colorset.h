
#ifndef COLORSET_H_
#define COLORSET_H_

#include "light.h"
#include "Utils/utils.h"

#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_LEFT 2
#define BOTTOM_RIGHT 3

#define NUM_CORNERS 4

#define LEFT_FACE 0
#define RIGHT_FACE 1
#define TOP_FACE 2
#define BOTTOM_FACE 3
#define NEAR_FACE 4

#define NUM_FACES 5

#define BRIGHTNESS_CAP 0.7f

#define OPTIMAL_DISTANCE 8.0f

#define	STANDARD_WIDTH 22
#define	STANDARD_DEPTH 48

#define NUM_LIGHTS 7

enum ColorSetPositions
{
	CP_BACKGROUND = 0,

	CP_LEFT_FACE_BL,
	CP_RIGHT_FACE_BL,
	CP_TOP_FACE_BL,
	CP_BOTTOM_FACE_BL,
	CP_NEAR_FACE_BL,

	CP_LEFT_FACE_TL,
	CP_RIGHT_FACE_TL,
	CP_TOP_FACE_TL,
	CP_BOTTOM_FACE_TL,
	CP_NEAR_FACE_TL,

	CP_LEFT_FACE_TR,
	CP_RIGHT_FACE_TR,
	CP_TOP_FACE_TR,
	CP_BOTTOM_FACE_TR,
	CP_NEAR_FACE_TR,

	CP_LEFT_FACE_BR,
	CP_RIGHT_FACE_BR,
	CP_TOP_FACE_BR,
	CP_BOTTOM_FACE_BR,
	CP_NEAR_FACE_BR,

	CP_BOTTOM_LEFT,
	CP_TOP_RIGHT,
	CP_TOP_LEFT,
	CP_BOTTOM_RIGHT,

	CP_PRIMARY_LIGHT,
	CP_DISTANT_LIGHT,
	CP_MID_LIGHT,

	CP_HAS_BACKGROUND,

};

#define LENGTH_OF_CUSTOM_ARRAY 29

struct ColorSetStruct
{
	color_array backgroundColor;
	color_array cornerColors[NUM_CORNERS][NUM_FACES];
	Light lights[7];
	bool hasBackground;

};

class ColorSet
{
public:

	static ColorSet *getCurrentColorSet();

	color_array **** coloredVertices;

	bool colorHasRecentlyChanged;

	void buildFaceColors(GLuint xBlocks, GLuint yBlocks, GLuint zBlocks);

	void deleteAllArrays();

	void setBackgroundColorInThread();

	GLuint* updateCurrentColorSet(const GLint *customColors = 0);

	ColorSet();
	~ColorSet();

	GLuint* setColorArrays(const GLuint newColors[LENGTH_OF_CUSTOM_ARRAY]);

	bool currentSetHasBackground();

	void clearUpdateFlag();


	GLuint *getApparentColors();

private:
	static ColorSet currentColorSet;
	static ColorSetStruct colorSetStruct;
	GLuint apparentColors[LENGTH_OF_CUSTOM_ARRAY];

	GLuint X_BLOCKS_PLUS_1, Y_BLOCKS_PLUS_1, Z_BLOCKS_PLUS_1;

	GLuint priorColors[LENGTH_OF_CUSTOM_ARRAY];

	void mergeColors(GLubyte in[4], color_array (&color1), color_array (&color2), GLfloat fraction1);

	void createDistancePaintsArray();

	void setCustomLightList(const GLuint newColors[LENGTH_OF_CUSTOM_ARRAY]);
	GLuint* setCustomCornerList(const GLuint newColors[LENGTH_OF_CUSTOM_ARRAY]);

	void updateBackground(const GLuint newColors[LENGTH_OF_CUSTOM_ARRAY]);

	bool checkRecentChanges(const GLuint *newColors);

	GLubyte* applyLightsToFace(GLfloat x_block_fraction,
							   GLfloat y_block_fraction,
							   GLfloat z_block_fraction,
							   color_array baseFaceColor, GLubyte face);

	GLuint applyLightsToFace(GLfloat x_block_fraction,
							 GLfloat y_block_fraction,
							 GLfloat z_block_fraction,
							 GLuint baseFaceColor, GLubyte face);

	GLuint* applyLightsToFaces(const GLuint newColors[LENGTH_OF_CUSTOM_ARRAY]);
};


#endif /* COLORSET_H_ */
