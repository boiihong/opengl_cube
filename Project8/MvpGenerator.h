#pragma once

#include "esUtil.h"


class MvpGenerator 
{

public:
	virtual int GenModelview() { return GL_FALSE; };
	int GenPerspective(float fovy, float aspect, float nearZ, float farZ) 
	{
		esPerspective(&perspectiveMatrix, fovy, aspect, nearZ, farZ);
		return GL_TRUE;
	}
	int GenMvp() 
	{
		esMatrixMultiply(&mvpMatrix, &modelviewMatrix, &perspectiveMatrix);
		return GL_TRUE;
	}

	ESMatrix modelviewMatrix;
	ESMatrix perspectiveMatrix;
	ESMatrix mvpMatrix;
};