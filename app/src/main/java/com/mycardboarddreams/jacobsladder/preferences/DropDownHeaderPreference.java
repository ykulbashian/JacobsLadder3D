package com.mycardboarddreams.jacobsladder.preferences;

import android.content.Context;
import android.preference.Preference;
import android.util.AttributeSet;
import android.view.View;

public class DropDownHeaderPreference extends Preference {
	
	private View layout;

	public DropDownHeaderPreference(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}
	public DropDownHeaderPreference(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	public DropDownHeaderPreference(Context context) {
		super(context);
	}
	
	@Override
	protected void onBindView(View view) {
		super.onBindView(view);
		layout = view;
	}
	
	public void setOpenedState(boolean opened){
		if(layout != null){
			layout.setBackgroundResource(opened ? android.R.color.holo_blue_dark : android.R.color.transparent);
		}
	}

}
