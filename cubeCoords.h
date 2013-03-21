#ifndef _CUBE_COORDS_
#define _CUBE_COORDS_

class cubeCoords{
	public:
		float xMin, xMax,
			zMin, zMax;

		cubeCoords(float xCenter, float zCenter, float wallSize);
};

#endif