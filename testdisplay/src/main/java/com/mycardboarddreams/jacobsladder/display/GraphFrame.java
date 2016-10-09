package com.mycardboarddreams.jacobsladder.display;

import com.mycardboarddreams.jacobsladder.AggregateStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.ArrayStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.DriverStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.GoalStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.GoalsFrameStats;
import com.mycardboarddreams.jacobsladder.AggregateStats.IdentityGridStats;
import com.mycardboarddreams.jacobsladder.display.StrategiesImpl.DriverStatsAlgorithm;
import com.mycardboarddreams.jacobsladder.display.StrategiesImpl.FrameStatsAlgorithm;
import com.mycardboarddreams.jacobsladder.display.StrategiesImpl.GoalStatsAlgorithm;
import com.mycardboarddreams.jacobsladder.display.StrategiesImpl.IdentityGridStatsAlgorithm;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JScrollPane;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;

public class GraphFrame extends JFrame implements ActionListener, ItemListener{
	
	private static final long serialVersionUID = 1L;
	
	private static final int numRows = 14;
	
	List<AggregateStats> resultSamples;
	
	GraphDrawPanel drawPanel;
	
	HashMap<String, JButton> buttonArray = new HashMap<String, JButton>();
		
	
	public GraphFrame(String title){
        super(title);
        
        drawPanel = new GraphDrawPanel();
        drawPanel.setPreferredSize(new Dimension(3000, 350));
        
		setSize(1600, 850);
		
		relayoutFrame(null);
		
        setupMenus();
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        
		setVisible(true);
	}
	
	private void relayoutFrame(HashMap<String, List<GoalStats>> goalsStats){
		getContentPane().removeAll();
		buttonArray.clear();
		
		if(goalsStats == null)
			return;
		
		int totalNumGoals = goalsStats.values().size();
	
		setLayout(new GridBagLayout());
		
		StrategiesImpl.addButtonsToStrategies(this);

		GridBagConstraints constr = new GridBagConstraints();
		constr.fill = GridBagConstraints.BOTH;
		constr.gridx = 0;
		constr.gridy = numRows;
		constr.weightx = 1;
		constr.weighty = 1.0 - (0.002*numRows);
		constr.gridwidth = totalNumGoals;

        JScrollPane scrollPane = new JScrollPane(drawPanel);
        add(scrollPane, constr);
	}

	@Override
	public void actionPerformed(ActionEvent event) {
		String actionCommand = event.getActionCommand();

		try{
			FrameStatsAlgorithm frameStats = FrameStatsAlgorithm.valueOf(actionCommand);
			drawPanel.setFrameStatsAlgorithm(frameStats);

			forceRedraw();
			return;
		} catch(IllegalArgumentException e){

		}
		
		try{
			GoalStatsAlgorithm goalStats = GoalStatsAlgorithm.valueOf(actionCommand);
			drawPanel.setGoalStatsAlgorithm(goalStats);

			forceRedraw();
			return;
		} catch(IllegalArgumentException e){}
		
		try{
			IdentityGridStatsAlgorithm identityStats = IdentityGridStatsAlgorithm.valueOf(actionCommand);
			drawPanel.setIdentityStatsAlgorithm(identityStats);

			forceRedraw();
			return;
		} catch(IllegalArgumentException e){}
		
		try{
			DriverStatsAlgorithm identityStats = DriverStatsAlgorithm.valueOf(actionCommand);
			drawPanel.setDriverStatsAlgorithm(identityStats);

			forceRedraw();
			return;
		} catch(IllegalArgumentException e){}
	}
	
	
	public GraphFrame(){
        this("Results");
   }
	
	public void setData(List<AggregateStats> stats){
		resultSamples = stats;
		recompileStats();
		
		forceRedraw();
	}
	
	private void forceRedraw(){
		SwingUtilities.invokeLater(new Runnable() {
		    public void run() {
		    	drawPanel.repaint();
				
				validate();
		    }
	    });
	}
	

	private void recompileStats(){

		ArrayStats[][] recompiledStats = new ArrayStats[4][resultSamples.size()];
		HashMap<String, List<GoalStats>>goalsRecompile = new HashMap<String, List<GoalStats>>();
		List<GoalsFrameStats> goalFrameStats = new ArrayList<GoalsFrameStats>();
		List<IdentityGridStats> identityGridStats = new ArrayList<IdentityGridStats>();
		List<DriverStats> driverStats = new ArrayList<DriverStats>();
		
		int highestGoal = 0;
	  
		for(int aggrSample = 0; aggrSample < resultSamples.size(); aggrSample++){
			AggregateStats sample = resultSamples.get(aggrSample);
			
			goalFrameStats.add(sample.goalAggregateStats.frameStats);
			
			identityGridStats.add(sample.identityGridStats);
			
			driverStats.add(sample.driverStats);
			
			GoalStats[] goalStatsForSample = sample.goalAggregateStats.goalStats;

			for (GoalStats singleGoalStat : goalStatsForSample) {
				if (singleGoalStat == null || singleGoalStat.id == 0)
					continue;

				String id = String.valueOf(singleGoalStat.id);

				if (!goalsRecompile.containsKey(id)) {

					if (highestGoal < singleGoalStat.id)
						highestGoal = singleGoalStat.id;

					ArrayList<GoalStats> arrayList = new ArrayList<>();
					for (int i = 0; i < aggrSample; i++)
						arrayList.add(null);

					goalsRecompile.put(id, arrayList);
				}

				goalsRecompile.get(id).add(singleGoalStat);
			}
		
			
			ArrayStats[] iterativeArrayStats = sample.arrayStats;
			
			for(int j = 0; j < iterativeArrayStats.length; j++){
				ArrayStats singleIterativeArrayStat = iterativeArrayStats[j];
				
				if(singleIterativeArrayStat != null){
					recompiledStats[j][aggrSample] = singleIterativeArrayStat;
				}
			}
		}
		
		drawPanel.drawGoalsEach = new boolean[highestGoal];
		
		resetGoalsVisibilityToTrue();
		
		relayoutFrame(goalsRecompile);

		drawPanel.setStatsValues(resultSamples.size(), 
				recompiledStats, 
				goalsRecompile, 
				goalFrameStats, 
				identityGridStats,
				driverStats);
		
	}

	JCheckBoxMenuItem drawGoalsCheckBox, drawArraysCheckBox;
	
	private void setupMenus(){
		//Where the GUI is created:
		JMenuBar menuBar;
		JMenu menu;

		//Create the menu bar.
		menuBar = new JMenuBar();

		//Build the first menu.
		menu = new JMenu("Show");
		menu.setMnemonic(KeyEvent.VK_A);
		menu.getAccessibleContext().setAccessibleDescription(
		        "Show arrays");
		menuBar.add(menu);

		//a group of check box menu items
		menu.addSeparator();
		drawArraysCheckBox = new JCheckBoxMenuItem("Draw arrays");
		drawArraysCheckBox.setSelected(drawPanel.drawArrays);
		drawArraysCheckBox.setMnemonic(KeyEvent.VK_C);
		menu.add(drawArraysCheckBox);
		drawArraysCheckBox.addItemListener(this);

		drawGoalsCheckBox = new JCheckBoxMenuItem("Draw goals");
		drawGoalsCheckBox.setSelected(drawPanel.drawGoals);
		drawGoalsCheckBox.setMnemonic(KeyEvent.VK_H);
		menu.add(drawGoalsCheckBox);
		drawGoalsCheckBox.addItemListener(this);


		setJMenuBar(menuBar);
	}

	@Override
	public void itemStateChanged(ItemEvent e) {
		if(((JCheckBoxMenuItem)e.getItem()).getText().equals("Draw arrays")){
			drawPanel.drawArrays = (e.getStateChange() == ItemEvent.SELECTED);
		}
		if(((JCheckBoxMenuItem)e.getItem()).getText().equals("Draw goals")){
			drawPanel.drawGoals = (e.getStateChange() == ItemEvent.SELECTED);
			
			if(e.getStateChange() == ItemEvent.SELECTED)
				resetGoalsVisibilityToTrue();
		}
		
		forceRedraw();
	}

	private void resetGoalsVisibilityToTrue(){
		for(int i = 0; i < drawPanel.drawGoalsEach.length; i++)
			drawPanel.drawGoalsEach[i] = true;
	}
}
