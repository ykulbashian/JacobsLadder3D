package com.mycardboarddreams.jacobsladder.display;

import java.awt.Color;
import java.util.HashMap;

public abstract class DrawableMapping<T, S> extends HashMap<T, S>{
	
	private static final long serialVersionUID = 1L;

	public abstract Color getGraphColor();

}
