
#include "Drawing/ColorSets/colorset.h"
#include <math.h>

#define LOG_TAG "Face Color"

void ColorSet::setBackgroundColorInThread()
{
	LOG_INFO_DEBUG("     Setting clear color...");

	color_array &background = ColorSet::colorSetStruct.backgroundColor;

	if(background)
		glClearColor(	background[0]/255.0f,
						background[1]/255.0f,
						background[2]/255.0f,
						background[3]/255.0f);

	LOG_INFO_DEBUG("     Clear color set.");
}


void ColorSet::buildFaceColors(GLuint xBlocks, GLuint yBlocks, GLuint zBlocks)
{

#ifdef DEBUG_MODE_TIMER
	Timer::startTimer();
#endif

	LOG_INFO_DEBUG("Building face colors... x %d  y %d  z %d, %s", xBlocks, yBlocks, zBlocks, "custom");

	color_array& backgroundColor = ColorSet::colorSetStruct.backgroundColor;

	color_array (*selectCorners)[4][NUM_FACES] = &(ColorSet::colorSetStruct.cornerColors);

	deleteAllArrays();

	X_BLOCKS_PLUS_1 = ++xBlocks;
	Y_BLOCKS_PLUS_1 = ++yBlocks;
	Z_BLOCKS_PLUS_1 = ++zBlocks;

	color_array faceColors[X_BLOCKS_PLUS_1][Y_BLOCKS_PLUS_1][NUM_FACES];

	createDistancePaintsArray();

#ifdef DEBUG_MODE_TIMER
	END_TIMER(SurfaceTimer::BUILD_COLORS_INIT);
	Timer::startTimer();
#endif


	//X lines=======================================================================
	for(GLuint x = 0; x < X_BLOCKS_PLUS_1; x++) {

		GLfloat fractionX = (GLfloat)x/(GLfloat)X_BLOCKS_PLUS_1;

		for(int face = 0; face < NUM_FACES; face++) {
			mergeColors(faceColors[x][Y_BLOCKS_PLUS_1 - 1][face],
						(*selectCorners)[TOP_RIGHT][face],
						(*selectCorners)[TOP_LEFT][face], fractionX);

			mergeColors(faceColors[x][0][face],
						(*selectCorners)[BOTTOM_RIGHT][face],
						(*selectCorners)[BOTTOM_LEFT][face], fractionX);
		}

	}


	//create columns===============================================================
	for(GLuint x = 0; x < X_BLOCKS_PLUS_1; x++)
	{
		for(GLuint y = 0; y < Y_BLOCKS_PLUS_1; y++)
		{
			GLfloat fractionY = (GLfloat)y/(GLfloat)Y_BLOCKS_PLUS_1;

			for(GLuint face = 0; face < NUM_FACES; face++)
				mergeColors(faceColors[x][y][face], faceColors[x][Y_BLOCKS_PLUS_1-1][face], faceColors[x][0][face], fractionY);
		}
	}

	LOG_INFO_DEBUG("  Creating final cube color array");


	//set paint color for Z positions=============================================
	GLfloat x_dist_increment = 1.0f/(GLfloat)X_BLOCKS_PLUS_1,
			y_dist_increment = 1.0f/(GLfloat)Y_BLOCKS_PLUS_1,
			z_dist_increment = 1.0f/(GLfloat)Z_BLOCKS_PLUS_1;

	GLfloat x_block_fraction = 0, y_block_fraction = 0, z_block_fraction = 0;

	color_array baseFaceColors = {0, 0, 0, 0};

	for(GLuint x = 0; x < X_BLOCKS_PLUS_1; x++)
	{
		x_block_fraction = x*x_dist_increment;

		for(GLuint y = 0; y < Y_BLOCKS_PLUS_1; y++)
		{
			y_block_fraction = y*y_dist_increment;

			for(GLuint z = 0; z < Z_BLOCKS_PLUS_1; z++)
			{
				z_block_fraction = z*z_dist_increment;

				GLfloat percent = ((GLfloat)z/(GLfloat)Z_BLOCKS_PLUS_1) - 0.5f;
				GLfloat fractionZ = 1.0f - (percent > 0 ? (percent)*2 : 0);

				for(GLubyte face = 0; face < NUM_FACES; face++)
				{
					baseFaceColors[0] = faceColors[x][y][face][0];
					baseFaceColors[1] = faceColors[x][y][face][1];
					baseFaceColors[2] = faceColors[x][y][face][2];

					applyLightsToFace(x_block_fraction,
									  y_block_fraction,
									  z_block_fraction,
									  baseFaceColors,
									  face);

					mergeColors(coloredVertices[x][y][z][face], baseFaceColors, backgroundColor, fractionZ);
				}
			}
		}
	}

	LOG_INFO_DEBUG("  Finished creating color bands in coloredVertices (%d x, %d y, %d z)", X_BLOCKS_PLUS_1, Y_BLOCKS_PLUS_1, Z_BLOCKS_PLUS_1);


#ifdef DEBUG_MODE_TIMER
	END_TIMER(SurfaceTimer::BUILD_COLORS_DERIVE);
#endif

}

GLuint ColorSet::applyLightsToFace(GLfloat x_block_fraction,
								   GLfloat y_block_fraction,
								   GLfloat z_block_fraction,
								   GLuint baseFaceColor,
								   GLubyte face) {
	color_array bytes = {0, 0, 0, 0};
	getColorBytesFromInt(baseFaceColor, bytes);

	GLubyte* adjusted = applyLightsToFace(x_block_fraction, y_block_fraction, z_block_fraction,
										  bytes, face);

	return getColorIntFromByteArray(adjusted);
}

GLubyte* ColorSet::applyLightsToFace(GLfloat x_block_fraction,
									 GLfloat y_block_fraction,
									 GLfloat z_block_fraction,
									 color_array baseFaceColor,
									 GLubyte face) {

	Light *lights = ColorSet::colorSetStruct.lights;

	for(GLuint i = 0; i < NUM_LIGHTS; i++) {
		const Light *light = &(lights[i]);

		double normalizedDistanceToCube =
				(pow(((*light).x_pos_fraction - x_block_fraction) * STANDARD_WIDTH, 2) +
				 pow(((*light).y_pos_fraction - y_block_fraction) * STANDARD_WIDTH, 2) +
				 pow(((*light).z_pos_fraction - z_block_fraction) * STANDARD_DEPTH, 2));


		double lightDropOff = BRIGHTNESS_CAP - normalizedDistanceToCube / ((*light).maxReach);

		if (lightDropOff > 0) {
			GLfloat orthogonalDiff = 0;

			switch (face) {
				case NEAR_FACE:
					orthogonalDiff = z_block_fraction - (*light).z_pos_fraction;
					orthogonalDiff *= STANDARD_DEPTH;
					break;
				case RIGHT_FACE:
					orthogonalDiff = (*light).x_pos_fraction - x_block_fraction;
					orthogonalDiff *= STANDARD_WIDTH;
					break;
				case LEFT_FACE:
					orthogonalDiff = x_block_fraction - (*light).x_pos_fraction;
					orthogonalDiff *= STANDARD_WIDTH;
					break;
				case TOP_FACE:
					orthogonalDiff = (*light).y_pos_fraction - y_block_fraction;
					orthogonalDiff *= STANDARD_WIDTH;
					break;
				case BOTTOM_FACE:
					orthogonalDiff = y_block_fraction - (*light).y_pos_fraction;
					orthogonalDiff *= STANDARD_WIDTH;
					break;
				default:
					break;
			}

			if (orthogonalDiff > OPTIMAL_DISTANCE)
				orthogonalDiff = OPTIMAL_DISTANCE;

			if (orthogonalDiff > 0) {
				double dropOffFraction = lightDropOff * (orthogonalDiff / OPTIMAL_DISTANCE);

				baseFaceColor[0] += ((*light).colors[0] - baseFaceColor[0]) * dropOffFraction;
				baseFaceColor[1] += ((*light).colors[1] - baseFaceColor[1]) * dropOffFraction;
				baseFaceColor[2] += ((*light).colors[2] - baseFaceColor[2]) * dropOffFraction;
			}
		}
	}

	return baseFaceColor;
}


void ColorSet::mergeColors(GLubyte in[4], color_array (&color1), color_array (&color2), GLfloat fraction1)
{
	GLfloat fraction2 = 1.0f-fraction1;

	in[0] = (GLubyte)(color1[0]*fraction1 + color2[0]*fraction2);
	in[1] = (GLubyte)(color1[1]*fraction1 + color2[1]*fraction2);
	in[2] = (GLubyte)(color1[2]*fraction1 + color2[2]*fraction2);
	in[3] = 255;
}


void ColorSet::createDistancePaintsArray()
{
	LOG_INFO_DEBUG("   Creating Distance arrays...");

	coloredVertices = new color_array***[X_BLOCKS_PLUS_1];

	for(GLuint x = 0; x < Y_BLOCKS_PLUS_1; x++){
		coloredVertices[x] = new color_array**[Y_BLOCKS_PLUS_1];

		for(GLuint y = 0; y < Y_BLOCKS_PLUS_1; y++){
			coloredVertices[x][y] = new color_array*[Z_BLOCKS_PLUS_1];

			for(GLuint z = 0; z < Z_BLOCKS_PLUS_1; z++){
				coloredVertices[x][y][z] = new color_array[NUM_FACES];
			}
		}
	}

	LOG_INFO_DEBUG("   DistancePaints arrays created. Size: %5.0f Kb",
			(X_BLOCKS_PLUS_1*Y_BLOCKS_PLUS_1*Z_BLOCKS_PLUS_1*NUM_FACES*sizeof(color_array))/1000.0f);
}

void ColorSet::deleteAllArrays()
{
	LOG_INFO_DEBUG("   Deleting Current color set...");

	if(coloredVertices){

		LOG_INFO_DEBUG("   Deleting Distance Paints... %p", coloredVertices);

		for(GLuint x = 0; x < X_BLOCKS_PLUS_1; x++){

			for(GLuint y = 0; y < Y_BLOCKS_PLUS_1; y++){

				for(GLuint z = 0; z < Z_BLOCKS_PLUS_1; z++){

					delete [] coloredVertices[x][y][z];
				}
				delete [] coloredVertices[x][y];
			}
			delete [] coloredVertices[x];
		}

		delete [] coloredVertices;

		coloredVertices = 0;

		LOG_INFO_DEBUG("   Deleted Distance Paints.");
	}

	LOG_INFO_DEBUG("   Current color set deleted.");

}
