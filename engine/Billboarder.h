//
// Created by dimitri on 11/25/2018.
//

#ifndef VOXYC_BILLBOARDER_H
#define VOXYC_BILLBOARDER_H

#include "Renderer.h"
#include "Mesh.h"
#include "Framebuffer.h"
#include "Model2.h"
#include "Object.h"

class Billboarder: Renderer
{
public:
    void init();
    void refresh(std::map<std::string, Object*> objects);
    void process(Model2 *model, int glTexID);
    int draw(Model2 *model, Mesh *mesh, float yaw, int glTexID);

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
    "uniform mat4 mvMatrix; " \
    "uniform mat4 projectionMatrix; " \

                    "varying lowp float distToCamera; " \

                    "void main() {" \
    "  gl_Position = projectionMatrix * mvMatrix * vPosition; "
                    "  vTexCoordsOut = vTexCoords * vTexSpan; " \
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
	"varying lowp vec2 vTexCoordsOut; " \
	"varying lowp vec4 posOut; " \
	"uniform lowp float useTexture; " \

                    "varying lowp float distToCamera; " \

                    "void main() {" \

                    "   lowp vec4 f = texture2D(uTexture, vTexCoordsOut.st); " \
		"   if (f.a == 0.0) " \
		"       discard; " \

                    "	lowp float visibility = 1.0; " \
		"   lowp float alpha = 1.0; " \

                    "   if (useTexture == 1.0)" \
		"   {" \
		"      gl_FragColor = texture2D(uTexture, vTexCoordsOut.st) * vColor; " \
		"   }" \
		"   else" \
		"   {" \
		"      gl_FragColor = vColor; " \
		"   }" \
		"}\n";

    int programMain = -1;
    Framebuffer fb;
    int program = -1;
    int vao = -1;
};

#endif //VOXYC_BILLBOARDER_H
