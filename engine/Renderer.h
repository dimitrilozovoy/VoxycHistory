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

#ifndef Renderer_h
#define Renderer_h

#include "Globals.hpp"
#include "GLIncludes.h"
#include "Object.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
//#define NDC_SCALE	0.001f
 #define NDC_SCALE	0.01f
//#define NDC_SCALE	1.00f
#else
#define NDC_SCALE	1.0f
#endif

#define MAX_DYNAMIC_LIGHTS 12

typedef struct DynamicLight
{
	Object *obj;
	float radius;
	glm::vec4 color;
} DynamicLight;

class Renderer
{
public:
	void setMatrix(int program, char *name, glm::mat4 matrix);
	void setMatrixArray(int program, char *name, int size, glm::mat4 matrix[]);
	void setUniform1f(int program, char *name, float x);
	void setUniform1fv(int program, char *name, int size, float *data);
	void setUniform2f(int program, char *name, float x, float y);
	void setUniform2fv(int program, char *name, int size, float *data);
	void setUniform4f(int program, char *name, float x, float y, float z, float w);
	void setUniform4fv(int program, char *name, int size, float *data);
    void setVertexAttrib(int program, char *name, int size, int type, bool normalized, int stride, int pointer);
	int loadProgram(char *vertexShaderCode, char *fragmentShaderCode, bool shadowMap);
	int loadShader(int type, const char *shaderCode);
	void setGlobalColor(glm::vec4 globalColor);
	void fadeGlobalColor(glm::vec4 globalColor, int ticks);
	void tick();
	void checkGLError(char *tag);
	std::string getFrameDump() { return frameDump; }
	void DumpFrame() { dumpFrame = true; };
	void setDynamicLights(std::map<std::string, DynamicLight> dynamicLights, Object *obj, int program, glm::mat4 rotate);

protected:
	glm::vec4 targetGlobalColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
	glm::vec4 globalColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
	glm::vec4 lastGlobalColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
	int fadeTimer = 0;
	int fadeLength = 0;
	glm::vec4 fadeStep;
	std::string frameDump;
	bool dumpFrame = false;
};

#endif
