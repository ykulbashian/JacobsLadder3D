#ifndef CUBEDRIVER_H_
#define CUBEDRIVER_H_

#include "Geometry/cube.h"
#include "Geometry/dimension.h"
#include "Drivers/Grid/grid.h"
#include "Utils/alliterativecollections.h"
#include "Stats/aggregatestats.h"
#include "Stats/driverstats.h"

#define MOVE_SELECTED_INCREMENT 10
#define CHECK_UNSUPPORTED_INCREMENT 10

#define MAX_RANDOM_MOVE_PER_FRAME 5

#define FRACTION_OF_SELECTION_TO_PRIORITY 0.1f

#define MAX_MOVING_AT_A_TIME 200u


enum TunnelWall
{
	RIGHT_WALL = 0,
	LEFT_WALL = 1,
	TOP_WALL = 2,
	BOTTOM_WALL = 3,
};



class CubeDriver : public Dimension{

public:

	enum TargetGridType
	{
		TARGET_NONE,
		TARGET_RANDOM,
		TARGET_Z_WALK,
	};

	Grid _grid;
	Goals goalSet;

	AllIterativeCollections allArrayCollections;

	bool shouldRefresh;

	CubeDriver();
	~CubeDriver();

	void setToNewColors();

	void resetIfNecessary();

	/* Instantiation methods */
	bool initializeDimensions(Dimension dimens);

	bool isDriverInitialized();

	/* Iteration methods */
	void iterateOneFrame(GLuint fractionOfPreferredFrame);

	/* touch event */
	void onTouchEvent(GLint x, GLint y, GLfloat xConst, GLfloat yConst, GLuint cameraMovementShift, GLboolean isSingleTap);


	/* TEST METHODS */
	AggregateStats getAggregateStats();

private:

	TargetGridType currentTargetType;

	GLuint numCubesMovedToSelection;
	GLuint unableToGetCandidate;
	bool failedToGetToPriority;
	GLuint priorityCubeCouldNotMove;
	GLuint selectionWasInGoals;
	GLuint numInUntenablePosition;

	bool isInitialized;

	void deleteACube(Cube *c);

	void setInitialCubePositions();
	bool setCubeRandomPos(Cube *c);

	void getDriverStats(DriverStats &stats);

	/* Instantiation methods */
	void revertDriver();

	bool tryToPlaceCubeInGrid(Cube *c, Position *pos);

	void initializeGoalValues();

	/* touch event */
	bool setSurroundFromTouch(GridPosition *touchedGridPosition);

	/* Iteration methods */
	void iterateMovingCubes(GLuint fractionOfPreferredFrame);

	void moveSelectionToPriority();
	bool startPriorityCubesMoving();

	void moveNewCandidatesToSelected();

	void stopCubeMoving(Cube *c);

	bool tryToGetNextMove(Cube *c);

	Cube *getCubeFurthestFromWall(Cube *c);
};




#endif /* CUBEDRIVER_H_ */
