package com.mycardboarddreams.jacobsladder.baselib;

import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.preference.PreferenceManager;
import android.service.wallpaper.WallpaperService;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

import com.mycardboarddreams.jacobsladder.BuildConfig;

public abstract class NativeGLWallpaperService<T extends NativeGLWallpaperService<T>.NativeGLWallpaperEngine> extends WallpaperService {

	private static final String LOG_TAG = NativeGLWallpaperService.class.getSimpleName();

	private static int ENGINE_ID = 1;

	/**
	 * Get an instance of your subclass of NativeGLWallpaperEngine.
	 *
	 * @return instance of your Engine subclass
	 */
	protected abstract T getInstance();


	@Override
	public Engine onCreateEngine() {
		Log.d(LOG_TAG, "Creating Engine Instance..");

		return getInstance();
	}

	/**
	 * The actual wallpaper engine. Many instances of your engine will be created,
	 * some simultaneously. Each has its own unique id in the field {@link #engineId engineId}
	 *
	 */
	public abstract class NativeGLWallpaperEngine extends Engine implements OnSharedPreferenceChangeListener {

		//Do not delete, used by native code
		protected long nativeHandle;

		private boolean paused = false;

		/**
		 * The id of this specific Engine instance
		 */
		protected int engineId = 0;

		{
			engineId = ENGINE_ID++;
		}

		/**
		 * Called when the wallpaper is going from an off state to
		 * on; this includes on orientation change.  Use this to set up
		 * preferences in the wallpaper
		 */
		protected abstract void onWallpaperResuming();

		@Override
		public void onCreate(SurfaceHolder surfaceHolder) {
			super.onCreate(surfaceHolder);
			if(BuildConfig.DEBUG)
				Log.d(LOG_TAG, "onCreate called (" + engineId + ")");

			nativeOnCreate(engineId);
			nativeOnStart(engineId);

			if(BuildConfig.DEBUG)
				Log.d(LOG_TAG, "onCreate done with pointer (" + Long.toHexString(nativeHandle) + ")");

			PreferenceManager.getDefaultSharedPreferences(NativeGLWallpaperService.this).registerOnSharedPreferenceChangeListener(this);
		}

		@Override
		public void onDestroy() {
			super.onDestroy();

			if(BuildConfig.DEBUG)
				Log.d(LOG_TAG, "On Destroy (" + engineId + ")");

			nativeOnPause(engineId);
			nativeOnStop(engineId);
			nativeOnFinish(engineId);

			Log.d(LOG_TAG, "onDestroy finished (" + engineId + ")");

			PreferenceManager.getDefaultSharedPreferences(NativeGLWallpaperService.this).unregisterOnSharedPreferenceChangeListener(this);
		}

		@Override
		public void onSurfaceCreated(SurfaceHolder holder) {
			super.onSurfaceCreated(holder);

			if(BuildConfig.DEBUG)
				Log.d(LOG_TAG, "On Surface Created (" + engineId + ")");
		}

		@Override
		public void onSurfaceDestroyed(SurfaceHolder holder)
		{
			super.onSurfaceDestroyed(holder);
			if(BuildConfig.DEBUG)
				Log.d(LOG_TAG, "Surface destroyed (" + engineId + ")");

			nativeSetSurface(engineId, null);
		}

		@Override
		public void onSurfaceChanged(SurfaceHolder holder, int format, int _width, int _height) {
			super.onSurfaceChanged(holder, format, _width, _height);
			if(BuildConfig.DEBUG)
				Log.d(LOG_TAG, "On Surface Changed.  Width: " + _width + "  height: " + _height + "  (" + engineId + ")");

			nativeOnPause(engineId);

			nativeSetSurface(engineId, holder.getSurface());

			if(!isPaused()) {
				onWallpaperResuming();

				nativeOnResume(engineId);
			}

		}


		@Override
		public void onVisibilityChanged(boolean visible) {
			super.onVisibilityChanged(visible);
			if(BuildConfig.DEBUG)
				Log.d(LOG_TAG, "Visibility set to " + (visible ? "visible" : "invisible") + " (" + engineId + ")");

			if (visible){
				pauseWallpaper(false);

			}else{
				pauseWallpaper(true);
			}
		}

		public void pauseWallpaper(boolean pause)
		{
			if(BuildConfig.DEBUG)
				Log.d(LOG_TAG, "Paused Wallpaper: " + pause);

			if(pause) {
				nativeOnPause(engineId);
			}else{
				onWallpaperResuming();

				nativeOnResume(engineId);
			}

			setPaused(pause);

		}


		private synchronized void setPaused(boolean paused)
		{
			this.paused = paused;
		}

		protected synchronized boolean isPaused()
		{
			return paused;
		}

		private native void nativeOnCreate(int engineId);
		private native void nativeOnResume(int engineId);
		private native void nativeOnPause(int engineId);
		private native void nativeOnStop(int engineId);
		private native void nativeOnFinish(int engineId);
		private native void nativeOnStart(int engineId);

		private native void nativeSetSurface(int engineId, Surface surface);
	}


	static {
		if(BuildConfig.DEBUG)
			Log.d(LOG_TAG, "Loading C++ library");
		try{
			System.loadLibrary("native-lib");
		} catch (UnsatisfiedLinkError e) {
			// only ignore exception in non-android env
			if ("Dalvik".equals(System.getProperty("java.vm.name"))) throw e;
		}
		if(BuildConfig.DEBUG)
			Log.d(LOG_TAG, "Done loading C++ library");
	}


}
