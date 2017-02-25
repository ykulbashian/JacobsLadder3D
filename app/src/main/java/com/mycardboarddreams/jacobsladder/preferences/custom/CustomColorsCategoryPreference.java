package com.mycardboarddreams.jacobsladder.preferences.custom;

import net.margaritov.preference.colorpicker.ColorPickerPreference;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.content.res.Resources;
import android.preference.Preference;
import android.preference.PreferenceCategory;
import android.preference.PreferenceManager;
import android.util.AttributeSet;
import android.util.Pair;
import android.util.SparseArray;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.mycardboarddreams.jacobsladder.JacobsLadderService;
import com.mycardboarddreams.jacobsladder.deluxe.R;
import com.mycardboarddreams.jacobsladder.preferences.CheckBoxVerticalPreference;
import com.mycardboarddreams.jacobsladder.preferences.DropDownHeaderPreference;
import com.mycardboarddreams.jacobsladder.view.RolloutView;
import com.mycardboarddreams.jacobsladder.view.WallpaperSurface;
import com.mycardboarddreams.jacobsladder.view.RolloutView.OpenClosedListener;

public class CustomColorsCategoryPreference extends PreferenceCategory
        implements OnClickListener, OpenClosedListener, OnSharedPreferenceChangeListener {

    private View dropDownLayout;
    protected RolloutView dropDownContainer;
    private FrameLayout wallpaperContainer;

    public CustomColorsCategoryPreference(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        setPersistent(true);
    }

    public CustomColorsCategoryPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        setPersistent(true);
    }


    private SparseArray<Pair<Integer, Integer>> bindings = new SparseArray<>();
    {
        bindings.put(R.string.key_custom_color_near_face_bl, new Pair<>(R.id.panel_faces_bottom_left, R.id.middle_square));
        bindings.put(R.string.key_custom_color_near_face_tl, new Pair<>(R.id.panel_faces_top_left, R.id.middle_square));
        bindings.put(R.string.key_custom_color_near_face_tr, new Pair<>(R.id.panel_faces_top_right, R.id.middle_square));
        bindings.put(R.string.key_custom_color_near_face_br, new Pair<>(R.id.panel_faces_bottom_right, R.id.middle_square));

        bindings.put(R.string.key_custom_color_top_face_bl, new Pair<>(R.id.panel_faces_bottom_left, R.id.top_square));
        bindings.put(R.string.key_custom_color_top_face_tl, new Pair<>(R.id.panel_faces_top_left, R.id.top_square));
        bindings.put(R.string.key_custom_color_top_face_tr, new Pair<>(R.id.panel_faces_top_right, R.id.top_square));
        bindings.put(R.string.key_custom_color_top_face_br, new Pair<>(R.id.panel_faces_bottom_right, R.id.top_square));

        bindings.put(R.string.key_custom_color_left_face_bl, new Pair<>(R.id.panel_faces_bottom_left, R.id.left_square));
        bindings.put(R.string.key_custom_color_left_face_tl, new Pair<>(R.id.panel_faces_top_left, R.id.left_square));
        bindings.put(R.string.key_custom_color_left_face_tr, new Pair<>(R.id.panel_faces_top_right, R.id.left_square));
        bindings.put(R.string.key_custom_color_left_face_br, new Pair<>(R.id.panel_faces_bottom_right, R.id.left_square));

        bindings.put(R.string.key_custom_color_bottom_face_bl, new Pair<>(R.id.panel_faces_bottom_left, R.id.bottom_square));
        bindings.put(R.string.key_custom_color_bottom_face_tl, new Pair<>(R.id.panel_faces_top_left, R.id.bottom_square));
        bindings.put(R.string.key_custom_color_bottom_face_tr, new Pair<>(R.id.panel_faces_top_right, R.id.bottom_square));
        bindings.put(R.string.key_custom_color_bottom_face_br, new Pair<>(R.id.panel_faces_bottom_right, R.id.bottom_square));

        bindings.put(R.string.key_custom_color_right_face_bl, new Pair<>(R.id.panel_faces_bottom_left, R.id.right_square));
        bindings.put(R.string.key_custom_color_right_face_tl, new Pair<>(R.id.panel_faces_top_left, R.id.right_square));
        bindings.put(R.string.key_custom_color_right_face_tr, new Pair<>(R.id.panel_faces_top_right, R.id.right_square));
        bindings.put(R.string.key_custom_color_right_face_br, new Pair<>(R.id.panel_faces_bottom_right, R.id.right_square));


        bindings.put(R.string.key_custom_color_background, new Pair<>(R.id.top_level_layout, R.id.container_center_background));
        bindings.put(R.string.key_custom_color_distant, new Pair<>(R.id.top_level_layout, R.id.container_center_higher));
        bindings.put(R.string.key_custom_color_mid, new Pair<>(R.id.top_level_layout, R.id.container_center));
        bindings.put(R.string.key_custom_color_primary, new Pair<>(R.id.top_level_layout, R.id.container_bottom_center));
        bindings.put(R.string.key_custom_color_top_left, new Pair<>(R.id.top_level_layout, R.id.container_top_left));
        bindings.put(R.string.key_custom_color_bottom_right, new Pair<>(R.id.top_level_layout, R.id.container_bottom_right));
        bindings.put(R.string.key_custom_color_top_right, new Pair<>(R.id.top_level_layout, R.id.container_top_right));
        bindings.put(R.string.key_custom_color_bottom_left, new Pair<>(R.id.top_level_layout, R.id.container_bottom_left));

        bindings.put(R.string.key_custom_has_background, new Pair<>(R.id.top_level_layout, R.id.container_background_checkbox));

        bindings.put(R.string.key_save_custom_set, new Pair<>(0, R.id.save_custom_theme_container));
        bindings.put(R.string.key_custom_colors_header, new Pair<>(0, R.id.custom_theme_composer_header));
    }

    @Override
    protected void onBindView(View view) {
        super.onBindView(view);
        PreferenceManager.getDefaultSharedPreferences(getContext()).registerOnSharedPreferenceChangeListener(this);

        if(JacobsLadderService.isPremiumLegacy(getContext())) {
            view.findViewById(R.id.custom_colors_wrapper).setVisibility(View.VISIBLE);
        }

        if(view instanceof ViewGroup) {
            ViewGroup layout = (ViewGroup) view;

            dropDownLayout = layout.findViewById(R.id.main_dropdown_container);
            dropDownContainer = (RolloutView)layout.findViewById(R.id.dropdown_wrapper);
            dropDownContainer.setOpenClosedListener(this);
            wallpaperContainer = (FrameLayout)layout.findViewById(R.id.custom_theme_wallpaper_surface);

            Resources res = getContext().getResources();

            synchronized (this) {
                for (int i = 0; i < getPreferenceCount(); i++) {
                    Preference pref = getPreference(i);

                    View prefView = pref.getView(null, null);

                    if(pref instanceof DropDownHeaderPreference)
                        prefView.setOnClickListener(this);

                    for(int j = 0; j < bindings.size(); j++) {
                        int key = bindings.keyAt(j);

                        if (pref.getKey().contains(res.getString(key))) {
                            Pair<Integer, Integer> destinations = bindings.get(key);

                            View grouping;

                            if(destinations.first == 0)
                                grouping = layout;
                            else
                                grouping = layout.findViewById(destinations.first);

                            if (grouping != null) {
                                ViewGroup destinationView = (ViewGroup) grouping.findViewById(destinations.second);
                                destinationView.addView(prefView);
                            }
                        }
                    }

                }
            }
        }
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences arg0, String key) {
        Preference pref = findPreference(key);
        if(pref != null){
            if(pref instanceof ColorPickerPreference)
                ((ColorPickerPreference)pref).refresh();
            if(pref instanceof CheckBoxVerticalPreference)
                ((CheckBoxVerticalPreference)pref).refresh();
        }
    }

    @Override
    protected boolean isOnSameScreenAsChildren() {
        return false;
    }

    @Override
    public boolean isEnabled() {
        return true;
    }

    @Override
    public boolean shouldDisableDependents() {
        return false;
    }

    @Override
    protected boolean shouldPersist() {
        return true;
    }

    @Override
    public boolean shouldCommit() {
        return true;
    }

    @Override
    public boolean isOrderingAsAdded() {
        return false;
    }

    @Override
    public View getView(View view, ViewGroup parent) {
        if(view == null)
            view = onCreateView(parent);
        onBindView(view);
        return view;
    }

    @Override
    public void onClick(View arg0) {
        if(dropDownLayout != null){
            DropDownHeaderPreference dropDownHeader = (DropDownHeaderPreference) findPreference(getContext().getString(R.string.key_custom_colors_header));

            if(dropDownContainer.isFullyOpened()){
                dropDownContainer.collapse();
                dropDownHeader.setOpenedState(false);
            }
            else if(dropDownContainer.isFullyClosed()){
                dropDownContainer.expand();
                dropDownHeader.setOpenedState(true);
            }
        }
    }

    @Override
    public void finishedTranslate(boolean opened) {
        addBackgroundWallpaper(opened);
    }

    public void addBackgroundWallpaper(boolean show){
        if(show){
            wallpaperContainer.removeAllViews();
            wallpaperContainer.addView(new WallpaperSurface(getContext()));
        }
        else{
            wallpaperContainer.removeAllViews();
        }
    }

}
