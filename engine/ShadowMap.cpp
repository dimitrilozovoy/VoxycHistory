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

#include "ShadowMap.h"
#include "GLIncludes.h"
#include "platform.h"
#include "DDLUtils.hpp"

void ShadowMap::bind()
{
	this->lightSize = lightSize;

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX

	const int size = 4096;

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	if (FramebufferName == -1)
	{
		glGenFramebuffers(1, &FramebufferName);
		checkGLError("glGenFramebuffers");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	checkGLError("glBindFramebuffer");

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	if (depthTexture == -1)
	{
		glGenTextures(1, &depthTexture);
		checkGLError("glGenTextures");
	}

	glBindTexture(GL_TEXTURE_2D, depthTexture);
	checkGLError("glBindTexture");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	checkGLError("glTexImage2D");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	checkGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	checkGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	checkGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	checkGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	checkGLError("glTexParameteri");

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	checkGLError("glFramebufferTexture");

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	checkGLError("glDrawBuffer");

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;
	checkGLError("glCheckFramebufferStatus");

	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	checkGLError("glBindFramebuffer");
	glViewport(0, 0, size, size); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	checkGLError("glViewport");

#endif

#ifdef PLATFORM_ANDROID

	const int size = 1024;

	if (fboId == -1)
	{
	    glGenFramebuffers(1, (GLuint*)&fboId);
		checkGLError("glGenFramebuffers");
	}

	if (depthTextureId == -1)
	{
	    glGenRenderbuffers(1, (GLuint*)&depthTextureId);
		checkGLError("glGenRenderbuffers");
	}
	
	glBindRenderbuffer(GL_RENDERBUFFER, depthTextureId);
	checkGLError("glBindRenderbuffer");
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size, size);
    checkGLError("glRenderbufferStorage");
	
	if (renderTextureId == -1)
	{
	    glGenTextures(1, (GLuint*)&renderTextureId);
		checkGLError("glGenTextures");
	}
	
	glBindTexture(GL_TEXTURE_2D, renderTextureId);
    checkGLError("glBindTexture");
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	checkGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    checkGLError("glTexParameteri");

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	checkGLError("glTexParameteri");
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    checkGLError("glTexParameteri");
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;
	checkGLError("glCheckFramebufferStatus");

	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    checkGLError("glTexImage2D");
	
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	checkGLError("glTexImage2D");
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTextureId, 0);
	checkGLError("glFramebufferTexture2D");
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTextureId);
    checkGLError("glFramebufferRenderbuffer");
	
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowMapWidth, mShadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, null);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderTextureId[0], 0);

#endif
}

void ShadowMap::unbind()
{
	float width = PLAT_GetWindowWidth();
	float height = PLAT_GetWindowHeight();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	checkGLError("glBindFramebuffer");
	glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right}
	checkGLError("glViewport");
}

glm::mat4 ShadowMap::getProjectionMatrix()
{
	return glm::ortho<float>(-lightSize, lightSize, -lightSize, lightSize, -lightSize, lightSize);
};

void ShadowMap::checkGLError(char *tag)
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
