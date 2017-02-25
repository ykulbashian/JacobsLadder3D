package com.mycardboarddreams.jacobsladder.preferences;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.preference.DialogPreference;
import android.preference.PreferenceManager;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;

import com.mycardboarddreams.jacobsladder.deluxe.R;
import com.mycardboarddreams.jacobsladder.ThemesHelper;

public class SaveCustomThemePreference extends DialogPreference {

	private View mView;

	public SaveCustomThemePreference(Context context, AttributeSet attrs, int deftype) {
		super(context, attrs, deftype);
	}

	public SaveCustomThemePreference(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	@Override
	protected void onBindView(View view) {
		super.onBindView(view);
		view.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				SaveCustomThemePreference.this.onClick();
			}
		});
	}

	@Override
	protected View onCreateDialogView() {
		mView = super.onCreateDialogView();
		return mView;
	}


	@Override
	protected void onDialogClosed(boolean positiveResult) {
		super.onDialogClosed(positiveResult);
		if(positiveResult){
			TextView title = (TextView) mView.findViewById(R.id.saved_text_view);

			String newCustomThemeTitle = title.getText().toString().trim();

			if(TextUtils.isEmpty(newCustomThemeTitle))
				return;

			String customThemePrefix = getContext().getResources().getString(R.string.custom_theme_prefix_);

			String prependedTheme = customThemePrefix + newCustomThemeTitle;
			SharedPreferences defaultSharedPreferences = PreferenceManager.getDefaultSharedPreferences(getContext());

			if(defaultSharedPreferences.contains(prependedTheme))
				showOverwriteThemeDialog(newCustomThemeTitle);
			else
				ThemesHelper.saveCustomColors(getContext(), newCustomThemeTitle);

		}
	}


	private void showOverwriteThemeDialog(final String newCustomThemeTitle) {
		AlertDialog.Builder builder = new Builder(getContext());
		builder.
				setTitle(R.string.custom_theme_save_overwrite_title).
				setMessage(getContext().getString(R.string.custom_theme_save_overwrite_body)).
				setPositiveButton(R.string.custom_theme_save_overwrite, new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {
						ThemesHelper.saveCustomColors(getContext(), newCustomThemeTitle);
					}
				}).
				setNegativeButton(R.string.custom_theme_save_cancel, null).
				show();
	}

}
