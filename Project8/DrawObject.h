#pragma once

#include "esUtil.h"

class DrawObject {
public:
	DrawObject(const char *vsSrcFile, const char *fsSrcFile);
	~DrawObject() {
		if (programObject != NULL) {
			glDeleteProgram(programObject);
		}
	}

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