

#ifndef GOALPOINT_H_
#define GOALPOINT_H_

#include "Drivers/Goals/goalobject.h"

class GoalCuboid  : public GoalObject {

public:

	GoalCuboid(unsigned short x_center, unsigned short y_center, unsigned short z_center,
				GLuint dist_x, GLuint dist_y, GLuint dist_z,
				GLuint max_x, GLuint max_y, GLuint max_z,
				GoalPointType type, GLubyte pullToWallsDir) : GoalObject(type, max_x, max_y, max_z) {

		groundDirection = pullToWallsDir;

		x = x_center;
		y = y_center;
		z = z_center;

		GLint st = (x - dist_x);
		x_start = GLuint(st < 0 ? 0 : st);
		st = (y - dist_y);
		y_start = GLuint(st < 0 ? 0 : st);
		st = (z - dist_z);
		z_start = GLuint(st < 0 ? 0 : st);

		x_end = x + dist_x;
		if(x_end > max_x) x_end = max_x;
		y_end = y + dist_y;
		if(y_end > max_y) y_end = max_y;
		z_end = z + dist_z;
		if(z_end > max_z) z_end = max_z;


		expectedTargetCountInCube = ((x_end - x_start)*(y_end - y_start)*2 + (z_end - z_start - 2)*(y_end - y_start)*2 + 20);

		availableTargets = new IterativeCollection<GridPosition*>(expectedTargetCountInCube, TARGET_LIST);
		filledTargets = new IterativeCollection<GridPosition*>(expectedTargetCountInCube, TARGET_LIST);

	}

	bool isUpForDestruction(){
		return true;
	}

};


#endif /* GOALPOINT_H_ */
