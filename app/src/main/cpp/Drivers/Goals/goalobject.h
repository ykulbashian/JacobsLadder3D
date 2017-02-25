
#ifndef GOALOBJECT_H_
#define GOALOBJECT_H_

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>

#include "Utils/utils.h"
#include "Drivers/Grid/gridposition.h"
#include "Utils/iterativecollection.h"
#include "Utils/alliterativecollections.h"
#include "Stats/goalstats.h"
#include "Drivers/Goals/goalpointtype.h"
#include <stdlib.h>
#include <climits>

class GoalObject : public IterativeCollectionMember<GoalObject*> {

public:

	unsigned int id;

	bool isViolated;

	GLuint x_start, y_start, z_start;
	GLuint x_end, y_end, z_end;
	GoalPointType goalType;

	GoalObject(GoalPointType type, GLuint x_blocks, GLuint y_blocks, GLuint z_blocks);

	virtual ~GoalObject();

	GoalPointType getGoalType();

	virtual bool containsPoint(GridPosition *pos);
	virtual bool populateGrid(GridPosition ****grid);
	virtual void removeFromGrid(AllIterativeCollections *allCollections,
								ReasonForDestruction reason);

	virtual GridPosition *getRandomTarget();

	virtual bool isUpForDestruction() = 0;

	virtual GLint distanceToCenterPoint(GridPosition *position);

	virtual void getGoalStats(GoalStats &stats);

	void removeTarget(GridPosition *target, AllIterativeCollections *allCollections, ReasonForDestruction reason);

	bool targetWasOccupied(GridPosition *occupiedTarget);

	bool targetWasFailed(GridPosition *failedTarget, unsigned int frustration);

	GLuint totalTargetNumber;
	GLuint failedTargets;

	GLuint numberOfRequests;

	long getPendingRequests();

	bool isGrounded();

	ReasonForDestruction getReasonForDestruction(unsigned int numSelected);

	IterativeCollection<GridPosition*> *availableTargets;
	IterativeCollection<GridPosition*> *filledTargets;

	void allowAsTarget(GridPosition *target);

	GLuint getNumAvailable();

private:

	GLuint X_BLOCKS, Y_BLOCKS, Z_BLOCKS;
	GLfloat requestToTargetRatio;

	GLfloat filledMovingAverage;

	GLuint cumulativeFrustration;

	GLint min(GLint val1, GLint val2);
	GLfloat absf(GLfloat val);

protected:

	GLubyte groundDirection;

	GLuint expectedTargetCountInCube;

};



#endif /* GOALOBJECT_H_ */
