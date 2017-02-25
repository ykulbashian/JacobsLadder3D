#include "Drivers/Grid/grid.h"

#define LOG_TAG "Grid Algorithm"


GLubyte Grid::findAndSetNewSuggestedDirection(Goals *goals, Cube *const c, bool isContinuing)
{
#ifdef COLLECT_STATS
	totalMoveAttempts++;
#endif

	if(!tryToSetTarget(goals, c)){
#ifdef COLLECT_STATS
		unableToGiveTarget++;
#endif
		return 0;
	}

	GLubyte dir = getBestDirectionForCube(c, isContinuing);

	if(dir){

#ifdef COLLECT_STATS

		switch(dir){
		case LEFT_SIDE:
			newDirectionLeft++;
			break;
		case RIGHT_SIDE:
			newDirectionRight++;
			break;
		case TOP_SIDE:
			newDirectionTop++;
			break;
		case BOTTOM_SIDE:
			newDirectionBottom++;
			break;
		case NEAR_SIDE:
			newDirectionNear++;
			break;
		case FAR_SIDE:
			newDirectionFar++;
			break;
		}
#endif
		setACubeAsMoving(c, dir);
	}
	else{
#ifdef COLLECT_STATS

		numUnableToMove++;
		if(!isContinuing && c->getGridElement()->isInUntenablePosition()){
			if(c->getGridElement()->goalNum < 0)
				numFloatingUnableToMoveInRepel++;
			else
				numFloatingUnableToMove++;
		}

#endif
	}

	return dir;
}

bool Grid::tryToSetTarget(Goals *goals, Cube *const c){

	if(!c->getNextPosition()){
		GridPosition *cubePosition = c->getGridElement();

		GridPosition *suggestTarget = findAdjacentTarget(cubePosition);

		if(!suggestTarget) {
			suggestTarget = goals->getRandomTarget(cubePosition);
			if(!suggestTarget || !isAcceptableTarget(cubePosition, suggestTarget))
				return false;
		}
#ifdef COLLECT_STATS
		else{
			foundAdjacentTarget++;
		}
#endif

		c->giveTargetGridPosition(suggestTarget, Z_BLOCKS);
#ifdef COLLECT_STATS
		settingNewTarget++;
#endif
	}
	return true;
}

bool Grid::isAcceptableTarget(GridPosition *toPosition, GridPosition *target)
{
	return !Utils::areOppositeDirections(target->pullToWallsDirection, toPosition->pullToWallsDirection);
}

GLubyte Grid::getBestDirectionForCube(Cube *const c, bool isContinuing){

	GridPosition *cubePosition = c->getGridElement();
	GLubyte ptwDir = cubePosition->pullToWallsDirection;
	GLubyte targetPtwDir = c->getNextPosition()->pullToWallsDirection;

	if(!isContinuing){
		if(cubePosition->isInUntenablePosition() && (c->_canMoveToBits & ptwDir) != 0){
			return ptwDir;
		}
	}

	if(ptwDir != targetPtwDir && (c->_canMoveToBits & targetPtwDir) != 0){
		return targetPtwDir;
	}

	GLubyte sorted[3] = {0, 0, 0};
	Cube::sortDistancesHighestToLowest(c, sorted);

	for(unsigned int i = 0; i < 3; i++){
		GLubyte checkdirTwo = sorted[i];
		GLubyte checkdirS = 0;

		if(!checkdirTwo)
			break;

		if(checkdirTwo == X_DIR){
			if(c->diff_destx != 0)
			checkdirS = c->diff_destx > 0 ? LEFT_SIDE : RIGHT_SIDE;
		}
		else if(checkdirTwo == Y_DIR){
			if(c->diff_desty != 0)
			checkdirS = c->diff_desty > 0 ? BOTTOM_SIDE : TOP_SIDE;
		}
		else if(checkdirTwo == Z_DIR){
			if(c->diff_destz != 0)
			checkdirS = c->diff_destz > 0 ? NEAR_SIDE : FAR_SIDE;
		}

		if(!checkdirS)
			break;

		if(c->shouldMoveInThisDirection(checkdirS)){
			return checkdirS;
		}
	}

	c->frustration++;
#ifdef COLLECT_STATS
	amountOfFrustration++;
#endif


	if(c->frustration > 5){

#ifdef COLLECT_STATS
		if(!c->_canMoveToBits)
			cantMoveInAnyDirection++;
		if(!c->_shouldMoveToBitsInContact)
			shouldntMoveInAnyDirection++;
#endif
		return 0;
	}

	GLubyte checkdir = FAR_SIDE;
	GLubyte bestdir = sorted[0];

	if(!bestdir)
		return 0;

	for(GLubyte tries = 0; tries < MAX_MOVE_TRIES; tries++)
	{
		if(bestdir & X_DIR){
			checkdir = X_NEXT[(GLubyte)(RANDOM(4))];
		}
		else if(bestdir & Y_DIR){
			checkdir = Y_NEXT[(GLubyte)(RANDOM(4))];
		}
		else if(bestdir & Z_DIR){
			checkdir = Z_NEXT[(GLubyte)(RANDOM(4))];
		}

		if(c->shouldMoveInThisDirection(checkdir)){
			return checkdir;
		}
	}

#ifdef COLLECT_STATS
	numRandomizerFailed++;
#endif

	return 0;
}


void Grid::cubeHasStoppedMoving(GridPosition *landedPosition)
{
	setDrawableSidesForSelfAndAdjacentLanded(landedPosition);

	_sliceIsDirty[landedPosition->z] = true;
}


void Grid::setACubeAsMoving(Cube *c, GLubyte dir) {

	assert(c);

	if (c->isJustStartingToMove()){
#ifdef COLLECT_STATS
		newlyStartedToMove++;
#endif

		GridPosition *cubePosition = c->getGridElement();

		setAdjacentCubesDrawableBitsOnEmpty(cubePosition);

		_sliceIsDirty[c->z] = true;
		if(cubePosition->farGridPos->state == STATE_STATIC)
			_sliceIsDirty[(c->z + 1) % Z_BLOCKS] = true;
	}

#ifdef COLLECT_STATS
	if(dir == c->priorMovedDirection)
		cubeReversedOnItself++;
#endif

	c->setNewDirectionVariables(dir);
}


bool Grid::shouldKeepMoving(GridPosition *position)
{
	assert(position);
	return position->isInUntenablePosition() || !(isInGoals(position));
}

bool Grid::isInGoals(GridPosition *gridPosition){
	return gridPosition && gridPosition->goalNum > 0;
}


bool Grid::isGoodCandidateToMove(Cube *c){
	if(!c || c->isMoving()) return false;

	GridPosition *cubePosition = c->getGridElement();

	return cubePosition->isInUntenablePosition() ||
			!(isInGoals(cubePosition)) ||
			findAdjacentTarget(cubePosition);

}

GridPosition *Grid::findAdjacentTarget(GridPosition *toPosition){
	if(!toPosition)
		return 0;

	GridPosition *toWalls = toPosition->getPositionInWallsDirection();
	if(toWalls && toWalls->isEmptyTarget())
		return toWalls;

	if(toPosition->leftGridPos && toPosition->leftGridPos->isEmptyTarget())
		return toPosition->leftGridPos;

	if(toPosition->rightGridPos && toPosition->rightGridPos->isEmptyTarget())
		return toPosition->rightGridPos;

	if(toPosition->upGridPos && toPosition->upGridPos->isEmptyTarget())
		return toPosition->upGridPos;

	if(toPosition->downGridPos && toPosition->downGridPos->isEmptyTarget())
		return toPosition->downGridPos;

	if(toPosition->nearGridPos && toPosition->nearGridPos->isEmptyTarget())
		return toPosition->nearGridPos;

	if(toPosition->farGridPos && toPosition->farGridPos->isEmptyTarget())
		return toPosition->farGridPos;

	return 0;
}
