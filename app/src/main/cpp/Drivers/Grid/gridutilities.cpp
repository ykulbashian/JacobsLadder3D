#include "Drivers/Grid/grid.h"

#define LOG_TAG "Grid Utilities"


GLubyte Grid::setAbleToMoveToBits(Cube *c)
{
	GLuint _x = c->x;
	GLuint _y = c->y;

	GridPosition *adjacentPos;
	GridPosition *cubePosition = c->getGridElement();
	GLubyte shouldMoveToInContact = 0x00;
	GLubyte canMoveTo = 0x00;
	GLubyte sidesCurrentlyInContact = cubePosition->sidesInStaticContact();

	if(_x > 0){
		adjacentPos = cubePosition->leftGridPos;
		if(adjacentPos->isNotBlockedInThisDirection(LEFT_SIDE)){
			canMoveTo |= LEFT_SIDE;
			if ((sidesCurrentlyInContact || adjacentPos->sidesInStaticContact())
					&& !Utils::areOppositeDirections(sidesCurrentlyInContact, LEFT_SIDE))
				shouldMoveToInContact |= LEFT_SIDE;
		}
	}

	if(_x < X_BLOCKS_MINUS_1){
		adjacentPos = cubePosition->rightGridPos;
		if(adjacentPos->isNotBlockedInThisDirection(RIGHT_SIDE)){
			canMoveTo |= RIGHT_SIDE;
			if ((sidesCurrentlyInContact || adjacentPos->sidesInStaticContact())
				&& !Utils::areOppositeDirections(sidesCurrentlyInContact, RIGHT_SIDE))
				shouldMoveToInContact |= RIGHT_SIDE;
		}
	}

	if(_y > 0){
		adjacentPos = cubePosition->downGridPos;
		if(adjacentPos->isNotBlockedInThisDirection(BOTTOM_SIDE)){
			canMoveTo |= BOTTOM_SIDE;
			if ((sidesCurrentlyInContact || adjacentPos->sidesInStaticContact())
				&& !Utils::areOppositeDirections(sidesCurrentlyInContact, BOTTOM_SIDE))
				shouldMoveToInContact |= BOTTOM_SIDE;
		}
	}

	if(_y < Y_BLOCKS_MINUS_1){
		adjacentPos = cubePosition->upGridPos;
		if(adjacentPos->isNotBlockedInThisDirection(TOP_SIDE)){
			canMoveTo |= TOP_SIDE;
			if ((sidesCurrentlyInContact || adjacentPos->sidesInStaticContact())
				&& !Utils::areOppositeDirections(sidesCurrentlyInContact, TOP_SIDE))
				shouldMoveToInContact |= TOP_SIDE;
		}
	}

	adjacentPos = cubePosition->nearGridPos;
	if(adjacentPos->isNotBlockedInThisDirection(NEAR_SIDE)){
		canMoveTo |= NEAR_SIDE;
		if ((sidesCurrentlyInContact || adjacentPos->sidesInStaticContact())
			&& !Utils::areOppositeDirections(sidesCurrentlyInContact, NEAR_SIDE))
			shouldMoveToInContact |= NEAR_SIDE;
	}

	adjacentPos = cubePosition->farGridPos;
	if(adjacentPos->isNotBlockedInThisDirection(FAR_SIDE)){
		canMoveTo |= FAR_SIDE;
		if ((sidesCurrentlyInContact || adjacentPos->sidesInStaticContact())
			&& !Utils::areOppositeDirections(sidesCurrentlyInContact, FAR_SIDE))
			shouldMoveToInContact |= FAR_SIDE;
	}

	c->_shouldMoveToBitsInContact = shouldMoveToInContact;
	c->_canMoveToBits = canMoveTo;

	return shouldMoveToInContact;
}


void Grid::setAdjacentCubesDrawableBitsOnEmpty(GridPosition *emptyPosition)
{
	GLuint empty_x = emptyPosition->x;
	GLuint _y = emptyPosition->y;

	if(empty_x > 0){
		emptyPosition->leftGridPos->sidesTouchingStaticCubes &= ~RIGHT_SIDE;
		if(empty_x < X_BLOCKS_MID)
			emptyPosition->leftGridPos->_drawableStaticBits |= RIGHT_SIDE;
	}

	if(empty_x < X_BLOCKS_MINUS_1){
		emptyPosition->rightGridPos->sidesTouchingStaticCubes &= ~LEFT_SIDE;
		if(empty_x >= X_BLOCKS_MID)
			emptyPosition->rightGridPos->_drawableStaticBits |= LEFT_SIDE;
	}

	if(_y > 0){
		emptyPosition->downGridPos->sidesTouchingStaticCubes &= ~TOP_SIDE;
		if(_y < Y_BLOCKS_MID)
			emptyPosition->downGridPos->_drawableStaticBits |= TOP_SIDE;
	}

	if(_y < Y_BLOCKS_MINUS_1){
		emptyPosition->upGridPos->sidesTouchingStaticCubes &= ~BOTTOM_SIDE;
		if(_y >= Y_BLOCKS_MID)
			emptyPosition->upGridPos->_drawableStaticBits |= BOTTOM_SIDE;
	}

	emptyPosition->farGridPos->_drawableStaticBits |= NEAR_SIDE;
	emptyPosition->farGridPos->sidesTouchingStaticCubes &= ~NEAR_SIDE;

	emptyPosition->nearGridPos->sidesTouchingStaticCubes &= ~FAR_SIDE;
}



void Grid::setDrawableSidesForSelfAndAdjacentLanded(GridPosition *thisPosition)
{
	GLuint x = thisPosition->x;
	GLuint y = thisPosition->y;

	GridPosition *adjPosition;

	GLubyte drawOwnSides = 0x00;

	if(x > 0){
		adjPosition = thisPosition->leftGridPos;
		adjPosition->sidesTouchingStaticCubes |= RIGHT_SIDE;

		if(x < X_BLOCKS_MID){
			adjPosition->_drawableStaticBits &= ~RIGHT_SIDE;
		}
		else if(adjPosition->isEmptyOrMovingSpot()){
			drawOwnSides |= LEFT_SIDE;
		}
	}
	else
		thisPosition->sidesTouchingStaticCubes |= LEFT_SIDE;

	if(x < X_BLOCKS_MINUS_1){
		adjPosition = thisPosition->rightGridPos;
		adjPosition->sidesTouchingStaticCubes |= LEFT_SIDE;

		if(x >= X_BLOCKS_MID){
			adjPosition->_drawableStaticBits &= ~LEFT_SIDE;
		}
		else if(adjPosition->isEmptyOrMovingSpot()){
			drawOwnSides |= RIGHT_SIDE;
		}
	}
	else
		thisPosition->sidesTouchingStaticCubes |= RIGHT_SIDE;

	if(y > 0){
		adjPosition = thisPosition->downGridPos;
		adjPosition->sidesTouchingStaticCubes |= TOP_SIDE;

		if(y < Y_BLOCKS_MID){
			adjPosition->_drawableStaticBits &= ~TOP_SIDE;
		}
		else if(adjPosition->isEmptyOrMovingSpot()){
			drawOwnSides |= BOTTOM_SIDE;
		}
	}
	else
		thisPosition->sidesTouchingStaticCubes |= BOTTOM_SIDE;

	if(y < Y_BLOCKS_MINUS_1){
		adjPosition = thisPosition->upGridPos;
		adjPosition->sidesTouchingStaticCubes |= BOTTOM_SIDE;

		if(y >= Y_BLOCKS_MID){
			adjPosition->_drawableStaticBits &= ~BOTTOM_SIDE;
		}
		else if(adjPosition->isEmptyOrMovingSpot()){
			drawOwnSides |= TOP_SIDE;
		}
	}
	else
		thisPosition->sidesTouchingStaticCubes |= TOP_SIDE;

	adjPosition = thisPosition->nearGridPos;
	adjPosition->sidesTouchingStaticCubes |= FAR_SIDE;

	if(adjPosition->isEmptyOrMovingSpot()){
		drawOwnSides |= NEAR_SIDE;
	}

	adjPosition = thisPosition->farGridPos;
	adjPosition->_drawableStaticBits &= ~NEAR_SIDE;

	adjPosition->sidesTouchingStaticCubes |= NEAR_SIDE;


	thisPosition->_drawableStaticBits = drawOwnSides;
}


void Grid::resetStatsVariables(){

	totalMoveAttempts = 0;
	amountOfFrustration = 0;
	numUnableToMove = 0;
	unableToGiveTarget = 0;
	numRandomizerFailed = 0;
	newlyStartedToMove = 0;
	numFloatingUnableToMove = 0;
	cubeReversedOnItself = 0;
	numFloatingUnableToMoveInRepel = 0;
	settingNewTarget = 0;
	foundAdjacentTarget = 0;
	cantMoveInAnyDirection = 0;
	shouldntMoveInAnyDirection = 0;

	newDirectionLeft = 0;
	newDirectionRight = 0;
	newDirectionTop = 0;
	newDirectionBottom = 0;
	newDirectionNear = 0;
	newDirectionFar = 0;
}


void Grid::getGridStats(GridStats &stats){
	stats.moveAttempts = totalMoveAttempts;
	stats.frustration = amountOfFrustration;
	stats.numUnableToMove = numUnableToMove;
	stats.unableToGiveTarget = unableToGiveTarget;
	stats.numRandomizerFailed = numRandomizerFailed;
	stats.newlyStartedToMove = newlyStartedToMove;
	stats.numFloatingUnableToMove = numFloatingUnableToMove;
	stats.numFloatingUnableToMoveInRepel = numFloatingUnableToMoveInRepel;
	stats.cubeReversedOnItself = cubeReversedOnItself;
	stats.foundAdjacentTarget = foundAdjacentTarget;
	stats.cantMoveInAnyDirection = cantMoveInAnyDirection;
	stats.shouldntMoveInAnyDirection = shouldntMoveInAnyDirection;

	stats.newDirectionLeft = newDirectionLeft;
	stats.newDirectionRight = newDirectionRight;
	stats.newDirectionTop = newDirectionTop;
	stats.newDirectionBottom = newDirectionBottom;
	stats.newDirectionNear = newDirectionNear;
	stats.newDirectionFar = newDirectionFar;
	stats.settingNewTarget = settingNewTarget;

	unsigned int numAttract = 0;
	unsigned int numStatic = 0;
	unsigned int numCubesInGoal = 0;
	unsigned int numEmptyGoal = 0;
	unsigned int numPaused = 0;
	unsigned int numMoving = 0;
	unsigned int numInvalidState = 0;
	unsigned int numMovingInvalid = 0;

	if(_gridPositionsArray)
		for(GLuint x = 0; x < X_BLOCKS; x++)
			for(GLuint y = 0; y < Y_BLOCKS; y++)
				for(GLuint z = 0; z < Z_BLOCKS; z++){
					GridPosition *gridPosition = _gridPositionsArray[x][y][z];
					if(gridPosition->goalNum > 0){
						numAttract++;
						if(gridPosition->cube)
							numCubesInGoal++;
					}
					if(gridPosition->state == STATE_STATIC)
						numStatic++;
					else if(gridPosition->state == STATE_NONE && gridPosition->goalNum > 0){
						numEmptyGoal++;
					}
					else if(gridPosition->state == STATE_MOVING){
						numMoving++;
					}
					else if(gridPosition->state == STATE_PAUSED){
						numPaused++;
					}

					if(!gridPosition->isInValidState()){
						numInvalidState++;
					}

					if(gridPosition->cube && !gridPosition->cube->isMoveStateValid()){
						numMovingInvalid++;
					}
				}

	stats.totalAttractIdentities = numAttract;
	stats.numStaticIdentities = numStatic;
	stats.numCubesInAttractGoalPositions = numCubesInGoal;
	stats.numEmptyGoals = numEmptyGoal;
	stats.numPausedIdentities = numPaused;
	stats.numMovingIdentities = numMoving;
	stats.numInvalidIdents = numInvalidState;
	stats.numMovingInvalid = numMovingInvalid;

	resetStatsVariables();
}


