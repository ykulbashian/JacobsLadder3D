#ifndef CUBEPOS_H_
#define CUBEPOS_H_

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>

#include "SurfaceRenderer/logger.h"
#include "Utils/iterativecollectionmember.h"
#include "Drivers/Grid/gridposition.h"

class Cube : public IterativeCollectionMember<Cube*>{


public:

	Cube();
	~Cube();

	GLuint stepMovedFragment;
	GLubyte moveDirection;

	GLuint frustration;
	bool wasTouched;

	GLubyte _shouldMoveToBitsInContact;
	GLubyte _canMoveToBits;

	GLubyte lastMovedDirection;
	GLubyte priorMovedDirection;

	GLint 	diff_destx,
			diff_desty,
			diff_destz;


	/* STATE METHODS */

	void placeCubeAt(GridPosition *position);
	GridPosition *updateVariablesAfterMoving();

	bool incrementMovement(GLuint increment);

	bool shouldMoveInThisDirection(GLubyte dir);
	void setNewDirectionVariables(GLubyte dir);
	void stopMoving();

	void giveTargetGridPosition(GridPosition *target, GLuint MAX_Z);

	GridPosition *getFailedTarget();


	/* GET PROPERTIES */

	GridPosition *getGridElement();

	GLboolean isMoving();

	bool shouldMoveToInContact(GLubyte dir);
	bool canMoveInThisDirectionInAPinch(GLubyte dir);

	bool isJustStartingToMove();

	void isUndermined();

	GridPosition *getNextPosition();

	static void sortDistancesHighestToLowest(Cube *c, GLubyte (&sorted)[3]);

	bool isMoveStateValid();

	bool isInGoal();

private:

	void resetVariables();

	GLuint currentJourneyLength;

	GridPosition *nextPosition;
	GridPosition *targetPosition;

	GridPosition *currPosition;

};



#endif /* CUBEPOS_H_ */
