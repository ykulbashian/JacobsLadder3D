
#ifndef IDENTITY_H_
#define IDENTITY_H_

#define NO_SIDE 0x00u
#define LEFT_SIDE 0x01u
#define RIGHT_SIDE 0x02u
#define BOTTOM_SIDE 0x04u
#define TOP_SIDE 0x08u
#define NEAR_SIDE 0x10u
#define FAR_SIDE 0x20u

#define X_DIR 0x03u //(LEFT_SIDE|RIGHT_SIDE)
#define Y_DIR 0x0Cu //(BOTTOM_SIDE|TOP_SIDE)
#define Z_DIR 0x30u //(NEAR_SIDE|FAR_SIDE)
#define ALL_DIR 0x3Fu //(X_DIR|Y_DIR|Z_DIR)

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>

#include "Utils/iterativecollectionmember.h"
#include "Drivers/Grid/gridpositionstate.h"

class Cube;

class GridPosition : public IterativeCollectionMember<GridPosition*>
{

public:


	Cube *cube;
	GridPositionState state;
	GLubyte movingDirection;

	bool isBasicGoalPosition;

	GLubyte pullToWallsDirection;
	GLubyte goalPullToWallsDirection;
	GLubyte pullAgainstWallsDirection;

	GLubyte sidesTouchingStaticCubes;
	GLubyte _drawableStaticBits;

	GridPosition *leftGridPos, *rightGridPos, *upGridPos, *downGridPos, *nearGridPos, *farGridPos;
	GridPosition *pullToWallsGridPos;

	GLint goalNum;

	GridPosition(Cube *d=0, GLint m=0, GridPositionState state=STATE_NONE, GLuint x_=0, GLuint y_=0, GLuint z_=0):
			IterativeCollectionMember<GridPosition*>(), cube(d),
			state(state){
		 x = x_;
		 y = y_;
		 z = z_;
		 movingDirection = 0;
		 sidesTouchingStaticCubes = NO_SIDE;

		 _drawableStaticBits = 0;

		 isBasicGoalPosition = false;

		 pullToWallsDirection = 0;
		 pullAgainstWallsDirection = 0;
		 goalPullToWallsDirection = 0;

		 goalNum = 0;

		 leftGridPos = 0;
		 rightGridPos = 0;
		 upGridPos = 0;
		 downGridPos = 0;
		 nearGridPos = 0;
		 farGridPos = 0;

		 pullToWallsGridPos = 0;
	}

	void setProperties(Cube *cubeRef, GridPositionState gpState, GLubyte movingToDirection);

	bool isNotBlockedInThisDirection(GLubyte dir);
	bool isEmptyOrMovingSpot();
	bool isInUntenablePosition();
	GLubyte sidesInStaticContact();
	bool isSupportedFromWall();

	GLubyte getDrawableFaces();

	void isUndermined();
	bool isEmptyTarget();

	GridPosition *getPositionInWallsDirection();

	void setPullToWallsPosition();

	GridPosition *findPositionInDirection(GLubyte dir);

	bool isInValidState();
};


#endif /* IDENTITY_H_ */
