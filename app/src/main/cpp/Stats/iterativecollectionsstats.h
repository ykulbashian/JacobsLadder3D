
#ifndef ITERATIVECOLLECTIONSTATS_H_
#define ITERATIVECOLLECTIONSTATS_H_

#include "Utils/iterativelisttypes.h"
#include <stdio.h>
#include <string>

struct IterativeCollectionStats{
public:
	AddedToListType type;
	unsigned int capacity;
	unsigned int filled;
	unsigned int movingAverage;

	std::string getAsJson(){
		char buff[200];
		snprintf(buff, sizeof(buff), "{filled:\"%d\",capacity:\"%d\",movingAverage:\"%d\"},",
				filled,
				capacity,
				movingAverage);

		return std::string(buff);
	}

};


#endif /* ITERATIVECOLLECTIONSTATS_H_ */
