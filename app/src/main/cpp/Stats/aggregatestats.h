
#ifndef AGGREGATESTATS_H_
#define AGGREGATESTATS_H_

#include "Stats/iterativecollectionsstats.h"
#include "Stats/goalstats.h"
#include "Stats/driverstats.h"
#include "Stats/gridstats.h"
#include <stdio.h>
#include <string>


struct AggregateStats {

public:
	DriverStats driverStats;

	GoalAggregateStats goalAggregateStats;

	IterativeCollectionStats arrayStats[4];

	GridStats gridStats;

	std::string getAsJson(){

		std::string fullStatsString("{arrayStats:[");

		std::string arrayStatsJson("");
		for(int i = 0; i < 4; i++){
			arrayStatsJson += arrayStats[i].getAsJson();
		}

		fullStatsString += arrayStatsJson + std::string("],goalAggregateStats:{");

		std::string goalStatsJson = goalAggregateStats.getAsJson();

		fullStatsString += goalStatsJson + std::string("},identityGridStats:");

		std::string gridStatsJson = gridStats.getAsJson();

		fullStatsString += gridStatsJson + std::string(",driverStats:");

		std::string driverStatsJson = driverStats.getAsJson();
		fullStatsString += driverStatsJson + std::string("}\r\n");

		return fullStatsString;
	}

};



#endif /* AGGREGATESTATS_H_ */
