#include "Stats/statssnapshot.h"

#define LOG_TAG "StatsSnapshot"

std::string StatsSnapshot::startRecording(CubeDriver *driver, GLuint iterations, GLuint iterationSpan, GLfloat speed){

	std::string statsJson("");

	for(uint i = 0; i < iterations; i++){

		for(uint j = 0; j < iterationSpan; j++){
			driver->iterateOneFrame(static_cast<GLuint>(fabsf(speed*FLOATING_POINT_PRECISION)));
		}

		AggregateStats stats = driver->getAggregateStats();

		statsJson += stats.getAsJson();
	}

	return statsJson;
}