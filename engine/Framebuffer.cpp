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

#include "Framebuffer.h"

void Framebuffer::bind()
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
		Log("Framebuffer: framebuffer incomplete");
//		return;
	}

	checkGLError("glCheckFramebufferStatus");

	// Render to our framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//	checkGLError("glBindFramebuffer");
	glViewport(0, 0, fbSize, fbSize);
	checkGLError("glViewport");
}

void Framebuffer::unbind()
{
	float width = PLAT_GetWindowWidth();
	float height = PLAT_GetWindowHeight();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	checkGLError("glBindFramebuffer");
	glViewport(0, 0, width, height);
	checkGLError("glViewport");
}

int Framebuffer::getGlTexId()
{
	return fbTexture;
}

void Framebuffer::checkGLError(char *str)
{
#ifdef DEBUG_BUILD
#if defined PLATFORM_OSX || defined PLATFORM_GVR || defined PLATFORM_ANDROID || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
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
