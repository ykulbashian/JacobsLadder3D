package com.mycardboarddreams.jacobsladder.view;

import android.animation.Animator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.widget.FrameLayout;

public class RolloutView extends FrameLayout{
	
	private OpenedState rolloutState = OpenedState.CLOSED;
	
	public interface OpenClosedListener{
		void finishedTranslate(boolean opened);
	}
	
	private OpenClosedListener openClosedListener;
	
	public void setOpenClosedListener(OpenClosedListener openClosedListener){
		this.openClosedListener = openClosedListener;
	}
	
	public enum OpenedState{
		OPENED,
		CLOSED,
		IN_TRANSIT,
	}

	public RolloutView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}
	public RolloutView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	public RolloutView(Context context) {
		super(context);
	}
	

	public boolean expand() {
		if(rolloutState != OpenedState.CLOSED)
			return false;
	     
		final int widthSpec = MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
		final int heightSpec = MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
		measure(widthSpec, heightSpec);
		 
		ValueAnimator mAnimator = slideAnimator(this, 0, getMeasuredHeight());
		
		mAnimator.addListener(new Animator.AnimatorListener() {
			@Override
			public void onAnimationEnd(Animator animator) {
				rolloutState = OpenedState.OPENED;
				if(openClosedListener != null)
					openClosedListener.finishedTranslate(true);
			}

			@Override
			public void onAnimationCancel(Animator arg0) { }

			@Override
			public void onAnimationRepeat(Animator arg0) { }

			@Override
			public void onAnimationStart(Animator arg0) {
			} 
		});
		
		this.setVisibility(View.VISIBLE);
		rolloutState = OpenedState.IN_TRANSIT;
		mAnimator.start();
		
		return true;
	}
	
	public boolean isFullyOpened(){
		return rolloutState == OpenedState.OPENED;
	}
	
	public boolean isFullyClosed(){
		return rolloutState == OpenedState.CLOSED;
	}
	 
	public boolean collapse() {
		if(rolloutState != OpenedState.OPENED)
			return false;
		
		int finalHeight = getHeight();

		ValueAnimator mAnimator = slideAnimator(this, finalHeight, 0);
			     
		mAnimator.addListener(new Animator.AnimatorListener() {
			@Override
			public void onAnimationEnd(Animator animator) {
				setVisibility(View.GONE);
				rolloutState = OpenedState.CLOSED;
				if(openClosedListener != null)
					openClosedListener.finishedTranslate(false);
			}

			@Override
			public void onAnimationCancel(Animator arg0) { }

			@Override
			public void onAnimationRepeat(Animator arg0) { }

			@Override
			public void onAnimationStart(Animator arg0) {
			} 
		});
		
		rolloutState = OpenedState.IN_TRANSIT;
		mAnimator.start();
		
		return true;
	}

	private ValueAnimator slideAnimator(final ViewGroup layout, int start, int end) {
		  
	    ValueAnimator animator = ValueAnimator.ofInt(start, end);
	    animator.setInterpolator(new AccelerateDecelerateInterpolator());
	  
	    animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
	         @Override
	         public void onAnimationUpdate(ValueAnimator valueAnimator) {
	            //Update Height
	            int value = (Integer) valueAnimator.getAnimatedValue();
	            ViewGroup.LayoutParams layoutParams = layout.getLayoutParams();
	            layoutParams.height = value;
	            layout.setLayoutParams(layoutParams);
	         }
	    });
	    return animator;
	}

}
