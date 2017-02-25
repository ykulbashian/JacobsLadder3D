#include "Drivers/Grid/grid.h"

#define LOG_TAG "Grid Create"

GLubyte Grid::X_NEXT[] = {NEAR_SIDE, FAR_SIDE, BOTTOM_SIDE, TOP_SIDE};
GLubyte Grid::Y_NEXT[] = {LEFT_SIDE, RIGHT_SIDE, NEAR_SIDE, FAR_SIDE};
GLubyte Grid::Z_NEXT[] = {LEFT_SIDE, RIGHT_SIDE, BOTTOM_SIDE, TOP_SIDE};

Grid::Grid() : Dimension()
{
	_gridPositionsArray = 0;

	_sliceIsDirty = 0;

	resetStatsVariables();

	resetGridAndGoalArrays();

	LOG_INFO_DEBUG("      Created Grid.");

}

void Grid::resetSlicesToDirty(){
	for(GLuint i = 0; i < Z_BLOCKS; i++)
		_sliceIsDirty[i] = true;
}

bool Grid::isReady(){
	return _gridPositionsArray != 0;
}


Grid::~Grid()
{
	resetGridAndGoalArrays();

}

void Grid::resetGridAndGoalArrays()
{
	deleteGridPositionArray();

	if(_sliceIsDirty){
		delete[] _sliceIsDirty;
		_sliceIsDirty = 0;
	}

	X_BLOCKS = 0;
	Y_BLOCKS = 0;
	Z_BLOCKS = 0;

	X_BLOCKS_MINUS_1 = 0;
	Y_BLOCKS_MINUS_1 = 0;
	Z_BLOCKS_MINUS_1 = 0;
}

void Grid::deleteGridPositionArray()
{
	if(_gridPositionsArray)
	{
		LOG_INFO_DEBUG("      ---Deleting GridPosition Grid...");

		for(GLuint x = 0; x < X_BLOCKS; x++){

			for(GLuint y = 0; y < Y_BLOCKS; y++){

				for(GLuint z = 0; z < Z_BLOCKS; z++){
					delete _gridPositionsArray[x][y][z];
				}
				delete[] _gridPositionsArray[x][y];
			}
			delete[] _gridPositionsArray[x];
		}
		delete[] _gridPositionsArray;
		_gridPositionsArray = 0;

		LOG_INFO_DEBUG("      Finished deleting GridPosition Grid...");
	}
}


void Grid::resetGridToDimensions(Dimension dimens)
{
	LOG_INFO_DEBUG("   Setting GridPosition Grid to dimensions (%d, %d, %d)...", dimens.X_BLOCKS, dimens.Y_BLOCKS, dimens.Z_BLOCKS);

	resetGridAndGoalArrays();

	X_BLOCKS = dimens.X_BLOCKS;
	Y_BLOCKS = dimens.Y_BLOCKS;
	Z_BLOCKS = dimens.Z_BLOCKS;

	X_BLOCKS_MINUS_1 = X_BLOCKS-1;
	Y_BLOCKS_MINUS_1 = Y_BLOCKS-1;
	Z_BLOCKS_MINUS_1 = Z_BLOCKS-1;

	X_BLOCKS_MID = ((GLfloat)X_BLOCKS)/2.0f;
	Y_BLOCKS_MID = ((GLfloat)Y_BLOCKS)/2.0f;

	_sliceIsDirty = new bool[Z_BLOCKS];

	for(GLuint i = 0; i < Z_BLOCKS; i++)
		_sliceIsDirty[i] = true;

	createGridPositionArray();

}


void Grid::createGridPositionArray()
{
	LOG_INFO_DEBUG("      Creating GridPosition Grid array...");

	if(_gridPositionsArray){
		LOG_ERROR_DEBUG("        GridPosition array was already defined right before its creation");
		return;
	}

	// define grid positions array-----------------------------
	_gridPositionsArray = new GridPosition***[X_BLOCKS];

	for (unsigned short x = 0; x < X_BLOCKS; ++x) {
		_gridPositionsArray[x] = new GridPosition**[Y_BLOCKS];

		for (unsigned short y = 0; y < Y_BLOCKS; ++y){
			_gridPositionsArray[x][y] = new GridPosition*[Z_BLOCKS];

			for (unsigned short z = 0; z < Z_BLOCKS; ++z){
				_gridPositionsArray[x][y][z] = new GridPosition(0, 0, STATE_NONE, x, y, z);
				if(x == 0)
					_gridPositionsArray[x][y][z]->sidesTouchingStaticCubes |= LEFT_SIDE;
				if(x == X_BLOCKS_MINUS_1)
					_gridPositionsArray[x][y][z]->sidesTouchingStaticCubes |= RIGHT_SIDE;
				if(y == 0)
					_gridPositionsArray[x][y][z]->sidesTouchingStaticCubes |= BOTTOM_SIDE;
				if(y == Y_BLOCKS_MINUS_1)
					_gridPositionsArray[x][y][z]->sidesTouchingStaticCubes |= TOP_SIDE;
			}
		}
	}

	setupGridPositionLinks();

	LOG_INFO_DEBUG("      Created GridPosition Grid array. Size: %5.0f Kb", (X_BLOCKS*Y_BLOCKS*Z_BLOCKS*sizeof(GridPosition))/1000.0f);

}

void Grid::setupGridPositionLinks(){

	GridPosition *position;

	for (unsigned int i = 0; i < X_BLOCKS; ++i) {

		for (unsigned int j = 0; j < Y_BLOCKS; ++j){

			for (unsigned int k = 0; k < Z_BLOCKS; ++k){
				position = _gridPositionsArray[i][j][k];
				setAdjacentGridPosRefs(position, i, j, k);
			}
		}
	}
}


void Grid::setAdjacentGridPosRefs(GridPosition *toPosition, GLuint x, GLuint y, GLuint z){

	if(x > 0)
		toPosition->leftGridPos = getGridElement(x-1, y, z);

	if(x < X_BLOCKS_MINUS_1)
		toPosition->rightGridPos = getGridElement(x+1, y, z);

	if(y > 0)
		toPosition->downGridPos = getGridElement(x, y-1, z);

	if(y < Y_BLOCKS_MINUS_1)
		toPosition->upGridPos = getGridElement(x, y+1, z);

	toPosition->farGridPos = getGridElement(x, y, (z+1) % Z_BLOCKS);

	toPosition->nearGridPos = getGridElement(x, y, (z + Z_BLOCKS_MINUS_1) % Z_BLOCKS);
}


void Grid::setGoalsAtEdges()
{
	LOG_INFO_DEBUG("     Setting goal grid as *edges*...");

	for(GLuint x = 0; x < X_BLOCKS; x++){

		for(GLuint y = 0; y < Y_BLOCKS; y++){

			//if edge has block, it is candidate, otherwise destination==========
			for(GLuint k = 0; k < Z_BLOCKS; k++)
			{
				GridPosition *position = getGridElement(x, y, k);
				if(x == 0 || x == X_BLOCKS_MINUS_1 || y == 0 || y == Y_BLOCKS_MINUS_1)
				{
					if(position->state == STATE_NONE)
					{
						position->isBasicGoalPosition = true;
					}
				}

				if(y > x){
					if(x < (Y_BLOCKS - y)){
						position->pullToWallsDirection = LEFT_SIDE;
						position->pullAgainstWallsDirection = RIGHT_SIDE;
					}
					else{
						position->pullToWallsDirection = TOP_SIDE;
						position->pullAgainstWallsDirection = BOTTOM_SIDE;
					}
				}
				else{
					if((X_BLOCKS - x) <= y){
						position->pullToWallsDirection = RIGHT_SIDE;
						position->pullAgainstWallsDirection = LEFT_SIDE;
					}
					else{
						position->pullToWallsDirection = BOTTOM_SIDE;
						position->pullAgainstWallsDirection = TOP_SIDE;
					}
				}

				position->setPullToWallsPosition();
			}
		}
	}

	LOG_INFO_DEBUG("     Set goal grid as *edges*.");
}


GridPosition *Grid::getGridElement(GLuint x, GLuint y, GLuint z)
{
	assert(_gridPositionsArray);

	return _gridPositionsArray[x][y][z];
}

