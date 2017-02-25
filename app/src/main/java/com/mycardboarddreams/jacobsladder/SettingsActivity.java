package com.mycardboarddreams.jacobsladder;


import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceActivity;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import com.mycardboarddreams.jacobsladder.deluxe.BuildConfig;
import com.mycardboarddreams.jacobsladder.deluxe.R;
import io.fabric.sdk.android.Fabric;

public class SettingsActivity extends PreferenceActivity implements SharedPreferences.OnSharedPreferenceChangeListener
{
	AllSettingsFragment settingsFragment;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		try{
			Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
		} catch (Exception e){
			Log.e("No Fabric", "This only works once you set up fabric");
		}

		// Display the fragment as the main content.
		if(savedInstanceState == null) {
			settingsFragment = new AllSettingsFragment();

			getFragmentManager().beginTransaction()
					.replace(android.R.id.content, settingsFragment)
					.commit();
		}

		if(!ThemesHelper.isCustomColorsSet(this)){
			ThemesHelper.setCurrentThemeToComposer(this);
		}

		JacobsLadderService.nativeIsInDebugMode(BuildConfig.DEBUG);
	}


	@Override
	protected void onResume() {
		super.onResume();

		SharedPreferences defaultSharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
		defaultSharedPreferences.registerOnSharedPreferenceChangeListener(this);
	}

	@Override
	protected void onPause() {
		super.onPause();

		SharedPreferences defaultSharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
		defaultSharedPreferences.unregisterOnSharedPreferenceChangeListener(this);
	}

	@Override
	protected boolean isValidFragment(String fragmentName) {
		return AllSettingsFragment.class.getName().equals(fragmentName);
	}

	@Override
	public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {

		if(TextUtils.equals(key, getString(R.string.select_color_theme))){
			AnalyticsHelper.trackThemeChanged(this, sharedPreferences.getString(key, getString(R.string.default_color_select)));
		} else if(TextUtils.equals(key, getString(R.string.key_color_brightness))){
			AnalyticsHelper.trackBrightnessChanged(this, sharedPreferences.getInt(key, getResources().getInteger(R.integer.default_brightness)));
		} else if(TextUtils.equals(key, getString(R.string.key_complexity_level))){
			AnalyticsHelper.trackDimensionsChanged(this, sharedPreferences.getInt(key, getResources().getInteger(R.integer.default_complexity_level)));
		}

	}

	public static class AllSettingsFragment extends PreferenceFragment {
		@Override
		public void onCreate(Bundle savedInstanceState) {
			super.onCreate(savedInstanceState);

			addPreferencesFromResource(R.xml.whitewallpaper_settings);
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			View v = super.onCreateView(inflater, container, savedInstanceState);
			ListView listView = (ListView)v.findViewById(android.R.id.list);
			listView.setDivider(getResources().getDrawable(R.drawable.preference_list_divider));
			listView.setBackgroundResource(R.color.background_dark_preference);
			listView.setPadding(0, 0, 0, 0);
			return v;
		}
	}

}
