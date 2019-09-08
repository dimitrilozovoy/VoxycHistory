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

#include "SoftCanvas.h"
#include "GLIncludes.h"

void SoftCanvas::init(int width, int height)
{
	this->width = width;
	this->height = height;
	
	data = new unsigned char[bytesPerPx * height * width];
	
	memset(data, 0, bytesPerPx * height * width);
}

void SoftCanvas::release()
{
	if (data != nullptr)
		delete data;
}

void SoftCanvas::clear(char r, char g, char b, char a)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			setPx(x, y, r, g, b, a);
		}
	}
	
/*	for (int x = 0; x < width; x++)
	{
		setPx(x, 5, 255, 0, 0, 255);
	}*/
}

void SoftCanvas::drawGrid(int size, char r, char g, char b, char a, bool showCenter)
{
	// Horizontal
	int gap = height / size;
	
	for (int y = 0; y < size; y++)
	{
		for (int px = 0; px < width; px++)
		{
			if (showCenter && y == size / 2)
			    setPx(px, y * gap, r * 1.5, g * 1.5, b * 1.5, a);
			else
				setPx(px, y * gap, r, g, b, a);
		}
	}
	
	for (int px = 0; px < width; px++)
	{
		setPx(px, height - 1, r, g, b, a);
	}
	
	// Horizontal
	gap = width / size;
	
	for (int x = 0; x < size; x++)
	{
		for (int py = 0; py < height; py++)
		{
		    if (showCenter && x == size / 2)
			    setPx(x * gap, py, r * 1.5, g * 1.5, b * 1.5, a);
			else
				setPx(x * gap, py, r, g, b, a);
		}
	}
	
	for (int py = 0; py < height; py++)
	{
		setPx(width - 1, py, r, g, b, a);
	}
}

void SoftCanvas::setPx(int x, int y, char r, char g, char b, char a)
{
	data[(y * width + x) * 4] = r;
	data[(y * width + x) * 4 + 1] = g;
	data[(y * width + x) * 4 + 2] = b;
	data[(y * width + x) * 4 + 3] = a;
}

int SoftCanvas::genTexture()
{
	glGenTextures(1, (GLuint *)&glTexId);
	checkGLError("glGenTextures");
	
    glBindTexture(GL_TEXTURE_2D, glTexId);
	checkGLError("glBindTexture");

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    checkGLError("glTexParameterf");
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	checkGLError("glTexParameterf");
	
#ifdef PLATFORM_ANDROID
    glTexImage2Db(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
#endif
#if defined PLATFORM_WINDOWS || defined PLATFORM_IOS
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
#endif

	checkGLError("glTexImage2D");
	
	return glTexId;
}

void SoftCanvas::checkGLError(char *str)
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
