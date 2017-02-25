
#ifndef DRIVERSTATS_H_
#define DRIVERSTATS_H_

#include <stdio.h>
#include <string>

struct DriverStats {
public:
	bool failedToGetToPriority;
	unsigned int numCubesMovedToSelect;
	unsigned int unableToGetCandidate;
	unsigned int priorityCubeCouldNotMove;
	unsigned int selectionWasInGoals;
	unsigned int numInUntenablePosition;

	std::string getAsJson(){
		char buffStart[250];
		snprintf(buffStart, sizeof(buffStart), "{failedToGetToPriority:\"%s\",numCubesMovedToSelect:\"%d\",unableToGetCandidate:\"%d\","
						"priorityCubeCouldNotMove:\"%d\",selectionWasInGoals:\"%d\",numInUntenablePosition:\"%d\"}",
				failedToGetToPriority ? "true" : "false",
				numCubesMovedToSelect,
				unableToGetCandidate,
				priorityCubeCouldNotMove,
				selectionWasInGoals,
				numInUntenablePosition);

		return std::string(buffStart);
	}

};


#endif /* DRIVERSTATS_H_ */
