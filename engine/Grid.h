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

#ifndef Grid_h
#define Grid_h

#include "Framebuffer.h"
#include "Renderer.h"
#include "TextureManager2.h"
#include "SpriteRenderer2D.hpp"

class Grid: public Renderer
{
public:
    void init(TextureManager2 *texMan, SpriteRenderer2D *spr);
	void draw(int size);
	void drawLine(float x1, float y1, float x2, float y2);
    int getGlTexId() { return fb.getGlTexId(); };
	void checkGLError(char *str);
	
private:

    TextureManager2 *texMan;
    SpriteRenderer2D *spr;

	//
	// VERTEX SHADER ES 2.0
	//

	const char *vertexShaderCodeES20 =

	"attribute vec4 vPosition;" \
	
	"void main() {" \
        "      gl_Position = vPosition; " \
        "}\n";


	//
	// FRAGMENT SHADER ES 2.0
	//

	const char *fragmentShaderCodeES20 =

    "uniform lowp vec4 vColor; " \

	"void main() {" \
		"      gl_FragColor = vColor; " \
		"}\n";

	int programMain = -1;
    bool initialized = false;
    Framebuffer fb;
};

#endif
