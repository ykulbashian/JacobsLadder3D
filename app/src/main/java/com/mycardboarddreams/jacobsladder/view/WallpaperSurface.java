package com.mycardboarddreams.jacobsladder.view;

import com.mycardboarddreams.jacobsladder.Dimensions;
import com.mycardboarddreams.jacobsladder.ThemesHelper;
import com.mycardboarddreams.jacobsladder.deluxe.BuildConfig;
import com.mycardboarddreams.jacobsladder.deluxe.R;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.content.res.Resources;
import android.preference.PreferenceManager;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

import java.util.Random;

public class WallpaperSurface extends SurfaceView implements Callback, OnSharedPreferenceChangeListener{

	private static final String LOG_TAG = WallpaperSurface.class.getSimpleName();

	private static int engineIdNum = -1;

	private static int getEngineId(){
		return engineIdNum--;
	}

	private int engineId;

	private boolean isReady = false;

	//Do not delete, used by native code
	protected long nativeHandle;

	public WallpaperSurface(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		initialize();
	}
	public WallpaperSurface(Context context, AttributeSet attrs) {
		super(context, attrs);
		initialize();
	}
	public WallpaperSurface(Context context) {
		super(context);
		initialize();
	}

	public void initialize(){
		Log.d(LOG_TAG, "Initialize");
		getHolder().addCallback(this);
		setWillNotDraw(false);

		engineId = getEngineId();
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		Log.d(LOG_TAG, "Surface Created");

		PreferenceManager.getDefaultSharedPreferences(getContext()).registerOnSharedPreferenceChangeListener(this);

		nativeSetSurface(engineId, getHolder().getSurface());

		nativeOnCreate(engineId);
		nativeOnStart(engineId);
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int _width, int _height) {
		Log.d(LOG_TAG, "Surface Changed");

		nativeOnPause(engineId);
		nativeSetSurface(engineId, getHolder().getSurface());
		isReady = true;

		postUpdateToUiThread();

	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		destroySurface();
	}

	public void destroySurface(){
		Log.d(LOG_TAG, "Surface Destroyed");
		isReady = false;

		PreferenceManager.getDefaultSharedPreferences(getContext()).unregisterOnSharedPreferenceChangeListener(this);


		nativeOnPause(engineId);
		nativeOnStop(engineId);

		nativeSetSurface(engineId, null);

		nativeOnFinish(engineId);
	}

	@Override
	public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
		Resources res = getResources();
		if(key.equals(res.getString(R.string.key_color_brightness)) ||
				key.equals(res.getString(R.string.key_complexity_level)) ||
				key.equals(res.getString(R.string.key_signal_composer_update)) ||
				//TODO this gets called too many times
				key.startsWith(res.getString(R.string.key_custom_color_prefix)))
		{
			postUpdateToUiThread();
		}

		if(key.equals(res.getString(R.string.key_select_theme))){
			ThemesHelper.setCurrentThemeToComposer(getContext());

			SharedPreferences.Editor editor = sharedPreferences.edit();
			editor.putString(res.getString(R.string.key_signal_composer_update), String.valueOf(new Random().nextInt()));
			editor.apply();
		}
	}

	private void postUpdateToUiThread()
	{
		if(!isReady)
			return;

		nativeOnPause(engineId);

		int[] colors = ThemesHelper.getDisplayColorSet(getContext());
		int[] adjusted = nativeSetupCustomColors(engineId, "custom", colors);

		Dimensions dimens = Dimensions.getDimensionsForComplexity(getContext());

		nativeSetupBlocks(engineId, dimens.columns, dimens.rows, dimens.depth, dimens.totalBlocks);

		nativeSetupSpeed(engineId, BuildConfig.DEBUG ? 10 : 7);

		//TODO What if it was paused?
		nativeOnResume(engineId);
	}

	private native void nativeOnCreate(int engineId);
	private native void nativeOnResume(int engineId);
	private native void nativeOnPause(int engineId);
	private native void nativeOnStop(int engineId);
	private native void nativeOnFinish(int engineId);
	private native void nativeOnStart(int engineId);

	private native void nativeSetSurface(int engineId, Surface surface);

	public native void nativeSetupBlocks(int engineId, int x, int y, int z, int total);
	public native int[] nativeSetupCustomColors(int engineId, String setName, int[] colors);
	public native void nativeSetupSpeed(int engineId, int speed);


	static {
		Log.d(LOG_TAG, "Loading C++ library");
		try{
			System.loadLibrary("native-lib");
		} catch (UnsatisfiedLinkError e) {
			// only ignore exception in non-android env
			if ("Dalvik".equals(System.getProperty("java.vm.name"))) throw e;
		}
		Log.d(LOG_TAG, "Done loading C++ library");
	}

}
