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

#ifndef TEXTUREATLAS_H        
#define TEXTUREATLAS_H

#include "TextureManager2.h"
#include "SpriteRenderer2D.hpp"
#include <vector>
#include "Framebuffer.h"

typedef struct UV
{
	float u;
	float v;
} UV;

typedef struct XYINTS
{
	int x;
	int y;
} XYINTS;

class TextureAtlas
{
public:
    void init(TextureManager2 *texMan, SpriteRenderer2D *renderer);
    void add(std::string tex);
    void refresh();
	void bind();
	void unbind();
	int getGlTexId();
	UV getUV(std::string texName, UV input);
	void checkGLError(char *tag);
    int getSize() { return size; }
	int getNeedsRefresh() { return needsRefresh; }
	TextureManager2 *getTexMan() {return texMan; } ;
	
private:
    std::vector<std::string> textures;
	std::map<std::string, XYINTS> texPos;
	int framebuffer = -1;
	int fbTexture = -1;
	int renderbuffer = -1;
	int fbSize = 2048;
	int size = 0;
	bool needsRefresh = true;
	
	TextureManager2 *texMan;
	SpriteRenderer2D *renderer;
	Framebuffer fb;
};

#endif
