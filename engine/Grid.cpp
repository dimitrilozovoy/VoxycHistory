/*
Copyright (C) 2018 Dimitri Lozovoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Grid.h"

void Grid::init(TextureManager2 *texMan, SpriteRenderer2D *spr)
{
	this->texMan = texMan;
	this->spr = spr;
	
	programMain = loadProgram((char *)vertexShaderCodeES20, (char *)fragmentShaderCodeES20, false);
	
	initialized = true;
}

void Grid::draw(int size)
{
	if (!initialized)
		init(nullptr, nullptr);
	
	fb.bind();
	
	glDisable(GL_BLEND);
	
	Texture *tex = texMan->find("0.png");
    spr->DrawSprite(0, 0,
	    1.0, 1.0, tex->glTexID,
	    1.0, 1.0, 1.0, 1.0, true);
	
//	glClear(GL_COLOR_BUFFER_BIT);
//	glClearColor(1.0, 0.0, 0.0, 1.0);
	
	// Horizontal
	float fgap = 2.0 / (float)size;
	
	for (int y = 0; y < size; y++)
	{
		float cy = -1.0 + (float)y * fgap;
		drawLine(-1.0, cy, 1.0, cy);
	}
	
	// Vertical
	for (int x = 0; x < size; x++)
	{
		float cx = -1.0 + (float)x * fgap;
		drawLine(cx, -1.0, cx, 1.0);
	}
	
	glEnable(GL_BLEND);
	
	fb.unbind();
}

void Grid::drawLine(float x1, float y1, float x2, float y2)
{
	float data[8];
	
	data[0] = x1;
	data[1] = y2;
	data[2] = -1.0;
	data[3] = 1.0;
	
	data[4] = x1;
	data[5] = y2;
	data[6] = -1.0;
	data[7] = 1.0;
	
	int vbo;
	
	// Generate VBO
	glGenBuffers(1, (GLuint *)&vbo);
	checkGLError("glGenBuffers");
	
	// Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLError("glBindBuffer");
	
	// Load data into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, data, GL_STATIC_DRAW);
    checkGLError("glBufferData");

	// Use program
	glUseProgram(programMain);
    checkGLError("glUseProgram");
	
	// Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLError("glBindBuffer");

	// Set position
	setVertexAttrib(programMain, "vPosition", 4, GL_FLOAT, false, 4 * sizeof(float), 0);
	
	// Set color
	setUniform4f(programMain, "vColor", 1.0f, 1.0f, 1.0, 1.0f);
	
    // Draw
    glDrawArrays(GL_LINES, 0, 2);
    checkGLError("glDrawArrays");
}

void Grid::checkGLError(char *str)
{
#ifdef DEBUG_BUILD
#if defined PLATFORM_OSX || defined PLATFORM_GVR || defined PLATFORM_ANDROID || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR || defined PLATFORM_IOS
#ifdef USE_OPENGL
    GLenum err = glGetError();

    switch (err)
    {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            Log("GL error GL_INVALID_ENUM", str);
            break;
        case GL_INVALID_VALUE:
            Log("GL error GL_INVALID_VALUE", str);
            break;
        case GL_INVALID_OPERATION:
            Log("GL error GL_INVALID_OPERATION", str);
            break;
#ifndef USE_OPENGL_3
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            Log("GL error GL_INVALID_FRAMEBUFFER_OPERATION", str);
            break;
#endif
        case GL_OUT_OF_MEMORY:
            Log("GL error GL_OUT_OF_MEMORY", str);
            break;
    }
#endif
#endif
#endif
}

