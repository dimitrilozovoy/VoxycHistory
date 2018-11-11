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

#ifndef FATELESS_SKYBOXRENDERER_H        
#define FATELESS_SKYBOXRENDERER_H
   
#include <vector>
#include "GLIncludes.h"
#include "Object.h"
#include "Renderer.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class SkyboxRenderer: public Renderer
{
public:
    void init();
	void draw(Object *camera, int glTexID);
    void setMatrix(char *name, glm::mat4 matrix);
    void setUniform1f(char *name, float x);
    void setUniform4f(char *name, float x, float y, float z, float w);
    void setVertexAttrib(char *name, int size, int type, bool normalized, int stride, int pointer);
    void loadVertices();
    void freeVertices();
    void loadProgram();
    int loadShader(int type, const char *shaderCode);
    void checkGLError(char *tag);

private:

	//
	// VERTEX SHADER DESKTOP
	//
 
	const char *vertexShaderCodeDesktop =

		"precision highp float;" \

		"in vec4 vPosition;" \
		"out vec4 posOut; " \
		"in vec2 vTexCoords;" \
		"out vec2 vTexCoordsOut; " \
		"uniform mat4 mvMatrix; " \
		"uniform mat4 projectionMatrix; " \
 
		"void main() {" \
		"  gl_Position = projectionMatrix * mvMatrix * vPosition; "
		"  vTexCoordsOut = vTexCoords ; " \
		"  posOut = gl_Position; " \
		"}\n";

	//
	// FRAGMENT SHADER DESKTOP
	//

	const char *fragmentShaderCodeDesktop =

		"precision highp float;" \

		"uniform sampler2D uTexture; " \
		"uniform vec4 vColor; " \
		"uniform vec4 globalColor; " \
		"in vec2 vTexCoordsOut; " \
		"in vec4 posOut; " \
		"out vec4 finalColor; " \

		"void main() {" \
		"      finalColor = texture(uTexture, vTexCoordsOut.st) * vColor * globalColor; " \
		"}\n";

	//
    // VERTEX SHADER ES 2.0
    //
    
    const char *vertexShaderCodeES20 =
    
    "attribute vec4 vPosition;" \
	"varying lowp vec4 posOut; " \
	"attribute vec2 vTexCoords;" \
	"varying lowp vec2 vTexCoordsOut; " \
	"uniform mat4 mvMatrix; " \
    "uniform mat4 projectionMatrix; " \

    "void main() {" \
    "  gl_Position = projectionMatrix * mvMatrix * vPosition; "
	"  vTexCoordsOut = vTexCoords; " \
	"  posOut = gl_Position; " \
    "}\n";


    //
    // FRAGMENT SHADER ES 2.0
    //
    
    const char *fragmentShaderCodeES20 =
    
	"uniform sampler2D uTexture; " \
    "uniform lowp vec4 vColor; " \
	"uniform lowp vec4 globalColor; " \
	"varying lowp vec2 vTexCoordsOut; " \
	"varying lowp vec4 posOut; " \

    "void main() {" \
    "      gl_FragColor = texture2D(uTexture, vTexCoordsOut.st) * vColor * globalColor; " \
    "}\n";

    int program = -1;
    int vao = -1;
    int vbo;
};

#endif //FATELESS_SkyboxRenderer_H
