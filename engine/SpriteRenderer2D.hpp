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

#ifndef SPRITERENDERER2D_HPP
#define SPRITERENDERER2D_HPP

#include "Renderer.h"

class SpriteRenderer2D: public Renderer
{
public:
	SpriteRenderer2D();
    ~SpriteRenderer2D();
	void Init();
    void OnGLFWError(int errorCode, const char* msg);
    void initVBOs();
    void freeVertices();
    int loadVertices();
    int loadShader(int type, const char *shaderCode);
    void compileShaders();
	void DrawCircle(float xshift, float yshift, float scale);
	void DrawSprite(float xshift, float yshift, float scalex, float scaley, int glTexID, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool noaspect = false);
	void checkError();
	void checkError(char *func);
	
private:
#if defined PLATFORM_OPENVR
	const char *vertexShaderCode =
		"#version 150\n"\

		"in vec4 vPosition;" \
		"in vec2 vTexCoords;" \
		"out lowp vec2 vTexCoordsOut; " \
		"uniform mat4 scaleMatrix; " \
		"uniform mat4 translateMatrix; " \

		"void main() {" \
		"  gl_Position = translateMatrix * scaleMatrix * vPosition; "\
		"  vTexCoordsOut = vTexCoords; " \
		"}\n";


	//
	// FRAGMENT SHADER
	//

	const char *fragmentShaderCode =
		"#version 150\n"\


		"uniform sampler2D uTexture; " \
		"uniform lowp vec4 vColor; " \
		"in lowp vec2 vTexCoordsOut; " \
		"out lowp vec4 finalColor; "\

		"void main() {" \
		"  finalColor = texture(uTexture, vTexCoordsOut.st) * vColor; "\
		"}\n";


#endif
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS
    const char *vertexShaderCode =
    "#version 150\n"\
    
    "in vec4 vPosition;" \
    "in vec2 vTexCoords;" \
    "out lowp vec2 vTexCoordsOut; " \
    "uniform mat4 scaleMatrix; " \
    "uniform mat4 translateMatrix; " \
    
    "void main() {" \
    "  gl_Position = translateMatrix * scaleMatrix * vPosition; "\
    "  vTexCoordsOut = vTexCoords; " \
    "}\n";
    

    //
    // FRAGMENT SHADER
    //
    
    const char *fragmentShaderCode =
    "#version 150\n"\
    
    
    "uniform sampler2D uTexture; " \
    "uniform lowp vec4 vColor; " \
    "in lowp vec2 vTexCoordsOut; " \
    "out lowp vec4 finalColor; "\
    
    "void main() {" \
    "  finalColor = texture(uTexture, vTexCoordsOut.st) * vColor; "\
    "}\n";


#endif
#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
    //
    // VERTEX SHADER
    //
    
    const char *vertexShaderCode =
    "#version 300 es\n"\
    
    "in vec4 vPosition;" \
	"in vec2 vTexCoords;" \
	"out lowp vec2 vTexCoordsOut; " \
	"uniform mat4 scaleMatrix; " \
    "uniform mat4 translateMatrix; " \
    
    "void main() {" \
	"  gl_Position = translateMatrix * scaleMatrix * vPosition; "\
	"  vTexCoordsOut = vTexCoords; " \
    "}\n";
    
    //
    // FRAGMENT SHADER
    //
    
    const char *fragmentShaderCode =
    "#version 300 es\n"\
    
	"uniform sampler2D uTexture; " \
    "uniform lowp vec4 vColor; " \
	"in lowp vec2 vTexCoordsOut; " \
	"out lowp vec4 finalColor; "\
    
    "void main() {" \
    "  finalColor = texture(uTexture, vTexCoordsOut.st) * vColor; "\
    "}\n";
	
	//
    // VERTEX SHADER
    //
    
    const char *vertexShaderCodeES20 =
    
    "attribute vec4 vPosition;" \
	"attribute vec2 vTexCoords;" \
	"varying lowp vec2 vTexCoordsOut; " \
	"uniform mat4 scaleMatrix; " \
    "uniform mat4 translateMatrix; " \
    
    "void main() {" \
	"  gl_Position = translateMatrix * scaleMatrix * vPosition; "\
	"  vTexCoordsOut = vTexCoords; " \
    "}\n";
    
    //
    // FRAGMENT SHADER
    //
    
    const char *fragmentShaderCodeES20 =
    
	"uniform sampler2D uTexture; " \
    "uniform lowp vec4 vColor; " \
	"uniform lowp vec4 globalColor; " \
	"varying lowp vec2 vTexCoordsOut; " \
	"varying lowp vec4 finalColor; "\

/*    "void main() {" \
    "  lowp vec4 color = texture2D(uTexture, vTexCoordsOut.st); " \
    "  color.w = 0.5; " \
    "  gl_FragColor = color; " \
    "}\n";*/

    "void main() {" \
    "  gl_FragColor = texture2D(uTexture, vTexCoordsOut.st) * vColor - vec4(0.0, 0.0, 0.0, 0.0); " \
    "}\n";

 /*    "void main() {" \
    "  lowp vec4 color = texture2D(uTexture, vTexCoordsOut.st); " \
    "  if (color.w < 1.0) " \
    "      gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); "\
    "  else " \
    "      gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); " \
    "}\n";*/

#endif

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
    
    int m_vao = -1;
    int m_vbos[6];
    bool m_vbosAllocated = false;
	int m_program = 0;
	float aspect = 1;
	int m_scaleMatrixHandle = 0;
    int m_translateMatrixHandle = 0;
	float m_scaleMatrixArr[16];
    float m_translateMatrixArr[16];
};

#endif
