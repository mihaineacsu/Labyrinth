#include "cubeCoords.h"

/* Stocheaza coordonatele cuburilor in vederea calcularii
coliziunilor */
cubeCoords::cubeCoords(float xCenter, float zCenter, float wallSize){
			wallSize /= 2;
			float x1 = xCenter + wallSize,
				x2 = xCenter - wallSize,
				z1 = zCenter + wallSize,
				z2 = zCenter - wallSize;

			if (x1 > x2){
				xMin = x2;
				xMax = x1;
			}
			else{
				xMin = x1;
				xMax = x2;
			}

			if (z1 > z2){
				zMin = z2;
				zMax = z1;
			}
			else{
				zMin = z1;
				zMax = z1;
			}
}