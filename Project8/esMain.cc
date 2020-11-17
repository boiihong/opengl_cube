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
#include "esUtil.h"
#include "DrawObject.h"

typedef struct
{
	TriangleDrawObject * triangle_drawObject;
   
} UserData;


///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   char vShaderStr[] = "Triangle_VS.txt";
   char fShaderStr[] = "Triangle_FS.txt";

	userData->triangle_drawObject = new TriangleDrawObject(vShaderStr, fShaderStr);
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
	const char *images[6] = { right_file, left_file, top_file, bottom_file, front_file, back_file };
	GLuint skyboxTextureId = loadCubemap(images);



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

   userData->triangle_drawObject->Draw();
}

void Shutdown ( ESContext *esContext )
{
	UserData *userData = (UserData *)esContext->userData;
	delete userData->triangle_drawObject;
}

int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}
