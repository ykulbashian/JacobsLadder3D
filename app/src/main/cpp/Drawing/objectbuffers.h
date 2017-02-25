
#ifndef OBJECTBUFFERS_H_
#define OBJECTBUFFERS_H_

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>
#include <pthread.h>
#include "Geometry/corner.h"
#include "Geometry/dimension.h"
#include "SurfaceRenderer/surfaceutils.h"
#include "Drawing/ColorSets/colorset.h"
#include "textures.h"

#define BACKGROUND_VERTEX_SIZE 24

#define NUM_VERTICES_PER_CUBE 6

#define MEMBER_OFFSET(s,m) ((void *)(offsetof(s,m)))

class ObjectBuffers {

public:

	static ObjectBuffers *getObjectBuffers();

	static ObjectBuffers *objectBuffers;

	pthread_mutex_t _mutex_buffer;
	pthread_mutexattr_t Attr;

	corner **BackgroundColoredVertices;
	corner **AllColoredVertexCorners;

	bool DRAW_BACKGROUND;
	bool DRAW_CUBES;

	void* TEXTURE_OFFSET;
	void* COLOR_OFFSET;

	GLushort *fullIndexList;
	GLuint *fullIndexListCount;

	GLuint *cubeVBO_Pointers;
	GLuint *backgroundVBO_Pointers;

	GLint CUBES_PER_SLICE;
	GLshort VERTICES_PER_SLICE;
	GLint FULL_VERTEX_BUFFER_PER_SLICE;

	Dimension *currentSize;
	unsigned short X_BLOCKS, Y_BLOCKS, Z_BLOCKS;
	GLfloat X_BLOCKS_MID, Y_BLOCKS_MID;

	ObjectBuffers();
	virtual ~ObjectBuffers();

	void deriveVerticesFromDimensions(Dimension dimens);

	bool isSimilarDimensions(Dimension dimens);

	void bindVerticesToBuffersInThread();
	void unbindVertexBuffers();

	bool isBufferReady();

	bool tryRederiveVertices(Dimension newDimens);

	int getObjectBufferVersion();

	void lock();
	void unlock();

	bool colorSetHasBackground();

private:

	bool isReady;
	Textures *textures;

	int objectBufferVersion;

	void deriveIndexLists();

	virtual void setDimension(Dimension dimens);

	void deleteAllArrays();
	void createAllArrays();

	void deleteIndexLists();
	void deleteBackgroundArrays();
	void deleteGridArrays();
	void deleteBuffers();

	void deriveBackground();
	void deriveBackgroundSlice( GLuint z_near, GLuint z_far, corner *backgroundSegment);
	void deriveForegroundVertices();
	void initializeVBOs();

};


#endif /* OBJECTBUFFERS_H_ */
