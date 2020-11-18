#pragma once

#include "esUtil.h"

#include <stdio.h>

struct ESCamera;

class MvpGenerator 
{

public:
	MvpGenerator(ESCamera *_camera_in) {
		if (_camera_in == NULL) {
			printf("camera is not initialized \n");
		}
		else {
			_camera = _camera_in;
		}
		esMatrixLoadIdentity(&modelMatrix);
		esMatrixLoadIdentity(&viewMatrix);
		esMatrixLoadIdentity(&perspectiveMatrix);
		esMatrixLoadIdentity(&mvpMatrix);
	}

	virtual int GenModel() { return GL_FALSE; }


	virtual int GenView(bool removeTranslation)
	{ return GL_FALSE; }

	int GenPerspective() 
	{
		esMatrixLoadIdentity(&perspectiveMatrix);
		esPerspective(&perspectiveMatrix, _camera->fovy, _camera->aspect, _camera->nearZ, _camera->farZ);
		return GL_TRUE;
	}
	int GenMvp() 
	{
		ESMatrix modelviewMatrix_temp;
		esMatrixMultiply(&modelviewMatrix_temp, &modelMatrix, &viewMatrix);
		esMatrixMultiply(&mvpMatrix, &modelviewMatrix_temp, &perspectiveMatrix);

		//printmat(mvpMatrix);

		return GL_TRUE;
	}


	ESCamera *_camera;
	ESMatrix modelMatrix;
	ESMatrix viewMatrix;
	ESMatrix perspectiveMatrix;
	ESMatrix mvpMatrix;
};