#ifndef IDENTITY_STATS_H_
#define IDENTITY_STATS_H_

#include <stdio.h>
#include <string>

struct GridStats {
public:
	unsigned int frustration;
	unsigned int moveAttempts;
	unsigned int numUnableToMove;
	unsigned int totalAttractIdentities;
	unsigned int unableToGiveTarget;
	unsigned int newlyStartedToMove;
	unsigned int numFloatingUnableToMove;
	unsigned int numStaticIdentities;
	unsigned int numCubesInAttractGoalPositions;
	unsigned int numEmptyGoals;
	unsigned int numPausedIdentities;
	unsigned int numMovingIdentities;
	unsigned int cubeReversedOnItself;
	unsigned int numFloatingUnableToMoveInRepel;
	unsigned int numInvalidIdents;
	unsigned int numMovingInvalid;
	unsigned int settingNewTarget;
	unsigned int foundAdjacentTarget;
	unsigned int numRandomizerFailed;
	unsigned int cantMoveInAnyDirection;
	unsigned int shouldntMoveInAnyDirection;

	unsigned int newDirectionLeft;
	unsigned int newDirectionRight;
	unsigned int newDirectionTop;
	unsigned int newDirectionBottom;
	unsigned int newDirectionNear;
	unsigned int newDirectionFar;

	std::string getAsJson(){
		char buffStart[695];
		snprintf(buffStart, sizeof(buffStart), "{frustration:\"%d\",moveAttempts:\"%d\",numUnableToMove:\"%d\","
						"totalAttractIdentities:\"%d\",unableToGiveTarget:\"%d\",newlyStartedToMove:\"%d\","
						"numFloatingUnableToMove:\"%d\",numStaticIdentities:\"%d\",numCubesInAttractGoalPositions:\"%d\","
						"numEmptyGoals:\"%d\",numPausedIdentities:\"%d\",numMovingIdentities:\"%d\","
						"cubeReversedOnItself:\"%d\",numFloatingUnableToMoveInRepel:\"%d\",numInvalidIdents:\"%d\",numMovingInvalid:\"%d\","
						"settingNewTarget:\"%d\",foundAdjacentTarget:\"%d\",numRandomizerFailed:\"%d\",cantMoveInAnyDirection:\"%d\","
						"shouldntMoveInAnyDirection:\"%d\","
						"newDirectionLeft:\"%d\",newDirectionRight:\"%d\",newDirectionTop:\"%d\","
						"newDirectionBottom:\"%d\",newDirectionNear:\"%d\",newDirectionFar:\"%d\"}",
				frustration,
				moveAttempts,
				numUnableToMove,
				totalAttractIdentities,
				unableToGiveTarget,
				newlyStartedToMove,
				numFloatingUnableToMove,
				numStaticIdentities,
				numCubesInAttractGoalPositions,
				numEmptyGoals,
				numPausedIdentities,
				numMovingIdentities,
				cubeReversedOnItself,
				numFloatingUnableToMoveInRepel,
				numInvalidIdents,
				numMovingInvalid,
				settingNewTarget,
				foundAdjacentTarget,
				numRandomizerFailed,
				cantMoveInAnyDirection,
				shouldntMoveInAnyDirection,

				newDirectionLeft,
				newDirectionRight,
				newDirectionTop,
				newDirectionBottom,
				newDirectionNear,
				newDirectionFar
		);

		return std::string(buffStart);
	}
};


#endif /* IDENTITY_STATS_H_ */
