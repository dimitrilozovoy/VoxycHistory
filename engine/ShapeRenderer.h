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

#ifndef FATELESS_SHAPERENDERER_H
#define FATELESS_SHAPERENDERER_H

#include <vector>
#include <map>
#include "GLIncludes.h"
#include "Object.h"
#include "ShadowMap.h"
#include "Renderer.h"
#include "TextureManager2.h"

class ShapeRenderer: public Renderer
{
public:
    void init(ShadowMap *shadowMap, bool useShadowMap, Object *mouseLook, TextureManager2 *texMan);
    void draw(int eye, std::map<std::string, Object*> objects, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap, std::map<std::string, DynamicLight> dynamicLights);
	void drawShape(Object *object, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap, std::map<std::string, DynamicLight> dynamicLights);
	void drawMesh(Object *object, Shape *shape, Mesh *mesh,Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap, std::map<int, std::string> *voxelTextures, std::map<std::string, DynamicLight> dynamicLights);
	void loadVertices();
	void freeVertices();

private:

#ifdef PLATFORM_OPENVR
	const float drawDistance = 1000.0;
#else
	const float drawDistance = 1400.0;
#endif

	/*
	========================================
	Desktop
	========================================
	*/

	// VERTEX SHADER DESKTOP USE SHADOWMAP

	const char *vertexShaderCodeDesktopUseShadowMap =

		"precision highp float;" \

		"in vec4 vPosition;" \
		"out vec4 posOut; " \
		"in vec2 vTexCoords;" \
		"out vec2 vTexCoordsOut; " \
		"in vec3 vNormal;" \
		"out vec3 vNormalOut;" \
		"uniform vec2 vTexSpan;" \
		"uniform mat4 mvMatrix; " \
		"uniform mat4 projectionMatrix; " \

		"out vec4 ShadowCoord; " \
		"uniform mat4 DepthBiasMVP; " \

		"out float distToCamera; " \

		"void main() {" \
		"  gl_Position = projectionMatrix * mvMatrix * vPosition; "
		"  vTexCoordsOut = vTexCoords * vTexSpan; " \
		"  posOut = gl_Position; " \

		"  ShadowCoord = DepthBiasMVP * vPosition; " \

		"  vec4 posBeforeProj = mvMatrix * vPosition;" \
		"  distToCamera = -posBeforeProj.z; " \

		"  vNormalOut = vNormal; " \
		"}\n";

	// FRAGMENT SHADER DESKTOP USE SHADOWMAP

	const char *fragmentShaderCodeDesktopUseShadowMap =
				"precision highp float;" \

		"uniform sampler2D uTexture; " \
		"uniform vec4 vColor; " \
		"uniform vec4 globalColor; " \
		"uniform vec4 ambientLight; " \
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
		"	float bias = 0.00; " \
    
		"   if ( texture( shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z) / ShadowCoord.w) ) < ShadowCoord.z - bias){ " \
		"		visibility = 0.5;	" \
		"   } " \

		"   float alpha = 1.0; " \

		"   if (distToCamera >= fadeFar) discard; " \

		"   if (distToCamera >= fadeNear) " \
		"		alpha = 1.0 - (distToCamera - fadeNear) * 3.0; " \

		"   if (useTexture == 1.0)" \
		"   {" \
		"      finalColor = texture(uTexture, vTexCoordsOut.st) * vColor * vec4(visibility, visibility, visibility, alpha) * globalColor * ambientLight; " \
		"   }" \
		"   else" \
		"   {" \
		"      finalColor = vColor * vec4(visibility, visibility, visibility, alpha) * globalColor * ambientLight; " \
		"   }" \
		"}\n";

	// VERTEX SHADER DESKTOP

	const char *vertexShaderCodeDesktop =

		"precision highp float;" \

		"in vec4 vPosition;" \
		"out vec4 posOut; " \
		"out vec4 worldPosOut; " \
		"in vec2 vTexCoords;" \
		"out vec2 vTexCoordsOut; " \
		"uniform vec2 vTexSpan;" \
		"in vec3 vNormal;" \
		"out vec4 vNormalOut;" \
		"in vec3 vVertexLight; " \
		"out vec3 vVertexLightOut; " \
		"uniform mat4 mvMatrix; " \
		"uniform mat4 projectionMatrix; " \
		"uniform mat4 modelMatrix; " \

		"out float distToCamera; " \

		"void main() {" \
		"  gl_Position = projectionMatrix * mvMatrix * vPosition; "
		"  vTexCoordsOut = vTexCoords * vTexSpan; " \
		"  posOut = gl_Position; " \
		"  worldPosOut = modelMatrix * vPosition; " \
		"  vNormalOut = modelMatrix * vec4(vNormal, 0.0); " \
		"  vVertexLightOut = vVertexLight; " \

		"  vec4 posBeforeProj = mvMatrix * vPosition;" \
		"  distToCamera = -posBeforeProj.z; " \
	"}\n";

	// FRAGMENT SHADER DESKTOP

	const char *fragmentShaderCodeDesktop =

		"precision highp float;" \

		"uniform sampler2D uTexture; " \
		"uniform vec4 vColor; " \
		"uniform vec4 globalColor; " \
		"uniform vec4 ambientLight; " \
		"in vec2 vTexCoordsOut; " \
		"in vec4 vNormalOut; " \
		"in vec4 posOut; " \
		"in vec4 worldPosOut; " \
		"out vec4 finalColor; " \
		"uniform float useTexture; " \
		"in vec3 vVertexLightOut; " \

		"uniform float fadeNear; " \
		"uniform float fadeFar; " \

		"in float distToCamera; " \

		"uniform vec4 lightsPos[8]; " \
		"uniform float lightsSize[8]; " \
		"uniform vec4 lightsColor[8]; " \

		"void main() {" \

		"   lowp vec4 f = texture(uTexture, vTexCoordsOut.st); " \
		"   if (f.a == 0.0) " \
		"       discard; " \

		"	float visibility = 1.0; " \
		"	float bias = 0.05; " \

		"   float alpha = 1.0; " \

		"   if (distToCamera >= fadeFar) discard; " \

		"   if (distToCamera >= fadeNear) " \
		"		alpha = 1.0 - (distToCamera - fadeNear) * 3.0; " \

		"   vec4 light = vec4(1.0, 1.0, 1.0, 1.0); " \

		"   for (int i = 0; i < 8; i++) " \
		"   { " \
		"       if (lightsSize[i] > 0.0) " \
		"       { " \
		"           float distToWorldPos = distance(worldPosOut, lightsPos[i]); " \
		"           if (distToWorldPos < lightsSize[i]) " \
		"           { " \
		"               float intensity = 1.0 - (distToWorldPos / lightsSize[i]); " \
		"               float diffuse = max(dot(normalize(vNormalOut), normalize(lightsPos[i] - worldPosOut)), 0.0); " \
		"               light = light * (vec4(1.0, 1.0, 1.0, 1.0) + (lightsColor[i] - vec4(1.0, 1.0, 1.0, 1.0)) * intensity * diffuse); " \
		"           } " \
		"       } " \
		"   } " \

		"   if (useTexture == 1.0)" \
		"   {" \
		"      finalColor = texture(uTexture, vTexCoordsOut.st) * vColor * vec4(visibility, visibility, visibility, alpha) * globalColor * vec4(vVertexLightOut, 1.0) * ambientLight * light; " \
		"   }" \
		"   else" \
		"   {" \
		"      finalColor = vColor * vec4(visibility, visibility, visibility, alpha) * globalColor * vec4(vVertexLightOut, 1.0) * ambientLight * light; " \
		"   }" \
		"}\n";

	/*
	========================================
	OpenVR
	========================================
	*/

	// VERTEX SHADER OPEN VR

	const char *vertexShaderCodeOpenVR =

		"precision highp float;" \

		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"out vec4 posOut; " \
		"in vec2 vTexCoords;" \
		"out vec2 vTexCoordsOut; " \
		"uniform vec2 vTexSpan;" \
		"uniform mat4 mvMatrix; " \
		"uniform mat4 projectionMatrix; " \

		"out float distToCamera; " \

		"void main() {" \
		"  gl_Position = projectionMatrix * mvMatrix * vPosition; "
		"  vTexCoordsOut = vTexCoords * vTexSpan; " \
		"  posOut = gl_Position; " \

		"  vec4 posBeforeProj = mvMatrix * vPosition;" \
		"  distToCamera = abs(posBeforeProj.z); " \
		"}\n";

	// FRAGMENT SHADER OPENVR

	const char *fragmentShaderCodeOpenVR =

		"precision highp float;" \

		"uniform sampler2D uTexture; " \
		"uniform vec4 vColor; " \
		"in vec2 vTexCoordsOut; " \
		"in vec4 posOut; " \
		"out vec4 finalColor; " \
		"uniform float useTexture; " \

		"in float distToCamera; " \

		"void main() {" \

		"	float visibility = 1.0; " \
		"	float bias = 0.05; " \

		"   float alpha = 1.0; " \

		"   if (distToCamera >= 1600.0) " \
		"		alpha = 1.0 - (distToCamera - 1600.0) * 0.01; " \

		"   if (useTexture == 1.0)" \
		"   {" \
		"      finalColor = texture(uTexture, vTexCoordsOut.st) * vColor * vec4(visibility, visibility, visibility, alpha); " \
		"   }" \
		"   else" \
		"   {" \
		"      finalColor = vColor * vec4(visibility, visibility, visibility, alpha); " \
		"   }" \
		"}\n";

	/*
	========================================
	ES 2.0
	========================================
	*/

	// VERTEX SHADER ES 2.0 USE SHADOWMAP

	const char *vertexShaderCodeES20UseShadowMap =

	"attribute vec4 vPosition;" \
	"varying lowp vec4 posOut; " \
	"attribute vec2 vTexCoords;" \
	"varying lowp vec2 vTexCoordsOut; " \
    "uniform vec2 vTexSpan;" \
    "attribute vec3 vNormal;" \
    "varying vec3 vNormalOut;" \
	"attribute vec4 vVertexLight; " \
    "varying vec4 vVertexLightOut; " \
    "uniform mat4 mvMatrix; " \
    "uniform mat4 projectionMatrix; " \

	"varying vec4 ShadowCoord; " \
	"uniform mat4 DepthBiasMVP; " \

	"varying lowp float distToCamera; " \

	"void main() {" \
    "  gl_Position = projectionMatrix * mvMatrix * vPosition; "
	"  vTexCoordsOut = vTexCoords * vTexSpan; " \
	"  posOut = gl_Position; " \
	"  vNormalOut = vNormal; " \

	"  vec4 posBeforeProj = mvMatrix * vPosition;" \
	"  distToCamera = -posBeforeProj.z; " \

	"  ShadowCoord = DepthBiasMVP * vPosition; " \

    "}\n";

	// FRAGMENT SHADER ES 2.0 USE SHADOWMAP

	const char *fragmentShaderCodeES20UseShadowMap =

	"uniform sampler2D uTexture; " \
    "uniform lowp vec4 vColor; " \
	"uniform lowp vec4 globalColor; " \
	"uniform lowp vec4 ambientLight; " \
	"varying lowp vec2 vTexCoordsOut; " \
	"varying lowp vec4 posOut; " \
	"uniform lowp float useTexture; " \

	"uniform float useShadowMap; " \
	"varying vec4 ShadowCoord; " \
	"uniform sampler2D shadowMap; " \

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
	"      gl_FragColor = texture2D(uTexture, vTexCoordsOut.st) * vColor * vec4(visibility, visibility, visibility, alpha) * globalColor * ambientLight; " \
	"   }" \
	"   else" \
	"   {" \
	"      gl_FragColor = vColor * vec4(visibility, visibility, visibility, alpha) * globalColor * ambientLight; " \
	"   }" \
	"}\n";

    // VERTEX SHADER ES 2.0
    
    const char *vertexShaderCodeES20 =
    
    "attribute vec4 vPosition;" \
	"varying lowp vec4 posOut; " \
	"attribute mediump vec2 vTexCoords;" \
	"varying mediump vec2 vTexCoordsOut; " \
    "uniform vec2 vTexSpan;" \
    "attribute vec3 vNormal;" \
    "varying vec3 vNormalOut;" \
	"attribute vec3 vVertexLight; " \
    "varying vec3 vVertexLightOut; " \
    "uniform mat4 mvMatrix; " \
    "uniform mat4 projectionMatrix; " \

	"varying lowp float distToCamera; " \

    "void main() {" \
    "  gl_Position = projectionMatrix * mvMatrix * vPosition; "
	"  vTexCoordsOut = vTexCoords * vTexSpan; " \
	"  posOut = gl_Position; " \
	"  vNormalOut = vNormal; " \
	"  vVertexLightOut = vVertexLight; " \

    "  vec4 posBeforeProj = mvMatrix * vPosition;" \
	"  distToCamera = -posBeforeProj.z; " \
    "}\n";


    // FRAGMENT SHADER ES 2.0
    
    const char *fragmentShaderCodeES20 =
    
	"uniform sampler2D uTexture; " \
    "uniform lowp vec4 vColor; " \
	"uniform lowp vec4 globalColor; " \
	"uniform lowp vec4 ambientLight; " \
	"varying mediump vec2 vTexCoordsOut; " \
	"varying lowp vec4 posOut; " \
	"uniform lowp float useTexture; " \
	"varying lowp vec3 vVertexLightOut; " \

    "uniform lowp float fadeNear; " \
	"uniform lowp float fadeFar; " \

	"varying lowp float distToCamera; " \

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
		"      gl_FragColor = texture2D(uTexture, vTexCoordsOut.st) * vColor * vec4(visibility, visibility, visibility, alpha) * globalColor * vec4(vVertexLightOut, 1.0) * ambientLight; " \
		"   }" \
		"   else" \
		"   {" \
		"      gl_FragColor = vColor * vec4(visibility, visibility, visibility, alpha) * globalColor * vec4(vVertexLightOut, 1.0) * ambientLight; " \
		"   }" \
		"}\n";
		
    // VERTEX SHADER ES 2.0 DYNAMIC LIGHTS
    
    const char *vertexShaderCodeES20DynamicLights =
    
    "attribute vec4 vPosition;" \
	"varying lowp vec4 posOut; " \
	"varying lowp vec4 worldPosOut; " \
	"attribute vec2 vTexCoords;" \
	"varying highp vec2 vTexCoordsOut; " \
    "uniform vec2 vTexSpan;" \
    "attribute vec3 vNormal;" \
    "varying vec4 vNormalOut;" \
	"attribute vec3 vVertexLight; " \
    "varying vec3 vVertexLightOut; " \
    "uniform mat4 mvMatrix; " \
    "uniform mat4 projectionMatrix; " \
	"uniform mat4 modelMatrix; " \
	
	"varying lowp float distToCamera; " \

    "void main() {" \
    "  gl_Position = projectionMatrix * mvMatrix * vPosition; "
	"  vTexCoordsOut = vTexCoords * vTexSpan; " \
	"  posOut = gl_Position; " \
	"  worldPosOut = modelMatrix * vPosition; " \
	"  vNormalOut = modelMatrix * vec4(vNormal, 0.0); " \
	"  vVertexLightOut = vVertexLight; " \

    "  vec4 posBeforeProj = mvMatrix * vPosition;" \
	"  distToCamera = -posBeforeProj.z; " \
    "}\n";


    // FRAGMENT SHADER ES 2.0 DYNAMIC LIGHTS
    
    const char *fragmentShaderCodeES20DynamicLights =
    
	"uniform sampler2D uTexture; " \
    "uniform lowp vec4 vColor; " \
	"uniform lowp vec4 globalColor; " \
	"uniform lowp vec4 ambientLight; " \
	"varying highp vec2 vTexCoordsOut; " \
	"varying lowp vec4 vNormalOut; " \
	"varying lowp vec4 posOut; " \
	"varying lowp vec4 worldPosOut; " \
	"uniform lowp float useTexture; " \
	"varying lowp vec3 vVertexLightOut; " \

    "uniform lowp float fadeNear; " \
	"uniform lowp float fadeFar; " \

	"varying lowp float distToCamera; " \
	
	"uniform mediump vec4 lightsPos[8]; " \
	"uniform mediump float lightsSize[8]; " \
	"uniform mediump vec4 lightsColor[8]; " \

	"void main() {" \
    
	    "   lowp vec4 f = texture2D(uTexture, vTexCoordsOut.st); " \
		"   if (f.a == 0.0) " \
		"       discard; " \
	
		"	lowp float visibility = 1.0; " \
		"   lowp float alpha = 1.0; " \

        "   if (distToCamera >= fadeFar) discard; " \

        "   if (distToCamera >= fadeNear) " \
		"		alpha = 1.0 - (distToCamera - fadeNear) * 3.0; " \
		
		"   lowp vec4 light = vec4(1.0, 1.0, 1.0, 1.0); " \
		
		"   for (int i = 0; i < 8; i++) " \
		"   { " \
		"       if (lightsSize[i] > 0.0) " \
		"       { " \
		"           lowp float distToWorldPos = distance(worldPosOut, lightsPos[i]); " \
		"           if (distToWorldPos < lightsSize[i]) " \
		"           { " \
		"               lowp float intensity = 1.0 - (distToWorldPos / lightsSize[i]); " \
		"               lowp float diffuse = max(dot(normalize(vNormalOut), normalize(lightsPos[i] - worldPosOut)), 0.0); " \
		"               light = light * (vec4(1.0, 1.0, 1.0, 1.0) + (lightsColor[i] - vec4(1.0, 1.0, 1.0, 1.0)) * intensity * diffuse); " \
		"           } " \
		"       } " \
		"   } " \

		"   if (useTexture == 1.0)" \
		"   {" \
		"      gl_FragColor = texture2D(uTexture, vTexCoordsOut.st) * vColor * vec4(visibility, visibility, visibility, alpha) * globalColor * vec4(vVertexLightOut, 1.0) * ambientLight * light; " \
		"   }" \
		"   else" \
		"   {" \
		"      gl_FragColor = vColor * vec4(visibility, visibility, visibility, alpha) * globalColor * vec4(vVertexLightOut, 1.0) * ambientLight * light; " \
		"   }" \
		"}\n";
		
		/*
				"           float dist = distance(worldPosOut, lightsPos[i]); " \
		"           if (dist < lightsSize[i]) " \

		*/

    int programMain = -1;
	int programReg = -1;
	int programDyn = -1;
	int programLow = -1;
	int programMed = -1;
	int programHi = -1;
	int programShadowMap = -1;
	int vao = -1;
    int vbos[NUM_SHAPES];

	Object *mouseLook;
	TextureManager2 *texMan;
};

#endif //FATELESS_SHAPERENDERER_H
