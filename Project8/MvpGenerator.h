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

		_change_flag = false;
	}

	virtual int GenModel() { return GL_FALSE; }


	int GenView(bool removeTranslation)
	{
		esMatrixLoadIdentity(&viewMatrix);

		// change up vector
		GLfloat x_dir = _camera->lookAtX - _camera->posX;
		GLfloat y_dir = _camera->lookAtY - _camera->posY;
		GLfloat z_dir = _camera->lookAtZ - _camera->posZ;
		Normalize(&x_dir, &y_dir, &z_dir);
		_camera->upX = -y_dir;
		_camera->upY = -x_dir;
		_camera->upZ = 0.0f;

		esMatrixLookAt(&viewMatrix, _camera->posX, _camera->posY, _camera->posZ,
			_camera->lookAtX, _camera->lookAtY, _camera->lookAtZ,
			_camera->upX, _camera->upY, _camera->upZ);
		if (removeTranslation)
		{
			viewMatrix.m[3][0] = viewMatrix.m[3][1] = viewMatrix.m[3][2] = viewMatrix.m[3][3] = 0.0;
			viewMatrix.m[0][3] = viewMatrix.m[1][3] = viewMatrix.m[2][2] = 0.0;
		}
		return GL_TRUE;
	}

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

	bool CheckFlag()
	{
		return _change_flag;
	}

	bool ClearFlag()
	{
		if (_change_flag == GL_TRUE)
		{
			_change_flag = GL_FALSE;
			return GL_TRUE;
		}
		else
		{
			return GL_FALSE;
		}
	}

	bool SetFlag()
	{
		if (_change_flag == GL_FALSE)
		{
			_change_flag = GL_TRUE;
			return GL_TRUE;
		}
		else 
		{
			return GL_FALSE;
		}
	}

	ESCamera *_camera;
	ESMatrix modelMatrix;
	ESMatrix viewMatrix;
	ESMatrix perspectiveMatrix;
	ESMatrix mvpMatrix;
private : 
	GLboolean _change_flag;
};