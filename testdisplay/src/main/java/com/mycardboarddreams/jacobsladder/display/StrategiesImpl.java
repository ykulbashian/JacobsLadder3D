package com.mycardboarddreams.jacobsladder.display;

import com.mycardboarddreams.jacobsladder.AggregateStats.ArrayStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.DriverStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.GoalStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.GoalsFrameStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.IdentityGridStats;

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.util.HashMap;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JLabel;
import javax.swing.JRadioButton;

public class StrategiesImpl {

	public enum ArrayStatsAlgorithm{
		FilledPercentage("Filled %");
		
		public String name;
		
		ArrayStatsAlgorithm(String name){this.name = name;}
		@Override
		public String toString(){return name;}
		
	}
	public enum GoalStatsAlgorithm{
		FilledPercent("Filled %"),
		RequestPerCapacity("Request/Total"),
		FrustrationPerCapacity("Frustration/Total"),
		FailurePerCapacity("Failed/Total"),
		CapacityPerExpected("Capacity/Expected");
		
		public String name;
		
		GoalStatsAlgorithm(String name){this.name = name;}
		@Override
		public String toString(){return name;}
	}
	public enum FrameStatsAlgorithm{
		GoalsTotalNum("# Goals"),
		GoalsOccupied("# Occupied Targets"),
		GoalsFailed("# Failed Targets"),
		GoalsNoNearby("Found No Nearby Goals"),
		GoalsNoTargets("Found No Targets in Goal"),
		GoalsNumCreated("# Goals Created"),
		GoalsNumDeleted("# Goals Deleted"),
		GoalsFailedToRecreate("Recreate Goal Failed"),
		GoalsFilledMovingAverage("Total filled Average");
		
		public String name;
		
		FrameStatsAlgorithm(String name){this.name = name;}
		@Override
		public String toString(){return name;}
	}
	public enum IdentityGridStatsAlgorithm{
		Frustration("Cumulative Frustration"),
		NumAttemptMove("# Attempts to Move"),
		NumUnableToMove("# Attempts Were Unable to Move"),
		TotalNumAttracts("# 'Attract' Identities"),
		UnableToSetTarget("Can't Find Target"),
		NewlyStartedToMove("Just Started to Move"),
		NumFloatingUnableToMove("Floating Cubes That Can't Move"),
		NumFloatingUnableToMoveInRepel("Cubes in 'Repel' that Can't Move"),
		NumStaticIdents("# 'Static' Identities"),
		NumCubesInGoals("# Cubes in Goal Positions"),
		NumEmptyGoals("# Empty Goal Positions"),
		NumMovingIdentities("# 'Moving' Identities"),
		NumPausedIdentities("# 'Paused' Identities"),
		CubeReverseDir("Cubes that Reversed Dir"),
		NumInvalidIdents("# Identities in Invalid State"),
		NumMovingInvalid("# Move Directions in Invalid State"),
		SettingNewTarget("# Setting a New Target for Cube"),
		FoundAdjacentTarget("# Found Adjacent Target"),
		NumRandomizerFailed("# Randomizer Failed to Find Dir"),
		NumCantMove("# Can't Move in Any Dir"),
		NumShouldntMove("# Shouldn't Move in Any Dir"),
		
		NewDirectionLeft("Direction Left"),
		NewDirectionRight("Direction Right"),
		NewDirectionTop("Direction Top"),
		NewDirectionBottom("Direction Bottom"),
		NewDirectionNear("Direction Near"),
		NewDirectionFar("Direction Far");
		
		public String name;
		
		IdentityGridStatsAlgorithm(String name){this.name = name;}
		@Override
		public String toString(){return name;}
	}
	public enum DriverStatsAlgorithm{
		NumCubesMovedToSelect("# Cubes Moved to Selected"),
		FailedToGetPriority("Didn't Get to Priority List"),
		UnableToGetCandidate("Failed to Find a Candidate for Select"),
		PriorityCubeUnableMove("# Priority Cubes That Can't Move"),
		SelectionCubeWasInGoals("# Selected Cubes That Were in Goals"),
		NumInUntenablePosition("# Cubes That Were Floating");

		public String name;
		
		DriverStatsAlgorithm(String name){this.name = name;}
		@Override
		public String toString(){return name;}
	}
	

	@SuppressWarnings("serial")
	static DrawableMapping<ArrayStatsAlgorithm, GraphStrategy<ArrayStats>> arrayStatsMapping =
			new DrawableMapping<ArrayStatsAlgorithm, GraphStrategy<ArrayStats>>(){
		public Color getGraphColor() { return Color.BLACK; }
	};

	@SuppressWarnings("serial")
	static DrawableMapping<FrameStatsAlgorithm, GraphStrategy<GoalsFrameStats>> frameStatsMapping =
			new DrawableMapping<FrameStatsAlgorithm, GraphStrategy<GoalsFrameStats>>(){
		public Color getGraphColor() { return Color.BLACK; }
	};

	@SuppressWarnings("serial")
	static DrawableMapping<GoalStatsAlgorithm, GraphStrategy<GoalStats>> goalStatsMapping =
			new DrawableMapping<GoalStatsAlgorithm, GraphStrategy<GoalStats>>(){
		public Color getGraphColor() { return new Color(0x009900); }
	};

	@SuppressWarnings("serial")
	static DrawableMapping<IdentityGridStatsAlgorithm, GraphStrategy<IdentityGridStats>> identityGridStatsMapping =
			new DrawableMapping<IdentityGridStatsAlgorithm, GraphStrategy<IdentityGridStats>>(){
		public Color getGraphColor() { return new Color(0x0099FF); }
	};

	@SuppressWarnings("serial")
	static DrawableMapping<DriverStatsAlgorithm, GraphStrategy<DriverStats>> driverStatsMapping =
			new DrawableMapping<DriverStatsAlgorithm, GraphStrategy<DriverStats>>(){
		public Color getGraphColor() { return new Color(0x0000FF); }
	};
	
	
	public static void addButtonsToStrategies(GraphFrame graphFrame) {

		addGroupTitle(graphFrame, 0, "General");

		addButtonRow(graphFrame, 0, FrameStatsAlgorithm.GoalsTotalNum, frameStatsMapping, stat -> (float)stat.numTotalGoals/30);

		addButtonRow(graphFrame, 0, FrameStatsAlgorithm.GoalsOccupied, frameStatsMapping, stat -> (float)stat.claimedAsOccupied/2000);
		
		addButtonRow(graphFrame, 0, FrameStatsAlgorithm.GoalsFailed, frameStatsMapping, stat -> (float)stat.claimedAsFailed/1000);
		
		addButtonRow(graphFrame, 0, FrameStatsAlgorithm.GoalsNoNearby, frameStatsMapping, stat -> (float)stat.noNearbyGoals/1000);
		
		addButtonRow(graphFrame, 0, FrameStatsAlgorithm.GoalsNoTargets, frameStatsMapping, stat -> (float)stat.noTargetsForNearbyGoal);
		
		addButtonRow(graphFrame, 0, FrameStatsAlgorithm.GoalsNumCreated, frameStatsMapping, stat -> (float)stat.numCreatedGoals/10);
		
		addButtonRow(graphFrame, 0, FrameStatsAlgorithm.GoalsNumDeleted, frameStatsMapping, stat -> (float)stat.numDeletedGoals/10);
		
		addButtonRow(graphFrame, 0, FrameStatsAlgorithm.GoalsFailedToRecreate, frameStatsMapping, stat -> (float)(stat.numDeletedGoals - stat.numCreatedGoals)/3f);
		
		addButtonRow(graphFrame, 0, FrameStatsAlgorithm.GoalsFilledMovingAverage, frameStatsMapping, stat -> (float)(stat.goalsMovingAverage)/300f);



		addGroupTitle(graphFrame, 1, "Goals");

		addButtonRow(graphFrame, 1, GoalStatsAlgorithm.FilledPercent, goalStatsMapping, stat -> ((float)stat.filledMovingAverage)/((float)(stat.capacity)));

		addButtonRow(graphFrame, 1, GoalStatsAlgorithm.RequestPerCapacity, goalStatsMapping, stat -> 0.02f*((float)stat.numRequests)/((float)(stat.capacity)));
		
		addButtonRow(graphFrame, 1, GoalStatsAlgorithm.FrustrationPerCapacity, goalStatsMapping, stat -> 0.01f*((float)stat.cumulativeFrustration)/((float)(stat.capacity)));
		
		addButtonRow(graphFrame, 1, GoalStatsAlgorithm.FailurePerCapacity, goalStatsMapping, stat -> 0.02f*((float)stat.numFailed)/((float)(stat.capacity)));
		
		addButtonRow(graphFrame, 1, GoalStatsAlgorithm.CapacityPerExpected, goalStatsMapping, stat -> ((float)stat.capacity)/((float)(stat.expectedTargetCountInCube)));


		addGroupTitle(graphFrame, 2, "GridPosition Grid");

		final float DIRECTION_Q = 800.0f;

		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.Frustration, identityGridStatsMapping, stat -> (float)stat.frustration/16000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumAttemptMove, identityGridStatsMapping, stat -> (float)stat.moveAttempts/15000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumUnableToMove, identityGridStatsMapping, stat -> stat.numUnableToMove/15000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.TotalNumAttracts, identityGridStatsMapping, stat -> stat.totalAttractIdentities/7000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.UnableToSetTarget, identityGridStatsMapping, stat -> stat.unableToGiveTarget/100f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NewlyStartedToMove, identityGridStatsMapping, stat -> stat.newlyStartedToMove/1000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumFloatingUnableToMove, identityGridStatsMapping, stat -> stat.numFloatingUnableToMove/3000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumFloatingUnableToMoveInRepel, identityGridStatsMapping, stat -> stat.numFloatingUnableToMoveInRepel/3000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumStaticIdents, identityGridStatsMapping, stat -> stat.numStaticIdentities/4000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumCubesInGoals, identityGridStatsMapping, stat -> stat.numCubesInAttractGoalPositions/2000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumEmptyGoals, identityGridStatsMapping, stat -> stat.numEmptyGoals/2000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumPausedIdentities, identityGridStatsMapping, stat -> stat.numPausedIdentities/500f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumMovingIdentities, identityGridStatsMapping, stat -> stat.numMovingIdentities/2000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.CubeReverseDir, identityGridStatsMapping, stat -> stat.cubeReversedOnItself/2000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumInvalidIdents, identityGridStatsMapping, stat -> (float)(stat.numInvalidIdents)/3000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumMovingInvalid, identityGridStatsMapping, stat -> (float)(stat.numMovingInvalid)/6000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.SettingNewTarget, identityGridStatsMapping, stat -> (float)(stat.settingNewTarget)/2000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.FoundAdjacentTarget, identityGridStatsMapping, stat -> (float)(stat.foundAdjacentTarget)/200f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumRandomizerFailed, identityGridStatsMapping, stat -> (float)(stat.numRandomizerFailed)/2000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumCantMove, identityGridStatsMapping, stat -> (float)(stat.cantMoveInAnyDirection)/2000f);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NumShouldntMove, identityGridStatsMapping, stat -> (float)(stat.shouldntMoveInAnyDirection)/2000f);

		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NewDirectionLeft, identityGridStatsMapping, stat -> stat.newDirectionLeft/DIRECTION_Q);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NewDirectionRight, identityGridStatsMapping, stat -> stat.newDirectionRight/DIRECTION_Q);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NewDirectionTop, identityGridStatsMapping, stat -> stat.newDirectionTop/DIRECTION_Q);

		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NewDirectionBottom, identityGridStatsMapping, stat -> stat.newDirectionBottom/DIRECTION_Q);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NewDirectionNear, identityGridStatsMapping, stat -> stat.newDirectionNear/DIRECTION_Q);
		
		addButtonRow(graphFrame, 2, IdentityGridStatsAlgorithm.NewDirectionFar, identityGridStatsMapping, stat -> stat.newDirectionFar/DIRECTION_Q);


		addGroupTitle(graphFrame, 3, "Cube Driver");


		addButtonRow(graphFrame, 3, DriverStatsAlgorithm.NumCubesMovedToSelect, driverStatsMapping, stat -> (float)stat.numCubesMovedToSelect/200);

		addButtonRow(graphFrame, 3, DriverStatsAlgorithm.FailedToGetPriority, driverStatsMapping, stat -> stat.failedToGetToPriority ? 1f : 0f);
		
		addButtonRow(graphFrame, 3, DriverStatsAlgorithm.UnableToGetCandidate, driverStatsMapping, stat -> (float)stat.unableToGetCandidate/300);
		
		addButtonRow(graphFrame, 3, DriverStatsAlgorithm.PriorityCubeUnableMove, driverStatsMapping, stat -> ((float)stat.priorityCubeCouldNotMove)/1000f);
		
		addButtonRow(graphFrame, 3, DriverStatsAlgorithm.SelectionCubeWasInGoals, driverStatsMapping, stat -> (float)stat.selectionWasInGoals/500);

		addButtonRow(graphFrame, 3, DriverStatsAlgorithm.NumInUntenablePosition, driverStatsMapping, stat -> (float)stat.numInUntenablePosition/500);

		
	}
	
	
	private static Map<Integer, ButtonGroup> radioButtonMap = new HashMap<>();

	private static void addGroupTitle(GraphFrame frame, int group, String title){

		JLabel label = new JLabel();
		label.setText(title);

		GridBagConstraints c = new GridBagConstraints();
		c.gridx = group*3;
		c.gridy = 0;
		c.weighty = 0.02f;
		c.weightx = 0.1f;
		frame.add(label, c);
	}
	
	public static <T extends Enum<T>, S> void addButtonRow
		(GraphFrame frame, int group, T enumer, DrawableMapping<T, GraphStrategy<S>> mapping, GraphStrategy<S> strategy){
		
		JRadioButton radioItem = new JRadioButton(enumer.toString());
		radioItem.setActionCommand(enumer.name());
		radioItem.addActionListener(frame);

		radioItem.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 1));
	    
	    if(!radioButtonMap.containsKey(group)){
	    	radioButtonMap.put(group, new ButtonGroup());
	    	radioItem.setSelected(true);
	    }

    	radioItem.setForeground(mapping.getGraphColor());
    	int row = radioButtonMap.get(group).getButtonCount();
	    
	    radioButtonMap.get(group).add(radioItem);

		GridBagConstraints c = new GridBagConstraints();
		c.gridx = group*3 + (row/9);
		c.gridy = row%9 + 1;
		c.weighty = 0.02f;
		c.weightx = 0.1f;
		frame.add(radioItem, c);
		
		mapping.put(enumer, strategy);
	}

	
	public static GraphStrategy<ArrayStats> filledPercent = stat -> (((float)stat.filled)/((float)stat.capacity));
	
	static {
		arrayStatsMapping.put(ArrayStatsAlgorithm.FilledPercentage, filledPercent);
	}
	
	public static GraphStrategy<ArrayStats> getArrayStats(ArrayStatsAlgorithm type){
		if(arrayStatsMapping.containsKey(type))
			return arrayStatsMapping.get(type);
		
		return filledPercent;
	}
	
	

	
	public static GraphStrategy<GoalStats> getGoalStats(GoalStatsAlgorithm type){
		if(goalStatsMapping.containsKey(type))
			return goalStatsMapping.get(type);
		
		return null;
	}
	
	
	public static GraphStrategy<GoalsFrameStats> getFrameStats(FrameStatsAlgorithm type){
		if(frameStatsMapping.containsKey(type))
			return frameStatsMapping.get(type);
		
		return null;
	}

	public static GraphStrategy<IdentityGridStats> getIdentityGridStats(IdentityGridStatsAlgorithm type){
		if(identityGridStatsMapping.containsKey(type))
			return identityGridStatsMapping.get(type);
		
		return null;
	}
	
	
	public static GraphStrategy<DriverStats> getDriverStats(DriverStatsAlgorithm type){
		if(driverStatsMapping.containsKey(type))
			return driverStatsMapping.get(type);
		
		return null;
	}

}
