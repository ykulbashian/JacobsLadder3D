#include "Utils/alliterativecollections.h"
#include "SurfaceRenderer/surfaceutils.h"

#define LOG_TAG "All Iterative Collections"


AllIterativeCollections::AllIterativeCollections(){
	selectGroup = 0;
	priorityGroup = 0;
	movingElements = 0;
	candidateGroup = 0;

	initializeArrays();
}

AllIterativeCollections::~AllIterativeCollections(){
	deleteArrays();
}

void AllIterativeCollections::initializeArrays(){

	if(!selectGroup)
		selectGroup = new IterativeCollection<Cube*>(LARGE_ARRAY_SIZE, SELECTED_TO_MOVE_LIST);

	if(!priorityGroup)
		priorityGroup = new IterativeCollection<Cube*>(LARGE_ARRAY_SIZE, PRIORITY_LIST);

	if(!movingElements)
		movingElements = new IterativeCollection<Cube*>(DEFAULT_ARRAY_SIZE, MOVING_LIST);

	if(!candidateGroup)
		candidateGroup = new IterativeCollection<Cube*>(LARGE_ARRAY_SIZE, CANDIDATE_LIST);

}

void AllIterativeCollections::deleteArrays(){

	if(selectGroup){
		delete selectGroup;
		selectGroup = 0;
	}

	if(priorityGroup){
		delete priorityGroup;
		priorityGroup = 0;
	}

	if(movingElements){
		delete movingElements;
		movingElements = 0;
	}

	if(candidateGroup){
		delete candidateGroup;
		candidateGroup = 0;
	}

}

void AllIterativeCollections::revertArrays(){

	if(selectGroup){
		selectGroup->resetArray(LARGE_ARRAY_SIZE);
	}

	if(priorityGroup){
		priorityGroup->resetArray(MID_ARRAY_SIZE);
	}

	if(movingElements){
		movingElements->resetArray(DEFAULT_ARRAY_SIZE);
	}

	if(candidateGroup){
		candidateGroup->resetArray(LARGE_ARRAY_SIZE);
	}

}


bool AllIterativeCollections::addCubeToList(Cube *cube, const AddedToListType type)
{
	if(cube->addedToList == type)
		return true;

	if(!doesListHaveRoom(type))
		return false;


	switch(cube->addedToList)
	{
	case NO_LIST:
		break;
	case SELECTED_TO_MOVE_LIST:
		selectGroup->removeItem(cube);
		break;
	case PRIORITY_LIST:
		priorityGroup->removeItem(cube);
		break;
	case MOVING_LIST:
		movingElements->removeItem(cube);
		break;
	case CANDIDATE_LIST:
		candidateGroup->removeItem(cube);
		break;
	case TARGET_LIST:
	case GOAL_LIST:
		break;
	}


	switch(type)
	{
	case NO_LIST:
		break;
	case SELECTED_TO_MOVE_LIST:
		{
			if(selectGroup->hasRoom()){
				selectGroup->addItem(cube);
				return true;
			}
			else{
				candidateGroup->addItem(cube);
				LOG_INFO_DEBUG("NO MORE ROOM in selectGroup");
			}
		}
		break;
	case PRIORITY_LIST:
		{
			if(priorityGroup->hasRoom()){
				priorityGroup->addItem(cube);
				return true;
			}
			else{
				candidateGroup->addItem(cube);
				LOG_INFO_DEBUG("NO MORE ROOM in priorityGroup");
			}

		}
		break;
	case MOVING_LIST:
		{
			if(movingElements->hasRoom()){
				movingElements->addItem(cube);
				return true;
			}
			else{
				LOG_INFO_DEBUG("NO MORE ROOM in movingElements");
			}
		}
		break;
	case CANDIDATE_LIST:
		{
			if(candidateGroup->hasRoom()){
				candidateGroup->addItem(cube);
				return true;
			}
			else{
				LOG_INFO_DEBUG("NO MORE ROOM in candidateGroup");
			}
		}
		break;
	case TARGET_LIST:
	case GOAL_LIST:
		break;
	}
	return false;
}

bool AllIterativeCollections::doesListHaveRoom(const AddedToListType type){
	bool hasRoom;

	switch(type)
	{
	case TARGET_LIST:
	case GOAL_LIST:
	case NO_LIST:
		return false;
	case SELECTED_TO_MOVE_LIST:
		hasRoom = selectGroup->hasRoom();
		if(!hasRoom)
			LOG_ERROR_DEBUG("Cannot move to selected list");
		return hasRoom;
	case PRIORITY_LIST:
		hasRoom = priorityGroup->hasRoom();
		if(!hasRoom)
			LOG_ERROR_DEBUG("Cannot move to priority list");
		return hasRoom;
	case MOVING_LIST:
		hasRoom = movingElements->hasRoom();
		if(!hasRoom)
			LOG_ERROR_DEBUG("Cannot move to moving list");
		return hasRoom;
	case CANDIDATE_LIST:
		hasRoom = candidateGroup->hasRoom();
		if(!hasRoom)
			LOG_ERROR_DEBUG("Cannot move to candidate list");
		return hasRoom;
	default:
		return false;
	}
}


GLuint AllIterativeCollections::moveUnsupportedToPriority(){
	GLuint numMoved = 0;
	numMoved += moveUnsupportedToPriorityList(selectGroup);

	numMoved += moveUnsupportedToPriorityList(candidateGroup);

	return numMoved;
}


GLuint AllIterativeCollections::moveUnsupportedToPriorityList(IterativeCollection<Cube*> *listToCheck){

	GLuint numMoved = 0;
	Cube *c;

	listToCheck->resetIterator();

	while(listToCheck->hasNext())
	{
		c = listToCheck->getNext();

		if(!c->getGridElement()->isSupportedFromWall() && !c->isInGoal()){
			addCubeToList(c, PRIORITY_LIST);
			numMoved++;
		}
	}

	return numMoved;
}



void AllIterativeCollections::getIterativeCollectionStats(IterativeCollectionStats (&stats)[4]){

	stats[0] = movingElements->getIterativeCollectionStats();
	stats[1] = candidateGroup->getIterativeCollectionStats();
	stats[2] = priorityGroup->getIterativeCollectionStats();
	stats[3] = selectGroup->getIterativeCollectionStats();

}

