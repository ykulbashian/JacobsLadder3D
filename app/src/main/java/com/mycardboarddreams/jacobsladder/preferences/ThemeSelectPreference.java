package com.mycardboarddreams.jacobsladder.preferences;


import android.app.AlertDialog.Builder;
import android.content.Context;
import android.preference.ListPreference;
import android.util.AttributeSet;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageView;

import com.mycardboarddreams.jacobsladder.deluxe.R;
import com.mycardboarddreams.jacobsladder.ThemeManager;
import com.mycardboarddreams.jacobsladder.ThemeManager.Theme;

public class ThemeSelectPreference extends ListPreference implements OnClickListener {

	private ThemeSelectAdapter adapter;
	private View preferenceView;

	public ThemeSelectPreference(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs);
	}
	public ThemeSelectPreference(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	public ThemeSelectPreference(Context context) {
		super(context);
	}

	@Override
	protected void onBindView(View view) {
		preferenceView = view;
		updateSelectedThemeImage();

		super.onBindView(view);
	}

	private void updateSelectedThemeImage()
	{
		if(preferenceView == null)
			return;

		Theme selectedTheme = ThemeManager.get(getContext()).getSelectedTheme(getContext(), getKey());

		if (selectedTheme == null)
			return;

		ImageView demo = ((ImageView)preferenceView.findViewById(R.id.demo_layout));

		if(demo != null)
			demo.setImageDrawable(selectedTheme.demoImage);

	}


	@Override
	protected void onPrepareDialogBuilder(Builder builder) {
		adapter = new ThemeSelectAdapter(getContext(), R.id.colors_title_text, this, getKey());

		builder.setAdapter(adapter, this);
		super.onPrepareDialogBuilder(builder);
	}


	public void dismissDialog()
	{
		if(getDialog() != null)
			getDialog().dismiss();
	}

	@Override
	public void onClick(View selectedView) {
		int position = selectedView.getId();
		Theme theme = adapter.getItem(position);

		persistString(theme.keyName);

		updateSelectedThemeImage();

		dismissDialog();
	}


}
