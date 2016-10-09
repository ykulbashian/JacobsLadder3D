package com.mycardboarddreams.jacobsladder.template;

import android.content.SharedPreferences;

import com.mycardboarddreams.jacobsladder.baselib.NativeGLWallpaperService;

public class TemplateWallpaperService extends NativeGLWallpaperService<TemplateWallpaperService.TemplateWallpaperEngine>{

	public static final String LOG_TAG = TemplateWallpaperService.class.getName();

	@Override
	protected TemplateWallpaperEngine getInstance() {
		return new TemplateWallpaperEngine();
	}

	public class TemplateWallpaperEngine extends NativeGLWallpaperService<TemplateWallpaperService.TemplateWallpaperEngine>.NativeGLWallpaperEngine{

		@Override
		protected void onWallpaperResuming() {

		}

		@Override
		public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {

		}

	}

}
