
#include "Drawing/drawing.h"


#define LOG_TAG "Engine"


GLushort Drawing::backgroundIdsLeft[] = {0, 1, 2, 3, 4, 5};
GLushort Drawing::backgroundIdsBottom[] = {6, 7, 8, 9, 10, 11};
GLushort Drawing::backgroundIdsRight[] = {12, 13, 14, 15, 16, 17};
GLushort Drawing::backgroundIdsTop[] = {18, 19, 20, 21, 22, 23};

ObjectBuffers *Drawing::getBuffer(){
	return ObjectBuffers::getObjectBuffers();
}

Drawing::Drawing()
{
	cameraMovement = 0.0f;
	cameraMovementShift = 0;

	LOG_INFO_DEBUG("    Engine Created");
}

Drawing::~Drawing()
{
	LOG_INFO_DEBUG("    Deleted Engine.");
}


void Drawing::drawCubes(GLfloat fractionOfPreferredFrame,
		GLfloat angleOfRotation,
		Grid *grid,
		IterativeCollection<Cube*> *movingElements,
		Goals* goalSet)
{

	ObjectBuffers *pBuffers = getBuffer();

	pBuffers->lock();
	
	GLuint x_blocks = pBuffers->X_BLOCKS;
	GLuint y_blocks = pBuffers->Y_BLOCKS;
	GLuint z_blocks = pBuffers->Z_BLOCKS;

	GLushort *fullIndexList = pBuffers->fullIndexList;
	GLuint *fullIndexListCount = pBuffers->fullIndexListCount;

	if(pBuffers->colorSetHasBackground()){
		glShadeModel(GL_SMOOTH);

		for(GLuint slice = 0; slice < z_blocks; slice++)
			drawBackgroundSegment(angleOfRotation, slice);

		glShadeModel(GL_FLAT);
	}


	//update the camera movement
	cameraMovement += CAMERA_ITERATION*fractionOfPreferredFrame;

	if(cameraMovement >= 1.0f){
		cameraMovement = 0;
		if(++cameraMovementShift >= z_blocks) cameraMovementShift = 0;
	}

	if(!pBuffers->DRAW_CUBES)
		return;

	//go through each z-slice------------------------------------------
	Cube* c;
	GridPosition* thisIdentity;

	GLushort currentIndexPos = 0;
	GLuint printStart = 0, pointStart = 0;

	for(unsigned short z_ordinal = 0; z_ordinal < z_blocks; z_ordinal++){

		GLuint zCorrect = (z_ordinal + cameraMovementShift) % z_blocks;

		pointStart = printStart = zCorrect * (pBuffers->FULL_VERTEX_BUFFER_PER_SLICE);

		if(grid->_sliceIsDirty[zCorrect]){

			currentIndexPos = 0;

			for(unsigned short xNormal = 0; xNormal < x_blocks; xNormal++){

				for(unsigned short yNormal = 0; yNormal < y_blocks; yNormal++){

					thisIdentity = grid->getGridElement(xNormal, yNormal, zCorrect);
					assert(thisIdentity);

					c = thisIdentity->cube;

					//========================================================================
					//if moving or static in this position------------------------------------
					if(c && !c->isMoving())
					{
						pointStart = printStaticCube(thisIdentity->getDrawableFaces(), currentIndexPos, pointStart);
					}

					currentIndexPos += NUM_VERTICES_PER_CUBE;

				}//end Y

			}//end X

			fullIndexListCount[zCorrect] = pointStart;

		}//endif sliceIsDirty

		grid->_sliceIsDirty[zCorrect] = false;


		//set up rendering-----------------------------
		//TODO this throws SIGSEGV
		glBindBuffer(GL_ARRAY_BUFFER, pBuffers->cubeVBO_Pointers[z_ordinal]);

		glVertexPointer(3, GL_FLOAT, sizeof(corner), (void*)0);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(corner), pBuffers->COLOR_OFFSET);
		glTexCoordPointer(2, GL_FLOAT, sizeof(corner), pBuffers->TEXTURE_OFFSET);

		//set up translation--------------------------
		glLoadIdentity();
		glRotatef(angleOfRotation, 0.0f, 1.0f, 0.0f);
		glTranslatef(0, 0, CAMERA_BACK_OFFSET + cameraMovement - (float)(z_ordinal));

		glDrawElements(GL_TRIANGLE_STRIP, fullIndexListCount[zCorrect] - printStart,
							GL_UNSIGNED_SHORT, fullIndexList + printStart);


	}//end Z


	movingElements->lock();

	movingElements->resetIterator();

	//draw moving cubes and determine if they are finished------------------


	while(movingElements->hasNext())
	{
		c = movingElements->getNext();

		if(c->wasTouched) {
			glBlendFunc(GL_ONE, GL_DST_ALPHA);
		}

		drawMovingCube(angleOfRotation, c);
		if(c->wasTouched) {
			glBlendFunc(GL_ONE, GL_ZERO);
		}
	}


	movingElements->unlock();

	if(false)
		renderGoalBoxes(angleOfRotation, goalSet);

	pBuffers->unlock();

}


//print a static cube with restricted faces------------------------------------------
GLuint Drawing::printStaticCube(GLubyte faces, GLushort currentIndexPos, GLuint pointStart){

	GLushort init = currentIndexPos;

	ObjectBuffers *pBuffers = getBuffer();
	GLushort *fullIndexList = pBuffers->fullIndexList;

	// print left-right and top-bottom ---------
	if((faces & (X_DIR | BOTTOM_SIDE)) != 0){

		GLushort start = init;
		GLbyte num = 0;

		if((faces & X_DIR) == 0){
			start+=2;
			num = 2;
		}
		else if((faces & BOTTOM_SIDE) == 0){
			num = 2;
		}

		fullIndexList[pointStart++] = start;
		fullIndexList[pointStart++] = start;

		fullIndexList[pointStart++] = start;
		fullIndexList[pointStart++] = start + 1;
		fullIndexList[pointStart++] = start + 2;
		fullIndexList[pointStart++] = start + 3;
		fullIndexList[pointStart++] = start + 4;
		fullIndexList[pointStart++] = start + 5;


		pointStart -= num;

		GLuint i = pointStart - 2;
		fullIndexList[pointStart++] = fullIndexList[i];

		GLuint j = pointStart - 3;
		fullIndexList[pointStart++] = fullIndexList[j];
	}


	//print top and near side-------------------------
	if((faces & (TOP_SIDE | NEAR_SIDE)) != 0){

		GLushort start = init + (pBuffers->VERTICES_PER_SLICE);

		GLbyte num = 0;

		if((faces & TOP_SIDE) == 0){
			start+=2;
			num = 2;
		}
		else if((faces & NEAR_SIDE) == 0){
			num = 2;
		}

		fullIndexList[pointStart++] = start;
		fullIndexList[pointStart++] = start;

		fullIndexList[pointStart++] = start;
		fullIndexList[pointStart++] = start + 1;
		fullIndexList[pointStart++] = start + 2;
		fullIndexList[pointStart++] = start + 3;
		fullIndexList[pointStart++] = start + 4;
		fullIndexList[pointStart++] = start + 5;


		pointStart -= num;

		GLuint i = pointStart - 2;
		fullIndexList[pointStart++] = fullIndexList[i];

		GLuint j = pointStart - 3;
		fullIndexList[pointStart++] = fullIndexList[j];
	}

	return pointStart;
}



//=======================================================================
//draw moving cubes------------------------------------------------------
void Drawing::drawMovingCube(GLfloat angleOfRotation, Cube* c){
	GLfloat xPosf = 0.0f,
			yPosf = 0.0f,
			zPosf = 0.0f;

	GLfloat stepMovedFrag = ((GLfloat)c->stepMovedFragment)/(GLfloat)FLOATING_POINT_PRECISION;

	GLubyte moveDir = c->moveDirection;

	switch(moveDir){

		case LEFT_SIDE:
			xPosf = -stepMovedFrag;
			break;
		case RIGHT_SIDE:
			xPosf = stepMovedFrag;
			break;
		case TOP_SIDE:
			yPosf = stepMovedFrag;
			break;
		case BOTTOM_SIDE:
			yPosf = -stepMovedFrag;
			break;
		case NEAR_SIDE:
			zPosf = stepMovedFrag;
			break;
		case FAR_SIDE:
			zPosf = -stepMovedFrag;
			break;
		default:
			break;
	}

	drawSingleCube(angleOfRotation, xPosf, yPosf, zPosf, c->x, c->y, c->z);

}

void Drawing::drawSingleCube(GLfloat angleOfRotation, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLuint x, GLuint y, GLuint z)
{
	ObjectBuffers *pBuffers = getBuffer();
	
	unsigned short Y_BLOCKS = pBuffers->Y_BLOCKS;
	unsigned short Z_BLOCKS = pBuffers->Z_BLOCKS;

	GLushort start = (GLushort)((x*Y_BLOCKS + y)*NUM_VERTICES_PER_CUBE);

	GLushort movingArray[12] = {
		start,
		start + 1, start + 2, start + 3, start + 4, start + 5,
		start += (pBuffers->VERTICES_PER_SLICE),
		start + 1, start + 2, start + 3, start + 4, start + 5,
	};


	//set up rendering-----------------------------
	GLuint zCorrect = (z - cameraMovementShift + Z_BLOCKS) % Z_BLOCKS;

	glBindBuffer(GL_ARRAY_BUFFER, pBuffers->cubeVBO_Pointers[zCorrect]);

	glVertexPointer(3, GL_FLOAT, sizeof(corner), (void*)0);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(corner), pBuffers->COLOR_OFFSET);
	glTexCoordPointer(2, GL_FLOAT, sizeof(corner), pBuffers->TEXTURE_OFFSET);

	//set up translation---------------------------
	glLoadIdentity();
	glRotatef(angleOfRotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(xPos, yPos, zPos + CAMERA_BACK_OFFSET + cameraMovement - (float)zCorrect);

	glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_SHORT, movingArray);
}


void Drawing::drawBackgroundSegment(GLfloat angleOfRotation, GLuint slice)
{

	//set up rendering-----------------------------
	ObjectBuffers *pBuffers = getBuffer();
	
	glBindBuffer(GL_ARRAY_BUFFER, pBuffers->backgroundVBO_Pointers[slice]);

	glVertexPointer(3, GL_FLOAT, sizeof(corner), (void*)0);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(corner), pBuffers->COLOR_OFFSET);
	glTexCoordPointer(2, GL_FLOAT, sizeof(corner), pBuffers->TEXTURE_OFFSET);

	glLoadIdentity();

	glRotatef(angleOfRotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(0, 0, CAMERA_BACK_OFFSET + cameraMovement + (cameraMovementShift % 2) + 0.5f);
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, backgroundIdsLeft);
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, backgroundIdsRight);
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, backgroundIdsTop);
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, backgroundIdsBottom);


}

void Drawing::renderGoalBoxes(GLfloat angleOfRotation, Goals *goalSet) {
	IterativeCollection<GoalObject*> *goalList = &(goalSet->goalList);
	goalList->lock();
	goalList->resetIterator();

	while(goalList->hasNext()) {
		GoalObject *gObject = goalList->getNext();

		if (gObject->goalType != REPEL) {
			drawGoalSets(angleOfRotation, gObject);
		}
	}
	goalList->unlock();
}

void Drawing::drawGoalSets(GLfloat angleOfRotation, GoalObject* goal){

	ObjectBuffers *pBuffers = getBuffer();

	unsigned short X_BLOCKS = pBuffers->X_BLOCKS;
	unsigned short Y_BLOCKS = pBuffers->Y_BLOCKS;
	unsigned short Z_BLOCKS = pBuffers->Z_BLOCKS;


	GLushort x_startLower = (GLushort)((goal->x_start*Y_BLOCKS + goal->y_start)*NUM_VERTICES_PER_CUBE);
	GLushort x_startUpper = (GLushort)((goal->x_start*Y_BLOCKS + goal->y_end - 1)*NUM_VERTICES_PER_CUBE);
	GLushort x_endLower = (GLushort)(((goal->x_end - 1)*Y_BLOCKS + goal->y_start)*NUM_VERTICES_PER_CUBE);
	GLushort x_endUpper = (GLushort)(((goal->x_end - 1)*Y_BLOCKS + goal->y_end - 1)*NUM_VERTICES_PER_CUBE);

	GLint z_diff = goal->z_end - goal->z_start;

	GLushort movingArray[6] = {
			x_endUpper,
			x_endUpper + 1, x_endLower + 2, x_endLower + 3, x_startLower + 4, x_startLower + 5,
	};

	x_startLower += (pBuffers->VERTICES_PER_SLICE);
	x_startUpper += (pBuffers->VERTICES_PER_SLICE);
	x_endLower += (pBuffers->VERTICES_PER_SLICE);
	x_endUpper += (pBuffers->VERTICES_PER_SLICE);

	GLushort movingArray2[4] = {
			x_endUpper + 2, x_startUpper + 3, x_startLower + 5, x_endLower + 4,
	};


	//set up rendering-----------------------------
	GLuint zCorrectStart = (goal->z_start - cameraMovementShift + Z_BLOCKS) % Z_BLOCKS;

//	LOG_INFO_DEBUG("Drawing Goal... y_start %d,  y_end %d, z_start %d, x_start %d, x_end %d",
//				   goal->y_start, goal->y_end, zCorrectStart, goal->x_start, goal->x_end);

	glLineWidth(5.0f);

	glBindBuffer(GL_ARRAY_BUFFER, pBuffers->cubeVBO_Pointers[zCorrectStart]);

	glVertexPointer(3, GL_FLOAT, sizeof(corner), (void*)0);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(corner), pBuffers->COLOR_OFFSET);
	glTexCoordPointer(2, GL_FLOAT, sizeof(corner), pBuffers->TEXTURE_OFFSET);

	//set up translation---------------------------
	glLoadIdentity();
	glRotatef(angleOfRotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(0, 0, 0 + CAMERA_BACK_OFFSET + cameraMovement - (float)zCorrectStart);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, movingArray2);

	glTranslatef(0, 0, -z_diff);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, movingArray2);
}

