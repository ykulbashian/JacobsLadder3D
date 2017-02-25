#include "objectbuffers.h"

#define LOG_TAG "Buffer Binding"

void ObjectBuffers::unbindVertexBuffers()
{
	pthread_mutex_lock(&_mutex_buffer);

	if(!Constants::RENDER_OPENGL && !DRAW_CUBES && !DRAW_BACKGROUND) {
		pthread_mutex_unlock(&_mutex_buffer);
		return;
	}

	START_TIMER


	if(!cubeVBO_Pointers && !backgroundVBO_Pointers)
		LOG_INFO_DEBUG("    No vertex buffers to unbind.");
	else
		LOG_INFO_DEBUG("    Unbinding vertex buffers...");

	if(cubeVBO_Pointers && glIsBuffer(cubeVBO_Pointers[0]))
	{
		LOG_INFO_DEBUG("     Deleting Cube Vertex Buffer Objects...");
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(Z_BLOCKS, cubeVBO_Pointers);

	}

	if(backgroundVBO_Pointers && glIsBuffer(backgroundVBO_Pointers[0]))
	{
		LOG_INFO_DEBUG("     Deleting Background Vertex Buffer Objects...");
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(Z_BLOCKS, backgroundVBO_Pointers);
	}

	END_TIMER(SurfaceTimer::UNSET_VERTICES)

	pthread_mutex_unlock(&_mutex_buffer);
}


void ObjectBuffers::bindVerticesToBuffersInThread()
{
	pthread_mutex_lock(&_mutex_buffer);

	textures->loadTextureRAW();

	if(!Constants::RENDER_OPENGL && !DRAW_CUBES && !DRAW_BACKGROUND) {
		pthread_mutex_unlock(&_mutex_buffer);
		return;
	}

	START_TIMER

	unbindVertexBuffers();

	ColorSet::getCurrentColorSet()->setBackgroundColorInThread();

	LOG_INFO_DEBUG("   Generating background VBOs %p", BackgroundColoredVertices);

	glGenBuffers(Z_BLOCKS, backgroundVBO_Pointers);

	for(GLuint i = 0; i < Z_BLOCKS; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO_Pointers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(corner)*BACKGROUND_VERTEX_SIZE, BackgroundColoredVertices[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	LOG_INFO_DEBUG("   Generating cube VBOs %p", AllColoredVertexCorners);

	glGenBuffers(Z_BLOCKS, cubeVBO_Pointers);

	GLsizeiptr bufferSize = sizeof(corner)*FULL_VERTEX_BUFFER_PER_SLICE;

	LOG_INFO_DEBUG("   Generated/bound background buffers. (%d depth).", Z_BLOCKS);

	for(GLuint z = 0; z < Z_BLOCKS; z++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO_Pointers[z]);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, AllColoredVertexCorners[z], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//set up rendering-----------------------------
	if(Z_BLOCKS > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO_Pointers[0]);
	}

	LOG_INFO_DEBUG("   Generated/bound cube buffers");

	glVertexPointer(3, GL_FLOAT, sizeof(corner), (void*)0);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(corner), COLOR_OFFSET);
	glTexCoordPointer(2, GL_FLOAT, sizeof(corner), TEXTURE_OFFSET);

	LOG_INFO_DEBUG("  Finished setting vertices/color grid buffers."
					"\n    Vertex buffer has %d corner points"
					"\n    Each print array has %d spots"
					"\n    VBO buffer size %5.0f Kb, corner size %ld (Offsets %ld %ld)",
					FULL_VERTEX_BUFFER_PER_SLICE,
					FULL_VERTEX_BUFFER_PER_SLICE,
				    bufferSize/1000.0f,
				    sizeof(corner),
				    (intptr_t)COLOR_OFFSET,
				    (intptr_t)TEXTURE_OFFSET);

	END_TIMER(SurfaceTimer::SET_VERTICES)

	pthread_mutex_unlock(&_mutex_buffer);
}

