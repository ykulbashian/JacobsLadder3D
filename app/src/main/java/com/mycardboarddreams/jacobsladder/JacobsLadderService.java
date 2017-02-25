package com.mycardboarddreams.jacobsladder;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.GestureDetector;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import com.mycardboarddreams.jacobsladder.baselib.NativeGLWallpaperService;
import com.mycardboarddreams.jacobsladder.deluxe.BuildConfig;
import com.mycardboarddreams.jacobsladder.deluxe.R;

import io.fabric.sdk.android.Fabric;


public class JacobsLadderService extends NativeGLWallpaperService<JacobsLadderService.WhiteWallpaperEngine> {

	public static final String LOG_TAG = JacobsLadderService.class.getSimpleName();

	@Override
	protected WhiteWallpaperEngine getInstance()
	{
		return new WhiteWallpaperEngine();
	}

	@Override
	public void onCreate() {
		super.onCreate();

		try {
			Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
		} catch (Exception e){
			Log.e("No Fabric", "This only works once you set up fabric");
		}

		JacobsLadderService.nativeIsInDebugMode(BuildConfig.DEBUG);

		AnalyticsHelper.trackSetWallpaper(this);
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		AnalyticsHelper.trackUnSetWallpaper(this);
	}

	public class WhiteWallpaperEngine extends NativeGLWallpaperService<JacobsLadderService.WhiteWallpaperEngine>.NativeGLWallpaperEngine {

		private GestureDetector gestureDetector;

		Toast toast = Toast.makeText(getApplicationContext(), "", Toast.LENGTH_SHORT);


		WhiteWallpaperEngine() {
			super();

			Log.w(LOG_TAG, "Initializing Engine with id " + engineId);

			gestureDetector = new GestureDetector(JacobsLadderService.this, new GestureDetector.SimpleOnGestureListener() {
				@Override
				public boolean onDoubleTap(MotionEvent e) {
					if(isPaused())
						AnalyticsHelper.trackUnpauseWallpaperByTouch(JacobsLadderService.this);
					else
						AnalyticsHelper.trackPauseWallpaperByTouch(JacobsLadderService.this);

					pauseWallpaper(!isPaused());

					if(isVisible()){

						View loader = LayoutInflater.from(getApplicationContext()).inflate(R.layout.custom_toast, null);
						TextView tv = (TextView)loader.findViewById(R.id.toast_text_view);
						tv.setText(isPaused() ? R.string.paused : R.string.unpaused);

						toast.setView(loader);
						toast.setGravity(Gravity.CENTER, 0, 0);
						toast.show();
					}

					return true;
				}

				@Override
				public boolean onSingleTapConfirmed(MotionEvent event) {
					Log.d(LOG_TAG, "Single tap at " + event.getX() + ", " + event.getY() + ".");
					nativeTouchEvent(engineId, event.getX(), event.getY(), true);
					return super.onSingleTapConfirmed(event);
				}

			});

			toast.setGravity(Gravity.CENTER, 0, 0);
		}


		@Override
		public void onTouchEvent(MotionEvent event) {
			super.onTouchEvent(event);

			if(event == null || gestureDetector.onTouchEvent(event))
				return;

			if(isPaused())
				return;

			int action = event.getAction() & MotionEvent.ACTION_MASK;

			switch(action){
				case MotionEvent.ACTION_MOVE:

					nativeTouchEvent(engineId, event.getX(), event.getY(), false);

					break;
				case MotionEvent.ACTION_UP:
				case MotionEvent.ACTION_DOWN:
					break;
			}

		}

		@Override
		public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {

			if(key.equals(getString(R.string.key_select_theme))){
				ThemesHelper.setCurrentThemeToComposer(getBaseContext());
			}
		}

		@Override
		public void onOffsetsChanged(float xOffset, float yOffset,
									 float xOffsetStep, float yOffsetStep, int xPixelOffset, int yPixelOffset) {
			if(xOffsetStep == Float.NEGATIVE_INFINITY || xOffsetStep == Float.POSITIVE_INFINITY || xOffsetStep < 0) {
				setCurrentRotation(0.5f);
			} else {
				setCurrentRotation(xOffset);
			}
			setOffsets();
		}


		//Rotation variables-----------
		private float CURRENT_OFFSET_NORMALIZED = 0.5f;
		private static final float MAX_ROTATION_ANGLE = 30.0f;

		void setCurrentRotation(float fractionNotNormalized)
		{
			CURRENT_OFFSET_NORMALIZED = 2.0f*(fractionNotNormalized - 0.5f) * (MAX_ROTATION_ANGLE/45.0f);
		}

		@Override
		protected void onWallpaperResuming()
		{
			Dimensions dimens = Dimensions.getDimensionsForComplexity(JacobsLadderService.this);

			//TODO this is order dependent!
			int[] colors = ThemesHelper.getDisplayColorSet(JacobsLadderService.this);
			int[] newColors = nativeSetupCustomColors(engineId, "custom", colors);

			nativeSetupBlocks(engineId, dimens.columns, dimens.rows, dimens.depth, dimens.totalBlocks);
			nativeSetupSpeed(engineId, BuildConfig.DEBUG ? 10 : 7);

			setOffsets();
		}

		private void setOffsets(){
			float currentAngle = (float) Math.toDegrees(Math.atan(CURRENT_OFFSET_NORMALIZED));
			nativeSetupAngle(engineId, currentAngle);
		}

		//Native methods========================================

		public native void nativeSetupBlocks(int engineId, int x, int y, int z, int total);
		public native int[] nativeSetupCustomColors(int engineId, String setName, int[] colors);
		public native void nativeSetupSpeed(int engineId, int speed);
		public native void nativeSetupAngle(int engineId, float angle);

		public native void nativeTouchEvent(int engineId, float x, float y, boolean isSingleTap);

	}

	public static native void nativeIsInDebugMode(boolean debug);


	public static boolean isPremiumLegacy(Context context){
		SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(context);
		return sharedPreferences.getBoolean(context.getString(R.string.key_is_premium), false);
	}

}
