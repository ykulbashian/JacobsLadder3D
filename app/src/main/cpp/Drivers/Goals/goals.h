
#ifndef GOALS_H_
#define GOALS_H_

#include "Drivers/Goals/goalobject.h"
#include "Utils/iterativecollection.h"
#include "Drivers/Grid/gridposition.h"
#include "Geometry/dimension.h"
#include "Stats/goalstats.h"

enum GoalType{
	GOALS_NONE = 0,
	GOALS_EDGE_WALK = 1,
};

class Goals : public Dimension{

public:

	GoalType currentGoalType;

	IterativeCollection<GoalObject*> goalList = IterativeCollection<GoalObject*>(MAX_GOAL_POINTS + 20, GOAL_LIST);

	Goals();
	~Goals();

	void createNewGoalObjects(GridPosition ****gridPositions);

	void markTargetAsFailed(GridPosition *failedTarget, GLuint frustration);

	void deleteGoalGrids();
	void createNewGoalsGrid(GoalType type, Dimension dimens);

	void recycleViolatedGoalObjects(GridPosition ****grid, AllIterativeCollections *allCollections, unsigned int numSelected, bool needMoreGoals);
	void tryToViolateGoalToTouch(GridPosition *pos);

	void claimTargetAsOccupied(GridPosition *targetPosition);

	GridPosition *getRandomTarget(GridPosition *pos);

	void getGoalStats(GoalAggregateStats &aggrStats);

	void moveOutOfGoal(Cube *cube);

	int getNumAvailableTargets();

private:

	/*** Goal stats**/
	GLuint claimedAsOccupied;
	GLuint claimedAsFailed;
	GLuint noNearbyGoals;
	GLuint noTargetsForNearbyGoal;
	GLuint numDeletedGoals;
	GLuint numCreatedGoals;

	GoalStats historyOfStats[MAX_GOAL_POINTS];

	void resetStatsValues();

	GoalObject *getClosestGoal(GridPosition *pos);

	GoalObject *createRandomGoal(GridPosition ****grid, AllIterativeCollections *allCollections);

};




#endif /* GOALS_H_ */
