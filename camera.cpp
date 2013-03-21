#include "camera.h"
#include "cubeCoords.h"
#include "glm\glm.h"
#include <iostream>

#define charScale 5

GLMmodel* pmodel1 = NULL;

Camera::Camera(){
}
Camera::~Camera(){
}


void drawmodel_box(void)
{
	// Load the model only if it hasn't been loaded before
	// If it's been loaded then pmodel1 should be a pointer to the model geometry data...otherwise it's null
    if (!pmodel1) 
	{
		// this is the call that actualy reads the OBJ and creates the model object
        pmodel1 = glmReadOBJ("Chell.obj");	
        if (!pmodel1) exit(0);
		// This will rescale the object to fit into the unity matrix
		// Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
        glmUnitize(pmodel1);
		// These 2 functions calculate triangle and vertex normals from the geometry data.
		// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
		// So if you can export these directly from you modeling tool do it and comment these line
		// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
        glmFacetNormals(pmodel1);        
		glmVertexNormals(pmodel1, 90.0);
    }
    // This is the call that will actualy draw the model
	// Don't forget to tell it if you want textures or not :))
    glmDraw(pmodel1, GLM_SMOOTH);	
}

bool Camera::checkForCollision(Vector3D futurePosition){
	 float charMargin = 0.26 * charScale;

	 if (futurePosition.x < end->xMax && futurePosition.x > end->xMin &&
	 	futurePosition.z < end->zMax && futurePosition.z > end->zMin)
	 		exit(0);

	for (int i = 0; i < cube.size(); ++i)
		if (futurePosition.x < cube[i].xMax + charMargin && futurePosition.x > cube[i].xMin - charMargin
			&& futurePosition.z < cube[i].zMax + charMargin && futurePosition.z > cube[i].zMin - charMargin)
				return true;

	return false;
}


void Camera::init(Vector3D startPos, std::vector<cubeCoords> cubes, cubeCoords *endPos){
	position = startPos;
	initial = startPos;

	forward = Vector3D(0,0,-1);
	initial = Vector3D(0, 0, -1);

	up = Vector3D(0,1,0);

	right = Vector3D(1,0,0);

	angle = Vector3D(0, -0.5, 0);

	cube = cubes;

	end = endPos;
}

void Camera::translateForward(int sign, int moveSpeed){
	toMove = Vector3D(forward.x, 0, forward.z);

	Vector3D futurePosition = position;
	futurePosition += (toMove * moveSpeed * sign);

	if (checkForCollision(futurePosition))
		;
	else
		position = futurePosition;
}
void Camera::translateUp(int sign, int moveSpeed){
	
}
void Camera::translateRight(int sign, int moveSpeed){
	toMove = Vector3D(-forward.z, 0, forward.x);

	Vector3D futurePosition = position;
	futurePosition += (toMove * moveSpeed * sign);

	if (checkForCollision(futurePosition)){
		;
	}
	else
		position = futurePosition;
}

void Camera::onMouseMove(int x, int y, float mouseSpeed, GameMode gameMode){
	static bool warp = false;

	if(!warp) {
		int windowWidth = glutGet(GLUT_WINDOW_WIDTH),
			windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

		angle.x -= (x - windowWidth / 2) * mouseSpeed;
		angle.y -= (y - windowHeight / 2) * mouseSpeed;

		switch(gameMode){
			case map:
			case first_person:
				
				//limiteaza privirea la anumite unghiuri
				if(angle.y < -M_PI / 2)
					angle.y = -M_PI / 2 + 0.0000001f;
				if(angle.y >= M_PI / 2)
					angle.y = M_PI / 2 - 0.0000001f;

				break;

			case third_person:
				if(angle.y < -M_PI / 2)
					angle.y = -M_PI / 2 + 0.0000001f;
				if(angle.y >= M_PI / 2)
					angle.y = M_PI / 2 - 0.2f;

				break;
		}

		warp = true; //se apeleaza din nou mouse callback
					//nu se ia considerare positia mouse-ului
					//la reapelare

		glutWarpPointer(windowWidth / 2, windowHeight / 2);
	}else 
		warp = false;	
}

void Camera::updateVectors(){
			forward.x = sinf(angle.x);
			forward.y = 0;
			forward.z = cosf(angle.x);

			right.x = -cosf(angle.x);
			right.y = 0;
			right.z = sinf(angle.x);

			lookAt.x = sinf(angle.x) * cosf(angle.y);
			lookAt.y = sinf(angle.y);
			lookAt.z = cosf(angle.x) * cosf(angle.y);

			up = right.CrossProduct(lookAt);
}

void Camera::render(GameMode gameMode, float radius){
	updateVectors();

	//am incercat sa implementez lumina unei lanterne
	GLfloat spotDir [] = {position.x + lookAt.x, position.y + lookAt.y, position.z + lookAt.z};
	switch(gameMode){
		case first_person:
			gluLookAt(position.x, position.y, position.z,
				position.x + lookAt.x, position.y + lookAt.y, position.z + lookAt.z,
				0, 1, 0);	

			break;

		case third_person:{
			for (int i = 0; i < cube.size(); ++i)
				while (position.x - forward.x * radius < cube[i].xMax && 
					position.x - forward.x * radius > cube[i].xMin &&
					position.z - forward.z * radius < cube[i].zMax &&
					position.z - forward.z * radius > cube[i].zMin)
					radius--;
			
			 float yRadius = angle.y * radius;

			if (yRadius < 0)
				yRadius = 0;

			while (yRadius > 30)
				yRadius = 30;

			spotDir[1] = position.x + forward.x;
			spotDir[1] = position.y + lookAt.y;
			spotDir[2] = position.z + forward.z;

			gluLookAt(position.x - forward.x * radius, yRadius, position.z - forward.z * radius,
				position.x + forward.x, position.y + lookAt.y + 2, position.z + forward.z,
				0, 1, 0);
				
			glPushMatrix();
			
			//scaleaza si roteste caracterul odata cu camera
			glTranslatef(position.x , 1, position.z);
			glRotatef(-initial.Angle(forward), 0, 1, 0);
			glScalef(charScale, charScale, charScale);
			glRotatef(-30, 0, 1, 0);
			drawmodel_box();

			glPopMatrix();

			break;
		}
		case mini_map:
			gluLookAt(position.x + forward.x, 100, position.z + forward.z,
				position.x + forward.x, position.y + lookAt.y, position.z + forward.z,
				0, 0, 1);
				
			glPushMatrix();
			
			glTranslatef(position.x , 1, position.z);
			glRotatef(-initial.Angle(forward), 0, 1, 0);
			glScalef(charScale, charScale, charScale);
			glRotatef(-30, 0, 1, 0);
			drawmodel_box();

			glPopMatrix();

			return;
			break;
		case map:
			gluLookAt(position.x + forward.x, 400, position.z + forward.z,
				position.x + forward.x, position.y + lookAt.y, position.z + forward.z,
				0, 0, 1);
				
			glPushMatrix();
			
			glTranslatef(position.x , 1, position.z);
			glRotatef(-initial.Angle(forward), 0, 1, 0);
			glScalef(charScale, charScale, charScale);
			glRotatef(-30, 0, 1, 0);
			drawmodel_box();

			glPopMatrix();

			break;
	}


	glEnable(GL_LIGHTING);

	GLfloat lightPos [] = {position.x, position.y , position.z, 0};
	glLightfv (GL_LIGHT0, GL_POSITION, lightPos);
	
	glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2);

	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90);

	glEnable(GL_LIGHT0);

	
	lightPos[1] = position.y + 10;
	glLightfv (GL_LIGHT1, GL_POSITION, lightPos);
	
	spotDir[0] = position.x;
	spotDir[1] = position.y;
	spotDir[2] = position.z;
	glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);

	
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.02);

	glEnable(GL_LIGHT1);

}