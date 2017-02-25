package com.mycardboarddreams.jacobsladder.deluxe.drivers;

import com.mycardboarddreams.jacobsladder.TestProperties;
import com.mycardboarddreams.jacobsladder.deluxe.R;

import android.content.res.Resources;
import android.os.Environment;
import android.test.AndroidTestCase;
import android.util.Log;

import org.junit.Test;

import java.io.File;
import java.io.FileOutputStream;

import static org.assertj.core.api.Java6Assertions.assertThat;

public class CubeDriverTest extends AndroidTestCase{

	private static final String LOG_TAG = CubeDriverTest.class.getSimpleName();

	private static final float SPEED = 1.0f;

	static {
		Log.d(LOG_TAG, "Loading native library");
		System.loadLibrary("native-lib");
    	Log.d(LOG_TAG, "Done loading native library");
	}

	@Test
	public void testRunDriverIterations(){
		Resources resources = getContext().getResources();
		long startTime = System.currentTimeMillis();

		File f = new File(Environment.getExternalStorageDirectory().getPath(), TestProperties.STATS_FILE_NAME);

		String driverInstance = createDriverInstance(
				resources.getInteger(R.integer.default_x_blocks),
				resources.getInteger(R.integer.default_y_blocks),
				resources.getInteger(R.integer.default_z_blocks),
				TestProperties.TOTAL_NUMBER_OF_SAMPLES, 10, SPEED);

		assertThat(driverInstance).isNotEmpty();

		FileOutputStream outputStream;

		try {
			outputStream = new FileOutputStream(f, false);
			outputStream.write(driverInstance.getBytes());
			outputStream.close();
		} catch (Exception e) {
			e.printStackTrace();
		}


		Log.d("TAG", driverInstance);

		long endTime = System.currentTimeMillis();
		
		Log.d(LOG_TAG, "Total time taken to run " + TestProperties.TOTAL_NUMBER_OF_SAMPLES +
				" samples was " + ((float)(endTime - startTime))/1000f + " seconds.");
	}
	
	//----------------------------
	public static native String createDriverInstance(int xBlocks, int yBlocks, int zBlocks, int iterations, int iterationSpan, float speed);


}
