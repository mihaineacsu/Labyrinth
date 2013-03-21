//clasa camera
#pragma once
#include <vector>
#include "Vector3D.h"
#include "cubeCoords.h"

enum GameMode{
	first_person,
	third_person,
	map,
	mini_map
};

class Camera{
	public:
		Camera();
		~Camera();

		void translateForward(int sign, int moveSpeed);
		void translateUp(int sign, int moveSpeed);
		void translateRight(int sign, int moveSpeed);

		void rotateFPS();

		void onMouseMove(int x, int y, float mouseSpeed, GameMode );

		void init(Vector3D startPos, std::vector<cubeCoords> cubes, cubeCoords *endPos);
		void render(GameMode gameMode, float radius);
		void updateVectors();

		bool checkForCollision(Vector3D futurePosition);

	private:
		Vector3D forward,
			up,
			right,
			position,
			lookAt,
			angle,
			toMove,
			initial;
		cubeCoords *end;

		std::vector<cubeCoords> cube;
};