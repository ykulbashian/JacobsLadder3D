#include "Drivers/cubedriver.h"

#define LOG_TAG "CubeDriver create"


CubeDriver::CubeDriver() : Dimension()
{
	LOG_INFO_DEBUG("    Creating Driver");

	shouldRefresh = true;

	numCubesMovedToSelection = 0;
	unableToGetCandidate = 0;
	priorityCubeCouldNotMove = 0;
	selectionWasInGoals = 0;
	failedToGetToPriority = false;
	isInitialized = false;

	revertDriver();

	LOG_INFO_DEBUG("  Finished setting up cube driver");
}

CubeDriver::~CubeDriver()
{
}

void CubeDriver::resetIfNecessary()
{
	if(shouldRefresh){
		setToNewColors();
		shouldRefresh = false;
	}
}

void CubeDriver::setToNewColors(){
	_grid.resetSlicesToDirty();
}

bool CubeDriver::isDriverInitialized(){
	return isInitialized;
}


bool CubeDriver::initializeDimensions(Dimension dimens)
{
	if(isSimilarDimensions(dimens) && isInitialized)
		return false;

	isInitialized = false;

	revertDriver();

	LOG_INFO_DEBUG("  Setting blocks in Cube Driver...");

	setDimension(dimens);

	_grid.resetGridToDimensions(dimens);

	LOG_INFO_DEBUG("       Setting Goal Grids to dimensions (%d, %d, %d) and type %d ...", X_BLOCKS, Y_BLOCKS, Z_BLOCKS, GOALS_EDGE_WALK);

	goalSet.createNewGoalsGrid(GOALS_EDGE_WALK, dimens);

	initializeGoalValues();

	setInitialCubePositions();

	isInitialized = true;

	LOG_INFO_DEBUG("      Finished setting up goal grids.");

	LOG_INFO_DEBUG("    Finished setting GridPosition and Goal Grids.");

	LOG_INFO_DEBUG("  Finished setting blocks in Cube Driver.");

	return true;
}

void CubeDriver::initializeGoalValues()
{

	LOG_INFO_DEBUG("  Initializing Goals to type: %d", goalSet.currentGoalType);

	switch (goalSet.currentGoalType){
	case GOALS_EDGE_WALK:
		_grid.setGoalsAtEdges();
		break;
	case GOALS_NONE:
		break;
	}

	goalSet.createNewGoalObjects(_grid._gridPositionsArray);

	LOG_INFO_DEBUG("  Finished setting goal grid values.");
}


void CubeDriver::revertDriver()
{
	LOG_INFO_DEBUG("    ---Reverting driver...");

	allArrayCollections.revertArrays();

	_grid.resetGridAndGoalArrays();

	currentTargetType = TARGET_NONE;

	LOG_INFO_DEBUG("     ---Finished reverting driver.");
}


void CubeDriver::setInitialCubePositions()
{
	START_TIMER

	LOG_INFO_DEBUG("    Initial cube positions. Initial number: %d (%d x blocks, %d y blocks, %d z blocks)",
			TOTAL_BLOCKS, X_BLOCKS, Y_BLOCKS, Z_BLOCKS);

	//place initial cubes set--------------------------------------
	for (GLuint i = 0; i < TOTAL_BLOCKS; i++)
	{
		Cube *c = new Cube();

		if(!setCubeRandomPos(c))
			delete c;
	}

	END_TIMER(SurfaceTimer::SET_CUBE_POSITIONS)
}



//Give a random position to a cube on a given wall-------------------------------------
bool CubeDriver::setCubeRandomPos(Cube *c){

	// 0:left, 1:right, 2:top, 3:bottom
	int currentWall = (RANDOM(4));

	Position *pos = new Position();
	GLuint i = 0;

	do {
		i++;
		switch(currentWall)
		{
			case RIGHT_WALL:
				pos->x = X_BLOCKS - 1;
				pos->y = (GLuint) (RANDOM(Y_BLOCKS));
				break;
			case LEFT_WALL:
				pos->x = 0;
				pos->y = (GLuint) (RANDOM(Y_BLOCKS));
				break;
			case TOP_WALL:
				pos->x = (GLuint) (RANDOM(X_BLOCKS));
				pos->y = Y_BLOCKS - 1;
				break;
			case BOTTOM_WALL:
				pos->x = (GLuint) (RANDOM(X_BLOCKS));
				pos->y = 0;
				break;
			default:
				//TODO ?
				break;
		}

		pos->z = (GLuint)(RANDOM(Z_BLOCKS));

		if(tryToPlaceCubeInGrid(c, pos))
			return true;

	}
	while (i < 20);

	delete pos;

	return false;
}



