#ifndef IDENTITY_CPP_
#define IDENTITY_CPP_

#include "Geometry/cube.h"


bool GridPosition::isEmptyTarget(){
	return (state == STATE_NONE && goalNum > 0);
}


void GridPosition::setProperties(Cube *cubeRef, GridPositionState gpState, GLubyte movingToDirection)
{
	cube = cubeRef;
	state = gpState;
	movingDirection = movingToDirection;
}

bool GridPosition::isNotBlockedInThisDirection(GLubyte dir)
{
	return state == STATE_NONE && goalNum >= 0;
}

bool GridPosition::isEmptyOrMovingSpot()
{
	return state != STATE_STATIC;
}

bool GridPosition::isInUntenablePosition(){
	return sidesTouchingStaticCubes == 0 || goalNum < 0;
}

bool GridPosition::isSupportedFromWall(){
	GridPosition *toWalls = getPositionInWallsDirection();
	return goalNum >= 0 && (!toWalls || toWalls->state == STATE_STATIC);
}

GLubyte GridPosition::sidesInStaticContact(){
	return sidesTouchingStaticCubes;
}


GLubyte GridPosition::getDrawableFaces(){
	return _drawableStaticBits;
}


void GridPosition::isUndermined(){
	if(!isBasicGoalPosition && goalNum == 0 && cube){
		cube->isUndermined();
		if(leftGridPos)
			leftGridPos->isUndermined();
		if(rightGridPos)
			rightGridPos->isUndermined();
		if(upGridPos)
			upGridPos->isUndermined();
		if(downGridPos)
			downGridPos->isUndermined();
		if(nearGridPos)
			nearGridPos->isUndermined();
		if(farGridPos)
			farGridPos->isUndermined();
	}

}

GridPosition *GridPosition::findPositionInDirection(GLubyte dir){
	switch(dir){
		case LEFT_SIDE:
			return leftGridPos;
		case RIGHT_SIDE:
			return rightGridPos;
		case TOP_SIDE:
			return upGridPos;
		case BOTTOM_SIDE:
			return downGridPos;
		case FAR_SIDE:
			return farGridPos;
		case NEAR_SIDE:
			return nearGridPos;
		default:
			return 0;
	}
}

void GridPosition::setPullToWallsPosition(){
	pullToWallsGridPos = findPositionInDirection(pullToWallsDirection);
}

GridPosition *GridPosition::getPositionInWallsDirection(){
	if(goalNum <= 0)
		return pullToWallsGridPos;
	else
		return findPositionInDirection(goalPullToWallsDirection);
}


bool GridPosition::isInValidState(){
	return (((sidesTouchingStaticCubes & LEFT_SIDE) == 0) ^ (!leftGridPos || (leftGridPos->cube && leftGridPos->state == STATE_STATIC))) &&
			(((sidesTouchingStaticCubes & RIGHT_SIDE) == 0) ^ (!rightGridPos || (rightGridPos->cube && rightGridPos->state == STATE_STATIC))) &&
			(((sidesTouchingStaticCubes & TOP_SIDE) == 0) ^ (!upGridPos || (upGridPos->cube && upGridPos->state == STATE_STATIC))) &&
			(((sidesTouchingStaticCubes & BOTTOM_SIDE) == 0) ^ (!downGridPos || (downGridPos->cube && downGridPos->state == STATE_STATIC))) &&
			(((sidesTouchingStaticCubes & NEAR_SIDE) == 0) ^ (nearGridPos->cube && nearGridPos->state == STATE_STATIC)) &&
			(((sidesTouchingStaticCubes & FAR_SIDE) == 0) ^ (farGridPos->cube && farGridPos->state == STATE_STATIC));
}

#endif /* IDENTITY_CPP_ */
