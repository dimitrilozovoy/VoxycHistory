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

#ifndef SoftCanvas_h
#define SoftCanvas_h

#include "GLIncludes.h"
#include "platform.h"
#include "DDLUtils.hpp"

class SoftCanvas
{
	public:
	    void init(int width, int height);
		void release();
		void clear(char r, char g, char b, char a);
		void drawGrid(int size, char r, char g, char b, char a, bool showCenter);
		void setPx(int x, int y, char r, char g, char b, char a);
		int genTexture();
		void checkGLError(char *str);
		unsigned char *getData() { return data; };
		int getWidth() { return width; };
		int getHeight() { return height; };

	private:
	    const int bytesPerPx = 4;
	
	    unsigned char *data = nullptr;
		int width = 0;
		int height = 0;
	    int glTexId = -1;
};

#endif
