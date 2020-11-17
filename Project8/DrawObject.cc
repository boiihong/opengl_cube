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
}

void SkyboxDrawObject::Draw()
{

}