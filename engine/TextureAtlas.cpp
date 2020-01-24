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

#include "TextureAtlas.h"
#include "GLIncludes.h"
#include "platform.h"
#include "DDLUtils.hpp"

void TextureAtlas::init(TextureManager2 *texMan, SpriteRenderer2D *renderer)
{
	this->texMan = texMan;
	this->renderer = renderer;
}

void TextureAtlas::add(std::string tex)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i] == tex)
			return;
	}
	
	textures.push_back(tex);
	
	needsRefresh = true;
}

void TextureAtlas::refresh()
{
//	Log("refresh texatlas");
	
	fb.bind();

	glClearColor(0.0, 0.0, 0.0, 0.0);
    checkGLError("glClearColor");
	glClear(GL_COLOR_BUFFER_BIT);
    checkGLError("glClear");
	
	glDisable(GL_BLEND);
    checkGLError("glDisable");

	int numTex = textures.size();
	
	size = 1;
	
	while (numTex > (size * size))
		size = size * 2;
		
	int texi = 0;
		
	for (int y = 0; y < size; y++)
	{
	   for (int x = 0; x < size; x++)
	   {
		   if (texi < numTex)
		   {
		       Texture *tex = texMan->find(textures[texi]);

		       if (tex != nullptr) {
				   float bigTileSizeX = (2.0 / (float) size);
				   float bigTileSizeY = (2.0 / (float) size);
				   float smallTileSizeX = (1.0 / (float) size) / tex->texSpanX;
				   float smallTileSizeY = (1.0 / (float) size) / tex->texSpanY;

				   if (tex->texSpanX == 1.0 && tex->texSpanY == 1.0) {
					   renderer->DrawSprite(-1.0 + bigTileSizeX * ((float) x + 0.5),
											-1.0 + bigTileSizeY * ((float) y + 0.5),
											1.0 / (float) size, 1.0 / (float) size, tex->glTexID,
											1.0, 1.0, 1.0, 1.0, true);
				   } else {
					   for (int yy = 0; yy < tex->texSpanY; yy++) {
						   for (int xx = 0; xx < tex->texSpanX; xx++) {
							   renderer->DrawSprite(-1.0 + bigTileSizeX * ((float) x + 0.5) -
													bigTileSizeX / 2.0 + smallTileSizeX +
													(smallTileSizeX * 2.0) *
													(float) xx,// - (bigTileSizeX) / (tex->texSpanX * 2.0),// + (smallTileSizeX * 2.0) * (float)xx,
													-1.0 + bigTileSizeY * ((float) y + 0.5) -
													bigTileSizeY / 2.0 + smallTileSizeY +
													(smallTileSizeY * 2.0) *
													(float) yy,// - (bigTileSizeY) / (tex->texSpanY * 2.0),// + (smallTileSizeY * 2.0) * (float)yy,
													smallTileSizeX, smallTileSizeY, tex->glTexID,
													1.0,
													1.0, 1.0, 1.0, true);
						   }
					   }
				   }

				   texPos[textures[texi]].x = x;
				   texPos[textures[texi]].y = y;
			   }
           }
		   
	       texi++;
	   }
	}

//    Texture *tex = texMan->find("ball.png");
//    renderer->DrawSprite(0.0, 0.0, 2.0, 2.0, tex->glTexID);

	glEnable(GL_BLEND);
    checkGLError("glEnable");
	
	fb.unbind();
	
//	Log("textures.size()", textures.size());
	
	needsRefresh = false;
}

void TextureAtlas::bind()
{
    // FRAMEBUFFER

	// Make framebuffer
	if (framebuffer == -1)
	{
		glGenFramebuffers(1, (GLuint*)&framebuffer);
		checkGLError("glGenFramebuffers");
	}

    // Bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    checkGLError("glBindFramebuffer");

    // TEXTURE

    // Make texture
	if (fbTexture == -1)
	{
		glGenTextures(1, (GLuint*)&fbTexture);
		checkGLError("glGenTextures");
	}

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, fbTexture);
    checkGLError("glBindTexture");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbSize, fbSize, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, nullptr);
    checkGLError("glTexImage2D");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkGLError("glTexParameteri");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    checkGLError("glTexParameteri");
/*    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    checkGLError("glTexParameteri");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    checkGLError("glTexParameteri");*/

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//	checkGLError("glTexParameteri");
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
//	checkGLError("glTexParameteri");

    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLError("glBindTexture");

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTexture, 0);
    checkGLError("glFramebufferTexture2D");

    // RENDERBUFFER

    // Make renderbuffer
	if (renderbuffer == -1)
	{
		glGenRenderbuffers(1, (GLuint*)&renderbuffer);
		checkGLError("glGenTextures");
	}

#ifndef PLATFORM_IOS
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbSize, fbSize);
    checkGLError("glRenderBufferStorage");

    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	checkGLError("glBindRenderbuffer");

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	checkGLError("glFramebufferRenderbuffer");
#endif
    
	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Log("TextureAtlas: framebuffer incomplete");
//		return;
	}

	checkGLError("glCheckFramebufferStatus");

	// Render to our framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//	checkGLError("glBindFramebuffer");
	glViewport(0, 0, fbSize, fbSize);
	checkGLError("glViewport");
}

void TextureAtlas::unbind()
{
	float width = PLAT_GetWindowWidth();
	float height = PLAT_GetWindowHeight();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	checkGLError("glBindFramebuffer");
	glViewport(0, 0, width, height);
	checkGLError("glViewport");
}

int TextureAtlas::getGlTexId()
{
	if (needsRefresh)
		refresh();
		
	return fb.getGlTexId();
//	return fbTexture;
}

UV TextureAtlas::getUV(std::string texName, UV in)
{
	UV out;

//	return in;

	XYINTS pos = texPos[texName];

	if (size == 0)
		size = 1;
	
	float texSizeNDC = 1.0 / (float)size;
	
	out.u = texSizeNDC * (float)pos.x + (texSizeNDC * in.u);
	out.v = texSizeNDC * (float)pos.y + (texSizeNDC * in.v);

    return out;
}

void TextureAtlas::checkGLError(char *tag)
{
#ifdef DEBUG_BUILD
#ifdef USE_OPENGL
    GLenum err = glGetError();

    switch (err)
    {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            Log("GL error GL_INVALID_ENUM", tag);
            break;
        case GL_INVALID_VALUE:
            Log("GL error GL_INVALID_VALUE", tag);
            break;
        case GL_INVALID_OPERATION:
            Log("GL error GL_INVALID_OPERATION", tag);
            break;
#ifndef USE_OPENGL_3
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            Log("GL error GL_INVALID_FRAMEBUFFER_OPERATION", str);
            break;
#endif
        case GL_OUT_OF_MEMORY:
            Log("GL error GL_OUT_OF_MEMORY", tag);
            break;
    }
#endif
#endif
}

