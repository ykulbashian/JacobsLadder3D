#include "Drivers/cubedriver.h"

#define LOG_TAG "Driver"



//========================================================================================
//the primary call point from the engine to set the next driver properties----------------
void CubeDriver::iterateOneFrame(GLuint fractionOfPreferredFrame) {
	if(!isInitialized)
		return;

	static long long nextMoveUnsupported = 0;
	static long long nextMoveSelected = 0;

	iterateMovingCubes(fractionOfPreferredFrame);

	if(++nextMoveUnsupported % CHECK_UNSUPPORTED_INCREMENT == 0)
	{
		GLuint numUnsupported = allArrayCollections.moveUnsupportedToPriority();
		numInUntenablePosition += numUnsupported;
	}

	bool needMoreGoals = !startPriorityCubesMoving();

	goalSet.recycleViolatedGoalObjects(	_grid._gridPositionsArray,
											&allArrayCollections,
											allArrayCollections.selectGroup->getSize(),
											needMoreGoals);

	if(++nextMoveSelected % MOVE_SELECTED_INCREMENT == 0)
		moveSelectionToPriority();

	if(allArrayCollections.movingElements->getSize() < MAX_MOVING_AT_A_TIME*0.6f)
		moveNewCandidatesToSelected();
}


void CubeDriver::iterateMovingCubes(GLuint moveDistance) {
	Cube *c;

	IterativeCollection<Cube*> *movingCollection = allArrayCollections.movingElements;

	movingCollection->lock();

	movingCollection->resetIterator();

	while(movingCollection->hasNext())
	{
		c = movingCollection->getNext();

		assert(c);

		if(!c->isMoving()){
			allArrayCollections.addCubeToList(c, SELECTED_TO_MOVE_LIST);
		}

		//if finished moving------------------------------
		else if(c->incrementMovement(moveDistance))
		{
			c->updateVariablesAfterMoving();

			if(!tryToGetNextMove(c))
				stopCubeMoving(c);
		}
	}

	movingCollection->unlock();
}


bool CubeDriver::tryToGetNextMove(Cube *c){
	GridPosition *cubePosition = c->getGridElement();

	if(_grid.shouldKeepMoving(cubePosition))
	{
		_grid.setAbleToMoveToBits(c);

		bool tooManyMoving = allArrayCollections.movingElements->getSize() > MAX_MOVING_AT_A_TIME;

		if(!c->_canMoveToBits || (tooManyMoving && !cubePosition->isInUntenablePosition())) {
			allArrayCollections.addCubeToList(c, SELECTED_TO_MOVE_LIST);
		} else if(_grid.findAndSetNewSuggestedDirection(&goalSet, c, true)){
			return true;
		}
		else {
			allArrayCollections.addCubeToList(c, PRIORITY_LIST);
		}
	}
	// can stop-----------------------------
	else{
		allArrayCollections.addCubeToList(c, CANDIDATE_LIST);
	}

	return false;
}

void CubeDriver::stopCubeMoving(Cube *c){

	GridPosition *landed = c->getGridElement();
	GridPosition *failedTarget = c->getFailedTarget();

	if(failedTarget)
	{
		goalSet.markTargetAsFailed(failedTarget, c->frustration);
	}

	goalSet.claimTargetAsOccupied(landed);

	c->stopMoving();

	_grid.cubeHasStoppedMoving(landed);

}


void CubeDriver::moveSelectionToPriority(){

	IterativeCollection<Cube*> *select = allArrayCollections.selectGroup;

	unsigned int numberOfCubesToMove = (unsigned int)((float) select->getSize()*FRACTION_OF_SELECTION_TO_PRIORITY);
	unsigned int maxSpacesLeft = MAX_MOVING_AT_A_TIME - allArrayCollections.movingElements->getSize();

	if(allArrayCollections.movingElements->getSize() > MAX_MOVING_AT_A_TIME)
		return;

	if(maxSpacesLeft < numberOfCubesToMove)
		numberOfCubesToMove = maxSpacesLeft;

	Cube *c;

	select->lock();

	select->resetIterator();

	GLuint i = 0;
	while(select->hasNext() && i < numberOfCubesToMove)
	{
		c = select->getNext();

		assert(c);

		if(_grid.isInGoals(c->getGridElement())){
#ifdef COLLECT_STATS
			selectionWasInGoals++;
#endif
			if(c->getGridElement()->isSupportedFromWall()){
				allArrayCollections.addCubeToList(c, CANDIDATE_LIST);
				continue;
			}
		}

		c = getCubeFurthestFromWall(c);

		if(c) {
			allArrayCollections.addCubeToList(c, PRIORITY_LIST);
			i++;
		}

	}

	select->unlock();
}

bool CubeDriver::startPriorityCubesMoving(){

	IterativeCollection<Cube*> *priority = allArrayCollections.priorityGroup;
	IterativeCollection<Cube*> *moving = allArrayCollections.movingElements;

	moving->lock();

	Cube *c;

	if(!moving->hasRoom()){
		LOG_ERROR_DEBUG("Unable to hit priority queue.");
		failedToGetToPriority = true;
		moving->unlock();
		return false;
	}

	//push whole priority cue--------------------------------------------------
	priority->lock();

	priority->resetIterator();

	while(priority->hasNext() && moving->hasRoom())
	{
		c = priority->getNext();

		_grid.setAbleToMoveToBits(c);

		if(c->isInGoal()){
			goalSet.moveOutOfGoal(c);
		}

		if(!c->_canMoveToBits) {
			allArrayCollections.addCubeToList(c, SELECTED_TO_MOVE_LIST);
			c->wasTouched = false;
		} else if(_grid.findAndSetNewSuggestedDirection(&goalSet, c, false)) {
			allArrayCollections.addCubeToList(c, MOVING_LIST);
		}
		else{
#ifdef COLLECT_STATS
			priorityCubeCouldNotMove++;
#endif
		}
	}

	priority->unlock();
	moving->unlock();

	return true;
}

void CubeDriver::moveNewCandidatesToSelected()
{
	IterativeCollection<Cube*> *candidate = allArrayCollections.candidateGroup;

	candidate->lock();

	Cube *c;

	for(GLuint i = 0; i < MAX_RANDOM_MOVE_PER_FRAME; i++)
	{
		c = candidate->getRandomEntry();
		if(c) {
			c = getCubeFurthestFromWall(c);
		}

		if(c){
			allArrayCollections.addCubeToList(c, SELECTED_TO_MOVE_LIST);
#ifdef COLLECT_STATS
			numCubesMovedToSelection++;
#endif
		}
		else{
#ifdef COLLECT_STATS
			unableToGetCandidate++;
#endif
		}
	}
	candidate->unlock();
}

Cube *CubeDriver::getCubeFurthestFromWall(Cube *c){
	assert(c);

	if(c->isMoving()){
		return 0;
	}

	GridPosition *chosenPosition = c->getGridElement();

	if(!chosenPosition->isSupportedFromWall())
		return c;

	GLubyte awayFromWall = chosenPosition->pullAgainstWallsDirection;

	if(!_grid.isInGoals(chosenPosition)){
		GridPosition *testPosition = chosenPosition->findPositionInDirection(awayFromWall);

		do{
			if(testPosition && testPosition->state == STATE_STATIC)
				c = testPosition->cube;
			else
				break;

			testPosition = testPosition->findPositionInDirection(awayFromWall);

		}
		while(!_grid.isInGoals(testPosition) && awayFromWall == testPosition->pullAgainstWallsDirection);

		return c;
	}

	return 0;
}


bool CubeDriver::tryToPlaceCubeInGrid(Cube *c, Position *pos)
{
	assert(c);

	if(_grid.placeACube(c, pos)){
		allArrayCollections.addCubeToList(c, CANDIDATE_LIST);
		return true;
	}

	return false;
}

void CubeDriver::deleteACube(Cube *c)
{
	assert(c);

	if(_grid.deleteACube(c)) {
		allArrayCollections.addCubeToList(c, NO_LIST);

		delete c;
	}
}

AggregateStats CubeDriver::getAggregateStats(){
	AggregateStats stats;

	getDriverStats(stats.driverStats);

	goalSet.getGoalStats(stats.goalAggregateStats);

	allArrayCollections.getIterativeCollectionStats(stats.arrayStats);

	_grid.getGridStats(stats.gridStats);

	return stats;
}


void CubeDriver::getDriverStats(DriverStats &stats){
	stats.failedToGetToPriority = failedToGetToPriority;
	stats.numCubesMovedToSelect = numCubesMovedToSelection;
	stats.unableToGetCandidate = unableToGetCandidate;
	stats.priorityCubeCouldNotMove = priorityCubeCouldNotMove;
	stats.selectionWasInGoals = selectionWasInGoals;
	stats.numInUntenablePosition = numInUntenablePosition;

	failedToGetToPriority = false;
	numCubesMovedToSelection = 0;
	unableToGetCandidate = 0;
	priorityCubeCouldNotMove = 0;
	selectionWasInGoals = 0;
	numInUntenablePosition = 0;
}

