package com.mycardboarddreams.jacobsladder.preferences;

import android.content.Context;
import android.preference.TwoStatePreference;
import android.util.AttributeSet;
import android.view.View;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;

import com.mycardboarddreams.jacobsladder.deluxe.R;

public class CheckBoxVerticalPreference extends TwoStatePreference implements OnCheckedChangeListener {

	public CheckBoxVerticalPreference(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public CheckBoxVerticalPreference(Context context, AttributeSet attrs) {
        this(context, attrs, android.R.attr.checkBoxPreferenceStyle);
    }

    public CheckBoxVerticalPreference(Context context) {
        this(context, null);
    }
    
    private View mView;

    @Override
    protected void onBindView(View view) {
        super.onBindView(view);
        mView = view;
        
    	CheckBox checkboxView = (CheckBox)mView.findViewById(R.id.preference_checkbox_box);
    	checkboxView.setOnCheckedChangeListener(this);
    	
        refresh();
    }

    @Override
    public boolean isChecked() {
    	return getPersistedBoolean(true);
    }
    
    public void refresh(){
        
        if (mView != null) {
        	CheckBox chBox = (CheckBox) mView.findViewById(R.id.preference_checkbox_box);
            
        	if(chBox.isChecked() == isChecked())
        		return;
        	
            chBox.setChecked(isChecked());
        }
    }

	@Override
	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
		persistBoolean(isChecked);
	}

}
