package com.mycardboarddreams.jacobsladder.preferences;

import android.content.Context;
import android.content.res.TypedArray;
import android.preference.DialogPreference;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

import com.mycardboarddreams.jacobsladder.deluxe.R;

public class TextListPopupPreference extends DialogPreference implements OnClickListener{
	
	String[] message;
	
	public TextListPopupPreference(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context, attrs);
	}
	public TextListPopupPreference(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context, attrs);
	}

	@Override
	protected void onBindView(View view) {
		super.onBindView(view);
		boolean isCustomThemeText = getKey() != null && getKey().equals(getContext().getString(R.string.key_custom_theme_help));
		view.setBackgroundResource(isCustomThemeText ? 
				R.color.background_faded_dark_grey : android.R.color.transparent);
		
		if(isCustomThemeText)
			view.setOnClickListener(this);
	}
	
	private void init(Context context, AttributeSet attrs) {
		final TypedArray array = context.obtainStyledAttributes(attrs, R.styleable.TextListPopupPreference);
		final int id = array.getResourceId(R.styleable.TextListPopupPreference_android_entryValues, 0);

		if (id != 0) {
			message = context.getResources().getStringArray(id);
		}
		array.recycle();
		
		setPositiveButtonText(R.string.help_dialog_ok);
		setNegativeButtonText(null);
	}
	
	@Override
	protected View onCreateDialogView() {
		ListView lv = (ListView)LayoutInflater.from(getContext()).inflate(R.layout.help_dialog, null);
		
		if(message == null || message.length == 0)
			return lv;
		
		final String[] messageStrings = new String[message.length];
		
		for(int i = 0; i < message.length; i++)
			messageStrings[i] = message[i].toString();
		
		lv.setAdapter(new ArrayAdapter<String>(getContext(), R.layout.help_dialog_item, R.id.text_item, message){
			
			@Override
			public View getView(int position, View convertView, ViewGroup parent){
				if(convertView == null){
					convertView = LayoutInflater.from(getContext()).inflate(R.layout.help_dialog_item, null);
				}
				
				TextView tv = (TextView)convertView.findViewById(R.id.icon_item);
				TextView tv1 = (TextView)convertView.findViewById(R.id.text_item);
				tv.setText(String.valueOf(position+1) + ". ");
				tv1.setText(messageStrings[position]);
				
				return convertView;
			}
		});
		
		return lv;
	}
	
	@Override
	public void onClick(View v) {
		onClick();
	}
	
}
