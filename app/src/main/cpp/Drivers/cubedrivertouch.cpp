#include "Drivers/cubedriver.h"
#include "Drawing/drawing.h"

#define LOG_TAG "Cube Driver Touch"

void CubeDriver::onTouchEvent(GLint x, GLint y, GLfloat xConst, GLfloat yConst, GLuint cameraMovementShift, GLboolean isSingleTap)
{
	GLuint _x = 0, _y = 0;
	GridPosition *position;

	if(	!allArrayCollections.movingElements ||
		!allArrayCollections.movingElements->hasRoom())
		return;

	if(!_grid.isReady())
		return;

	for(GLuint z = 0; z < Z_BLOCKS; z++){

		_x = (GLint)((xConst)*(CUBE_SIDE*(GLfloat)z - CAMERA_BACK_OFFSET)) + X_BLOCKS_MID;
		_y = (GLint)((yConst)*(CUBE_SIDE*(GLfloat)z - CAMERA_BACK_OFFSET)) + Y_BLOCKS_MID;

		if(_x >= X_BLOCKS || _y >= Y_BLOCKS)
			break;

		position = _grid.getGridElement(_x, _y, ((z + cameraMovementShift) % Z_BLOCKS));

		Cube *cube = position->cube;
		if(cube){
			cube->wasTouched = true;

			if(isSingleTap){
				goalSet.tryToViolateGoalToTouch(position);
			}
			else if(!cube->isMoving()) {
				allArrayCollections.addCubeToList(cube, PRIORITY_LIST);
				setSurroundFromTouch(position);
			}
			break;

		}
	}
}

bool CubeDriver::setSurroundFromTouch(GridPosition *touchedGridPosition)
{
	GLuint 	x = touchedGridPosition->x,
			y = touchedGridPosition->y;
	Cube *cube;

	if(x > 0){
		cube = touchedGridPosition->leftGridPos->cube;
		if(cube && (touchedGridPosition->leftGridPos->state == STATE_STATIC)) {
			allArrayCollections.addCubeToList(cube, PRIORITY_LIST);
			cube->wasTouched = true;
		}
	}

	if(x < X_BLOCKS-1){
		cube = touchedGridPosition->rightGridPos->cube;
		if(cube && (touchedGridPosition->rightGridPos->state == STATE_STATIC)) {
			allArrayCollections.addCubeToList(cube, PRIORITY_LIST);
			cube->wasTouched = true;
		}
	}

	if(y > 0){
		cube = touchedGridPosition->downGridPos->cube;
		if(cube && (touchedGridPosition->downGridPos->state == STATE_STATIC)) {
			allArrayCollections.addCubeToList(cube, PRIORITY_LIST);
			cube->wasTouched = true;
		}
	}

	if(y < Y_BLOCKS-1){
		cube = touchedGridPosition->upGridPos->cube;
		if(cube && (touchedGridPosition->upGridPos->state == STATE_STATIC)) {
			allArrayCollections.addCubeToList(cube, PRIORITY_LIST);
			cube->wasTouched = true;
		}
	}

	cube = touchedGridPosition->nearGridPos->cube;
	if(cube && (touchedGridPosition->nearGridPos->state == STATE_STATIC)) {
		allArrayCollections.addCubeToList(cube, PRIORITY_LIST);
		cube->wasTouched = true;
	}

	cube = touchedGridPosition->farGridPos->cube;
	if(cube && (!touchedGridPosition->farGridPos->state == STATE_STATIC)) {
		allArrayCollections.addCubeToList(cube, PRIORITY_LIST);
		cube->wasTouched = true;
	}

	return true;
}

