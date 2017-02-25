#include "Drawing/objectbuffers.h"


//Background elements===============================
#define TEMP_ARRAY(array) {array[0],array[1],array[2],array[3]}


ObjectBuffers *ObjectBuffers::objectBuffers = new ObjectBuffers();

ObjectBuffers *ObjectBuffers::getObjectBuffers(){
	return objectBuffers;
}


#define LOG_TAG "Object buffers"

#define CUBE_HALF 0.50f

ObjectBuffers::ObjectBuffers() {

	textures = new Textures();

	BackgroundColoredVertices = 0;
	AllColoredVertexCorners = 0;

	objectBufferVersion = 0;

	fullIndexList = 0;
	fullIndexListCount = 0;

	cubeVBO_Pointers = 0;
	backgroundVBO_Pointers = 0;

	CUBES_PER_SLICE = 0;
	VERTICES_PER_SLICE = 0;
	FULL_VERTEX_BUFFER_PER_SLICE = 0;

	TEXTURE_OFFSET = MEMBER_OFFSET(corner, texture);
	COLOR_OFFSET = MEMBER_OFFSET(corner, color);

	DRAW_BACKGROUND = true;

	X_BLOCKS = 0;
	Y_BLOCKS = 0;
	Z_BLOCKS = 0;

	X_BLOCKS_MID = 0;
	Y_BLOCKS_MID = 0;

	DRAW_CUBES = true;

	currentSize = new Dimension();

	isReady = false;

	pthread_mutexattr_init(&Attr);
	pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&_mutex_buffer, &Attr);
}

ObjectBuffers::~ObjectBuffers(){
	deleteAllArrays();

	pthread_mutex_destroy(&_mutex_buffer);
}

bool ObjectBuffers::isSimilarDimensions(Dimension dimens){
	lock();

	bool isSimilar = currentSize->isSimilarDimensions(dimens);

	unlock();

	return isSimilar;
}

void ObjectBuffers::deleteAllArrays(){
	deleteIndexLists();
	deleteBackgroundArrays();
	deleteGridArrays();
	deleteBuffers();
}

void ObjectBuffers::createAllArrays(){
	deriveIndexLists();
	deriveBackground();
	deriveForegroundVertices();
	initializeVBOs();
}

bool ObjectBuffers::isBufferReady() {
	return isReady;
}

bool ObjectBuffers::tryRederiveVertices(Dimension newDimens) {

	bool colorHasRecentlyChanged = ColorSet::getCurrentColorSet()->colorHasRecentlyChanged;

	//TODO this should not care about colors
	if(!isSimilarDimensions(newDimens) || colorHasRecentlyChanged)
	{
		LOG_INFO_DEBUG("   Setting up scene...");

		deriveVerticesFromDimensions(newDimens);

		LOG_INFO_DEBUG("   Finished setting up scene.");

		ColorSet::getCurrentColorSet()->clearUpdateFlag();

		return true;
	} else {
		LOG_INFO_DEBUG("  No need to re-derive vertices...");
		return false;
	}

}

void ObjectBuffers::setDimension(Dimension dimens) {
	currentSize->setDimension(dimens);
	X_BLOCKS = dimens.X_BLOCKS;
	Y_BLOCKS = dimens.Y_BLOCKS;
	Z_BLOCKS = dimens.Z_BLOCKS;

	X_BLOCKS_MID = dimens.X_BLOCKS_MID;
	Y_BLOCKS_MID = dimens.Y_BLOCKS_MID;

	//setup the array of vertex positions for one XY slice of the grid
	CUBES_PER_SLICE = X_BLOCKS*Y_BLOCKS;
	VERTICES_PER_SLICE = CUBES_PER_SLICE * NUM_VERTICES_PER_CUBE;
	FULL_VERTEX_BUFFER_PER_SLICE = VERTICES_PER_SLICE * 2;
}

void ObjectBuffers::deriveIndexLists(){

	LOG_INFO_DEBUG("      Creating the (IBO) index lists...");

	//set up print array after initializing arrays------------
	fullIndexList = new GLushort[FULL_VERTEX_BUFFER_PER_SLICE * Z_BLOCKS];
	fullIndexListCount = new GLuint[Z_BLOCKS];

	for(GLuint i = 0; i < Z_BLOCKS; i++)
		fullIndexListCount[i] = 0;

	LOG_INFO_DEBUG("      Finished creating the (IBO) index lists...");
}

void ObjectBuffers::deleteIndexLists(){

	LOG_INFO_DEBUG("      Deleting the (IBO) index lists...");

	if(fullIndexList){
		delete[] fullIndexList;
		fullIndexList = 0;
	}

	if(fullIndexListCount){
		delete[] fullIndexListCount;
		fullIndexListCount = 0;
	}

	LOG_INFO_DEBUG("      Finished deleting the (IBO) index lists...");
}


void ObjectBuffers::deriveVerticesFromDimensions(Dimension dimens)
{
	isReady = false;

	lock();

	LOG_INFO_DEBUG("    Deriving vertices...");

	objectBufferVersion++;

	//delete existing Grid/background arrays======================
	deleteAllArrays();

	setDimension(dimens);

	//recreate Grid/background arrays============================

	ColorSet *colorSet = ColorSet::getCurrentColorSet();

	//initialize face colors---------------------------------
	colorSet->buildFaceColors(X_BLOCKS, Y_BLOCKS, Z_BLOCKS);

	createAllArrays();

	colorSet->deleteAllArrays();

	LOG_INFO_DEBUG("     Finished deriving vertices...");

	END_TIMER(SurfaceTimer::DERIVE_VERTICES)

	unlock();

	isReady = true;
}

void ObjectBuffers::initializeVBOs(){

	deleteBuffers();

	LOG_INFO_DEBUG("   Creating vertex buffer objects (background, cube)...");

	cubeVBO_Pointers = new GLuint[Z_BLOCKS];

	backgroundVBO_Pointers = new GLuint[Z_BLOCKS];

}

void ObjectBuffers::deleteBuffers(){

	unbindVertexBuffers();

	if(cubeVBO_Pointers || backgroundVBO_Pointers)
		LOG_INFO_DEBUG("   Deleting vertex buffer objects (background, cube)...");

	if(cubeVBO_Pointers){
		delete [] cubeVBO_Pointers;
		cubeVBO_Pointers = 0;
	}

	if(backgroundVBO_Pointers){
		delete [] backgroundVBO_Pointers;
		backgroundVBO_Pointers = 0;
	}

	LOG_INFO_DEBUG("   Finished deleting vertex buffer objects (background, cube)...");
}

void ObjectBuffers::deleteBackgroundArrays(){

	if(BackgroundColoredVertices){
		for(GLuint i = 0; i < Z_BLOCKS; i++)
			delete[] BackgroundColoredVertices[i];

		delete[] BackgroundColoredVertices;
		BackgroundColoredVertices = 0;
	}
}

void ObjectBuffers::deriveBackground()
{
	if(!DRAW_BACKGROUND)
		return;

	BackgroundColoredVertices = new corner*[Z_BLOCKS];

	for(GLuint i = 0; i < Z_BLOCKS; i++)
		BackgroundColoredVertices[i] = new corner[BACKGROUND_VERTEX_SIZE];

	START_TIMER

	for(GLuint i = 0; i < Z_BLOCKS; i++){
		deriveBackgroundSlice(i, i+1, BackgroundColoredVertices[i]);
	}

	END_TIMER(SurfaceTimer::DERIVE_BACKGROUND)

}


void ObjectBuffers::deriveBackgroundSlice(GLuint z_near, GLuint z_far, corner *backgroundSegment)
{
	GLfloat z_far_f = -((GLfloat)z_far);
	GLfloat z_near_f = -((GLfloat)z_near);
	GLfloat z_far_texture_f = -((z_far_f - z_near_f)/2.0f);

	ColorSet *colorSet = ColorSet::getCurrentColorSet();

	color_array **colorArrayTopLeft = colorSet->coloredVertices[0][Y_BLOCKS];
	color_array **colorArrayTopRight = colorSet->coloredVertices[X_BLOCKS][Y_BLOCKS];
	color_array **colorArrayBottomLeft = colorSet->coloredVertices[0][0];
	color_array **colorArrayBottomRight = colorSet->coloredVertices[X_BLOCKS][0];

	color_array **colorArrayTopMiddle = colorSet->coloredVertices[(GLuint)X_BLOCKS_MID][Y_BLOCKS];
	color_array **colorArrayRightMiddle = colorSet->coloredVertices[X_BLOCKS][(GLuint)Y_BLOCKS_MID];
	color_array **colorArrayLeftMiddle = colorSet->coloredVertices[0][(GLuint)Y_BLOCKS_MID];
	color_array **colorArrayBottomMiddle = colorSet->coloredVertices[(GLuint)X_BLOCKS_MID][0];


	/* LEFT WALL */

	backgroundSegment[0] = (corner){ //top left
				(Vertex)	{	-X_BLOCKS_MID, Y_BLOCKS_MID, z_far_f }, (Texture){	Y_BLOCKS_MID, z_far_texture_f, 0.0f	}, TEMP_ARRAY(colorArrayTopLeft[z_far][RIGHT_FACE]),	};
	backgroundSegment[1] = (corner){
				(Vertex)	{	-X_BLOCKS_MID, Y_BLOCKS_MID, z_near_f }, (Texture){	Y_BLOCKS_MID, 0.0f, 0.0f	}, TEMP_ARRAY(colorArrayTopLeft[z_near][RIGHT_FACE]),	};

	backgroundSegment[2] = (corner){ //middle left
				(Vertex)	{-X_BLOCKS_MID, 0, z_far_f }, (Texture){	Y_BLOCKS_MID/2.0f, z_far_texture_f, 0	},	TEMP_ARRAY(colorArrayLeftMiddle[z_far][RIGHT_FACE]),	};
	backgroundSegment[3] = (corner){
				(Vertex)	{-X_BLOCKS_MID, 0, z_near_f }, (Texture){	Y_BLOCKS_MID/2.0f, 0.0f, 0	},	TEMP_ARRAY(colorArrayLeftMiddle[z_near][RIGHT_FACE]),	};

	backgroundSegment[4] = (corner){ //bottom left
				(Vertex)	{-X_BLOCKS_MID, -Y_BLOCKS_MID, z_far_f }, (Texture){	0.0f, z_far_texture_f, 0.0f	}, TEMP_ARRAY(colorArrayBottomLeft[z_far][RIGHT_FACE]),	};
	backgroundSegment[5] = (corner){
				(Vertex)	{-X_BLOCKS_MID, -Y_BLOCKS_MID, z_near_f }, (Texture){	0.0f, 0.0f, 0.0f	}, TEMP_ARRAY(colorArrayBottomLeft[z_near][RIGHT_FACE]),	};


	/* BOTTOM WALL */

	backgroundSegment[6] = (corner){ //bottom left
				(Vertex)	{-X_BLOCKS_MID, -Y_BLOCKS_MID, z_far_f }, (Texture){	0.0f, z_far_texture_f, 0.0f	}, TEMP_ARRAY(colorArrayBottomLeft[z_far][TOP_FACE]),	};
	backgroundSegment[7] = (corner){
				(Vertex)	{-X_BLOCKS_MID, -Y_BLOCKS_MID, z_near_f }, (Texture){	0.0f, 0.0f, 0.0f	}, TEMP_ARRAY(colorArrayBottomLeft[z_near][TOP_FACE]),	};

	backgroundSegment[8] = (corner){ //bottom middle
				(Vertex)	{0, -Y_BLOCKS_MID, z_far_f }, (Texture){	X_BLOCKS_MID/2.0f, z_far_texture_f, 0.0f	},	TEMP_ARRAY(colorArrayBottomMiddle[z_far][TOP_FACE]),	};
	backgroundSegment[9] = (corner){
				(Vertex)	{0, -Y_BLOCKS_MID, z_near_f }, (Texture){	X_BLOCKS_MID/2.0f, 0.0f, 0.0f	},	TEMP_ARRAY(colorArrayBottomMiddle[z_near][TOP_FACE]),	};

	backgroundSegment[10] = (corner){ //bottom right
				(Vertex)	{X_BLOCKS_MID, -Y_BLOCKS_MID, z_far_f }, (Texture){	X_BLOCKS_MID, z_far_texture_f, 0.0f	}, TEMP_ARRAY(colorArrayBottomRight[z_far][TOP_FACE]),	};
	backgroundSegment[11] = (corner){
				(Vertex)	{X_BLOCKS_MID, -Y_BLOCKS_MID, z_near_f }, (Texture){ X_BLOCKS_MID, 0.0f, 0.0f	}, TEMP_ARRAY(colorArrayBottomRight[z_near][TOP_FACE]),	};


	/* RIGHT WALL */

	backgroundSegment[12] = (corner){ //bottom right
					(Vertex)	{X_BLOCKS_MID, -Y_BLOCKS_MID, z_far_f }, (Texture){	Y_BLOCKS_MID, z_far_texture_f, 0.0f	}, TEMP_ARRAY(colorArrayBottomRight[z_far][LEFT_FACE]),	};
	backgroundSegment[13] = (corner){
				(Vertex)	{X_BLOCKS_MID, -Y_BLOCKS_MID, z_near_f }, (Texture){ Y_BLOCKS_MID, 0.0f, 0.0f	}, TEMP_ARRAY(colorArrayBottomRight[z_near][LEFT_FACE]),	};

	backgroundSegment[14] = (corner){ //middle right
				(Vertex)	{X_BLOCKS_MID, 0, z_far_f }, (Texture){	Y_BLOCKS_MID/2.0f, z_far_texture_f, 0.0f	},	TEMP_ARRAY(colorArrayRightMiddle[z_far][LEFT_FACE]),	};
	backgroundSegment[15] = (corner){
				(Vertex)	{X_BLOCKS_MID, 0, z_near_f }, (Texture){	Y_BLOCKS_MID/2.0f, 0.0f, 0.0f	},	TEMP_ARRAY(colorArrayRightMiddle[z_near][LEFT_FACE]),	};

	backgroundSegment[16] = (corner){ //top right
				(Vertex)	{X_BLOCKS_MID, Y_BLOCKS_MID, z_far_f }, (Texture){	0.0f, z_far_texture_f, 0.0f	}, TEMP_ARRAY(colorArrayTopRight[z_far][LEFT_FACE]),	};
	backgroundSegment[17] = (corner){
				(Vertex)	{X_BLOCKS_MID, Y_BLOCKS_MID, z_near_f }, (Texture){	0.0f, 0.0f, 0.0f	}, TEMP_ARRAY(colorArrayTopRight[z_near][LEFT_FACE]),	};


	/* TOP WALL */

	backgroundSegment[18] = (corner){ //top right
				(Vertex)	{X_BLOCKS_MID, Y_BLOCKS_MID, z_far_f }, (Texture){	0.0f, z_far_texture_f, 0.0f	}, TEMP_ARRAY(colorArrayTopRight[z_far][BOTTOM_FACE]),	};
	backgroundSegment[19] = (corner){
				(Vertex)	{X_BLOCKS_MID, Y_BLOCKS_MID, z_near_f }, (Texture){	0.0f, 0.0f, 0.0f	}, TEMP_ARRAY(colorArrayTopRight[z_near][BOTTOM_FACE]),	};

	backgroundSegment[20] = (corner){ //top middle
				(Vertex)	{0, Y_BLOCKS_MID, z_far_f }, (Texture){	X_BLOCKS_MID/2.0f, z_far_texture_f, 0.0f	},	TEMP_ARRAY(colorArrayTopMiddle[z_far][BOTTOM_FACE]),	};
	backgroundSegment[21] = (corner){
				(Vertex)	{0, Y_BLOCKS_MID, z_near_f }, (Texture){	X_BLOCKS_MID/2.0f, 0.0f, 0.0f	},	TEMP_ARRAY(colorArrayTopMiddle[z_near][BOTTOM_FACE]),	};

	backgroundSegment[22] = (corner){ //top left
				(Vertex)	{	-X_BLOCKS_MID, Y_BLOCKS_MID, z_far_f }, (Texture){	0.0f, z_far_texture_f, 0.0f	}, TEMP_ARRAY(colorArrayTopLeft[z_far][BOTTOM_FACE]),	};
	backgroundSegment[23] = (corner){
				(Vertex)	{	-X_BLOCKS_MID, Y_BLOCKS_MID, z_near_f }, (Texture){	0.0f, 0.0f, 0.0f	}, TEMP_ARRAY(colorArrayTopLeft[z_near][BOTTOM_FACE]),	};


}

void ObjectBuffers::deleteGridArrays()
{
	LOG_INFO_DEBUG("  Deleting grid vertex arrays...");

	if(AllColoredVertexCorners){
		for(GLuint z = 0; z < Z_BLOCKS; z++)
			delete[] AllColoredVertexCorners[z];

		delete[] AllColoredVertexCorners;
		AllColoredVertexCorners = 0;
	}

	LOG_INFO_DEBUG("  Deleted grid vertex arrays.");
}


void ObjectBuffers::deriveForegroundVertices(){

	START_TIMER

	LOG_INFO_DEBUG("  Starting vertex/index arrays");
	AllColoredVertexCorners = new corner*[Z_BLOCKS];

	for(GLuint z = 0; z < Z_BLOCKS; z++)
		AllColoredVertexCorners[z] = new corner[VERTICES_PER_SLICE * 4];

	LOG_ERROR_DEBUG(" Size of AllColoredVertexCorners: %5.0f Kb", (Z_BLOCKS*VERTICES_PER_SLICE * 4*sizeof(corner))/1000.0f);

	ColorSet *colorSet = ColorSet::getCurrentColorSet();

	GLfloat X_BLOCKS_MID_OFFSET = 0.5f - X_BLOCKS_MID;
	GLfloat Y_BLOCKS_MID_OFFSET = 0.5f - Y_BLOCKS_MID;

	for(GLuint z = 0; z < Z_BLOCKS; z++){

		for(GLuint x = 0; x < X_BLOCKS; x++){

			for(GLuint y = 0; y < Y_BLOCKS; y++){

				GLuint currPos = (GLuint)(x*Y_BLOCKS + y)*NUM_VERTICES_PER_CUBE;

				color_array* cubeColorArray = colorSet->coloredVertices[(GLuint)x][(GLuint)y][z];

				bool leftSide = x < X_BLOCKS_MID;//, bottomSide = y < Y_BLOCKS_MID;
				float 	first_x = leftSide ? CUBE_HALF : -CUBE_HALF,
						second_x = leftSide ? -CUBE_HALF : CUBE_HALF;
/*
				8			1, 7
				X --------- X
				|\			|\
				| \			| \
				|  \ 10		|  \
				|  	X --------- X 2, 9
			  5 X --|------ X	|
				 \ 	|	   3 \	|
				  \	|		  \	|
				   \|		   \|
				    X --------- X
				6, 12			4, 11
 */
				//==============================================================
				//far top right (1)---------------------------------------------
				AllColoredVertexCorners[z][currPos] = (corner){
					(Vertex){ 	x + first_x + X_BLOCKS_MID_OFFSET,
								y + CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								second_x },
					(Texture){1.0f, 0.0f},
					{	cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][0],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][1],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][2],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][3]	},
				};

				//far top right (7)---------------------------------------------
				AllColoredVertexCorners[z][currPos+VERTICES_PER_SLICE] = (corner){
					(Vertex){ 	x + CUBE_HALF + X_BLOCKS_MID_OFFSET,
								y + CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								-CUBE_HALF },
					(Texture){0.5f, 0.0f},
					{	cubeColorArray[TOP_FACE][0],
						cubeColorArray[TOP_FACE][1],
						cubeColorArray[TOP_FACE][2],
						cubeColorArray[TOP_FACE][3]		},
				};

				//==============================================================
				//near top right (2)--------------------------------------------
				AllColoredVertexCorners[z][currPos+1] = (corner){
					(Vertex){ x + first_x + X_BLOCKS_MID_OFFSET,
							 y + CUBE_HALF + Y_BLOCKS_MID_OFFSET,
							 first_x },
					(Texture){0.5f, 0.0f},
					{	cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][0],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][1],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][2],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][3]	},
				};

				//far top left (8)----------------------------------------------
				AllColoredVertexCorners[z][currPos+1+VERTICES_PER_SLICE] = (corner){
					(Vertex){ 	x - CUBE_HALF + X_BLOCKS_MID_OFFSET,
								y + CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								-CUBE_HALF },
					(Texture){0.0f, 0.0f},
					{	cubeColorArray[TOP_FACE][0],
						cubeColorArray[TOP_FACE][1],
						cubeColorArray[TOP_FACE][2],
						cubeColorArray[TOP_FACE][3]		},
				};

				//==============================================================
				//far bottom right (3)------------------------------------------
				AllColoredVertexCorners[z][currPos+2] = (corner){
					(Vertex){ 	x + first_x + X_BLOCKS_MID_OFFSET,
								y - CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								second_x },
					(Texture){1.0f, 0.5f},
					{	cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][0],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][1],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][2],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][3]	},
				};

				//near top right (9)--------------------------------------------
				AllColoredVertexCorners[z][currPos+2+VERTICES_PER_SLICE] = (corner){
					(Vertex){ 	x + CUBE_HALF + X_BLOCKS_MID_OFFSET,
								y + CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								CUBE_HALF },
					(Texture){0.5f, 0.5f},
					{	cubeColorArray[TOP_FACE][0],
						cubeColorArray[TOP_FACE][1],
						cubeColorArray[TOP_FACE][2],
						cubeColorArray[TOP_FACE][3]		},
				};

				//==============================================================
				//near bottom right (4)-----------------------------------------
				AllColoredVertexCorners[z][currPos+3] = (corner){
					(Vertex){ 	x + first_x + X_BLOCKS_MID_OFFSET,
								y - CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								first_x },
					(Texture){0.5f, 0.5f},
					{	cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][0],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][1],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][2],
						cubeColorArray[leftSide ? RIGHT_FACE : LEFT_FACE][3]	},
				};

				//near top left (10)--------------------------------------------
				AllColoredVertexCorners[z][currPos+3+VERTICES_PER_SLICE] = (corner){
					(Vertex){ 	x - CUBE_HALF + X_BLOCKS_MID_OFFSET,
								y + CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								CUBE_HALF },
					(Texture){0.0f, 0.5f},
					{	cubeColorArray[TOP_FACE][0],
						cubeColorArray[TOP_FACE][1],
						cubeColorArray[TOP_FACE][2],
						cubeColorArray[TOP_FACE][3]		},
				};

				//==============================================================
				//far bottom left (5)-------------------------------------------
				AllColoredVertexCorners[z][currPos+4] = (corner){
					(Vertex){ 	x + second_x + X_BLOCKS_MID_OFFSET,
								y - CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								second_x },
					(Texture){1.0f, 1.0f},
					{	cubeColorArray[BOTTOM_FACE][0],
						cubeColorArray[BOTTOM_FACE][1],
						cubeColorArray[BOTTOM_FACE][2],
						cubeColorArray[BOTTOM_FACE][3]	},
				};

				//near bottom right (11)----------------------------------------
				AllColoredVertexCorners[z][currPos+4+VERTICES_PER_SLICE] = (corner){
					(Vertex){ 	x + CUBE_HALF + X_BLOCKS_MID_OFFSET,
								y - CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								CUBE_HALF },
					(Texture){0.5f, 1.0f},
					{	cubeColorArray[NEAR_FACE][0],
						cubeColorArray[NEAR_FACE][1],
						cubeColorArray[NEAR_FACE][2],
						cubeColorArray[NEAR_FACE][3]	},
				};

				//==============================================================
				//near bottom left (6)------------------------------------------
				AllColoredVertexCorners[z][currPos+5] = (corner){
					(Vertex){ 	x + second_x + X_BLOCKS_MID_OFFSET,
								y - CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								first_x },
					(Texture){0.5f, 1.0f},
					{	cubeColorArray[BOTTOM_FACE][0],
						cubeColorArray[BOTTOM_FACE][1],
						cubeColorArray[BOTTOM_FACE][2],
						cubeColorArray[BOTTOM_FACE][3]	},
				};

				//near bottom left (12)-----------------------------------------
				AllColoredVertexCorners[z][currPos+5+VERTICES_PER_SLICE] = (corner){
					(Vertex){ 	x - CUBE_HALF + X_BLOCKS_MID_OFFSET,
								y - CUBE_HALF + Y_BLOCKS_MID_OFFSET,
								CUBE_HALF },
					(Texture){0.0f, 1.0f},
					{	cubeColorArray[NEAR_FACE][0],
						cubeColorArray[NEAR_FACE][1],
						cubeColorArray[NEAR_FACE][2],
						cubeColorArray[NEAR_FACE][3]	},
				};

			}
		}

	}

	LOG_ERROR_DEBUG("    Size of Full Index List: %5.0f Kb", (FULL_VERTEX_BUFFER_PER_SLICE * Z_BLOCKS * sizeof(GLushort))/1000.0f);

}

bool ObjectBuffers::colorSetHasBackground() {
	return ColorSet::getCurrentColorSet()->currentSetHasBackground();
}

int ObjectBuffers::getObjectBufferVersion() {
	return objectBufferVersion;
}

void ObjectBuffers::unlock() {
	pthread_mutex_unlock(&_mutex_buffer);

}

void ObjectBuffers::lock() {
	pthread_mutex_lock(&_mutex_buffer);
}









