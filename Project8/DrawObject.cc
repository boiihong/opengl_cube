#include "DrawObject.h"
#include "esUtil.h"
#include <stdio.h>
#include <string.h>

DrawObject::DrawObject(const char *vsSrcFile, const char *fsSrcFile) 
{
	_vsSrcFile = vsSrcFile;
	_fsSrcFile = fsSrcFile;
}

int DrawObject::Init() 
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = esLoadShader(GL_VERTEX_SHADER, _vsSrcFile);
	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, _fsSrcFile);

	// Create the program object
	programObject = glCreateProgram();

	if (programObject == 0)
	{
		printf("cannor create program object !!! ");
		return GL_FALSE;
	}

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char *infoLog = (char *)malloc(sizeof(char) * infoLen);

			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			esLogMessage("Error linking program:\n%s\n", infoLog);

			free(infoLog);
		}

		glDeleteProgram(programObject);
		return GL_FALSE;
	}

	// Store the program object
	this->programObject = programObject;

	return GL_TRUE;
}

int TriangleDrawObject::Init()
{
	GLboolean res;
	res = DrawObject::Init();

	return res;
}
void TriangleDrawObject::Draw()
{
	GLfloat vVertices[] = { 0.0f,  0.5f, 0.0f,
						 -0.5f, -0.5f, 0.0f,
						 0.5f, -0.5f, 0.0f
	};

	// Use the program object
	glUseProgram(programObject);

	// Load the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

}
void TriangleDrawObject::Update(float deltaTime)
{

}


// get uniform location and texture id.
int SkyboxDrawObject::Init(GLuint skyboxTexture_in)
{
	DrawObject::Init();

	// texture 
	this->_skyboxTextureId = skyboxTexture_in;
	this->_skyboxUniformLoc = glGetUniformLocation(DrawObject::programObject, this->_skyboxName);
	if (this->_skyboxUniformLoc == GL_INVALID_VALUE || this->_skyboxUniformLoc == GL_INVALID_OPERATION)
	{
		printf("cannot find skybox uniform value\n");
		return GL_FALSE;
	}

	// mvp
	this->_mvpUniformLoc = glGetUniformLocation(DrawObject::programObject, this->_mvpName);
	if (this->_mvpUniformLoc == GL_INVALID_VALUE || this->_mvpUniformLoc == GL_INVALID_OPERATION)
	{
		printf("cannot find mvp uniform value\n");
		return GL_FALSE;
	}

	GenModel();
	GenView(TRUE);
	GenPerspective();
	GenMvp();

	return GL_TRUE;
}

int SkyboxDrawObject::GenModel()
{
	esMatrixLoadIdentity(&modelMatrix);
	esTranslate(&modelMatrix, 0.0, 0.0, 0.0);

	return GL_TRUE;
}
int SkyboxDrawObject::GenView(bool removeTranslation)
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
		viewMatrix.m[3][0] = viewMatrix.m[3][1]  = viewMatrix.m[3][2] = viewMatrix.m[3][3]=  0.0;
		viewMatrix.m[0][3] = viewMatrix.m[1][3]  = viewMatrix.m[2][2] = 0.0;
	}
	return GL_TRUE;
}
void SkyboxDrawObject::Draw()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f

	};
	
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	
	glUseProgram(programObject);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BINDING_CUBE_MAP, _skyboxTextureId);
	glUniform1i(_skyboxUniformLoc, 0);

	// Load the vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT,
		GL_FALSE, 3 * sizeof(GLfloat), skyboxVertices);
	glEnableVertexAttribArray(0);

	glUniformMatrix4fv(this->_mvpUniformLoc, 1, GL_FALSE, (GLfloat *)&mvpMatrix.m[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
}

void SkyboxDrawObject::CameraMove(unsigned char input)
{
	if (input == (unsigned char)'a')
	{
		printf(" add look At ...\b");
		// move camera ... 
		_camera->lookAtY += 0.1;

		SetFlag();
	}
	else if (input == (unsigned char)'j')
	{
		_camera->posY += 1.0f;
		_camera->lookAtY += 1.0f;
		SetFlag();
	}
}

void SkyboxDrawObject::Update(float deltaTime)
{
	if ( CheckFlag() )
	{
		GenModel();
		GenView(TRUE);
		GenPerspective();
		GenMvp();

		ClearFlag();
	}
}