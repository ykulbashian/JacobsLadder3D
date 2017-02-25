package com.mycardboarddreams.jacobsladder;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.preference.PreferenceManager;

import com.mycardboarddreams.jacobsladder.deluxe.R;

public class Dimensions {

	final public int rows;
	final public int columns;
	final public int depth;

	final public int totalBlocks;
		
	public Dimensions(int rows, int columns, int depth, int totalBlocks){
		this.rows = rows;
		this.columns = columns;
		this.depth = depth;
		this.totalBlocks = totalBlocks;
	}

	public static Dimensions getDimensionsForComplexity(Context context){

		Resources res = context.getResources();
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);

		int complexityLevel = prefs.getInt(res.getString(R.string.key_complexity_level), res.getInteger(R.integer.default_complexity_level));

		int minComplexity = context.getResources().getInteger(R.integer.min_complexity_level);
		int maxComplexity = context.getResources().getInteger(R.integer.max_complexity_level);

		int minRows = context.getResources().getInteger(R.integer.min_y_blocks);
		int maxRows = context.getResources().getInteger(R.integer.max_y_blocks);

		int minCols = context.getResources().getInteger(R.integer.min_x_blocks);
		int maxCols = context.getResources().getInteger(R.integer.max_x_blocks);

		int minDepth = context.getResources().getInteger(R.integer.min_z_blocks);
		int maxDepth = context.getResources().getInteger(R.integer.max_z_blocks);

		int minTotal = context.getResources().getInteger(R.integer.min_total_blocks);
		int maxTotal = context.getResources().getInteger(R.integer.max_total_blocks);

		float complexityFraction = (float)(complexityLevel - minComplexity)/(float)(maxComplexity - minComplexity);

		int rows = minRows + (int)((maxRows - minRows)*complexityFraction);
		int columns = minCols + (int)((maxCols - minCols)*complexityFraction);
		int depth = minDepth + (int)((maxDepth - minDepth)*complexityFraction);
		int totalBlocks = minTotal + (int)((maxTotal - minTotal)*complexityFraction);

		return new Dimensions(rows, columns, depth, totalBlocks);
	}


}
