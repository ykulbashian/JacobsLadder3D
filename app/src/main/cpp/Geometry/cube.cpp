#include "cube.h"
#include "SurfaceRenderer/surfaceutils.h"
#include "Utils/utils.h"
#include <stdlib.h>

#define LOG_TAG "Cube"


Cube::Cube() : IterativeCollectionMember()
{

	resetVariables();
}

Cube::~Cube(){

}

void Cube::resetVariables(){

	wasTouched = false;
	moveDirection = 0;
	lastMovedDirection = 0;
	_shouldMoveToBitsInContact = 0;
	_canMoveToBits = 0;
	priorMovedDirection = 0;
	currentJourneyLength = 0;

	diff_destx =
	diff_desty =
	diff_destz = 0;

	frustration = 0;

	stepMovedFragment = 0;

	currPosition = 0;
	nextPosition = 0;
}


void Cube::setNewDirectionVariables(GLubyte dir)
{
	moveDirection = dir;

	stepMovedFragment = 0;

	nextPosition = currPosition->findPositionInDirection(dir);

	nextPosition->setProperties(0, STATE_MOVING, dir);
	currPosition->setProperties(this, STATE_MOVING, dir);
}

bool Cube::incrementMovement(GLuint increment)
{
	stepMovedFragment += increment;

	//if finished moving------------------------------
	return (stepMovedFragment >= FLOATING_POINT_PRECISION);
}


void Cube::placeCubeAt(GridPosition *position)
{
	resetVariables();

	position->setProperties(this, STATE_STATIC, NO_SIDE);

	x = position->x;
	y = position->y;
	z = position->z;

	currPosition = position;
}

GridPosition *Cube::getFailedTarget(){
	if(currPosition != targetPosition)
		return targetPosition;

	return 0;
}

GridPosition *Cube::updateVariablesAfterMoving()
{
	lastMovedDirection = moveDirection;
	priorMovedDirection = moveDirection;
	wasTouched = false;

	switch (lastMovedDirection) {
		case RIGHT_SIDE:
			diff_destx++;
			break;
		case LEFT_SIDE:
			diff_destx--;
			break;
		case TOP_SIDE:
			diff_desty++;
			break;
		case BOTTOM_SIDE:
			diff_desty--;
			break;
		case FAR_SIDE:
			diff_destz++;
			break;
		case NEAR_SIDE:
			diff_destz--;
			break;
		default:
			break;
	}

	currentJourneyLength++;

	currPosition->setProperties(0, STATE_NONE, NO_SIDE);

	currPosition = currPosition->findPositionInDirection(lastMovedDirection);

	x = currPosition->x;
	y = currPosition->y;
	z = currPosition->z;

	currPosition->setProperties(this, STATE_PAUSED, NO_SIDE);

	moveDirection = 0;
	stepMovedFragment = 0;

	return 0;
}


void Cube::stopMoving(){

	currPosition->setProperties(this, STATE_STATIC, NO_SIDE);

	lastMovedDirection = 0;
	wasTouched = false;

	nextPosition = 0;

	currentJourneyLength = 0;
	frustration = 0;

	diff_destx = 0;
	diff_desty = 0;
	diff_destz = 0;
}

bool Cube::shouldMoveInThisDirection(GLubyte dir){
	return shouldMoveToInContact(dir) && !Utils::areOppositeDirections(lastMovedDirection, dir);
}


bool Cube::shouldMoveToInContact(GLubyte dir)
{
	return canMoveInThisDirectionInAPinch(dir) != 0
		   && (_shouldMoveToBitsInContact & dir) != 0
		   && !Utils::areOppositeDirections((_canMoveToBits^ALL_DIR), dir);
}

bool Cube::canMoveInThisDirectionInAPinch(GLubyte dir)
{
	return (_canMoveToBits & dir) != 0;
}

GLboolean Cube::isMoving()
{
	return moveDirection;
}

bool Cube::isJustStartingToMove()
{
	return currPosition->state == STATE_STATIC;
}

GridPosition *Cube::getGridElement(){
	return currPosition;
}

void Cube::isUndermined(){

}

void Cube::giveTargetGridPosition(GridPosition *target, GLuint MAX_Z){
	nextPosition = target;
	targetPosition = target;

	if(target){
		diff_destx = x - target->x;
		diff_desty = y - target->y;
		int d = abs(z - target->z);
		diff_destz = (d < MAX_Z/2) ? (z - target->z) : (z - (MAX_Z + target->z));
	}
	else{
		diff_destx = 0;
		diff_desty = 0;
		diff_destz = 0;
	}

	currentJourneyLength = 0;
	frustration = 0;

}

GridPosition *Cube::getNextPosition(){
	return nextPosition;
}

bool Cube::isMoveStateValid(){
	return isMoving() || (
			(((_canMoveToBits & LEFT_SIDE) == 0) ^ (currPosition->leftGridPos && currPosition->leftGridPos->state == STATE_NONE)) &&
			(((_canMoveToBits & RIGHT_SIDE) == 0) ^ (currPosition->rightGridPos && currPosition->rightGridPos->state == STATE_NONE)) &&
			(((_canMoveToBits & TOP_SIDE) == 0) ^ (currPosition->upGridPos && currPosition->upGridPos->state == STATE_NONE)) &&
			(((_canMoveToBits & BOTTOM_SIDE) == 0) ^ (currPosition->downGridPos && currPosition->downGridPos->state == STATE_NONE)) &&
			(((_canMoveToBits & NEAR_SIDE) == 0) ^ (currPosition->nearGridPos && currPosition->nearGridPos->state == STATE_NONE)) &&
			(((_canMoveToBits & FAR_SIDE) == 0) ^ (currPosition->farGridPos && currPosition->farGridPos->state == STATE_NONE))
			);
}


void Cube::sortDistancesHighestToLowest(Cube *c, GLubyte (&sorted)[3])
{
	int xAbsoluteLength = abs(c->diff_destx);
	int yAbsoluteLength = abs(c->diff_desty);
	int zAbsoluteLength = abs(c->diff_destz);

	if(xAbsoluteLength < yAbsoluteLength){
		if(xAbsoluteLength < zAbsoluteLength){
			if(yAbsoluteLength < zAbsoluteLength){
				sorted[0]= Z_DIR;
				sorted[1]= Y_DIR;
				sorted[2]= X_DIR;
			}
			else{
				sorted[0]= Y_DIR;
				sorted[1]= Z_DIR;
				sorted[2]= X_DIR;
			}
		}
		else{
			sorted[0]= Y_DIR;
			sorted[1]= X_DIR;
			sorted[2]= Z_DIR;
		}
	}
	else {
		if(yAbsoluteLength < zAbsoluteLength){
			if(xAbsoluteLength < zAbsoluteLength){
				sorted[0]= Z_DIR;
				sorted[1]= X_DIR;
				sorted[2]= Y_DIR;
			}
			else{
				sorted[0]= X_DIR;
				sorted[1]= Z_DIR;
				sorted[2]= Y_DIR;
			}
		}
		else{
			sorted[0]= X_DIR;
			sorted[1]= Y_DIR;
			sorted[2]= Z_DIR;
		}
	}

}

bool Cube::isInGoal() {
	return currPosition->goalNum > 0;
}



