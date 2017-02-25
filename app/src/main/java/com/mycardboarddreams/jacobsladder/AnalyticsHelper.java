package com.mycardboarddreams.jacobsladder;

import android.content.Context;

public class AnalyticsHelper {
    private static final String CATEGORY_USAGE = "Usage";
    private static final String CATEGORY_SETTINGS = "Settings";

    private static final String TRACK_SET_WALLPAPER = "Set wallpaper";
    private static final String TRACK_UNSET_WALLPAPER = "Unset wallpaper";
    private static final String TRACK_PAUSE_TAP_WALLPAPER = "Pause by tap";
    private static final String TRACK_UNPAUSE_TAP_WALLPAPER = "Unpause by tap";
    private static final String TRACK_TOUCH_TO_MANIPULATE = "Touch manipulate";

    private static final String TRACK_CHANGE_DIMENSIONS = "Change dimensions";
    private static final String TRACK_CHANGE_THEME = "Change theme";
    private static final String TRACK_CHANGE_BRIGHTNESS = "Change brightness";


    // This class is deliberately empty to prevent analytics crashes


    public static void trackSetWallpaper(Context context) {

    }

    public static void trackUnSetWallpaper(Context context) {

    }

    public static void trackPauseWallpaperByTouch(Context context) {

    }

    public static void trackUnpauseWallpaperByTouch(Context context) {

    }

    public static void trackTouchToManipulate(Context context) {

    }

    public static void trackDimensionsChanged(Context context, int size) {

    }

    public static void trackThemeChanged(Context context, String theme) {

    }

    public static void trackBrightnessChanged(Context context, int brightnessLevel) {

    }

}
