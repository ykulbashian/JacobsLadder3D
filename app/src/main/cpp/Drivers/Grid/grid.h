#ifndef GRID_H
#define GRID_H

#include "Utils/utils.h"
#include "SurfaceRenderer/surfaceutils.h"

#include "Geometry/cube.h"
#include "Geometry/dimension.h"
#include "Stats/gridstats.h"
#include "Drivers/Goals/goals.h"
#include <math.h>
#include <stdlib.h>

#define MAX_MOVE_TRIES 4


class Grid : public Dimension
{
private:

	GLuint X_BLOCKS_MINUS_1, Y_BLOCKS_MINUS_1, Z_BLOCKS_MINUS_1;
	GLfloat X_BLOCKS_MID, Y_BLOCKS_MID;

	GLuint numFloatingUnableToMove;
	GLuint numFloatingUnableToMoveInRepel;
	GLuint numUnableToMove;
	GLuint amountOfFrustration;
	GLuint totalMoveAttempts;
	GLuint unableToGiveTarget;
	GLuint newlyStartedToMove;
	GLuint cubeReversedOnItself;
	GLuint settingNewTarget;
	GLuint foundAdjacentTarget;
	GLuint numRandomizerFailed;
	GLuint cantMoveInAnyDirection;
	GLuint shouldntMoveInAnyDirection;

	GLuint newDirectionLeft;
	GLuint newDirectionRight;
	GLuint newDirectionTop;
	GLuint newDirectionBottom;
	GLuint newDirectionNear;
	GLuint newDirectionFar;


	static GLubyte X_NEXT[];
	static GLubyte Y_NEXT[];
	static GLubyte Z_NEXT[];


	/* Instantiation methods */
	void deleteGridPositionArray();

	void setupGridPositionLinks();


	/* Utils */
	void setAdjacentCubesDrawableBitsOnEmpty(GridPosition *emptyPosition);

	void setDrawableSidesForSelfAndAdjacentLanded(GridPosition *thisPosition);

	void setAdjacentGridPosRefs(GridPosition *toPosition, GLuint x, GLuint y, GLuint z);

	/* Goal behavior */
	bool tryToSetTarget(Goals * goalSet, Cube *c);
	bool isAcceptableTarget(GridPosition *toPosition, GridPosition *target);

	GLubyte getBestDirectionForCube(Cube *c, bool isContinuing);

	GridPosition *findAdjacentTarget(GridPosition *toPosition);

	void resetStatsVariables();

public:
	GridPosition ****_gridPositionsArray;

	bool *_sliceIsDirty;

	Grid();
	~Grid();

	/* Instantiation methods */
	void createGridPositionArray();

	void resetGridAndGoalArrays();
	void resetGridToDimensions(Dimension dimens);

	bool deleteACube(Cube *c);
	bool placeACube(Cube *c, Position *pos);

	bool isReady();

	void resetSlicesToDirty();


	/* Cube behavior */
	GridPosition *getGridElement(GLuint x, GLuint y, GLuint z);

	void cubeHasStoppedMoving(GridPosition *landedPosition);

	void setACubeAsMoving(Cube *c, GLubyte dir);

	GLubyte setAbleToMoveToBits(Cube *c);

	/* Goal behavior */
	void setGoalsAtEdges();

	GLubyte findAndSetNewSuggestedDirection(Goals * goalSet, Cube *c, bool isContinuing);

	bool shouldKeepMoving(GridPosition *position);
	bool isGoodCandidateToMove(Cube *c);

	bool isInGoals(GridPosition *gridPosition);

	void getGridStats(GridStats &stats);

};


#endif /* GRID_H */
