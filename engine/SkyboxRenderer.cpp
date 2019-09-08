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
#include "SkyboxRenderer.h"
#include "DDLUtils.hpp"
#include <vector>
#include "platform.h"

void SkyboxRenderer::init()
{
    loadVertices();
    loadProgram();
}

void SkyboxRenderer::draw(Object *camera, int glTexID)
{
    int numCoords = 0;
    int stride = 6 * sizeof(float);

    numCoords = 6 * 6;

    float width = PLAT_GetWindowWidth();
    float height = PLAT_GetWindowHeight();
#ifdef PLATFORM_OPENVR
	float aspect = 1.0;
#else
	float aspect = width / height;
#endif

    glDisable(GL_DEPTH_TEST);
    checkGLError("glDisable");
//	glDepthMask(false);

    glEnable(GL_CULL_FACE);
    checkGLError("glEnable");
    glFrontFace(GL_CW);
    checkGLError("glFrontFace");
    glCullFace(GL_BACK);
    checkGLError("glCullFace");

    // Add program to OpenGL environment
    glUseProgram(program);
    checkGLError("glUseProgram");

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
	// Bind the VAO
	glBindVertexArray(vao);
	checkGLError("glBindVertexArray");
#endif
    
	// Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLError("glBindBuffer");

    // Set the matrices
	glm::mat4 mvMatrix;
	glm::mat4 projectionMatrix;

	mvMatrix =
		glm::rotate(glm::mat4(), glm::radians(camera->roll), glm::vec3(0, 0, 1)) // Camera roll
		* glm::rotate(glm::mat4(), -glm::radians(camera->pitch), glm::vec3(1, 0, 0)) // Camera pitch
		* glm::rotate(glm::mat4(), glm::radians(camera->yaw), glm::vec3(0, 1, 0)) // Camera yaw

/*	    * glm::translate(glm::mat4(), glm::vec3(object->position.x, object->position.y, object->position.z)) // Translate
		* glm::rotate(glm::mat4(), glm::radians(-object->yaw), glm::vec3(0, 1, 0)) // Model yaw
		* glm::rotate(glm::mat4(), glm::radians(-object->pitch), glm::vec3(1, 0, 0)) // Model pitch*/

        * glm::scale(glm::mat4(), glm::vec3(SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE)); // Scale

	setMatrix("mvMatrix", mvMatrix);
	
#ifdef PLATFORM_OPENVR
	glm::mat4 projMatrix = glm::make_mat4((const GLfloat *)g_projectionMatrix.get()) * glm::make_mat4((const GLfloat *)g_poseEyeMatrix.get());
	setMatrix("projectionMatrix", projMatrix);

//	setMatrix("projectionMatrix", glm::perspective(VIEW_FOV, aspect, 0.001f, 100.0f) * glm::make_mat4((const GLfloat *)g_poseEyeMatrix.get()));
//	setMatrix("projectionMatrix", glm::make_mat4((const GLfloat *)g_poseEyeProjectionMatrix.get()));
#else
	setMatrix("projectionMatrix", glm::perspective(VIEW_FOV, aspect, 0.001f, 1000.0f));
#endif

    // Set variables
    setUniform4f("vColor", 1.0, 1.0, 1.0, 1.0);
	setUniform4f("globalColor", globalColor.x, globalColor.y, globalColor.z, globalColor.w);

    // Set texture
    if (glTexID != -1) {
        glActiveTexture(GL_TEXTURE0);
        checkGLError("glActiveTexture");
        glBindTexture(GL_TEXTURE_2D, glTexID);
        checkGLError("glBindTexture");

        // Set texture unit number
        int uTexture = glGetUniformLocation(program, "uTexture");
        checkGLError("glGetUniformLocation");
        glUniform1i(uTexture, 0);
        checkGLError("glUniform1i");
     }

    // Set attributes
    setVertexAttrib("vPosition", 4, GL_FLOAT, false, stride, 0);
    setVertexAttrib("vTexCoords", 2, GL_FLOAT, false, stride, 4);

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, numCoords);
    checkGLError("glDrawArrays");

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Reset
    glBindVertexArray(0);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLError("glBindTexture");
    glUseProgram(0);
    checkGLError("glUseProgram");
}

void SkyboxRenderer::setMatrix(char *name, glm::mat4 matrix)
{
    float arr[16];

    const float *p = (const float *)glm::value_ptr(matrix);
    for (int i = 0; i < 16; i++)
        arr[i] = p[i];

    int handle = glGetUniformLocation(program, name);
    checkGLError("glGetUniformLocation");

    if (handle != -1)
    {
        glUniformMatrix4fv(handle, 1, false, arr);
        checkGLError("glUniformMatrix4fv");
    }
}

void SkyboxRenderer::setUniform1f(char *name, float x)
{
    int handle = glGetUniformLocation(program, name);
    checkGLError("glGetUniformLocation");
    glUniform1f(handle, x);
    checkGLError("glUniform1f");
}

void SkyboxRenderer::setUniform4f(char *name, float x, float y, float z, float w)
{
    int handle = glGetUniformLocation(program, name);
    checkGLError("glGetUniformLocation");
    glUniform4f(handle, x, y, z, w);
    checkGLError("glUniform4f");
}

void SkyboxRenderer::setVertexAttrib(char *name, int size, int type, bool normalized, int stride, int pointer)
{
    GLint handle = glGetAttribLocation(program, name);
    checkGLError("glGetAttribLocation");

    if (handle == -1)
    {
        Log(name, "is -1");
        return;
    }

    glEnableVertexAttribArray(handle);
    checkGLError("glEnableVertexAttribArray");

    glVertexAttribPointer(handle, size,
                          type, normalized,
                          stride, BUFFER_OFFSET(pointer * sizeof(float)));
    checkGLError("glVertexAttribPointer");
}

void SkyboxRenderer::loadVertices()
{
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Generate VAO
	glGenVertexArrays(1, (GLuint *)&vao);
	checkGLError("glGenVertexArrays");
	glBindVertexArray(vao);
	checkGLError("glBindVertexArray");

	// Generate VAO
    glGenVertexArrays(1, (GLuint *)&vao);
    checkGLError("glGenVertexArrays");
    glBindVertexArray(vao);
    checkGLError("glBindVertexArray");
#endif

    // Generate VBOs
    glGenBuffers(1, (GLuint *)&vbo);
    checkGLError("glGenBuffers");

    //
    // SHAPE_BLOCK
    //

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLError("glBindBuffer");

    float block[36 * 6];

    // FRONT

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1

    // Vertex 0
    block[0] = -1.0f;
    block[1] = 1.0f;
    block[2] = 1.0f;
    block[3] = 1.0f;

    block[4] = 0.0f;
    block[5] = 1.0f;

    // Vertex 1
    block[6] = -1.0f;
    block[7] = -1.0f;
    block[8] = 1.0f;
    block[9] = 1.0f;

    block[10] = 0.0f;
    block[11] = 0.0f;

    // Vertex 2
    block[12] = 1.0f;
    block[13] = -1.0f;
    block[14] = 1.0f;
    block[15] = 1.0f;

    block[16] = 1.0f;
    block[17] = 0.0f;

    // Triangle 2

    // Vertex 3
    block[18] = 1.0f;
    block[19] = -1.0f;
    block[20] = 1.0f;
    block[21] = 1.0f;

    block[22] = 1.0f;
    block[23] = 0.0f;

    // Vertex 4
    block[24] = 1.0f;
    block[25] = 1.0f;
    block[26] = 1.0f;
    block[27] = 1.0f;

    block[28] = 1.0f;
    block[29] = 1.0f;

    // Vertex 5
    block[30] = -1.0f;
    block[31] = 1.0f;
    block[32] = 1.0f;
    block[33] = 1.0f;

    block[34] = 0.0f;
    block[35] = 1.0f;

    // RIGHT SIDE

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1
    int o = 36;

    // Vertex 0
    block[o + 0] = 1.0f;
    block[o + 1] = 1.0f;
    block[o + 2] = 1.0f;
    block[o + 3] = 1.0f;

    block[o + 4] = 1.0f;
    block[o + 5] = 1.0f;

    // Vertex 1
    block[o + 6] = 1.0f;
    block[o + 7] = -1.0f;
    block[o + 8] = 1.0f;
    block[o + 9] = 1.0f;

    block[o + 10] = 1.0f;
    block[o + 11] = 0.0f;

    // Vertex 2
    block[o + 12] = 1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = -1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 0.0f;
    block[o + 17] = 0.0f;

    // Triangle 2

    // Vertex 3
    block[o + 18] = 1.0f;
    block[o + 19] = -1.0f;
    block[o + 20] = -1.0f;
    block[o + 21] = 1.0f;

    block[o + 22] = 0.0f;
    block[o + 23] = 0.0f;

    // Vertex 4
    block[o + 24] = 1.0f;
    block[o + 25] = 1.0f;
    block[o + 26] = -1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 0.0f;
    block[o + 29] = 1.0f;

    // Vertex 5
    block[o + 30] = 1.0f;
    block[o + 31] = 1.0f;
    block[o + 32] = 1.0f;
    block[o + 33] = 1.0f;

    block[o + 34] = 1.0f;
    block[o + 35] = 1.0f;

    // BACK SIDE

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1
    o = 36 * 2;

    // Vertex 0
    block[o + 0] = 1.0f;
    block[o + 1] = 1.0f;
    block[o + 2] = -1.0f;
    block[o + 3] = 1.0f;

    block[o + 4] = 0.0f;
    block[o + 5] = 1.0f;

    // Vertex 1
    block[o + 6] = 1.0f;
    block[o + 7] = -1.0f;
    block[o + 8] = -1.0f;
    block[o + 9] = 1.0f;

    block[o + 10] = 0.0f;
    block[o + 11] = 0.0f;

    // Vertex 2
    block[o + 12] = -1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = -1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 1.0f;
    block[o + 17] = 0.0f;

    // Triangle 2

    // Vertex 3
    block[o + 18] = -1.0f;
    block[o + 19] = -1.0f;
    block[o + 20] = -1.0f;
    block[o + 21] = 1.0f;

    block[o + 22] = 1.0f;
    block[o + 23] = 0.0f;

    // Vertex 4
    block[o + 24] = -1.0f;
    block[o + 25] = 1.0f;
    block[o + 26] = -1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 1.0f;

    // Vertex 5
    block[o + 30] = 1.0f;
    block[o + 31] = 1.0f;
    block[o + 32] = -1.0f;
    block[o + 33] = 1.0f;

    block[o + 34] = 0.0f;
    block[o + 35] = 1.0f;

    // LEFT SIDE

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1
    o = 36 * 3;

    // Vertex 0
    block[o + 0] = -1.0f;
    block[o + 1] = 1.0f;
    block[o + 2] = -1.0f;
    block[o + 3] = 1.0f;

    block[o + 4] = 1.0f;
    block[o + 5] = 1.0f;

    // Vertex 1
    block[o + 6] = -1.0f;
    block[o + 7] = -1.0f;
    block[o + 8] = -1.0f;
    block[o + 9] = 1.0f;

    block[o + 10] = 1.0f;
    block[o + 11] = 0.0f;

    // Vertex 2
    block[o + 12] = -1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = 1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 0.0f;
    block[o + 17] = 0.0f;

    // Triangle 2

    // Vertex 3
    block[o + 18] = -1.0f;
    block[o + 19] = -1.0f;
    block[o + 20] = 1.0f;
    block[o + 21] = 1.0f;

    block[o + 22] = 0.0f;
    block[o + 23] = 0.0f;

    // Vertex 4
    block[o + 24] = -1.0f;
    block[o + 25] = 1.0f;
    block[o + 26] = 1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 0.0f;
    block[o + 29] = 1.0f;

    // Vertex 5
    block[o + 30] = -1.0f;
    block[o + 31] = 1.0f;
    block[o + 32] = -1.0f;
    block[o + 33] = 1.0f;

    block[o + 34] = 1.0f;
    block[o + 35] = 1.0f;

    // BOTTOM SIDE

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1
    o = 36 * 4;

    // Vertex 0
    block[o + 0] = -1.0f;
    block[o + 1] = -1.0f;
    block[o + 2] = 1.0f;
    block[o + 3] = 1.0f;

    block[o + 4] = 0.0f;
    block[o + 5] = 1.0f;

    // Vertex 1
    block[o + 6] = -1.0f;
    block[o + 7] = -1.0f;
    block[o + 8] = -1.0f;
    block[o + 9] = 1.0f;

    block[o + 10] = 0.0f;
    block[o + 11] = 0.0f;

    // Vertex 2
    block[o + 12] = 1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = -1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 1.0f;
    block[o + 17] = 0.0f;

    // Triangle 2

    // Vertex 3
    block[o + 18] = 1.0f;
    block[o + 19] = -1.0f;
    block[o + 20] = -1.0f;
    block[o + 21] = 1.0f;

    block[o + 22] = 1.0f;
    block[o + 23] = 0.0f;

    // Vertex 4
    block[o + 24] = 1.0f;
    block[o + 25] = -1.0f;
    block[o + 26] = 1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 1.0f;

    // Vertex 5
    block[o + 30] = -1.0f;
    block[o + 31] = -1.0f;
    block[o + 32] = 1.0f;
    block[o + 33] = 1.0f;

    block[o + 34] = 0.0f;
    block[o + 35] = 1.0f;

    // TOP SIDE

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1
    o = 36 * 5;

    // Vertex 0
    block[o + 0] = -1.0f;
    block[o + 1] = 1.0f;
    block[o + 2] = -1.0f;
    block[o + 3] = 1.0f;

    block[o + 4] = 0.0f;
    block[o + 5] = 1.0f;

    // Vertex 1
    block[o + 6] = -1.0f;
    block[o + 7] = 1.0f;
    block[o + 8] = 1.0f;
    block[o + 9] = 1.0f;

    block[o + 10] = 0.0f;
    block[o + 11] = 0.0f;

    // Vertex 2
    block[o + 12] = 1.0f;
    block[o + 13] = 1.0f;
    block[o + 14] = 1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 1.0f;
    block[o + 17] = 0.01f;

    // Triangle 2

    // Vertex 3
    block[o + 18] = 1.0f;
    block[o + 19] = 1.0f;
    block[o + 20] = 1.0f;
    block[o + 21] = 1.0f;

    block[o + 22] = 1.0f;
    block[o + 23] = 0.0f;

    // Vertex 4
    block[o + 24] = 1.0f;
    block[o + 25] = 1.0f;
    block[o + 26] = -1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 1.0f;

    // Vertex 5
    block[o + 30] = -1.0f;
    block[o + 31] = 1.0f;
    block[o + 32] = -1.0f;
    block[o + 33] = 1.0f;

    block[o + 34] = 0.0f;
    block[o + 35] = 1.0f;

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 6, block, GL_STATIC_DRAW);
    checkGLError("glBufferData");
}

void SkyboxRenderer::freeVertices()
{
}

void SkyboxRenderer::loadProgram()
{
	int vertexShader;
	int fragmentShader;
	
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX || defined PLATFORM_OPENVR
	const int len = 10000;

	char vertexShaderStr[len];
	snprintf(vertexShaderStr, len, "#version 150\n%s", vertexShaderCodeDesktop);
	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderStr);

	char fragmentShaderStr[len];
	snprintf(fragmentShaderStr, len, "#version 150\n%s", fragmentShaderCodeDesktop);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderStr);
#endif
#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderCodeES20);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderCodeES20);
#endif

	program = glCreateProgram();             // Create empty OpenGL ES Program
	checkGLError("glCreateProgram");
	glAttachShader(program, vertexShader);   // Add the vertex shader to program
	checkGLError("glAttachShader");
	glAttachShader(program, fragmentShader); // Add the fragment shader to program
	checkGLError("glAttachShader");

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
	glBindFragDataLocation(program, 0, "finalColor");
//	checkError();
#endif

	glLinkProgram(program);                  // Creates OpenGL ES program executables

											 // Throw exception if linking failed
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		Log("Program linking failure: ");

		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		Log(strInfoLog);
		delete[] strInfoLog;

		glDeleteProgram(program);
		program = 0;
	}
}

int SkyboxRenderer::loadShader(int type, const char *shaderCode)
{
	int shader = glCreateShader(type);
	checkGLError("glCreateShader");

	// Add the source code to the shader and compile it
	char const *codeArr[1];
	codeArr[0] = shaderCode;
	glShaderSource(shader, 1, codeArr, NULL);
	checkGLError("glShaderSource");
	glCompileShader(shader);
	checkGLError("glShaderSource");

	GLint compile_ok = GL_FALSE;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
	checkGLError("glGetShaderiv");

	if (compile_ok == GL_FALSE)
	{
		Log("main shader compile error; type = ", type);
		char *errorMsg = (char *)malloc(MAX_STR_LEN);
		GLsizei len;
		glGetShaderInfoLog(shader, MAX_STR_LEN, &len, errorMsg);
		Log(errorMsg);
		Log("len =", len);
		free(errorMsg);
	}

	return shader;
}

void SkyboxRenderer::checkGLError(char *tag)
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
