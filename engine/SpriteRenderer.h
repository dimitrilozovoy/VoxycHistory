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

#ifndef SPRITERENDERER_H        
#define SPRITERENDERER_H

#define DO_FAST_SPRITES

#include "Object.h"
#include "Renderer.h"
#include "TextureAtlas.h"
#include "TextureManager2.h"

class SpriteRenderer: public Renderer
{
	public:
	    void init(TextureManager2 *texMan, SpriteRenderer2D *spr2d);
//		void draw(int eye, std::map<std::string, Object*> objects, Object *camera);
        void draw(int eye, std::map<std::string, Object*> objects, Object *camera);
//        void drawCall(int eye, std::map<std::string, Object*> objects, Object *camera, int call, int numCalls);

	private:

	//
	// VERTEX SHADER ES 2.0
	//

	const char *vertexShaderCodeES20 =

	"attribute vec4 vPosition;" \
	"varying lowp vec4 posOut; " \
	"attribute vec2 vTexCoords;" \
	"varying lowp vec2 vTexCoordsOut; " \
    "uniform vec2 vTexSpan;" \
    "attribute vec4 vNormal;" \
    "varying vec4 vNormalOut;" \
	"attribute vec4 vVertexLight; " \
    "varying vec4 vVertexLightOut; " \
    "uniform mat4 projectionMatrix; " \
	"varying lowp float distToCamera; " \
	
	"attribute vec4 mvMatrixPt1; " \
	"attribute vec4 mvMatrixPt2; " \
	"attribute vec4 mvMatrixPt3; " \
	"attribute vec4 mvMatrixPt4; " \
	
	"attribute vec4 vColor; " \
	"varying vec4 vColorOut;" \
		
	"attribute mat4 oldmvMatrix; " \

	
	"void main() {" \
	
	"  mat4 mvMatrix; " \
	
	"  mvMatrix[0] = mvMatrixPt1; " \
	"  mvMatrix[1] = mvMatrixPt2; " \
	"  mvMatrix[2] = mvMatrixPt3; " \
	"  mvMatrix[3] = mvMatrixPt4; " \

    "  gl_Position = projectionMatrix * mvMatrix * vPosition; "
					"  vTexCoordsOut = vTexCoords * vTexSpan; " \
	"  posOut = gl_Position; " \

					"  vec4 posBeforeProj = mvMatrix * vPosition;" \
	"  distToCamera = -posBeforeProj.z; " \
	
	"  vColorOut = vColor; " \
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
	"uniform lowp float useTexture; " \

	"uniform lowp float fadeNear; " \
	"uniform lowp float fadeFar; " \

	"varying lowp float distToCamera; " \
    "varying lowp vec4 vColorOut; " \

	"void main() {" \

		"   lowp vec4 f = texture2D(uTexture, vTexCoordsOut.st); " \
		"   if (f.a == 0.0) " \
		"       discard; " \

		"	lowp float visibility = 1.0; " \
		"   lowp float alpha = 1.0; " \

		"   if (distToCamera >= fadeFar) discard; " \

		"   if (distToCamera >= fadeNear) " \
		"		alpha = 1.0 - (distToCamera - fadeNear) * 3.0; " \

		"   if (useTexture == 1.0)" \
		"   {" \
		"      gl_FragColor = texture2D(uTexture, vTexCoordsOut.st) * vColorOut * vec4(visibility, visibility, visibility, alpha) * globalColor; " \
		"   }" \
		"   else" \
		"   {" \
		"      gl_FragColor = vColorOut * vec4(visibility, visibility, visibility, alpha) * globalColor; " \
		"   }" \
		"}\n";

	//
	// VERTEX SHADER DESKTOP
	//

	const char *vertexShaderCodeDesktop =

		"precision highp float;" \
 
		"in vec4 vPosition;" \
		"out lowp vec4 posOut; " \
		"in vec2 vTexCoords;" \
		"out vec2 vTexCoordsOut; " \
		"uniform vec2 vTexSpan;" \
		"in vec4 vNormal;" \
		"out vec4 vNormalOut;" \
		"in vec4 vVertexLight; " \
		"out vec4 vVertexLightOut; " \
		"uniform mat4 projectionMatrix; " \
		"out float distToCamera; " \

		"in vec4 mvMatrixPt1; " \
		"in vec4 mvMatrixPt2; " \
		"in vec4 mvMatrixPt3; " \
		"in vec4 mvMatrixPt4; " \

		"in vec4 vColor; " \
		"out vec4 vColorOut;" \

		"in mat4 oldmvMatrix; " \


		"void main() {" \

		"  mat4 mvMatrix; " \

		"  mvMatrix[0] = mvMatrixPt1; " \
		"  mvMatrix[1] = mvMatrixPt2; " \
		"  mvMatrix[2] = mvMatrixPt3; " \
		"  mvMatrix[3] = mvMatrixPt4; " \

		"  gl_Position = projectionMatrix * mvMatrix * vPosition; "
		"  vTexCoordsOut = vTexCoords * vTexSpan; " \
		"  posOut = gl_Position; " \

		"  vec4 posBeforeProj = mvMatrix * vPosition;" \
		"  distToCamera = -posBeforeProj.z; " \

		"  vColorOut = vColor; " \
		"}\n";


	//
	// FRAGMENT SHADER DESKTOP
	//

	const char *fragmentShaderCodeDesktop =

		"precision highp float;" \

		"uniform sampler2D uTexture; " \
		"uniform lowp vec4 vColor; " \
		"uniform lowp vec4 globalColor; " \
		"out lowp vec2 vTexCoordsOut; " \
		"out lowp vec4 posOut; " \
		"out vec4 finalColor; " \
		"uniform lowp float useTexture; " \

		"uniform lowp float fadeNear; " \
		"uniform lowp float fadeFar; " \

		"out float distToCamera; " \
		"out vec4 vColorOut; " \

		"void main() {" \

		"   lowp vec4 f = texture(uTexture, vTexCoordsOut.st); " \
		"   if (f.a == 0.0) " \
		"       discard; " \

		"	lowp float visibility = 1.0; " \
		"   lowp float alpha = 1.0; " \

		"   if (distToCamera >= fadeFar) discard; " \

		"   if (distToCamera >= fadeNear) " \
		"		alpha = 1.0 - (distToCamera - fadeNear) * 3.0; " \

		"   if (useTexture == 1.0)" \
		"   {" \
		"      finalColor = texture(uTexture, vTexCoordsOut.st) * vColorOut * vec4(visibility, visibility, visibility, alpha) * globalColor; " \
		"   }" \
		"   else" \
		"   {" \
		"      finalColor = vColorOut * vec4(visibility, visibility, visibility, alpha) * globalColor; " \
		"   }" \
		"}\n";

		int programMain = -1;
		int vao = -1;
		int vbo = -1;

	    TextureAtlas texAtlas;
};

#endif
