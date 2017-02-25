package com.mycardboarddreams.jacobsladder.preferences;

/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import com.mycardboarddreams.jacobsladder.deluxe.R;

import android.content.Context;
import android.content.res.TypedArray;
import android.os.Parcel;
import android.os.Parcelable;
import android.preference.Preference;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.View;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;

/**
 * @hide
 */
public class SeekBarPreference extends Preference implements OnSeekBarChangeListener {

    protected int currentValue;
    private int range;
    public int _max, _min, _default;
    private boolean mTrackingTouch;
    
    private SeekBar seekBar;
    
    public SeekBarPreference(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        
        TypedArray a = context.obtainStyledAttributes(attrs, R.styleable.ProgressBar, defStyle, 0);
        _max = a.getInt(R.styleable.ProgressBar_max, range);
        _min = a.getInt(R.styleable.ProgressBar_min, 0);
        _default = a.getInt(R.styleable.ProgressBar_default_value, _min);
        setMax(_max - _min);
        setCurrentValue(getPersistedInt(_default), false);
        a.recycle();
    			
        setLayoutResource(R.layout.preference_widget_seekbar);
    }

    public SeekBarPreference(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public SeekBarPreference(Context context) {
        this(context, null);
    }

    @Override
    protected void onBindView(View view) {
        super.onBindView(view);
        seekBar = (SeekBar) view.findViewById(R.id.seekbar);
        seekBar.setOnSeekBarChangeListener(this);
        seekBar.setMax(range);
        seekBar.setProgress(currentValue - _min);
        seekBar.setEnabled(isEnabled());
    }

    @Override
    public CharSequence getSummary() {
        return null;
    }
    
    @Override
    public boolean isEnabled() {
    	return true;
    }

    @Override
    protected void onSetInitialValue(boolean restoreValue, Object defaultValue) {
        setProgress(restoreValue ? getPersistedInt(currentValue) : (Integer) getDefaultValue());
    }
    
	public Object getDefaultValue() {
		return _default;
	}

    @Override
    protected Object onGetDefaultValue(TypedArray a, int index) {
    	_default = a.getInt(index, 0);
        return _default;
    }

//    @Override
    public boolean onKey(View v, int keyCode, KeyEvent event) {
        if (event.getAction() != KeyEvent.ACTION_UP) {
            if (keyCode == KeyEvent.KEYCODE_PLUS
                    || keyCode == KeyEvent.KEYCODE_EQUALS) {
                setProgress(getProgress() + 1);
                return true;
            }
            if (keyCode == KeyEvent.KEYCODE_MINUS) {
                setProgress(getProgress() - 1);
                return true;
            }
        }
        return false;
    }

    public void setMax(int max) {
        if (max != range) {
            range = max;
            notifyChanged();
        }
    }

    public void setProgress(int progress) {
        setCurrentValue(progress, true);
    }

    private void setCurrentValue(int value, boolean notifyChanged) { 
        if (value > _max) {
            value = _max;
        }
        if (value < _min) {
            value = _min;
        }
        if (value != currentValue) {
            currentValue = value;
            
        	persistInt(value);
            if (notifyChanged) {
                notifyChanged();
            }
        }
    }

    public int getProgress() {
        return currentValue;
    }

    /**
     * Persist the seekBar's progress value if callChangeListener
     * returns true, otherwise set the seekBar's progress to the stored value
     */
    void syncProgress(SeekBar seekBar) {
        int currentSeekbarProgress = seekBar.getProgress();
        if (currentSeekbarProgress != currentValue - _min) {
            if (callChangeListener(currentSeekbarProgress)) {
                setCurrentValue(currentSeekbarProgress + _min, false);
            } else {
                seekBar.setProgress(currentValue - _min);
            }
        }
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        if (fromUser && !mTrackingTouch) {
            syncProgress(seekBar);
        }
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {
        mTrackingTouch = true;
    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        mTrackingTouch = false;
        if (seekBar.getProgress() != currentValue) {
            syncProgress(seekBar);
        }
    }

    @Override
    protected Parcelable onSaveInstanceState() {
        /*
         * Suppose a client uses this preference type without persisting. We
         * must save the instance state so it is able to, for example, survive
         * orientation changes.
         */

        final Parcelable superState = super.onSaveInstanceState();
        if (isPersistent()) {
            // No need to save instance state since it's persistent
            return superState;
        }

        // Save the instance state
        final SavedState myState = new SavedState(superState);
        myState.progress = currentValue;
        myState.max = range;
        return myState;
    }

    @Override
    protected void onRestoreInstanceState(Parcelable state) {
        if (!state.getClass().equals(SavedState.class)) {
            // Didn't save state for us in onSaveInstanceState
            super.onRestoreInstanceState(state);
            return;
        }

        // Restore the instance state
        SavedState myState = (SavedState) state;
        super.onRestoreInstanceState(myState.getSuperState());
        currentValue = myState.progress;
        range = myState.max;
        notifyChanged();
    }

    /**
     * SavedState, a subclass of {@link BaseSavedState}, will store the state
     * of MyPreference, a subclass of Preference.
     * <p>
     * It is important to always call through to super methods.
     */
    private static class SavedState extends BaseSavedState {
        int progress;
        int max;

        public SavedState(Parcel source) {
            super(source);

            // Restore the click counter
            progress = source.readInt();
            max = source.readInt();
        }

        @Override
        public void writeToParcel(Parcel dest, int flags) {
            super.writeToParcel(dest, flags);

            // Save the click counter
            dest.writeInt(progress);
            dest.writeInt(max);
        }

        public SavedState(Parcelable superState) {
            super(superState);
        }

        @SuppressWarnings("unused")
        public static final Creator<SavedState> CREATOR =
                new Creator<SavedState>() {
            public SavedState createFromParcel(Parcel in) {
                return new SavedState(in);
            }

            public SavedState[] newArray(int size) {
                return new SavedState[size];
            }
        };
    }
}
