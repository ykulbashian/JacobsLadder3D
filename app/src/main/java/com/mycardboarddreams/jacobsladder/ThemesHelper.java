package com.mycardboarddreams.jacobsladder;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.preference.PreferenceManager;
import android.support.annotation.Nullable;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

import com.mycardboarddreams.jacobsladder.deluxe.BuildConfig;
import com.mycardboarddreams.jacobsladder.deluxe.R;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Locale;
import java.util.Set;

public class ThemesHelper {

	private static final String TAG = ThemesHelper.class.getSimpleName();


	public static int[] getDisplayColorSet(Context context){
		int[] colorsToReturn = getCustomColorSetFromComposer(context);

		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);

		if(colorsToReturn == null){
			String themeName = prefs.getString(context.getString(R.string.key_select_theme), context.getString(R.string.default_color_select));
			colorsToReturn = getColorSetForThemeName(themeName, context);
		}

		checkValidColorArray(context, colorsToReturn);

		colorsToReturn = adjustColorBrightness(colorsToReturn, getCurrentBrightness(context));

		return colorsToReturn;
	}

	private static void checkValidColorArray(Context context, int[] colorsToReturn) {
		int allKeysLength = context.getResources().getStringArray(R.array.custom_color_all_keys).length;

		if(colorsToReturn.length != allKeysLength) {
			throw new IllegalStateException(
					String.format(Locale.CANADA,
							"Somehow failed to retrieve any color set from anywhere. colors length: %d, required length: %d",
							colorsToReturn.length, allKeysLength));
		}
	}

	public static void setCurrentThemeToComposer(Context context){

		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
		Resources res = context.getResources();

		String themeName = prefs.getString(context.getString(R.string.key_select_theme), context.getString(R.string.default_color_select));
		int[] listOfColorValues = getColorSetForThemeName(themeName, context);

		if(listOfColorValues != null) {
			setColorsToComposer(listOfColorValues, prefs, res);
		}
	}

	private static int[] getCustomColorSetFromComposer(Context context)
	{
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);

		String[] colKeys = context.getResources().getStringArray(R.array.custom_color_all_keys);

		int[] colors = new int[colKeys.length];

		for(int i = 0; i < colKeys.length-1; i++){
			if(!prefs.contains(colKeys[i])){
				return null;
			}
			colors[i] = prefs.getInt(colKeys[i], 0);
		}
		colors[colKeys.length-1] = prefs.getBoolean(colKeys[colKeys.length-1], true) ? 1 : 0;

		return colors;
	}

	private static void setColorsToComposer(int[] colorSet, SharedPreferences prefs, Resources res)
	{
		String[] colKeys = res.getStringArray(R.array.custom_color_all_keys);

		checkValidColorSetComposer(colorSet, colKeys);

		Editor editor = prefs.edit();

		for(int i = 0; i < colKeys.length-1; i++){
			editor.putInt(colKeys[i], colorSet[i]);
		}

		editor.putBoolean(colKeys[colKeys.length-1], colorSet[colKeys.length-1] == 1);

		editor.apply();

	}

	private static void checkValidColorSetComposer(int[] colorSet, String[] colKeys) {
		if(colorSet == null || colorSet.length == 0 || colorSet.length != colKeys.length) {

			throw new IllegalArgumentException(
					String.format(Locale.CANADA,
							"Tried to set improper color array to composer: %s; length: %d",
							TextUtils.join(", ", Arrays.asList(colorSet)), colorSet == null ? 0 : colorSet.length));
		}
	}


	private static int[] getColorSetForThemeName(String themeName, Context context){
		if(TextUtils.isEmpty(themeName)) {
			throw new IllegalArgumentException("Setting colors with empty theme name.");
		}

		if(BuildConfig.DEBUG)
			Log.d(TAG, "Setting theme to: " + themeName);

		Resources res = context.getResources();

		int[] colorsToReturn = null;

		if(themeName.startsWith(res.getString(R.string.custom_theme_prefix_))){
			colorsToReturn = getColorValuesOfSavedCustomTheme(themeName, context);

			if(colorsToReturn == null) {
				themeName = context.getString(R.string.default_color_select);
			} else {
				return colorsToReturn;
			}
		}

		String[] colKeys = res.getStringArray(R.array.color_options_readable);

		for (int index = 0; index < colKeys.length; index++)
		{
			if(colKeys[index].equals(themeName)){

				TypedArray allColorArrays = res.obtainTypedArray(R.array.all_theme_arrays);

				int id = allColorArrays.getResourceId(index, 0);
				allColorArrays.recycle();

				if (id > 0){
					colorsToReturn = res.getIntArray(id);
				}

				break;
			}
		}

		return colorsToReturn;
	}

	public static boolean isCustomColorsSet(Context context){
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);

		return prefs.contains(context.getString(R.string.key_custom_has_background));
	}





	private static float getCurrentBrightness(Context context){
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
		int brightnessGain = prefs.getInt(context.getString(R.string.key_color_brightness), context.getResources().getInteger(R.integer.default_brightness));
		int maxBrightness = context.getResources().getInteger(R.integer.max_brightness);
		return (float)brightnessGain/(float)maxBrightness;
	}

	private static int[] adjustColorBrightness(int[] originalColors, float brightness){
		for(int i = 0; i < originalColors.length; i++){
			originalColors[i] = adjustColorBrightness(originalColors[i], brightness);
		}
		return originalColors;
	}

	private static int adjustColorBrightness(int originalColor, float brightness){
		int alpha = (originalColor) & 0xFF000000;
		int red = (originalColor >> 16) & 0xFF;
		int green = (originalColor >> 8) & 0xFF;
		int blue = (originalColor) & 0xFF;

		return (alpha) | ((int)Math.ceil(red * brightness) << 16) | ((int)Math.ceil(green * brightness) << 8) | ((int)Math.ceil(blue * brightness));
	}




	@Nullable
	private static int[] getColorValuesOfSavedCustomTheme(String prependedThemeName, Context context){

		SharedPreferences customSharedPrefs =
				context.getSharedPreferences(context.getString(R.string.key_custom_theme_prefs),
				Context.MODE_PRIVATE);
		String themeColorString = customSharedPrefs.getString(prependedThemeName, null);
		String[] colorStrings = TextUtils.split(themeColorString, ";");

		if(colorStrings == null || colorStrings.length < 2) {
			return null;
		}

		int[] colorInts = new int[colorStrings.length];
		int i = 0;

		for(String color : colorStrings){
			colorInts[i++] = Integer.parseInt(color);
		}

		return colorInts;
	}

	public static void saveCustomColors(Context context, String newCustomThemeTitle){

		String customThemePrefix = context.getResources().getString(R.string.custom_theme_prefix_);

		String prependedTheme = customThemePrefix + newCustomThemeTitle;

		SharedPreferences customSharedPrefs =
				context.getSharedPreferences(context.getString(R.string.key_custom_theme_prefs),
						Context.MODE_PRIVATE);

		String customColorsKey = context.getString(R.string.key_custom_color_set_keys);

		int[] colors = getCustomColorSetFromComposer(context);
		String serializedColors = serializeIntArray(colors);

		Set<String> currentCustomSet = customSharedPrefs.getStringSet(customColorsKey, new HashSet<String>());
		currentCustomSet.add(prependedTheme);

		Editor editor = customSharedPrefs.edit();
		editor.putStringSet(customColorsKey, currentCustomSet);
		editor.putString(prependedTheme, serializedColors);
		editor.apply();

		Toast.makeText(context, context.getString(R.string.saved_custom_theme_toast), Toast.LENGTH_SHORT).show();
	}


	private static String serializeIntArray(int[] colors) {
		Integer[] colorIntegers = new Integer[colors.length];
		for(int i = 0; i < colors.length; i++)
			colorIntegers[i] = colors[i];
		return TextUtils.join(";", colorIntegers);
	}

}
