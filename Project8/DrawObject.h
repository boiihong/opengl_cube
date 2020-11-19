#pragma once

#include "esUtil.h"
#include "MvpGenerator.h"

class DrawObject {
public:
	DrawObject(const char *vsSrcFile, const char *fsSrcFile);
	~DrawObject() {
		if (programObject != NULL) {
			glDeleteProgram(programObject);
		}
	}

	// make program object using shader source
	int Init();
	virtual void Draw() {};
	virtual void Update(float deltaTime) {};

private :
	const char *_vsSrcFile;
	const char *_fsSrcFile;

protected:
	ESContext *esContext;
	GLuint programObject;

};

class TriangleDrawObject : DrawObject {
public: 
	TriangleDrawObject(const char *vsSrcFile, const char *fsSrcFile)
		:DrawObject(vsSrcFile, fsSrcFile) {};

	int Init();
	void Draw();
	void Update(float deltaTime);
};


class SkyboxDrawObject : DrawObject, MvpGenerator {
public:
	SkyboxDrawObject(const char *vsSrcFile, const char *fsSrcFile, ESCamera *camera_in) :
		DrawObject(vsSrcFile, fsSrcFile) , MvpGenerator(camera_in) { };
	
	int Init(GLuint skyboxTexture_in);
	void Draw();
	void Update(float deltaTime) override;

	// Mvp related methods 
	int GenModel() override;
	
	// Control methods 
	void CameraMove(unsigned char input);

private:

	GLuint _skyboxTextureId;
	const char *_skyboxName = "skybox";
	GLint _skyboxUniformLoc;
	const char *_mvpName = "u_mvpMatrix";
	GLint _mvpUniformLoc;

};


class RotatingCubeObject : DrawObject, MvpGenerator {
public:
	RotatingCubeObject(const char *vsSrcFile, const char *fsSrcFile, ESCamera *camera_in)
		:DrawObject(vsSrcFile, fsSrcFile), MvpGenerator(camera_in) {};
	~RotatingCubeObject();
	int Init(GLuint skyboxTexture_in);
	void Draw();
	void Update(float deltaTime) override;

	// Mvp related methods 
	int GenModel() override;

	// control related methods 
	void CameraMove(unsigned char input);

private:
	GLfloat *_vertices;
	GLuint	*_indices;
	GLint	_num_vertices;
	GLfloat _angle;

	GLuint _skyboxTextureId;
	const char *_skyboxName = "skybox";
	GLint _skyboxUniformLoc;
	const char *_mvpName = "u_mvpMatrix";
	GLint _mvpUniformLoc;
};

