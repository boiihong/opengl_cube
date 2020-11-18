// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this
//    example is to demonstrate the basic concepts of
//    OpenGL ES 3.0 rendering.
#include "DrawObject.h"
#include "esUtil.h"


typedef struct
{
	TriangleDrawObject	*triangle_drawObject;
	SkyboxDrawObject	*skybox_drawObject;
	ESCamera			*viewer;

} UserData;


void InitCam(ESContext *esContext, ESCamera *cam)
{
	cam->posX = 3.0f;
	cam->posY = -5.0f; 
	cam->posZ = 0.0f;
	cam->lookAtX = 0.0f;
	cam->lookAtZ = 0.0f;
	cam->lookAtY = 1.0f;
	cam->upX = 0.0f;
	cam->upY = 0.0f;
	cam->upY = 1.0f;
	cam->aspect = (GLfloat)esContext->width / (GLfloat)esContext->height;
	cam->nearZ = 0.5f;
	cam->farZ = 30.0f;
	cam->fovy = 100.0f;
}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;

   // set initial camera position
   userData->viewer = new ESCamera();
   ESCamera *cam = userData->viewer;
   InitCam(esContext, cam);

   // initialize triangle drawing object .
   char vShaderStr_tri[] = "Triangle_VS.txt";
   char fShaderStr_tri[] = "Triangle_FS.txt";
	userData->triangle_drawObject = new TriangleDrawObject(vShaderStr_tri, fShaderStr_tri);
	if (!userData->triangle_drawObject->Init()) {
		return FALSE;
	}

	// load cubemap image 
	const char *back_file = "D:\\Download\\skybox\\skybox\\back.jpg";
	const char *bottom_file = "D:\\Download\\skybox\\skybox\\bottom.jpg";
	const char *front_file = "D:\\Download\\skybox\\skybox\\front.jpg";
	const char *left_file = "D:\\Download\\skybox\\skybox\\left.jpg";
	const char *right_file = "D:\\Download\\skybox\\skybox\\right.jpg";
	const char *top_file = "D:\\Download\\skybox\\skybox\\top.jpg";
	const char *images[6] = { right_file, left_file, top_file, bottom_file, back_file, front_file };
	GLuint skyboxTextureId = loadCubemap(images);

	// initializing skybox draw object
	char vShaderStr_sky[] = "Skybox_VS.txt";
	char fShaderStr_sky[] = "Skybox_FS.txt";
	userData->skybox_drawObject = new SkyboxDrawObject(vShaderStr_sky, fShaderStr_sky, cam);
	if (!userData->skybox_drawObject->Init(skyboxTextureId)) {
		return FALSE;
	}

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;


   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   userData->skybox_drawObject->Draw();
   userData->triangle_drawObject->Draw();

}

void Shutdown ( ESContext *esContext )
{
	UserData *userData = (UserData *)esContext->userData;
	delete userData->triangle_drawObject;
}


void KeyStroke(ESContext *esContext, unsigned char c, int x, int y)
{
	UserData *userData = (UserData *)esContext->userData;
	userData->skybox_drawObject->CameraMove(c);
	
}

void Update(ESContext *esContext, float deltaTime)
{
	UserData *userData = (UserData *)esContext->userData;
	userData->skybox_drawObject->Update(deltaTime);
}

int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Hello Triangle", 640, 480, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterUpdateFunc(esContext, Update);
   esRegisterDrawFunc ( esContext, Draw );
   esRegisterKeyFunc(esContext, KeyStroke);

   return GL_TRUE;
}
