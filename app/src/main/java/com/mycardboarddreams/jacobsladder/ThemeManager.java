package com.mycardboarddreams.jacobsladder;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.drawable.Drawable;
import android.preference.PreferenceManager;

import com.mycardboarddreams.jacobsladder.deluxe.R;

public class ThemeManager {

	private List<String> listOfColorSetNames;
	private List<Drawable> demoImageDrawables;

	private Theme[] allThemes;

	private int baseSize = 0;

	private static ThemeManager instance;

	public static ThemeManager get(Context context){
		if(instance == null){
			instance = new ThemeManager(context);
		}
		return instance;
	}

	private ThemeManager(Context context){

		listOfColorSetNames = new ArrayList<>(Arrays.asList(context.getResources().getStringArray(R.array.color_options_readable)));
		baseSize = listOfColorSetNames.size();

		Resources res = context.getResources();
		TypedArray allColorArrays = res.obtainTypedArray(R.array.all_color_arrays);
		demoImageDrawables = new ArrayList<>();

		for (int i = 0; i < allColorArrays.length(); i++) {
			int id = allColorArrays.getResourceId(i, 0);
			if (id > 0) {
				demoImageDrawables.add(res.getDrawable(id));
			}
		}
		allColorArrays.recycle();

		List<String> customThemeNames = getCustomThemeNames(context);

		listOfColorSetNames.addAll(customThemeNames);

		allThemes = getAllThemes(context, context.getString(R.string.select_color_theme));
	}

	private List<String> getCustomThemeNames(Context context){
		SharedPreferences customSharedPrefs = context.getSharedPreferences(context.getString(R.string.key_custom_theme_prefs), Context.MODE_PRIVATE);

		Set<String> customThemeKeys = customSharedPrefs.getStringSet(context.getString(R.string.key_custom_color_set_keys), new HashSet<String>());

		return new ArrayList<>(customThemeKeys);
	}

	private Theme getThemeForPosition(int position, Context context, String preferenceKey){
		if (position < 0){
			position = 0;
		}

		String selectedName = PreferenceManager.getDefaultSharedPreferences(context).getString(preferenceKey, context.getString(R.string.default_color_select));

		String themeName = listOfColorSetNames.get(position);
		boolean isSelectedTheme = themeName.equals(selectedName);

		if(position >= baseSize) {
			return Theme.newCustomTheme(themeName, context, isSelectedTheme);
		}

		Theme theme = new Theme();
		theme.readableName = themeName;
		theme.keyName = themeName;
		theme.demoImage = demoImageDrawables.get(position);
		theme.isSelected = isSelectedTheme;
		theme.isCustomTheme = false;

		return theme;
	}

	public Theme getSelectedTheme(Context context, String key){
		String themeName = PreferenceManager.getDefaultSharedPreferences(context).getString(key, context.getString(R.string.default_color_select));

		int position = listOfColorSetNames.indexOf(themeName);
		return getThemeForPosition(position, context, key);
	}

	public Theme[] getAllThemes(Context context, String preferenceKey){
		Theme[] themes = new Theme[listOfColorSetNames.size()];

		for(int i = 0; i < listOfColorSetNames.size(); i++){
			themes[i] = getThemeForPosition(i, context, preferenceKey);
		}

		return themes;
	}

	public static class Theme {
		public String readableName;
		public String keyName;
		public Drawable demoImage;
		public boolean isSelected;
		public boolean isCustomTheme;

		public Theme(){}

		public Theme(Theme other){
			readableName = other.readableName;
			keyName = other.keyName;
			demoImage = other.demoImage;
			isSelected = other.isSelected;
			isCustomTheme = other.isCustomTheme;
		}

		static Theme newCustomTheme(String themeName, Context context, boolean isSelected){
			Theme customTheme = new Theme();
			customTheme.readableName = themeName.replace(context.getString(R.string.custom_theme_prefix_), "");
			customTheme.keyName = themeName;
			customTheme.isCustomTheme = true;
			customTheme.demoImage = context.getResources().getDrawable(R.drawable.custom_theme);

			customTheme.isSelected = isSelected;
			return customTheme;
		}
	}


}
