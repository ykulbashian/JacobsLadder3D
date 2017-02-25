/*
 * Copyright (C) 2011 Sergey Margaritov
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

package net.margaritov.preference.colorpicker;

import android.content.Context;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.os.Bundle;
import android.os.Parcel;
import android.os.Parcelable;
import android.preference.Preference;
import android.util.AttributeSet;
import android.util.TypedValue;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.mycardboarddreams.jacobsladder.deluxe.R;

import java.util.Arrays;
import java.util.List;

/**
 * A preference type that allows a user to choose a time
 * @author Sergey Margaritov
 */
public class ColorPickerPreference
	extends
		Preference
	implements
		Preference.OnPreferenceClickListener,
		ColorPickerDialog.OnColorChangedListener,
        View.OnClickListener {
	
	enum ShapeType{
		rectangle,
		lightcircle,
	} 

	View mView;
	ColorPickerDialog mDialog;
	private int mValue = Color.BLACK;

	private boolean mAlphaSliderEnabled = false;
	private boolean mHexValueEnabled = false;
	
	private ShapeType shapeType;
	
	final private int BORDER_WIDTH_IN_DP = 1;
	final private float borderWidth;

	public native int[] nativeGetApparentCustomColors();

	public ColorPickerPreference(Context context) {
		super(context);
		Resources r = context.getResources();
		borderWidth = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, BORDER_WIDTH_IN_DP, r.getDisplayMetrics());
		init(context, null);
	}

	public ColorPickerPreference(Context context, AttributeSet attrs) {
		super(context, attrs);
		Resources r = context.getResources();
		borderWidth = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, BORDER_WIDTH_IN_DP, r.getDisplayMetrics());
		init(context, attrs);
	}

	public ColorPickerPreference(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		Resources r = context.getResources();
		borderWidth = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, BORDER_WIDTH_IN_DP, r.getDisplayMetrics());
		
		init(context, attrs);
	}

	@Override
	protected Object onGetDefaultValue(TypedArray a, int index) {
		return a.getColor(index, Color.BLACK);
	}
	
	public void refresh(){
		onColorChanged(getPersistedInt(mValue));
	}

	@Override
	protected void onSetInitialValue(boolean restoreValue, Object defaultValue) {
		onColorChanged(restoreValue ? getPersistedInt(mValue) : (Integer) defaultValue);
	}

	private void init(Context context, AttributeSet attrs) {
		setOnPreferenceClickListener(this);

		if (attrs != null) {
			mAlphaSliderEnabled = attrs.getAttributeBooleanValue(null, "alphaSlider", false);
			mHexValueEnabled = attrs.getAttributeBooleanValue(null, "hexValue", false);
			String typeAttribute = attrs.getAttributeValue(null, "type");
			
			if(typeAttribute != null)
				shapeType = ShapeType.valueOf(typeAttribute);
		}
	}
	
	@Override
	public View onCreateView(ViewGroup parent) {
		return new ImageView(getContext());
	}

	private int getApparentColor(){
		String key = getKey();
		List<String> allKeys = Arrays.asList(getContext().getResources().getStringArray(R.array.custom_color_all_keys));

		int position = allKeys.indexOf(key);

		if(position >= 0){
			return nativeGetApparentCustomColors()[position];
		}

		return 0;
	}
	
	
	@Override
	protected void onBindView(View view) {
		super.onBindView(view);
		mView = view;
		view.setOnClickListener(this);
		setPreviewColor();
	}

	private void setPreviewColor() {
		if (mView == null) return;
		ImageView widgetFrameView = ((ImageView)mView);

		GradientDrawable backgroundShape = new GradientDrawable();
		switch(shapeType){
		case rectangle:
			backgroundShape.mutate();
            backgroundShape.setShape(GradientDrawable.RECTANGLE);
            if(!getKey().contains("near_face"))
			    backgroundShape.setStroke((int)borderWidth, 0xFFEEEEEE);
			backgroundShape.setColor(mValue);
			break;
		default:
			backgroundShape.mutate();
            backgroundShape.setShape(GradientDrawable.OVAL);
			backgroundShape.setGradientType(GradientDrawable.RADIAL_GRADIENT);
			backgroundShape.setColors(new int[]{mValue, mValue, mValue & 0x00FFFFFF});
			backgroundShape.setGradientRadius(300);
			backgroundShape.setGradientCenter(0.5f, 0.5f);
		    break;
				
		}

        widgetFrameView.setImageDrawable(backgroundShape);

	}

	@Override
	public void onColorChanged(int color) {
		if (isPersistent()) {
			persistInt(color);
		}
		mValue = mAlphaSliderEnabled ? color : (color | 0xFF000000);
		setPreviewColor();
		try {
			getOnPreferenceChangeListener().onPreferenceChange(this, color);
		} catch (NullPointerException e) {

		}
	}

	public boolean onPreferenceClick(Preference preference) {
		showDialog(null);
		return false;
	}
	
	protected void showDialog(Bundle state) {
		mDialog = new ColorPickerDialog(getContext(), mValue);
		mDialog.setOnColorChangedListener(this);

		if (mAlphaSliderEnabled) {
			mDialog.setAlphaSliderVisible(true);
		}
		if (mHexValueEnabled) {
			mDialog.setHexValueEnabled(true);
		}
		if (state != null) {
			mDialog.onRestoreInstanceState(state);
		}
		mDialog.show();
	}

	/**
	 * Toggle Alpha Slider visibility (by default it's disabled)
	 * @param enable
	 */
	public void setAlphaSliderEnabled(boolean enable) {
		mAlphaSliderEnabled = enable;
	}

	/**
	 * Toggle Hex Value visibility (by default it's disabled)
	 * @param enable
	 */
	public void setHexValueEnabled(boolean enable) {
		mHexValueEnabled = enable;
	}

	/**
	 * For custom purposes. Not used by ColorPickerPreferrence
	 * @param color
	 * @author Unknown
	 */
    public static String convertToARGB(int color) {
        String alpha = Integer.toHexString(Color.alpha(color));
        String red = Integer.toHexString(Color.red(color));
        String green = Integer.toHexString(Color.green(color));
        String blue = Integer.toHexString(Color.blue(color));

        if (alpha.length() == 1) {
            alpha = "0" + alpha;
        }

        if (red.length() == 1) {
            red = "0" + red;
        }

        if (green.length() == 1) {
            green = "0" + green;
        }

        if (blue.length() == 1) {
            blue = "0" + blue;
        }

        return "#" + alpha + red + green + blue;
    }
    
    /**
	 * For custom purposes. Not used by ColorPickerPreference
	 * @param color
	 * @author Charles Rosaaen
	 * @return A string representing the hex value of color,
	 * without the alpha value
	 */
    public static String convertToRGB(int color) {
        String red = Integer.toHexString(Color.red(color));
        String green = Integer.toHexString(Color.green(color));
        String blue = Integer.toHexString(Color.blue(color));

        if (red.length() == 1) {
            red = "0" + red;
        }

        if (green.length() == 1) {
            green = "0" + green;
        }

        if (blue.length() == 1) {
            blue = "0" + blue;
        }

        return "#" + red + green + blue;
    }

    /**
     * For custom purposes. Not used by ColorPickerPreferrence
     * @param argb
     * @throws NumberFormatException
     * @author Unknown
     */
	public static int convertToColorInt(String argb) throws IllegalArgumentException {

		if (!argb.startsWith("#")) {
			argb = "#" + argb;
		}

		return Color.parseColor(argb);
	}
    
    @Override
    protected Parcelable onSaveInstanceState() {
        final Parcelable superState = super.onSaveInstanceState();
        if (mDialog == null || !mDialog.isShowing()) {
            return superState;
        }

        final SavedState myState = new SavedState(superState);
        myState.dialogBundle = mDialog.onSaveInstanceState();
        return myState;
    }

    @Override
    protected void onRestoreInstanceState(Parcelable state) {
        if (state == null || !(state instanceof SavedState)) {
            // Didn't save state for us in onSaveInstanceState
            super.onRestoreInstanceState(state);
            return;
        }

        SavedState myState = (SavedState) state;
        super.onRestoreInstanceState(myState.getSuperState());
        showDialog(myState.dialogBundle);
    }

    @Override
    public void onClick(View view) {
        showDialog(null);
    }

    private static class SavedState extends BaseSavedState {
        Bundle dialogBundle;
        
        public SavedState(Parcel source) {
            super(source);
            dialogBundle = source.readBundle();
        }

        @Override
        public void writeToParcel(Parcel dest, int flags) {
            super.writeToParcel(dest, flags);
            dest.writeBundle(dialogBundle);
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