#include <Drivers/cubedriver.h>

#define LOG_TAG "Goal Object"


GoalObject::GoalObject(GoalPointType type, GLuint x_blocks, GLuint y_blocks, GLuint z_blocks) {
	static unsigned int idNum = 1;

	goalType = type;
	isViolated = false;

	x = 0;
	y = 0;
	z = 0;

	id = idNum++;

	availableTargets = 0;
	filledTargets = 0;

	numberOfRequests = 0;
	requestToTargetRatio = 0.0f;
	totalTargetNumber = 0;

	filledMovingAverage = 0;

	failedTargets = 0;

	expectedTargetCountInCube = 0;
	cumulativeFrustration = 0;

	groundDirection = 0;

	X_BLOCKS = x_blocks;
	Y_BLOCKS = y_blocks;
	Z_BLOCKS = z_blocks;

	x_start = x_end = y_start = y_end = z_start = z_end = 0;
}


GoalObject::~GoalObject() {
	if(availableTargets){
		delete availableTargets;
		availableTargets = 0;
	}
	if(filledTargets){
		delete filledTargets;
		filledTargets = 0;
	}
}

GoalPointType GoalObject::getGoalType(){
	return goalType;
}

GridPosition *GoalObject::getRandomTarget(){
	//TODO should find closest to cube

	if(availableTargets && availableTargets->getSize() > 0){
		requestToTargetRatio = ((GLfloat)numberOfRequests)/((GLfloat)totalTargetNumber);
		GridPosition *target = availableTargets->getRandomEntry();
		if(!target) {
			LOG_ERROR_DEBUG("Failed to get random target: size of targets: %d",
							availableTargets->getSize());
			isViolated = true;
		}
		else{
			numberOfRequests += 1;
		}
		return target;
	}
	return 0;
}

GLint GoalObject::distanceToCenterPoint(GridPosition *position){
	GLuint numAvailable = availableTargets->getSize();
	if(goalType != ATTRACT ||
			Utils::areOppositeDirections(position->pullToWallsDirection, groundDirection) ||
			numAvailable < 2)
		return INT_MAX;

	GLint z_dist = abs(z - position->z);

	return abs(x - position->x) + abs(y - position->y) + min(z_dist, Z_BLOCKS - z_dist);// + 10*(GLint)requestToTargetRatio;
}


ReasonForDestruction GoalObject::getReasonForDestruction(unsigned int numSelected){
	unsigned int numFilledTargets = filledTargets->getSize();

	filledMovingAverage = 0.99f*filledMovingAverage + 0.01f*(GLfloat)(numFilledTargets);

	GLfloat slope = absf(filledMovingAverage - (GLfloat)numFilledTargets);

	ReasonForDestruction reason = notDestroyed;

	if(isViolated) {
		reason = goalIsViolated;
	} else if(requestToTargetRatio > 75.0f) {
		reason = tooManyRequests;
	} else if(((float)cumulativeFrustration/(GLfloat)totalTargetNumber > 75.0f) || ((float)failedTargets/(GLfloat)totalTargetNumber > 75.0f)) {
		reason = frustration;
	} else if(availableTargets->getSize() < 2 && slope < 0.05f){
		reason = tooFewTargets;
	} else if(numFilledTargets > 5 && requestToTargetRatio > 0.3f && slope < 0.1f){
		reason = flattenedOut;
	}

	return reason;
}

GLint GoalObject::min(GLint val1, GLint val2){
	return ((val1 < val2) ? val1 : val2);
}

GLfloat GoalObject::absf(GLfloat val) {
    return (val < 0) ? -val : val;
}


long GoalObject::getPendingRequests(){
	return numberOfRequests - failedTargets - (totalTargetNumber - availableTargets->getSize());
}


bool GoalObject::populateGrid(GridPosition ****grid){

	if(!isGrounded()){
		LOG_INFO_DEBUG("Is not grounded, rejecting goal object.");
		return false;
	}

	for(unsigned int _x = x_start; _x < x_end; _x++) {
		for (unsigned int _y = y_start; _y < y_end; _y++) {
			for (unsigned int _z = z_start; _z < z_end; _z++) {

				if (_x != x_start && _x != x_end - 1 && _y != y_start && _y != y_end - 1 &&
					_z != z_start && _z != z_end - 1)
					continue;

				GridPosition *position = grid[_x][_y][_z];


				if (goalType == REPEL) {
					position->goalNum = -1;
				}
				else if (position->goalNum >= 0) {

					if (goalType == ATTRACT) {

						if (!position->cube && availableTargets->addItem(position)) {
							totalTargetNumber++;
							position->goalNum++;
							position->goalPullToWallsDirection = groundDirection;
						}
					}
				}

			}
		}
	}

	unsigned int numTargets = availableTargets->getSize();

	if(numTargets < 5 || (numTargets < (4/5)*expectedTargetCountInCube)){
		LOG_ERROR_DEBUG("Not enough targets, rejecting goal object. %d < %d", numTargets, (4/5)*expectedTargetCountInCube);
		return false;
	}
	else
		LOG_INFO_DEBUG("New goal created at %d, %d, %d with id: %d", x, y, z, id);

	return true;
}

GLuint GoalObject::getNumAvailable(){
	return availableTargets->getSize();
}

void GoalObject::allowAsTarget(GridPosition *target) {
	if(filledTargets->contains(target)){
		filledTargets->removeItem(target);
		availableTargets->addItem(target);
	}
}


void GoalObject::removeFromGrid(AllIterativeCollections *allCollections,
								ReasonForDestruction reason){

	while(availableTargets->getSize() > 0){
		removeTarget(availableTargets->getFirst(), allCollections, reason);
	}

	while(filledTargets->getSize() > 0){
		removeTarget(filledTargets->getFirst(), allCollections, reason);
	}

	if(totalTargetNumber > 0)
		LOG_ERROR_DEBUG("**Failed to restore all grid positions");

}

void GoalObject::removeTarget(GridPosition *target, AllIterativeCollections *allCollections, ReasonForDestruction reason){

	availableTargets->removeItem(target);
	filledTargets->removeItem(target);
	target->goalNum--;
	totalTargetNumber--;

	Cube *cube = target->cube;

	if(cube && !cube->isMoving() && target->goalNum == 0){
		switch (reason){
			case flattenedOut:
				allCollections->addCubeToList(cube, SELECTED_TO_MOVE_LIST);
				break;
			case goalIsViolated:
				cube->wasTouched = true;
			default:
				allCollections->addCubeToList(cube, PRIORITY_LIST);
		}
	}
}

bool GoalObject::isGrounded(){
	return (x_start == 0 || x_end == X_BLOCKS) ||
		   (y_start == 0 || y_end == Y_BLOCKS);

}


bool GoalObject::containsPoint(GridPosition *pos){
	return availableTargets->contains(pos) || filledTargets->contains(pos);
}

bool GoalObject::targetWasOccupied(GridPosition *occupiedTarget){ //TODO Double counting?
	if(occupiedTarget && availableTargets->contains(occupiedTarget)){
		availableTargets->removeItem(occupiedTarget);
		filledTargets->addItem(occupiedTarget);

		return true;
	}
	return false;
}


bool GoalObject::targetWasFailed(GridPosition *failedTarget, unsigned int frustration){
	if(failedTarget && availableTargets->contains(failedTarget)){
		failedTargets += 1;
		cumulativeFrustration += frustration;
		return true;
	}
	return false;
}

void GoalObject::getGoalStats(GoalStats &stats){
	stats.id = id;
	stats.filled = filledTargets ? filledTargets->getSize() : 0;
	stats.capacity = totalTargetNumber;
	stats.numRequests = numberOfRequests;
	stats.numFailed = failedTargets;
	stats.filledMovingAverage = (uint)filledMovingAverage;
	stats.cumulativeFrustration = cumulativeFrustration;
	stats.expectedTargetCountInCube = expectedTargetCountInCube;
	stats.reasonForDestruction = notDestroyed;
}



