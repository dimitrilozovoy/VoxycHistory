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

#ifndef SKELETALRENDERER_H
#define SKELETALRENDERER_H

#include <vector>
#include <map>
#include "GLIncludes.h"
#include "Object.h"
#include "ShadowMap.h"
#include "Renderer.h"

class SkeletalRenderer: public Renderer
{
public:
    void init(ShadowMap *shadowMap, bool useShadowMap, Object *mouseLook);
    void draw(int eye, std::map<std::string, Object*> objects, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap);
	void drawModel(Object *object, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap);
	void drawMesh(Object *object, Model2 *model, Mesh *mesh, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap);
    void loadVertices();
    void freeVertices();

	int getVAO() { return vao; };

private:

	//
	// VERTEX SHADER DESKTOP
	//

	const char *vertexShaderCodeDesktopUseShadowMap =

		"precision highp float;" \

		"in vec4 vPosition;" \
		"out vec4 posOut; " \
		"in vec2 vTexCoords;" \
		"out vec2 vTexCoordsOut; " \
		"uniform vec2 vTexSpan;" \
		"uniform mat4 mvMatrix; " \
		"uniform mat4 projectionMatrix; " \

		"out vec4 ShadowCoord; " \
		"uniform mat4 DepthBiasMVP; " \

		"out float distToCamera; " \

		"void main() {" \
		"  gl_Position = projectionMatrix * mvMatrix * vPosition; "
		"  vTexCoordsOut = vTexCoords; " \
		"  posOut = gl_Position; " \

		"  ShadowCoord = DepthBiasMVP * vPosition; " \

		"  vec4 posBeforeProj = mvMatrix * vPosition;" \
		"  distToCamera = abs(posBeforeProj.z); " \
		"}\n";

	//
	// FRAGMENT SHADER DESKTOP
	//

	const char *fragmentShaderCodeDesktopUseShadowMap =

		"precision highp float;" \

		"uniform sampler2D uTexture; " \
		"uniform vec4 vColor; " \
		"uniform vec4 globalColor; " \
		"in vec2 vTexCoordsOut; " \
		"in vec4 posOut; " \
		"out vec4 finalColor; " \
		"uniform float useTexture; " \

		"uniform float useShadowMap; " \
		"in vec4 ShadowCoord; " \
		"uniform sampler2DShadow shadowMap; " \

		"uniform float fadeNear; " \
		"uniform float fadeFar; " \

		"in float distToCamera; " \

		"void main() {" \

		"	float visibility = 1.0; " \
		"	float bias = 0.005; " \

		"   if ( texture( shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z) / ShadowCoord.w) ) < ShadowCoord.z - bias){ " \
		"		visibility = 0.5;	" \
		"   } " \

		"   float alpha = 1.0; " \

		"   if (distToCamera >= fadeFar) discard; " \

		"   if (distToCamera >= fadeNear) " \
		"		alpha = 1.0 - (distToCamera - fadeNear) * 3.0; " \

		"   if (useTexture == 1.0)" \
		"   {" \
		"      finalColor = texture(uTexture, vTexCoordsOut.st) * vColor * vec4(visibility, visibility, visibility, alpha) * globalColor; " \
		"   }" \
		"   else" \
		"   {" \
		"      finalColor = vColor * visibility; " \
		"   }" \
		"}\n";
		
	//
	// VERTEX SHADER DESKTOP
	//

	const char *vertexShaderCodeDesktop =

		"precision highp float;" \

		"in vec4 vPosition;" \
		"out vec4 posOut; " \
		"in vec2 vTexCoords;" \
		"out vec2 vTexCoordsOut; " \
		"uniform vec2 vTexSpan;" \
		"uniform mat4 mvMatrix; " \
		"uniform mat4 projectionMatrix; " \

		"out float distToCamera; " \

		"void main() {" \
		"  gl_Position = projectionMatrix * mvMatrix * vPosition; "
		"  vTexCoordsOut = vTexCoords; " \
		"  posOut = gl_Position; " \

		"  vec4 posBeforeProj = mvMatrix * vPosition;" \
		"  distToCamera = abs(posBeforeProj.z); " \

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
		"uniform float useTexture; " \

		"uniform float fadeNear; " \
		"uniform float fadeFar; " \

		"in float distToCamera; " \

		"void main() {" \

		"	float visibility = 1.0; " \
		"	float bias = 0.005; " \

		"   float alpha = 1.0; " \

		"   if (distToCamera >= fadeFar) discard; " \

		"   if (distToCamera >= fadeNear) " \
		"		alpha = 1.0 - (distToCamera - fadeNear) * 3.0; " \

		"   if (useTexture == 1.0)" \
		"   {" \
		"      finalColor = texture(uTexture, vTexCoordsOut.st) * vColor * vec4(visibility, visibility, visibility, alpha) * globalColor; " \
		"   }" \
		"   else" \
		"   {" \
		"      finalColor = vColor * visibility * globalColor; " \
		"   }" \
		"}\n";

/*	"      finalColor = texture(uTexture, vTexCoordsOut.st) * vColor * vec4(visibility, visibility, visibility, 1.0); " \*/
	
	//
    // VERTEX SHADER ES 2.0
    //
    
    const char *vertexShaderCodeES20 =
    
    "attribute vec4 vPosition;" \
	"varying lowp vec4 posOut; " \
	"attribute vec2 vTexCoords;" \
	"varying lowp vec2 vTexCoordsOut; " \
	"attribute vec4 boneIDs;" \
    "attribute vec4 weights;" \
	"uniform mat4 mvMatrix; " \
    "uniform mat4 projectionMatrix; " \
	
	"uniform mat4 bones[100]; " \

    "varying lowp float distToCamera; " \

    "void main() {" \
    "  gl_Position = projectionMatrix * mvMatrix * vPosition; "
	"  vTexCoordsOut = vTexCoords; " \
	"  posOut = gl_Position; " \

    "  vec4 posBeforeProj = mvMatrix * vPosition;" \
	"  distToCamera = -posBeforeProj.z; " \
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

    "void main() {" \
		"	lowp float visibility = 1.0; " \
		"   lowp float alpha = 1.0; " \

        "   if (distToCamera >= fadeFar) discard; " \

        "   if (distToCamera >= fadeNear) " \
		"		alpha = 1.0 - (distToCamera - fadeNear) * 3.0; " \

		"   if (useTexture == 1.0)" \
		"   {" \
		"      gl_FragColor = texture2D(uTexture, vTexCoordsOut.st) * vColor * vec4(visibility, visibility, visibility, alpha) * globalColor; " \
		"   }" \
		"   else" \
		"   {" \
		"      gl_FragColor = vColor * globalColor; " \
		"   }" \
    "}\n";

	int programMain = -1;
	int programShadowMap = -1;
	int vao = -1;
    int vbos[NUM_SHAPES];

	Object *mouseLook;
};

#endif //SKELETALRENDERER_H
