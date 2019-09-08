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

#include "Globals.hpp"
#include "GLIncludes.h"
#include "TextureManager2.h"
#include "platform.h"
#include "DDLUtils.hpp"
#include "../thirdparty/lodepng/lodepng.hpp"

Texture *TextureManager2::find(std::string name)
{
	Texture *tex = textures[name];

	if (tex == nullptr)
	{
		load(name);
		tex = textures[name];
	}

	if (tex != nullptr)
		return tex;
	else
	    return nullptr;
}

void flip_vertically(unsigned char *pixels, const size_t width, const size_t height, const size_t bytes_per_pixel)
{
	const size_t stride = width * bytes_per_pixel;
	unsigned char *row = (unsigned char *)malloc(stride);
	unsigned char *low = pixels;
	unsigned char *high = &pixels[(height - 1) * stride];

	for (; low < high; low += stride, high -= stride) {
		memcpy(row, low, stride);
		memcpy(low, high, stride);
		memcpy(high, row, stride);
	}

	free(row);
}

void TextureManager2::load(std::string name, bool external)
{
    Texture *t = new Texture();
    t->name = name;

#ifdef PLATFORM_IOS
    if (name == "")
        return;
    
    int dotPos = name.find(".");

    std::string nameNoExt = "";
    std::string ext = "";

    if (dotPos != -1)
    {
        nameNoExt = name.substr(0, dotPos);
        ext = name.substr(dotPos, name.length());
    }
    else
    {
        return;
    }
    
    NSString *nsNameNoExt = [NSString stringWithFormat:@"%s", nameNoExt.c_str()];
    NSString *nsExt = [NSString stringWithFormat:@"%s", ext.c_str()];
    
    GLKTextureInfo *spriteTexture;
    NSError *theError;
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:nsNameNoExt ofType:nsExt];
    
    if (filePath == nil)
        return;
    
    NSDictionary *opts = @{
                                    @"GLKTextureLoaderOriginBottomLeft" : @"YES"
                                    };
    
    spriteTexture = [GLKTextureLoader textureWithContentsOfFile:filePath options:opts error:&theError]; // 2
    t->glTexID = spriteTexture.name;
    
    glBindTexture(GL_TEXTURE_2D, t->glTexID);
    checkGLError("glBindTexture");
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    checkGLError("glTexParameterf");
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    checkGLError("glTexParameterf");
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    checkGLError("glTexParameterf");
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    checkGLError("glTexParameterf");

#else
    int glTexID;
    glGenTextures(1, (GLuint *)&glTexID);
    checkGLError("glGenTextures");
	t->glTexID = glTexID;
#endif
    
	textures[name] = t;

	// If name is blank, do not try to load texture
	if (name == "")
		return;

#ifdef PLATFORM_ANDROID
#ifdef USE_EXTERNAL_ASSETS
    std::string fullfname = g_assetsDir + "/" + name;
    PLAT_LoadTextureInJava(glTexID, fullfname, true);
#else
	PLAT_LoadTextureInJava(glTexID, name, g_assetsDir);
#endif
#endif

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR

	char fullFilename[MAX_STR_LEN];

#ifdef USE_EXTERNAL_ASSETS
	printFullExternalResourceFilename((char *)name.c_str(), fullFilename);
#else
	printFullResourceFilename((char *)name.c_str(), fullFilename);
#endif

	// Load file and decode image
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, fullFilename);

	// We have a problem; display an error and use fallback texture
	if (error != 0)
	{
		Log("Error loading PNG: ", (char *)lodepng_error_text(error));
		Log(name);

		Texture *fb = find("fallback");
		t->glTexID = fb->glTexID;

		return;
	}

	// Flip image vertically
	unsigned char *flippedImage = (unsigned char *)malloc(width * height * 4);
	memcpy(flippedImage, reinterpret_cast<char*>(image.data()), width * height * 4);
	flip_vertically(flippedImage, width, height, 4);

	// Load image into GPU memory
	glBindTexture(GL_TEXTURE_2D, glTexID);
	checkGLError("glBindTexture");
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	checkGLError("glTexParameterf");
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkGLError("glTexParameterf");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, flippedImage);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	checkGLError("glTexImage2D");
#endif
}

void TextureManager2::add(std::string name, int glTexId)
{
    Texture *t = new Texture();
    t->name = name;
	t->glTexID = glTexId;
	
	textures[name] = t;
}

void TextureManager2::free()
{
	for(const auto &pair: textures)
	{
		Texture *tex = pair.second;
        glDeleteTextures(1, (GLuint *)&tex->glTexID);
        checkGLError("glDeleteTextures");
    }
}

void TextureManager2::checkGLError(char *tag)
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
