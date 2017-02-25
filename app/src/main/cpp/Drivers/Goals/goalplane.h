
#ifndef GOALPLANE_H_
#define GOALPLANE_H_

#include "Drivers/Goals/goalobject.h"

class GoalPlane  : public GoalObject{

public:

	GLubyte normalDirection;
	unsigned short x_y_z_value;

	GoalPlane(unsigned short _x_y_z_value, GLubyte normalAxis,
			  unsigned short max_x, unsigned short max_y, unsigned short max_z,
				GoalPointType type, GLubyte pullToWallsDir) :
					GoalObject(type, max_x, max_y, max_z) {

		groundDirection = pullToWallsDir;

		normalDirection = normalAxis;
		x_y_z_value = _x_y_z_value;

		switch(normalDirection){
			case X_DIR:
				x = x_y_z_value;
				y = max_y/2u;
				z = max_z/2u;

				y_start = 0;
				z_start = 0;

				y_end = max_y;
				z_end = max_z;

				x_start = x_y_z_value;
				x_end = x_y_z_value + 1;

				expectedTargetCountInCube = (max_y*max_z);
				break;
			case Y_DIR:
				x = max_x/2;
				y = x_y_z_value;
				z = max_z/2;

				x_start = 0;
				z_start = 0;

				x_end = max_x;
				z_end = max_z;

				y_start = x_y_z_value;
				y_end = x_y_z_value + 1;

				expectedTargetCountInCube = (max_x*max_z);
				break;
			case Z_DIR:
				x = max_x/2;
				y = max_y/2;
				z = x_y_z_value;

				x_start = 0;
				y_start = 0;

				x_end = max_x;
				y_end = max_y;

				z_start = x_y_z_value;
				z_end = x_y_z_value + 1;

				expectedTargetCountInCube = (max_x*max_y);
				break;
			default:
				//TODO?
				break;
		}

		availableTargets = new IterativeCollection<GridPosition*>(expectedTargetCountInCube, TARGET_LIST);
		filledTargets = new IterativeCollection<GridPosition*>(expectedTargetCountInCube, TARGET_LIST);
	}

	bool isUpForDestruction(){
		return true;
	}

};



#endif /* GOALPLANE_H_ */
