
#ifndef GOALLINE_H_
#define GOALLINE_H_

#include "Drivers/Goals/goalobject.h"
#include <cstdlib>

class GoalLine : public GoalObject{

public:
	GLubyte _axis;

	GoalLine(GLuint u, GLuint v,
				GLuint dist_u, GLuint dist_v,
				GLuint max_x, GLuint max_y, GLuint max_z,
				GLubyte axis, GoalPointType type, GLubyte pullToWallsDir) :
					GoalObject(type, max_x, max_y, max_z) {

		groundDirection = pullToWallsDir;

		_axis = axis;
		GLint st = 0;
		GLuint axisLength = 0;

		switch(_axis){
			case X_DIR:
				x_start = 0;
				x_end = max_x;
				x = max_x/2;
				axisLength = max_x;

				st = (v - dist_v);
				y_start = GLuint(st < 0 ? 0 : st);
				y_end = v + dist_v;
				if(y_end > max_y) y_end = max_y;
				y = v;

				st = (u - dist_u);
				z_start = GLuint(st < 0 ? 0 : st);
				z_end = u + dist_u;
				if(z_end > max_z) z_end = max_z;
				z = u;

				break;
			case Y_DIR:
				y_start = 0;
				y_end = max_y;
				y = max_y/2;
				axisLength = max_y;

				st = (u - dist_u);
				x_start = GLuint(st < 0 ? 0 : st);
				x_end = u + dist_u;
				if(x_end > max_x) x_end = max_x;
				x = u;

				st = (v - dist_v);
				z_start = GLuint(st < 0 ? 0 : st);
				z_end = v + dist_v;
				if(z_end > max_z) z_end = max_z;
				z = v;

				break;
			case Z_DIR:
				z_start = 0;
				z_end = max_z;
				z = max_z/2;
				axisLength = max_z;

				st = (u - dist_u);
				x_start = GLuint(st < 0 ? 0 : st);
				x_end = u + dist_u;
				if(x_end > max_x) x_end = max_x;
				x = u;

				st = (v - dist_v);
				y_start = GLuint(st < 0 ? 0 : st);
				y_end = v + dist_v;
				if(y_end > max_y) y_end = max_y;
				y = v;

				break;
			default:
				//TODO?
				break;
		}

		expectedTargetCountInCube = ((x_end - x_start)*(y_end - y_start)*axisLength);

		availableTargets = new IterativeCollection<GridPosition*>(expectedTargetCountInCube, TARGET_LIST);
		filledTargets = new IterativeCollection<GridPosition*>(expectedTargetCountInCube, TARGET_LIST);
	}

	bool isUpForDestruction(){
		return true;
	}

};


#endif /* GOALLINE_H_ */
