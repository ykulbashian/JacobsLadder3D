#include "Drivers/Grid/grid.h"

bool Grid::deleteACube(Cube *c){
	bool ret = false;

	GridPosition *cubePosition = c->getGridElement();
	setAdjacentCubesDrawableBitsOnEmpty(cubePosition);

	if(c->isMoving()){
		ret = true;
	}
	else
		_sliceIsDirty[c->z] = true;

	c->getGridElement()->setProperties(c, STATE_NONE, NO_SIDE);

	return ret;
}


bool Grid::placeACube(Cube *c, Position *pos){

	GridPosition *cubePosition = getGridElement(pos->x, pos->y, pos->z);

	if(!cubePosition->cube)
	{
		c->placeCubeAt(cubePosition);

		setDrawableSidesForSelfAndAdjacentLanded(cubePosition);

		return true;
	}
	return false;

}

