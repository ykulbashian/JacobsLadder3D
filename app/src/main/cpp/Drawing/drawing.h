
#ifndef ENGINE_H_
#define ENGINE_H_

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>
#include "Geometry/cube.h"
#include "Geometry/dimension.h"
#include "Geometry/corner.h"
#include "Drivers/Grid/grid.h"
#include "Utils/iterativecollection.h"
#include "objectbuffers.h"

#define CAMERA_ITERATION 0.1f
#define CAMERA_BACK_OFFSET 0.0f

#define CUBE_SIDE 1.0f

#define ZOOM_FACTOR 1.1f

#define NEAR_FIELD 1.0f

class Drawing {

public:
	Drawing();
	~Drawing();

	//Animation===============================================
	void drawCubes(GLfloat fractionOfPreferredFrame,
			GLfloat angleOfRotation,
			Grid *grid,
			IterativeCollection<Cube*> *movingElements,
		    Goals* goalSet);

	GLuint cameraMovementShift;

private:

	void drawBackgroundSegment(GLfloat angleOfRotation, GLuint slice);
	void drawMovingCube(GLfloat angleOfRotation,Cube* c);
	GLuint printStaticCube(GLubyte faces, GLushort currentIndexPos, GLuint pointStart);

	GLfloat cameraMovement;

	static GLushort backgroundIdsLeft[];
	static GLushort backgroundIdsBottom[];
	static GLushort backgroundIdsRight[];
	static GLushort backgroundIdsTop[];


	void drawSingleCube(GLfloat angleOfRotation, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLuint x, GLuint y, GLuint z);

	ObjectBuffers *getBuffer();


	void drawGoalSets(GLfloat angleOfRotation, GoalObject* goal);

	void renderGoalBoxes(GLfloat angleOfRotation, Goals *goalSet);
};

#endif /* ENGINE_H_ */

