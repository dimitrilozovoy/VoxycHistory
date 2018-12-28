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
#include "SkeletalRenderer.h"
#include "DDLUtils.hpp"
#include <vector>
#include <map>
#include "platform.h"

void SkeletalRenderer::init(ShadowMap *shadowMap, bool useShadowMap, Object *mouseLook)
{
	this->mouseLook = mouseLook;

    loadVertices();

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
	// Generate VAO
	glGenVertexArrays(1, (GLuint *)&vao);
	checkGLError("glGenVertexArrays");
#endif
    
	// Load shaders
	const int len = 10000;

	char vertexShaderStr[len];
	char fragmentShaderStr[len];

	// Compile regular program

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX || defined PLATFORM_OPENVR
	if (useShadowMap)
	{
		snprintf(vertexShaderStr, len, "#version 330 core\n%s", vertexShaderCodeDesktopUseShadowMap);
		snprintf(fragmentShaderStr, len, "#version 330 core\n%s", fragmentShaderCodeDesktopUseShadowMap);
	}
	else
	{
		snprintf(vertexShaderStr, len, "#version 330 core\n%s", vertexShaderCodeDesktop);
		snprintf(fragmentShaderStr, len, "#version 330 core\n%s", fragmentShaderCodeDesktop);
	}
#endif
#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
	snprintf(vertexShaderStr, len, "%s", vertexShaderCodeES20);
	snprintf(fragmentShaderStr, len, "%s", fragmentShaderCodeES20);
#endif

	programMain = loadProgram(vertexShaderStr, fragmentShaderStr, false);

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
	// Compile shadow map program
	snprintf(vertexShaderStr, len, "#version 330 core\n%s", shadowMap->vertexShaderCodeShadowMapDesktop);
	snprintf(fragmentShaderStr, len, "#version 330 core\n%s", shadowMap->fragmentShaderCodeShadowMapDesktop);

	programShadowMap = loadProgram(vertexShaderStr, fragmentShaderStr, true);
#endif
}

void SkeletalRenderer::draw(int eye, std::map<std::string, Object*> objects, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap)
{
	frameDump = "";

    for(const auto &pair: objects)
    {
        Object *obj = pair.second;
  
        if (obj != nullptr && obj->visible && obj->type == OBJTYPE_MODEL)
        {
//            drawModel(obj, camera, toShadowMap, useShadowMap, shadowMap);
        }
    }
}

void SkeletalRenderer::drawModel(Object *object, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap)
{
    Model2 *model = object->model;

    if (model == nullptr)
        return;

    for (int m = 0; m < model->meshes.size(); m++)
    {
        Mesh *mesh = model->meshes[m];
        drawMesh(object, model, mesh, camera, toShadowMap, useShadowMap, shadowMap);
    }
}

void SkeletalRenderer::drawMesh(Object *object, Model2 *model, Mesh *mesh, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap)
{
    int numCoords = 0;
    int stride = 9 * sizeof(float);

    float width = PLAT_GetWindowWidth();
    float height = PLAT_GetWindowHeight();
#ifdef PLATFORM_OPENVR
	float aspect = 1.0;
#else
	float aspect = width / height;
#endif

//	glPolygonOffset(4.0f, 20.0f);
	
	glEnable(GL_CULL_FACE);
//	glDisable(GL_CULL_FACE);
    checkGLError("glEnable");
    glFrontFace(GL_CCW);
    checkGLError("glFrontFace");

	if (toShadowMap)
	{
		glCullFace(GL_BACK);
//		glCullFace(GL_FRONT);
		checkGLError("glCullFace");
	}
	else
	{
		glCullFace(GL_BACK);
		checkGLError("glCullFace");
	}

	// Disable depth test for the player UFO canopy so we can see through it 
/*	if (object->name == "player" && mesh->index == 1)
		glDisable(GL_DEPTH_TEST);
	else
		glEnable(GL_DEPTH_TEST);*/

	// Add program to OpenGL environment
	int curProgram = -1;

	if (toShadowMap)
		curProgram = programShadowMap;
	else
		curProgram = programMain;

	glUseProgram(curProgram);
	checkGLError("ModelRenderer glUseProgram");

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
	// Bind the VAO
	glBindVertexArray(vao);
	checkGLError("glBindVertexArray");
#endif
    
	// Bind the VBOs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexVBO);
	checkGLError("glBindBuffer");

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    checkGLError("glBindBuffer");

	// Set the matrices
	glm::mat4 mvMatrix;
	glm::mat4 projectionMatrix;

	glm::mat4 cameraRotate;
	glm::mat4 cameraTranslate;
	glm::mat4 rotate;
	glm::mat4 scale;
	
	glm::mat4 scaleToNDC;
	
#ifdef PLATFORM_OPENVR
	scaleToNDC = glm::scale(glm::mat4(), glm::vec3(VRSCALE, VRSCALE, VRSCALE));
#else
/*    // HACK: Fixing cut-off weapon on Android
    if (object->name == "weapon") {
        scaleToNDC = glm::scale(glm::mat4(), glm::vec3(0.01, 0.01, 0.01));
    }
    else {*/
        scaleToNDC = glm::scale(glm::mat4(), glm::vec3(NDC_SCALE, NDC_SCALE, NDC_SCALE));
//    }
#endif

	// Calculate scale of model based on min and max
	float scaleX = 1.0 / (model->maxX - model->minX) * model->proportionScale.x;
	float scaleY = 1.0 / (model->maxY - model->minY) * model->proportionScale.y;
	float scaleZ = 1.0 / (model->maxZ - model->minZ) * model->proportionScale.z;

	scale = glm::scale(glm::mat4(), glm::vec3(scaleX * object->scale.x, scaleY * object->scale.y, scaleZ * object->scale.z)); // Normalization scale

	// Rotate
	if (object->secondaryYawMesh != -1 && mesh->index == object->secondaryYawMesh)
		rotate = glm::rotate(glm::mat4(), glm::radians(-object->secondaryYaw - model->yaw), glm::vec3(0, 1, 0)) // Model yaw
		* glm::rotate(glm::mat4(), glm::radians(-object->pitch - model->pitch), glm::vec3(1, 0, 0)); // Model pitch
	else if (object->alwaysFacePlayer)
		rotate = glm::rotate(glm::mat4(), glm::radians(-camera->yaw - model->yaw), glm::vec3(0, 1, 0)) // Model yaw
		* glm::rotate(glm::mat4(), glm::radians(camera->pitch - model->pitch), glm::vec3(1, 0, 0)); // Model pitch
	else
		rotate = glm::rotate(glm::mat4(), glm::radians(-object->yaw - model->yaw), glm::vec3(0, 1, 0)) // Model yaw
		* glm::rotate(glm::mat4(), glm::radians(-object->pitch - model->pitch), glm::vec3(1, 0, 0)); // Model pitch

	cameraRotate = glm::rotate(glm::mat4(), glm::radians(camera->roll), glm::vec3(0, 0, 1)) // Camera roll
	* glm::rotate(glm::mat4(), -glm::radians(camera->pitch), glm::vec3(1, 0, 0)) // Camera pitch
	* glm::rotate(glm::mat4(), glm::radians(camera->yaw), glm::vec3(0, 1, 0)); // Camera yaw

	// Translate
	cameraTranslate = glm::translate(glm::mat4(), glm::vec3(-camera->position.x, -camera->position.y, -camera->position.z)); // Camera translate

    mvMatrix =
            scaleToNDC
            * cameraRotate
            * cameraTranslate
            * glm::translate(glm::mat4(), glm::vec3(object->position.x, object->position.y,
                                                    object->position.z)) // World translate
            * rotate
            * scale;
//		* glm::scale(glm::mat4(), object->scale); // Scale

	glm::mat4 projMatrix;
	glm::mat4 depthMVP;

	if (toShadowMap)
	{
		glm::mat4 depthProjectionMatrix = shadowMap->getProjectionMatrix();

		setMatrix(curProgram, "mvMatrix", mvMatrix);

		projMatrix = depthProjectionMatrix;
		setMatrix(curProgram, "projectionMatrix", projMatrix);

		depthMVP = projMatrix * mvMatrix;
	}
	else
	{
		setMatrix(curProgram, "mvMatrix", mvMatrix);

#ifdef PLATFORM_OPENVR
		projMatrix = glm::make_mat4((const GLfloat *)g_projectionMatrix.get()) * glm::make_mat4((const GLfloat *)g_poseEyeMatrix.get());
//		projMatrix = glm::perspective(VIEW_FOV, aspect, 0.001f, 100.0f) * glm::make_mat4((const GLfloat *)g_poseEyeMatrix.get());
//		projMatrix = glm::make_mat4((const GLfloat *)g_poseEyeProjectionMatrix.get());
#else
		projMatrix = glm::perspective(VIEW_FOV, aspect, 0.001f, 1000.0f);
#endif

		setMatrix(curProgram, "projectionMatrix", projMatrix);
	}

	if (toShadowMap)
	{
		// Save the shadow MVP with the object
		object->DepthMVP = depthMVP;
	}

	if (useShadowMap)
	{
		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		glm::mat4 depthBiasMVP = biasMatrix * object->DepthMVP;

		setMatrix(curProgram, "DepthBiasMVP", depthBiasMVP);
	}

	if (!toShadowMap)
	{
		// Set variables
		setUniform4f(curProgram, "vColor", object->color.x * mesh->color.x, object->color.y * mesh->color.y, object->color.z * mesh->color.z, object->color.w * mesh->color.w);
		setUniform4f(curProgram, "globalColor", globalColor.x, globalColor.y, globalColor.z, globalColor.w);

		// Set texture
		if (object->glTexID != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			checkGLError("glActiveTexture");
			glBindTexture(GL_TEXTURE_2D, object->glTexID);
			checkGLError("glBindTexture");

			// Set texture unit number
			int uTexture = glGetUniformLocation(curProgram, "uTexture");
			checkGLError("glGetUniformLocation");
			glUniform1i(uTexture, 0);
			checkGLError("glUniform1i");

			setUniform1f(curProgram, "useTexture", 1.0);
		}
		else
		{
			setUniform1f(curProgram, "useTexture", 0.0);
		}

		if (useShadowMap)
		{
//			setUniform1f(curProgram, "useShadowMap", 1.0);

			// Set shadow texture
			glActiveTexture(GL_TEXTURE1);
			checkGLError("glActiveTexture");

			int uShadowMap = glGetUniformLocation(curProgram, "shadowMap");
			checkGLError("glGetUniformLocation");

			int shadowGLTexID = shadowMap->getRenderedTexture();

			glBindTexture(GL_TEXTURE_2D, shadowGLTexID);
			checkGLError("glBindTexture");
			glUniform1i(uShadowMap, 1);
			checkGLError("glUniform1i");
		}
		else
		{
//			setUniform1f(curProgram, "useShadowMap", 0.0);
		}
	}

	// Set fade distnace
	if (!toShadowMap)
	{
		setUniform1f(curProgram, "fadeNear", object->fadeNearDistance * NDC_SCALE);
		setUniform1f(curProgram, "fadeFar", object->fadeFarDistance * NDC_SCALE);
	}
	
	// Set attributes
    setVertexAttrib(curProgram, "vPosition", 4, GL_FLOAT, false, stride, 0);
	if (!toShadowMap)
		setVertexAttrib(curProgram, "vTexCoords", 2, GL_FLOAT, false, stride, 4);

//	setVertexAttrib("vNormal", 2, GL_FLOAT, false, stride, 6);

    // Draw
	glDrawElements(GL_TRIANGLES, mesh->numPolys * 3, GL_UNSIGNED_INT, 0);
	checkGLError("glDrawElements");

    // Reset
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    glBindVertexArray(0);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

}

void SkeletalRenderer::loadVertices()
{
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Generate VAO
    glGenVertexArrays(1, (GLuint *)&vao);
    checkGLError("glGenVertexArrays");
    glBindVertexArray(vao);
    checkGLError("glBindVertexArray");
#endif
    
    // Generate VBOs
    glGenBuffers(NUM_SHAPES, (GLuint *)&vbos);
    checkGLError("glGenBuffers");

    //
    // SHAPE_QUAD
    //

    glBindBuffer(GL_ARRAY_BUFFER, vbos[SHAPE_QUAD]);
    checkGLError("glBindBuffer");
	
	float quad[36];
	
	//   ______
	// |\\5   4|
	// |0\\    |
	// |  \\   |
	// |   \\  |
	// |    \\3|
	// |1__2_\\|
	
	// Triangle 1
	
	// Vertex 0
	quad[0] = -1.0f;
	quad[1] = 1.0f;
	quad[2] = 0.0f;
	quad[3] = 1.0f;
	
	quad[4] = 0.0f;
	quad[5] = 0.0f;
	
	// Vertex 1
	quad[6] = -1.0f;
	quad[7] = -1.0f;
	quad[8] = 0.0f;
	quad[9] = 1.0f;
	
	quad[10] = 0.0f;
	quad[11] = 1.0f;

	// Vertex 2
	quad[12] = 1.0f;
	quad[13] = -1.0f;
	quad[14] = 0.0f;
	quad[15] = 1.0f;
		
	quad[16] = 1.0f;
	quad[17] = 1.0f;

	// Triangle 2
	
	// Vertex 3
	quad[18] = 1.0f;
	quad[19] = -1.0f;
	quad[20] = 0.0f;
	quad[21] = 1.0f;
		
	quad[22] = 1.0f;
	quad[23] = 1.0f;

	// Vertex 4
	quad[24] = 1.0f;
	quad[25] = 1.0f;
	quad[26] = 0.0f;
	quad[27] = 1.0f;
	
	quad[28] = 1.0f;
	quad[29] = 0.0f;

	// Vertex 5
	quad[30] = -1.0f;
	quad[31] = 1.0f;
	quad[32] = 0.0f;
	quad[33] = 1.0f;
	
	quad[34] = 0.0f;
	quad[35] = 0.0f;

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36, quad, GL_STATIC_DRAW);
    checkGLError("glBufferData");

    //
    // SHAPE_BLOCK
    //

    glBindBuffer(GL_ARRAY_BUFFER, vbos[SHAPE_BLOCK]);
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
    block[5] = 0.0f;

    // Vertex 1
    block[6] = -1.0f;
    block[7] = -1.0f;
    block[8] = 1.0f;
    block[9] = 1.0f;

    block[10] = 0.0f;
    block[11] = 1.0f;

    // Vertex 2
    block[12] = 1.0f;
    block[13] = -1.0f;
    block[14] = 1.0f;
    block[15] = 1.0f;

    block[16] = 1.0f;
    block[17] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[18] = 1.0f;
    block[19] = -1.0f;
    block[20] = 1.0f;
    block[21] = 1.0f;

    block[22] = 1.0f;
    block[23] = 1.0f;

    // Vertex 4
    block[24] = 1.0f;
    block[25] = 1.0f;
    block[26] = 1.0f;
    block[27] = 1.0f;

    block[28] = 1.0f;
    block[29] = 0.0f;

    // Vertex 5
    block[30] = -1.0f;
    block[31] = 1.0f;
    block[32] = 1.0f;
    block[33] = 1.0f;

    block[34] = 0.0f;
    block[35] = 0.0f;

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

    block[o + 4] = 0.0f;
    block[o + 5] = 0.0f;

    // Vertex 1
    block[o + 6] = 1.0f;
    block[o + 7] = -1.0f;
    block[o + 8] = 1.0f;
    block[o + 9] = 1.0f;

    block[o + 10] = 0.0f;
    block[o + 11] = 1.0f;

    // Vertex 2
    block[o + 12] = 1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = -1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 1.0f;
    block[o + 17] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[o + 18] = 1.0f;
    block[o + 19] = -1.0f;
    block[o + 20] = -1.0f;
    block[o + 21] = 1.0f;

    block[o + 22] = 1.0f;
    block[o + 23] = 1.0f;

    // Vertex 4
    block[o + 24] = 1.0f;
    block[o + 25] = 1.0f;
    block[o + 26] = -1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 0.0f;

    // Vertex 5
    block[o + 30] = 1.0f;
    block[o + 31] = 1.0f;
    block[o + 32] = 1.0f;
    block[o + 33] = 1.0f;

    block[o + 34] = 0.0f;
    block[o + 35] = 0.0f;

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
    block[o + 5] = 0.0f;

    // Vertex 1
    block[o + 6] = 1.0f;
    block[o + 7] = -1.0f;
    block[o + 8] = -1.0f;
    block[o + 9] = 1.0f;

    block[o + 10] = 0.0f;
    block[o + 11] = 1.0f;

    // Vertex 2
    block[o + 12] = -1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = -1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 1.0f;
    block[o + 17] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[o + 18] = -1.0f;
    block[o + 19] = -1.0f;
    block[o + 20] = -1.0f;
    block[o + 21] = 1.0f;

    block[o + 22] = 1.0f;
    block[o + 23] = 1.0f;

    // Vertex 4
    block[o + 24] = -1.0f;
    block[o + 25] = 1.0f;
    block[o + 26] = -1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 0.0f;

    // Vertex 5
    block[o + 30] = 1.0f;
    block[o + 31] = 1.0f;
    block[o + 32] = -1.0f;
    block[o + 33] = 1.0f;

    block[o + 34] = 0.0f;
    block[o + 35] = 0.0f;

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

    block[o + 4] = 0.0f;
    block[o + 5] = 0.0f;

    // Vertex 1
    block[o + 6] = -1.0f;
    block[o + 7] = -1.0f;
    block[o + 8] = -1.0f;
    block[o + 9] = 1.0f;

    block[o + 10] = 0.0f;
    block[o + 11] = 1.0f;

    // Vertex 2
    block[o + 12] = -1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = 1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 1.0f;
    block[o + 17] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[o + 18] = -1.0f;
    block[o + 19] = -1.0f;
    block[o + 20] = 1.0f;
    block[o + 21] = 1.0f;

    block[o + 22] = 1.0f;
    block[o + 23] = 1.0f;

    // Vertex 4
    block[o + 24] = -1.0f;
    block[o + 25] = 1.0f;
    block[o + 26] = 1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 0.0f;

    // Vertex 5
    block[o + 30] = -1.0f;
    block[o + 31] = 1.0f;
    block[o + 32] = -1.0f;
    block[o + 33] = 1.0f;

    block[o + 34] = 0.0f;
    block[o + 35] = 0.0f;

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
    block[o + 5] = 0.0f;

    // Vertex 1
    block[o + 6] = -1.0f;
    block[o + 7] = -1.0f;
    block[o + 8] = -1.0f;
    block[o + 9] = 1.0f;

    block[o + 10] = 0.0f;
    block[o + 11] = 1.0f;

    // Vertex 2
    block[o + 12] = 1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = -1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 1.0f;
    block[o + 17] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[o + 18] = 1.0f;
    block[o + 19] = -1.0f;
    block[o + 20] = -1.0f;
    block[o + 21] = 1.0f;

    block[o + 22] = 1.0f;
    block[o + 23] = 1.0f;

    // Vertex 4
    block[o + 24] = 1.0f;
    block[o + 25] = -1.0f;
    block[o + 26] = 1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 0.0f;

    // Vertex 5
    block[o + 30] = -1.0f;
    block[o + 31] = -1.0f;
    block[o + 32] = 1.0f;
    block[o + 33] = 1.0f;

    block[o + 34] = 0.0f;
    block[o + 35] = 0.0f;

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
    block[o + 5] = 0.0f;

    // Vertex 1
    block[o + 6] = -1.0f;
    block[o + 7] = 1.0f;
    block[o + 8] = 1.0f;
    block[o + 9] = 1.0f;

    block[o + 10] = 0.0f;
    block[o + 11] = 1.0f;

    // Vertex 2
    block[o + 12] = 1.0f;
    block[o + 13] = 1.0f;
    block[o + 14] = 1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 1.0f;
    block[o + 17] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[o + 18] = 1.0f;
    block[o + 19] = 1.0f;
    block[o + 20] = 1.0f;
    block[o + 21] = 1.0f;

    block[o + 22] = 1.0f;
    block[o + 23] = 1.0f;

    // Vertex 4
    block[o + 24] = 1.0f;
    block[o + 25] = 1.0f;
    block[o + 26] = -1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 0.0f;

    // Vertex 5
    block[o + 30] = -1.0f;
    block[o + 31] = 1.0f;
    block[o + 32] = -1.0f;
    block[o + 33] = 1.0f;

    block[o + 34] = 0.0f;
    block[o + 35] = 0.0f;

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 6, block, GL_STATIC_DRAW);
    checkGLError("glBufferData");
}

void SkeletalRenderer::freeVertices()
{
}
