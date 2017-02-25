package com.mycardboarddreams.jacobsladder.preferences;

import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.support.annotation.NonNull;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.mycardboarddreams.jacobsladder.deluxe.R;
import com.mycardboarddreams.jacobsladder.ThemeManager;
import com.mycardboarddreams.jacobsladder.ThemeManager.Theme;


class ThemeSelectAdapter extends ArrayAdapter<Theme> {

	private OnClickListener onSelectCallback;

	private  Resources resources;

	private Resources Resources() {
		if (resources == null)
			resources = getContext().getResources();
		return resources;
	}

	public ThemeSelectAdapter(Context context, int textViewResourceId, OnClickListener lp, String key) {
		super(context, R.layout.preference_theme_list_item, textViewResourceId, ThemeManager.get(context).getAllThemes(context, key));
		onSelectCallback = lp;
	}

	public ThemeSelectAdapter(Context context, int resource,
		  int textViewResourceId, List<Theme> objects) {
		super(context, resource, textViewResourceId, objects);
	}

	public ThemeSelectAdapter(Context context, int resource,
		  int textViewResourceId) {
		super(context, resource, textViewResourceId);
	}
	public ThemeSelectAdapter(Context context, int resource) {
		super(context, resource);
	}

	@Override
	public boolean hasStableIds() {
		return true;
	}

	@Override
	@NonNull
	public View getView(int position, View convertView, @NonNull ViewGroup parent) {

		// inflate layout
		LayoutInflater inflater = ((Activity) getContext()).getLayoutInflater();

		if (convertView == null)
			convertView = inflater.inflate(R.layout.preference_theme_list_item,
					parent, false);
		convertView.setId(position);

		Theme theme = getItem(position);

		// set on click listener for row
		convertView.setOnClickListener(onSelectCallback);

		// set name
		TextView swatchTitle = (TextView) convertView.findViewById(R.id.colors_title_text);
		swatchTitle.setText(theme.readableName);
		swatchTitle.setAlpha(1f);
		swatchTitle.setBackgroundResource(theme.isSelected ? android.R.color.holo_blue_light : android.R.color.white);
		swatchTitle.setTextColor(Resources().getColor(theme.isSelected ? android.R.color.white : android.R.color.black));

		convertView.findViewById(R.id.theme_title_background).setVisibility(View.VISIBLE);
		convertView.findViewById(R.id.theme_title_background_side).setVisibility(View.VISIBLE);

		convertView.setBackgroundColor(Resources().getColor(
				theme.isSelected ? android.R.color.holo_blue_light : android.R.color.background_dark));

		// set colors
		ImageView backgroundImage = (ImageView)convertView.findViewById(R.id.theme_background_image);

		if (theme.demoImage != null){
			backgroundImage.setImageDrawable(theme.demoImage);
			backgroundImage.setImageAlpha(255);
		}
		else{
			backgroundImage.setImageDrawable(null);
			backgroundImage.setImageAlpha(255);
		}


		return convertView;
	}



}
