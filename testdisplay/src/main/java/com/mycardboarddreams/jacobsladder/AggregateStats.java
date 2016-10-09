package com.mycardboarddreams.jacobsladder;

public class AggregateStats {
	
	public DriverStats driverStats;
	public GoalsAggregateStats goalAggregateStats;
	public ArrayStats[] arrayStats;
	public IdentityGridStats identityGridStats;
	

	public static class DriverStats{
		public boolean failedToGetToPriority;
		public int numCubesMovedToSelect;
		public int unableToGetCandidate;
		public int priorityCubeCouldNotMove;
		public int selectionWasInGoals;
		public int numInUntenablePosition;
	}

	public static class GoalsFrameStats{

		public int claimedAsOccupied;
		public int claimedAsFailed;
		public int noNearbyGoals;
		public int noTargetsForNearbyGoal;
		public int numDeletedGoals;
		public int numCreatedGoals;
		public int numTotalGoals;
		public int goalsMovingAverage;
		
	}
	
	public static class GoalStats {
		public int id;
		public int filled;
		public int capacity;
		public int numRequests;
		public int numFailed;
		public int filledMovingAverage;
		public int cumulativeFrustration;
		public int expectedTargetCountInCube;
		public int reasonForDestruction;
	}
	
	public static class GoalsAggregateStats{

		public GoalsFrameStats frameStats;
		
		public GoalStats[] goalStats;
	}
	
	public static class ArrayStats {
		public int filled;
		public int capacity;
		public int movingAverage;
	}
	
	public static class IdentityGridStats{
		public int frustration;
		public int moveAttempts;
		public int numUnableToMove;
		public int totalAttractIdentities;
		public int unableToGiveTarget;
		public int newlyStartedToMove;
		public int numFloatingUnableToMove;
		public int numStaticIdentities;
		public int numCubesInAttractGoalPositions;
		public int numEmptyGoals;
		public int numPausedIdentities;
		public int numMovingIdentities;
		public int cubeReversedOnItself;
		public int numFloatingUnableToMoveInRepel;
		public int numInvalidIdents;
		public int numMovingInvalid;
		public int settingNewTarget;
		public int foundAdjacentTarget;
		public int numRandomizerFailed;
		public int cantMoveInAnyDirection;
		public int shouldntMoveInAnyDirection;
		
		public int newDirectionLeft;
		public int newDirectionRight;
		public int newDirectionTop;
		public int newDirectionBottom;
		public int newDirectionNear;
		public int newDirectionFar;
	}
}
