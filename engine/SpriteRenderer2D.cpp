/*
Copyright (C) 2018 Dimitri Lozovoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright no3tice and this permission notice shall be included in all
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
#ifdef PLATFORM_OPENVR
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#ifdef PLATFORM_WINDOWS
#include <iostream>
#include "../thirdparty/GL/glew.h"  
#include "../thirdparty/GLFW/glfw3.h"
#endif
#ifdef PLATFORM_OSX
#include <iostream>
#include "../thirdparty/macos/GL/glew.h"
#include "../thirdparty/macos/GLFW/glfw3.h"
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#endif
#include "DDLUtils.hpp"
#include "platform.h"

#include "GLIncludes.h"

#include "SpriteRenderer2D.hpp"
#include <math.h>

#ifdef PLATFORM_ANDROID
#ifdef USE_JAVAGL
#ifdef GAME_RELEASE_DIR_TREE
#include "../javagl.h"
#else
#include "../platform/android/Voxyc/app/src/main/cpp/javagl.h"
#endif
#else
#include "EGL/egl.h"
#include "GLES3/gl3.h"
#include "../platform/android/Voxyc/app/src/main/cpp/android.h"
#endif
#endif

#define NUM_CTRL_SEGMENTS 16

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

SpriteRenderer2D::SpriteRenderer2D()
{
}

SpriteRenderer2D::~SpriteRenderer2D()
{
}

void SpriteRenderer2D::Init()
{
    initVBOs();
    compileShaders();
		
	float width = PLAT_GetWindowWidth();
	float height = PLAT_GetWindowHeight();
	
//	Log("PLAT_GetWindowWidth() ", PLAT_GetWindowWidth());
//	Log("PLAT_GetWindowHeight() ", PLAT_GetWindowHeight());
	
	aspect = height / width;
	
	loadVertices();
	
//	Log("Set aspect to ", aspect * 1000);
}

void SpriteRenderer2D::OnGLFWError(int errorCode, const char* msg) {
    Log("GLFW error");
    Log((char *)msg, errorCode);
}

void SpriteRenderer2D::initVBOs()
{
    m_vbosAllocated = false;
}

void SpriteRenderer2D::freeVertices()
{
    // Free VAO
    if (m_vao != -1)
    {
#if defined PLATFORM_GVR || defined PLATFORM_ANDROID
        glDeleteVertexArrays(1, (GLuint *)&m_vao);
#endif
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
        glDeleteVertexArrays(1, (unsigned int *)&m_vao);
#endif
    }

    // Free VBO's
    if (m_vbosAllocated)
    {
#if defined PLATFORM_GVR || defined PLATFORM_ANDROID
        glDeleteBuffers(1, (GLuint *)m_vbos);
#endif
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
        glDeleteBuffers(1, (unsigned int *)m_vbos);
#endif
        m_vbosAllocated = false;
    }
}

int SpriteRenderer2D::loadVertices()
{
//	Log("aspect =", aspect * 1000);
	
	// Generate circle
	float *circleVerts = (float *)malloc(NUM_CTRL_SEGMENTS * 3 * 4 * sizeof(float));
	
	for (int s = 0; s < NUM_CTRL_SEGMENTS; s++)
	{
		float angle = 360.0f / (float)NUM_CTRL_SEGMENTS * (float)s;
		float nextAngle = 360.0f / (float)(NUM_CTRL_SEGMENTS) * (float)(s + 1);
		
		float ax = 0.0f;
		float ay = 0.0f;
		float az = 0.0f;
		float aw = 1.0f;
		
		float bx = cos(ToRadians(angle)) * aspect;
		float by = sin(ToRadians(angle));
		float bz = 0.0f;
		float bw = 1.0f;
		
		float cx = cos(ToRadians(nextAngle)) * aspect;
		float cy = sin(ToRadians(nextAngle));
		float cz = 0.0f;
		float cw = 1.0f;
		
		circleVerts[(3 * 4) * s] = ax;
		circleVerts[(3 * 4) * s + 1] = ay;
		circleVerts[(3 * 4) * s + 2] = az;
		circleVerts[(3 * 4) * s + 3] = aw;
		
	    circleVerts[(3 * 4) * s + 4] = bx;
		circleVerts[(3 * 4) * s + 5] = by;
		circleVerts[(3 * 4) * s + 6] = bz;
		circleVerts[(3 * 4) * s + 7] = bw;

		circleVerts[(3 * 4) * s + 8] = cx;
		circleVerts[(3 * 4) * s + 9] = cy;
		circleVerts[(3 * 4) * s + 10] = cz;
		circleVerts[(3 * 4) * s + 11] = cw;
	}
	
	// Generate sprite
	float *spriteVerts = (float *)malloc(2 * 3 * 6 * sizeof(float));
	
	//   ______
	// |\\3   4|
	// |0\\    |
	// |  \\   |
	// |   \\  |
	// |    \\5|
	// |2__1_\\|
	
	// Poly 1
	
	spriteVerts[0] = -1.0f * aspect;
	spriteVerts[1] = 1.0f;
	spriteVerts[2] = 0.0f;
	spriteVerts[3] = 1.0f;
	
	spriteVerts[4] = 0.0f;
	spriteVerts[5] = 1.0f - 0.01;
	
	spriteVerts[6] = 1.0f * aspect;
	spriteVerts[7] = -1.0f;
	spriteVerts[8] = 0.0f;
	spriteVerts[9] = 1.0f;
	
	spriteVerts[10] = 1.0f;
	spriteVerts[11] = 0.0f;

	spriteVerts[12] = -1.0f * aspect;
	spriteVerts[13] = -1.0f;
	spriteVerts[14] = 0.0f;
	spriteVerts[15] = 1.0f;
		
	spriteVerts[16] = 0.0f;
	spriteVerts[17] = 0.0f;

	// Poly 2
	
	spriteVerts[18] = -1.0f * aspect;
	spriteVerts[19] = 1.0f;
	spriteVerts[20] = 0.0f;
	spriteVerts[21] = 1.0f;
		
	spriteVerts[22] = 0.0f;
	spriteVerts[23] = 1.0f - 0.01;

	spriteVerts[24] = 1.0f * aspect;
	spriteVerts[25] = 1.0f;
	spriteVerts[26] = 0.0f;
	spriteVerts[27] = 1.0f;
	
	spriteVerts[28] = 1.0f;
	spriteVerts[29] = 1.0f;

	spriteVerts[30] = 1.0f * aspect;
	spriteVerts[31] = -1.0f;
	spriteVerts[32] = 0.0f;
	spriteVerts[33] = 1.0f;
	
	spriteVerts[34] = 1.0f;
	spriteVerts[35] = 0.0f;

    // Generate sprite
    float *quadVerts = (float *)malloc(2 * 3 * 6 * sizeof(float));

    //   ______
    // |\\3   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\5|
    // |2__1_\\|

    // Poly 1

    quadVerts[0] = -1.0f;
    quadVerts[1] = 1.0f;
    quadVerts[2] = 0.0f;
    quadVerts[3] = 1.0f;

    quadVerts[4] = 0.0f;
    quadVerts[5] = 1.0f;

    quadVerts[6] = 1.0f;
    quadVerts[7] = -1.0f;
    quadVerts[8] = 0.0f;
    quadVerts[9] = 1.0f;

    quadVerts[10] = 1.0f;
    quadVerts[11] = 0.0f;

    quadVerts[12] = -1.0f;
    quadVerts[13] = -1.0f;
    quadVerts[14] = 0.0f;
    quadVerts[15] = 1.0f;

    quadVerts[16] = 0.0f;
    quadVerts[17] = 0.0f;

    // Poly 2

    quadVerts[18] = -1.0f;
    quadVerts[19] = 1.0f;
    quadVerts[20] = 0.0f;
    quadVerts[21] = 1.0f;

    quadVerts[22] = 0.0f;
    quadVerts[23] = 1.0f;

    quadVerts[24] = 1.0f;
    quadVerts[25] = 1.0f;
    quadVerts[26] = 0.0f;
    quadVerts[27] = 1.0f;

    quadVerts[28] = 1.0f;
    quadVerts[29] = 1.0f;

    quadVerts[30] = 1.0f;
    quadVerts[31] = -1.0f;
    quadVerts[32] = 0.0f;
    quadVerts[33] = 1.0f;

    quadVerts[34] = 1.0f;
    quadVerts[35] = 0.0f;

    // Make and bind the VAO
#if defined PLATFORM_GVR || defined PLATFORM_ANDROID
    glGenVertexArrays(1, (GLuint *)&m_vao);
#endif
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
    glGenVertexArrays(1, (unsigned int *)&m_vao);
#endif
    checkError("glGenVertexArrays");
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    glBindVertexArray(m_vao);
    checkError("glBindVertexArray");
#endif
    
    // Make and bind the VBO
#if defined PLATFORM_GVR || defined PLATFORM_ANDROID || defined PLATFORM_IOS
    glGenBuffers(3, (GLuint *)m_vbos);
#endif
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
    glGenBuffers(2, (unsigned int *)m_vbos);
#endif
    checkError("glGenBuffers");

    m_vbosAllocated = true;
	
	// Circle
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
    checkError("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUM_CTRL_SEGMENTS * 3 * 4, circleVerts, GL_STATIC_DRAW);
    checkError("glBufferData");
	
	// Sprite
	
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
    checkError("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 3 * 6, spriteVerts, GL_STATIC_DRAW);
    checkError("glBufferData");

    // Quad

    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2]);
    checkError("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 3 * 6, quadVerts, GL_STATIC_DRAW);
    checkError("glBufferData");

	// Free
	
	free(spriteVerts);
	free(circleVerts);
    free(quadVerts);

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Unbind the VAO
    glBindVertexArray(0);
#endif
    
    return 1;
}

int SpriteRenderer2D::loadShader(int type, const char *shaderCode)
{
    int shader = glCreateShader(type);
    checkError("glCreateShader");
//	Log("shader =", shader);
    
    // Add the source code to the shader and compile it
    char const *codeArr[1];
    codeArr[0] = shaderCode;
    glShaderSource(shader, 1, codeArr, NULL);
    checkError("glShaderSource");
    glCompileShader(shader);
    checkError("glCompileShader");
    
    GLint compile_ok = GL_FALSE;
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
    checkError("glGetShaderiv");
    
    if (compile_ok == GL_FALSE)
    {
        Log("SoriteRenderer2D shader compile error; type = ", type);
        char *errorMsg = (char *)malloc(MAX_STR_LEN);
        GLsizei len;
		Log("shader =", shader);
        glGetShaderInfoLog(shader, MAX_STR_LEN, &len, errorMsg);
		checkError("glGetShaderInfoLog");
//		Log("len = ", len);
        Log(errorMsg);
        free(errorMsg);
    }
    
    return shader;
}

void SpriteRenderer2D::compileShaders()
{
	int vertexShader = 0;
	int fragmentShader = 0;

#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderCodeES20);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderCodeES20);
#else
	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderCode);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
#endif

    m_program = glCreateProgram();             // Create empty OpenGL ES Program
    checkError("glCreateProgram");
    glAttachShader(m_program, vertexShader);   // Add the vertex shader to program
    checkError("glAttachShader");
    glAttachShader(m_program, fragmentShader); // Add the fragment shader to program
    checkError("glAttachShader");

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
    glBindFragDataLocation(m_program, 0, "finalColor");
    checkError("glBindFragDataLocation");
#endif
    
    glLinkProgram(m_program);                  // Creates OpenGL ES program executables
    checkError("glLinkProgram");
    
    // Throw exception if linking failed
    GLint status;
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    checkError("glGetProgramiv");
    if (status == GL_FALSE) {
        Log("Program linking failure: ");
        
        GLint infoLogLength;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
        checkError("glGetProgramiv");
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(m_program, infoLogLength, NULL, strInfoLog);
        checkError("glGetProgramInfoLog");
        Log(strInfoLog);
        delete[] strInfoLog;
        
        glDeleteProgram(m_program);
        checkError("glDeleteProgram");
        m_program = 0;
    }
}

void SpriteRenderer2D::DrawCircle(float xshift, float yshift, float cscale)
{
	// Use program
	glUseProgram(m_program);
    checkError();
	
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
	// Bind the VAO
    glBindVertexArray(m_vao);
#endif
    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);    
	
	// Scale
    glm::mat4 scaleMatrix;
    scaleMatrix = glm::scale(glm::mat4(), glm::vec3(cscale));
                        
    const float *p = (const float *)glm::value_ptr(scaleMatrix);
    for (int i = 0; i < 16; i++)
        m_scaleMatrixArr[i] = p[i];
		
	m_scaleMatrixHandle = glGetUniformLocation(m_program, "scaleMatrix");
    checkError("glGetUniformLocation");
	
	glUniformMatrix4fv(m_scaleMatrixHandle, 1, false, m_scaleMatrixArr);
    checkError("glUniformMatrix4fv");
                                  
    // Translate
    glm::mat4 translateMatrix;
    translateMatrix = glm::translate(glm::mat4(), glm::vec3(xshift, yshift, 0.0f));
                        
    p = (const float *)glm::value_ptr(translateMatrix);
    for (int i = 0; i < 16; i++)
        m_translateMatrixArr[i] = p[i];
		
    m_translateMatrixHandle = glGetUniformLocation(m_program, "translateMatrix");
    checkError("glGetUniformLocation");
		
	glUniformMatrix4fv(m_translateMatrixHandle, 1, false, m_translateMatrixArr);
    checkError("glUniformMatrix4fv");

    // Position
            
    GLint vPosition = glGetAttribLocation(m_program, "vPosition");
    checkError("glGetAttribLocation");
    if (vPosition == -1)
    {
        Log("SpriteRenderer2D vPosition is -1");
    }
            
    glEnableVertexAttribArray(vPosition);
    checkError("glEnableVertexAttribArray");
    glVertexAttribPointer(vPosition, 4,
                                  GL_FLOAT, false,
                                  sizeof(float) * 4, 0);
    checkError("glVertexAttribPointer");
	
	// Color
                        
	GLint colorHandle = glGetUniformLocation(m_program, "vColor");
    checkError("glGetUniformLocation");
    glUniform4f(colorHandle, 0.0f, 0.0f, 1.0f, 0.2f);
	checkError("glUniform4f");
            
	setUniform4f(m_program, "globalColor", globalColor.x, globalColor.y, globalColor.z, globalColor.w);
	
	//
    // Draw the triangles
    //
            
    glDrawArrays(GL_TRIANGLES, 0, NUM_CTRL_SEGMENTS * 3);
    checkError("glDrawArrays");
            
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    glBindVertexArray(0);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void SpriteRenderer2D::DrawSprite(float xshift, float yshift, float scalex, float scaley, int glTexID, float r, float g, float b, float a, bool noaspect)
{
	// Is the sprite visible on the screen?
	if (xshift < (-1.0 - scalex / 2.0) || xshift >(1.0 + scalex / 2.0)
		|| yshift < (-1.0 - scaley / 2.0) || xshift >(1.0 + scaley / 2.0))
		return;

/*	char str[512];
	sprintf(str, "xshift = %d, yshift = %d, scalex = %d, scaley = %d, glTexID = %d, r = %d, g = %d, b = %d, a = %d\n", (int)(xshift * 100), (int)(yshift * 100), (int)(scalex * 100), (int)(scaley * 100), glTexID, (int)(r * 100), (int)(g * 100), (int)(b * 100), (int)(a * 100));
    Log(str);*/
	
    glDisable(GL_DEPTH_TEST);
	checkError("glDisable");
	
/*	glEnable(GL_BLEND);
	checkError("glEnable");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    checkError("glBlendFunc");*/
	
	glDisable(GL_CULL_FACE);
    checkError();
	
	// Use program
	glUseProgram(m_program);
    checkError();
	
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
	// Bind the VAO
    glBindVertexArray(m_vao);
    checkError();
#endif
    // Bind the VBO
    if (noaspect)
        glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2]);
    else
        glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
    checkError();
	
	// Scale
    glm::mat4 scaleMatrix;
    scaleMatrix = glm::scale(glm::mat4(), glm::vec3(scalex, scaley, 1.0f));
                        
    const float *p = (const float *)glm::value_ptr(scaleMatrix);
    for (int i = 0; i < 16; i++)
        m_scaleMatrixArr[i] = p[i];
		
	m_scaleMatrixHandle = glGetUniformLocation(m_program, "scaleMatrix");
    checkError("glGetUniformLocation");
	
	glUniformMatrix4fv(m_scaleMatrixHandle, 1, false, m_scaleMatrixArr);
    checkError("glUniformMatrix4fv");
                                  
    // Translate
    glm::mat4 translateMatrix;
    translateMatrix = glm::translate(glm::mat4(), glm::vec3(xshift, yshift, 0.0f));
                        
    p = (const float *)glm::value_ptr(translateMatrix);
    for (int i = 0; i < 16; i++)
        m_translateMatrixArr[i] = p[i];
		
    m_translateMatrixHandle = glGetUniformLocation(m_program, "translateMatrix");
    checkError("glGetUniformLocation");
		
	glUniformMatrix4fv(m_translateMatrixHandle, 1, false, m_translateMatrixArr);
    checkError("glUniformMatrix4fv");

    // Position
            
    GLint vPosition = glGetAttribLocation(m_program, "vPosition");
    checkError("glGetAttribLocation");
    if (vPosition == -1)
    {
        Log("SpriteRenderer2D vPosition is -1");
    }
            
    glEnableVertexAttribArray(vPosition);
    checkError("glEnableVertexAttribArray");
    glVertexAttribPointer(vPosition, 4,
                                  GL_FLOAT, false,
                                  sizeof(float) * 6, 0);
    checkError("glVertexAttribPointer");
	
	// TexCoords
	
    GLint vTexCoords = glGetAttribLocation(m_program, "vTexCoords");
    checkError("glGetAttribLocation");
    if (vTexCoords == -1)
    {
        Log("SpriteRenderer2D vTexCoords is -1");
    }
            
    glEnableVertexAttribArray(vTexCoords);
    checkError("glEnableVertexAttribArray");
    glVertexAttribPointer(vTexCoords, 2,
                                  GL_FLOAT, false,
                                  sizeof(float) * 6, BUFFER_OFFSET(4 * sizeof(float)));
    checkError("glVertexAttribPointer");
	
	// Color
                        
	GLint colorHandle = glGetUniformLocation(m_program, "vColor");
    checkError("glGetUniformLocation");
    glUniform4f(colorHandle, r * (g_fadeMultiplier + g_redout), g * (g_fadeMultiplier - g_redout), b * (g_fadeMultiplier - g_redout), a);
	checkError("glUniform4f");
	
	// Texture
    glActiveTexture(GL_TEXTURE0);
    checkError("glActiveTexture");

	glBindTexture(GL_TEXTURE_2D, glTexID);
    checkError("glBindTexture");
    
    //
    // Draw the triangles
    //
            
    glDrawArrays(GL_TRIANGLES, 0, 6);
    checkError("glDrawArrays");
            
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    glBindVertexArray(0);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);
    checkError("glBindTexture");
    glUseProgram(0);
    checkError("glUseProgram");
}

void SpriteRenderer2D::checkError()
{
#ifdef USE_OPENGL
    GLenum err = glGetError();
    
    switch (err)
    {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            Log("GL error GL_INVALID_ENUM");
            break;
        case GL_INVALID_VALUE:
            Log("GL error GL_INVALID_VALUE");
            break;
        case GL_INVALID_OPERATION:
            Log("GL error GL_INVALID_OPERATION");
            break;
#ifndef USE_OPENGL_3
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            Log("GL error GL_INVALID_FRAMEBUFFER_OPERATION");
            break;
#endif
        case GL_OUT_OF_MEMORY:
            Log("GL error GL_OUT_OF_MEMORY");
            break;
    }
#endif
}

void SpriteRenderer2D::checkError(char *str)
{
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
}

