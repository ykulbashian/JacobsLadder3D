package com.mycardboarddreams.jacobsladder.display;

import com.google.gson.Gson;
import com.mycardboarddreams.jacobsladder.AggregateStats;
import com.mycardboarddreams.jacobsladder.TestProperties;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class DisplayResults {
	
	private static final String LOCAL_STATS_PATH = System.getProperty("user.dir");


	public static void main(String args[]){

		//runTests();
		
		GraphFrame graphFrame = new GraphFrame("JacobsLadder Test Results");
		
		callAdbCopyStatsFile();
		List<AggregateStats> stats = readStatsFile();
		
		if(stats == null)
			return;
		
		graphFrame.setData(stats);
	}

	private static void runTests() {
		System.out.println("Started tests...");

		try {
			Process tests =
					Runtime.getRuntime().exec(LOCAL_STATS_PATH + "/gradlew connectedAndroidTest");
			tests.waitFor();
		} catch (IOException | InterruptedException e) {
			e.printStackTrace();
			return;
		}

		System.out.println("Finished tests");
	}

	private static void callAdbCopyStatsFile(){
		System.out.println("Copying file over");

		int result = 0;

		try {
			Process p =
				     Runtime.getRuntime().exec("adb pull " + TestProperties.PATH_TO_STATS_FILE_ON_DEVICE + " " + LOCAL_STATS_PATH
			);
			result = p.waitFor();
		} catch (IOException | InterruptedException e) {
			e.printStackTrace();
			return;
		}

		if(result != 0)
			System.out.println("Got non-zero result :(");

		System.out.println("Finished copying file");
	}
	
	private static List<AggregateStats> readStatsFile() {

        File fileIn  = new File(LOCAL_STATS_PATH + "/" + TestProperties.STATS_FILE_NAME);

		BufferedReader reader = null;

		List<AggregateStats> allStats = new ArrayList<>();

         try {

			 reader = new BufferedReader(new FileReader(fileIn),
					 TestProperties.TOTAL_NUMBER_OF_SAMPLES*1100);

			 String line;

			 while ((line = reader.readLine()) != null)
			 {
				 AggregateStats stats;
				 try {
					 stats = new Gson().fromJson(line, AggregateStats.class);
				 } catch(Exception e){
					 System.out.println("Gson is unable to parse: " + line);
					 continue;
				 }
				 allStats.add(stats);
			 }
		} catch (IOException e1) {
			e1.printStackTrace();
		}
        finally{
			try {
				if(reader != null)
					reader.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
        }

        return allStats;
	}
	
}
