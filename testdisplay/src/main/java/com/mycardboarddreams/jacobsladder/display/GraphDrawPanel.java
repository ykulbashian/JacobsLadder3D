package com.mycardboarddreams.jacobsladder.display;

import com.mycardboarddreams.jacobsladder.AggregateStats.ArrayStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.DriverStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.GoalStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.GoalsFrameStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.IdentityGridStats;
import com.mycardboarddreams.jacobsladder.display.StrategiesImpl.ArrayStatsAlgorithm;
import com.mycardboarddreams.jacobsladder.display.StrategiesImpl.DriverStatsAlgorithm;
import com.mycardboarddreams.jacobsladder.display.StrategiesImpl.FrameStatsAlgorithm;
import com.mycardboarddreams.jacobsladder.display.StrategiesImpl.GoalStatsAlgorithm;
import com.mycardboarddreams.jacobsladder.display.StrategiesImpl.IdentityGridStatsAlgorithm;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Ellipse2D;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.JPanel;

public class GraphDrawPanel extends JPanel {
	
	private enum GraphType{
		LineGraph,
		BarGraph,
	}
	
	boolean drawGoals = true;
	boolean drawArrays = true;
	boolean drawGoalsEach[];
	
	private static final int MARGIN = 40;
	
	private static final int NUM_AXIS_VALUES = 20;
	
	private final int CIRCLE_RADIUS = 6;
	private final int HALF_CIRCLE_RADIUS = 3;

	ArrayStats[][] statsRecompile;
	HashMap<String, List<GoalStats>> goalsRecompile;
	List<GoalsFrameStats> goalFrameRecompile;
	List<IdentityGridStats> identityGridStats;
	List<DriverStats> driverStats;
	
	private static final long serialVersionUID = 1L;
	
	private final Color[] arrayColors = {
			new Color(0xFF9900), //moving Orange
			Color.MAGENTA, //candidate Purple
			new Color(0xFF0000), //priority Red
			Color.PINK, //select Green
	};
	
	private final Color[] destructionColors = {
			new Color(0x000000), //none
			new Color(0xFF9900), //flattenedOut
			new Color(0x009900), //tooFewTargets
			new Color(0xFF0000), //frustration
			new Color(0x3366FF), //tooManyRequests
			new Color(0x9966CC), //goalIsViolated
	};
	
	private final String[] destructionReasons = {
			"None",
			"Flattened Out",
			"Too Few Targets",
			"Frustration",
			"Too Many Requests",
			"Goal Is Violated",
	};
	
	private final String[] cubeArrayContents = {
			"Moving",
			"Candidate",
			"Priority",
			"Selected",
	};
	
	private FrameStatsAlgorithm FRAME_STATS_ALGORITHM = FrameStatsAlgorithm.GoalsOccupied;
	private GoalStatsAlgorithm GOAL_STATS_ALGORITHM = GoalStatsAlgorithm.FilledPercent;
	private IdentityGridStatsAlgorithm IDENTITY_STATS_ALGORITHM = IdentityGridStatsAlgorithm.Frustration;
	private DriverStatsAlgorithm DRIVER_STATS_ALGORITHM = DriverStatsAlgorithm.NumCubesMovedToSelect;
	
	private int samples = 1;
	private float heightInsideMargin = 1;
	private float sampleStepSize = 1;
	
	public void setDrawBooleans(boolean drawArrays, boolean drawGoals, boolean[] drawGoalsEach){
		this.drawArrays = drawArrays;
		this.drawGoals = drawGoals;
		this.drawGoalsEach = drawGoalsEach;
	}
	
	public void setStatsValues(int totalSamples, 
			ArrayStats[][] statsRecompile, 
			HashMap<String, List<GoalStats>> goalsRecompile, 
			List<GoalsFrameStats> goalFrameRecompile,
			List<IdentityGridStats> identityGridStats,
			List<DriverStats> driverStats){
		
		samples = totalSamples;
		
		this.statsRecompile = statsRecompile;
		this.goalsRecompile = goalsRecompile;
		this.goalFrameRecompile = goalFrameRecompile;
		this.identityGridStats = identityGridStats;
		this.driverStats = driverStats;
	}
	
	public void setFrameStatsAlgorithm(FrameStatsAlgorithm algo){
		FRAME_STATS_ALGORITHM = algo;
	}
	
	public void setGoalStatsAlgorithm(GoalStatsAlgorithm algo){
		GOAL_STATS_ALGORITHM = algo;
	}
	
	public void setIdentityStatsAlgorithm(IdentityGridStatsAlgorithm algo){
		IDENTITY_STATS_ALGORITHM = algo;
	}
	
	public void setDriverStatsAlgorithm(DriverStatsAlgorithm algo){
		DRIVER_STATS_ALGORITHM = algo;
	}
	

	public void paintComponent(Graphics g){
		super.paintComponent(g);
		
		if(goalsRecompile == null || statsRecompile == null || goalFrameRecompile == null)
			return;
		
		Graphics2D g2 = (Graphics2D) g.create();
		
		setDrawParameters();
		
		if(drawGoals){
			drawGoalStats(g2, goalsRecompile, StrategiesImpl.getGoalStats(GOAL_STATS_ALGORITHM));
			drawStats(g2, goalFrameRecompile, StrategiesImpl.getFrameStats(FRAME_STATS_ALGORITHM), StrategiesImpl.frameStatsMapping.getGraphColor());
		}
		
		if(drawArrays)
			drawArrayStats(g2, statsRecompile, StrategiesImpl.getArrayStats(ArrayStatsAlgorithm.FilledPercentage));

		drawStats(g2, driverStats,  StrategiesImpl.getDriverStats(DRIVER_STATS_ALGORITHM), StrategiesImpl.driverStatsMapping.getGraphColor());

		drawStats(g2, identityGridStats, StrategiesImpl.getIdentityGridStats(IDENTITY_STATS_ALGORITHM), StrategiesImpl.identityGridStatsMapping.getGraphColor());
		
		drawAxis(g2);
		
		drawLegend(g2);
		
		g2.dispose();
		
   }
	
	private void setDrawParameters(){
		heightInsideMargin = (float)getHeight() - 2*MARGIN;
		sampleStepSize = (((float)getWidth()-MARGIN)/(samples + 1));
	}
	
	private void drawLegend(Graphics2D g2){
		g2.setColor(new Color(0xFFEEEEEE));
		g2.fillRect(20, 20, 130, 100);
		g2.setStroke(new BasicStroke(1));
		g2.setColor(new Color(0xFF222222));
		g2.drawRect(20, 20, 130, 100);
		
		for(int i = 0; i < destructionColors.length; i++){
			g2.setColor(destructionColors[i]);
			g2.drawString(destructionReasons[i], 20 + 10, 35 + i*15 );
		}
		
		int additionalHeight = 130;
		
		g2.setColor(new Color(0xFFEEEEEE));
		g2.fillRect(20, 20 + additionalHeight, 130, 70);
		g2.setStroke(new BasicStroke(1));
		g2.setColor(new Color(0xFF222222));
		g2.drawRect(20, 20 + additionalHeight, 130, 70);
		
		for(int i = 0; i < arrayColors.length; i++){
			g2.setColor(arrayColors[i]);
			g2.drawString(cubeArrayContents[i], 20 + 10, 35 + i*15 + additionalHeight );
		}
	}
	
	private void drawAxis(Graphics2D g2){
		int stepSize = samples/NUM_AXIS_VALUES;
		g2.setColor(Color.BLACK);
		
		float height = (float)getHeight();
		
		for(int i = 0; i < samples; i += stepSize){
			g2.drawString(String.valueOf(i), sampleStepSize*i, (int)(height - 5) );
		}
		
	}
	
	
	private void drawArrayStats(Graphics2D g2, ArrayStats[][] recompiledArrayStats, GraphStrategy<ArrayStats> drawingStrategy){
		
		for(int arrayIndex = 0; arrayIndex < recompiledArrayStats.length; arrayIndex++){
			ArrayStats[] iterativeArrayStats = recompiledArrayStats[arrayIndex];
			List<ArrayStats> statsAsList = Arrays.asList(iterativeArrayStats);
			
			g2.setColor(arrayColors[arrayIndex]);
			g2.setStroke(new BasicStroke(2));
			
			drawSingleGraphLine(g2, statsAsList, drawingStrategy, GraphType.LineGraph);
			
		}
	}
	
	private <T> void drawSingleGraphLine(Graphics2D g2, Collection<T> pointsArray, GraphStrategy<T> drawingStrategy, GraphType type){
		int currXIndex = 0;
		int previousHeight = (int)heightInsideMargin + MARGIN;
		
		for(T statSample : pointsArray){
							
			int currHeight = (int)(heightInsideMargin - heightInsideMargin*drawingStrategy.newValueFraction(statSample)) + MARGIN;
			if(type == GraphType.LineGraph)
				g2.drawLine((int)((currXIndex - 1)*sampleStepSize), previousHeight, (int)(currXIndex*sampleStepSize), currHeight);
			else if(type == GraphType.BarGraph)
				g2.drawLine((int)((currXIndex)*sampleStepSize), (int)heightInsideMargin + MARGIN, (int)(currXIndex*sampleStepSize), currHeight);
			
			previousHeight = currHeight;
			
			currXIndex++;
		}
	}
	

	private void drawGoalStats(Graphics2D g2, HashMap<String, List<GoalStats>> statsRecompile, GraphStrategy<GoalStats> strategy){
		float avgFilled = 0;
		int totalNumGoals = 0;
		
		Font textFont = new Font("Arial", Font.PLAIN, 10);  
		g2.setFont(textFont);

		for (Map.Entry<String, List<GoalStats>> pairs : statsRecompile.entrySet()) {

			int key = Integer.valueOf(pairs.getKey());

			List<GoalStats> array = pairs.getValue();

			if (array == null || key >= drawGoalsEach.length || !drawGoalsEach[key]) {
				continue;
			}

			int currXIndex = 0;
			int previousHeight = (int) heightInsideMargin + MARGIN;
			Color graphColor = new Color(120, 120 + (int) (120 * Math.random()), 120);

			g2.setColor(graphColor);

			GoalStats lastStat = null;

			for (GoalStats stat : array) {

				if (stat != null) {
					lastStat = stat;

					int currHeight = (int) (heightInsideMargin - heightInsideMargin * strategy.newValueFraction(stat)) + MARGIN;
					g2.drawLine((int) ((currXIndex - 1) * sampleStepSize), previousHeight, (int) (currXIndex * sampleStepSize), currHeight);
					previousHeight = currHeight;

				}

				currXIndex++;
			}

			Color destrColor = destructionColors[lastStat == null ? 0 : lastStat.reasonForDestruction];

			g2.setColor(destrColor);

			if (lastStat != null && lastStat.capacity > 0) {
				g2.setColor(Color.WHITE);
				g2.drawString(String.valueOf(lastStat.id), (int) ((currXIndex - 1) * sampleStepSize) - 9, previousHeight - 9);
				g2.setColor(destrColor);
				g2.drawString(String.valueOf(lastStat.id), (int) ((currXIndex - 1) * sampleStepSize) - 10, previousHeight - 10);
				avgFilled += (float) lastStat.filled / (float) lastStat.capacity;
				totalNumGoals++;
			}

			g2.fill(new Ellipse2D.Double((int) ((currXIndex - 1) * sampleStepSize) - HALF_CIRCLE_RADIUS, previousHeight - HALF_CIRCLE_RADIUS, CIRCLE_RADIUS, CIRCLE_RADIUS));

		}
	    
	    avgFilled /= (float)totalNumGoals;
	    System.out.println(" Avg: " + avgFilled*100 + "% over " + totalNumGoals + " goals");
	}
	
	
	private <T> void drawStats(Graphics2D g2, List<T> statsList, GraphStrategy<T> strategy, Color color){
		g2.setStroke(new BasicStroke(1));
		g2.setColor(color);
		g2.setStroke(new BasicStroke(2));
		drawSingleGraphLine(g2, statsList, strategy, GraphType.LineGraph);
	}
	
	
	
}
