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
#include "ShapeRenderer.h"
#include "DDLUtils.hpp"
#include <vector>
#include <map>
#include <string>
#include "platform.h"

/*
========================================
init
========================================
*/

void ShapeRenderer::init(ShadowMap *shadowMap, bool useShadowMap, Object *mouseLook, TextureManager2 *texMan)
{
	this->mouseLook = mouseLook;
	this->texMan = texMan;

    loadVertices();

	const int len = 10000;

	char vertexShaderStr[len];
	char fragmentShaderStr[len];
	char vertexShaderStrDyn[len];
	char fragmentShaderStrDyn[len];

	// Compile regular program

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
	if (useShadowMap)
	{
		snprintf(vertexShaderStrDyn, len, "#version 330 core\n%s", vertexShaderCodeDesktopUseShadowMap);
		snprintf(fragmentShaderStrDyn, len, "#version 330 core\n%s", fragmentShaderCodeDesktopUseShadowMap);
        snprintf(vertexShaderStr, len, "#version 330 core\n%s", vertexShaderCodeDesktopUseShadowMap);
        snprintf(fragmentShaderStr, len, "#version 330 core\n%s", fragmentShaderCodeDesktopUseShadowMap);
	}
	else
	{
		snprintf(vertexShaderStrDyn, len, "#version 330 core\n%s", vertexShaderCodeDesktop);
		snprintf(fragmentShaderStrDyn, len, "#version 330 core\n%s", fragmentShaderCodeDesktop);
		snprintf(vertexShaderStr, len, "#version 330 core\n%s", vertexShaderCodeDesktop);
		snprintf(fragmentShaderStr, len, "#version 330 core\n%s", fragmentShaderCodeDesktop);
	}
#endif
#if defined PLATFORM_OPENVR
	snprintf(vertexShaderStr, len, "#version 330 core\n%s", vertexShaderCodeOpenVR);
	snprintf(fragmentShaderStr, len, "#version 330 core\n%s", fragmentShaderCodeOpenVR);
#endif
#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
	if (useShadowMap)
	{
		snprintf(vertexShaderStr, len, "%s", vertexShaderCodeES20UseShadowMap);
		snprintf(fragmentShaderStr, len, "%s", fragmentShaderCodeES20UseShadowMap);
	}
	else
	{
		    snprintf(vertexShaderStrDyn, len, "%s", vertexShaderCodeES20DynamicLights);
		    snprintf(fragmentShaderStrDyn, len, "%s", fragmentShaderCodeES20DynamicLights);
		    snprintf(vertexShaderStr, len, "%s", vertexShaderCodeES20);
		    snprintf(fragmentShaderStr, len, "%s", fragmentShaderCodeES20);
    }
#endif

	programReg = loadProgram(vertexShaderStr, fragmentShaderStr, false);
    programDyn = loadProgram(vertexShaderStrDyn, fragmentShaderStrDyn, false);
    programLow = programMain;
	programMed = programDyn;
	programHi = programDyn;

	if (useShadowMap) {
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX || defined PLATFORM_OPENVR
        // Compile shadow map program
        snprintf(vertexShaderStr, len, "#version 330 core\n%s", shadowMap->vertexShaderCodeShadowMapDesktop);
        snprintf(fragmentShaderStr, len, "#version 330 core\n%s", shadowMap->fragmentShaderCodeShadowMapDesktop);

        programShadowMap = loadProgram(vertexShaderStr, fragmentShaderStr, true);
#endif
#if defined PLATFORM_ANDROID
		// Compile shadow map program
		snprintf(vertexShaderStr, len, "%s", shadowMap->vertexShaderCodeShadowMapES20);
		snprintf(fragmentShaderStr, len, "%s", shadowMap->fragmentShaderCodeShadowMapES20);

		programShadowMap = loadProgram(vertexShaderStr, fragmentShaderStr, true);
#endif
	}
}

/*
========================================
draw
========================================
*/

void ShapeRenderer::draw(int eye, std::map<std::string, Object*> objects, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap,std::map<std::string, DynamicLight> dynamicLights)
{
	// Choose program
	switch(g_common.graphics)
	{
		case 0:
			programMain = programReg;
			break;
#ifdef PLATFORM_IOS
        case 1:
            programMain = programReg;
            break;
#else
        case 1:
            programMain = programDyn;
            break;
#endif
		case 2:
			programMain = programDyn;
			break;
	}
	
	frameDump = "";
	
	if (dumpFrame)
	    frameDump += "===== SHAPE RENDERER ====\n";
	
	// Two passes, first for regular 3D shapes, second one for quads/sprites with transparency

	// Regular 3D shapes
    for(const auto &pair: objects)
    {
        Object *obj = pair.second;

		if (obj != nullptr)
		{
			float distance = camera->distanceTo(obj);

			if (obj->visible && obj->type == OBJTYPE_SHAPE && (obj->shapeType != SHAPE_QUAD && obj->shapeType != SHAPE_SPRITE) && (distance <= drawDistance))
			{
                if (!obj->mainOfBatch && !obj->partOfBatch)
				{
				    drawShape(obj, camera, toShadowMap, useShadowMap, shadowMap, dynamicLights);
				}
				
				if (obj->mainOfBatch)
				{
					drawMesh(obj, obj->shape, obj->batchedMesh, camera, toShadowMap, useShadowMap, shadowMap, nullptr, dynamicLights);
				}
			}
		}
    }

	// Quads/Sprites
	for (const auto &pair : objects)
	{
		Object *obj = pair.second;

		if (obj != nullptr)
		{
			float distance = camera->distanceTo(obj);

			if (obj->visible && obj->type == OBJTYPE_SHAPE && (obj->shapeType == SHAPE_QUAD || obj->shapeType == SHAPE_SPRITE) && (distance <= drawDistance || obj->isPartOfVehicle))
				drawShape(obj, camera, toShadowMap, useShadowMap, shadowMap, dynamicLights);

// HACK: Sprite renderer is broken on Windows and iOS; draw spirtes separately as shapes
#if defined PLATFORM_WINDOWS || defined PLATFORM_IOS || defined PLATFORM_OSX
			if (obj->visible && obj->type == OBJTYPE_SPRITE && (distance <= drawDistance || obj->isPartOfVehicle))
				drawShape(obj, camera, toShadowMap, useShadowMap, shadowMap, dynamicLights);
#endif
		}
	}
	
	dumpFrame = false;
}

/*
========================================
drawShape
========================================
*/

void ShapeRenderer::drawShape(Object *object, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap, std::map<std::string, DynamicLight> dynamicLights)
{
	// FrameDump stuff
	if (dumpFrame)
	{
		frameDump += "Shape\n";
		frameDump += "Obj: " + object->toString() + "\n";
	}
	
    if (object->shapeType == SHAPE_SPRITE || object->shapeType == SHAPE_QUAD || object->shapeType == SHAPE_BLOCK)
	{
		// Simple shape
		glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        checkGLError("glEnable");
        
		drawMesh(object, nullptr, nullptr, camera, toShadowMap, useShadowMap, shadowMap, nullptr, dynamicLights);
	}
	else
	{
		// Complex shape
		glEnable(GL_DEPTH_TEST);
		checkGLError("glEnable");
		glEnable(GL_CULL_FACE);
		checkGLError("glEnable");
		glCullFace(GL_BACK);
		checkGLError("glCullFace");

		Shape *shape = object->shape;

		if (shape == nullptr)
			return;

        Voxels *voxels = shape->voxels;

		if (shape->needsRebuild)
		{
            if (voxels == nullptr)
                return;
				
            shape->rebuild(texMan);
			
			if (dumpFrame)
	        {
		        frameDump += "rebuild\n";
				if (shape->meshes.size() == 0)
					frameDump += "meshes size 0\n";
	        }
			
            return;
		}
		
		bool draw = true;

        if (object->shape->getState() == SHAPE_BUILDING)
		{
			draw = false;
			
			// Shape is building; check to see if it built
			if (object->shape->type == SHAPE_VOXELS && object->shape->voxels != nullptr)
			{
				Voxels *vox = object->shape->voxels;
				
				if (vox->build_done)
				{
					object->shape->meshes = voxels->getMeshes();
		            object->shape->state = SHAPE_READY;
					draw = true;
				}
			}
		}

        if (!draw)
			return;

		for (int m = 0; m < shape->meshes.size(); m++)
		{
			if (dumpFrame)
	        {
		        frameDump += "drawMesh\n";
	        }
			
			Mesh *mesh = shape->meshes[m];
			drawMesh(object, shape, mesh, camera, toShadowMap, useShadowMap, shadowMap, voxels->getVoxelTextures(), dynamicLights);
		}
	}
}

/*
========================================
drawMesh
========================================
*/

void ShapeRenderer::drawMesh(Object *object, Shape *shape, Mesh *mesh, Object *camera, bool toShadowMap, bool useShadowMap, ShadowMap *shadowMap, std::map<int, std::string> *voxelTextures, std::map<std::string, DynamicLight> dynamicLights)
{
	// FrameDump stuff
	if (dumpFrame)
	{
		frameDump += "Mesh\n";
		frameDump += "Obj: " + object->toString() + "\n";
	}

	int numCoords = 0;
	int floatsPerCoord = 0;
	
	if (mesh != nullptr)
		floatsPerCoord = mesh->floatsPerCoord;

	if (object->shapeType == SHAPE_SPRITE)
	{
		numCoords = 12 * 6;
		floatsPerCoord = 12;
	}
	else if (object->shapeType == SHAPE_QUAD)
	{
		numCoords = 12 * 6;
		floatsPerCoord = 12;
	}
	else if (object->shapeType == SHAPE_BLOCK)
	{
		numCoords = 12 * 6 * 6;
		floatsPerCoord = 12;
	}
	else if (object->shapeType == SHAPE_CUSTOM)
	{
//		Log("SHAPE_CUSTOM " + object->name);
//		Log((void *)mesh);
//		std::string s = mesh->numCoords;
//		Log(s);
//		Log((int)	mesh->vbo);
		numCoords = mesh->numCoords;
	}

	int stride = floatsPerCoord * sizeof(float);
	int vbo = -1;

	float width = PLAT_GetWindowWidth();
	float height = PLAT_GetWindowHeight();
#ifdef PLATFORM_OPENVR
	float aspect = 1.0;
#else
	float aspect = width / height;
#endif

//	glPolygonOffset(4.0f, 20.0f);

//    glEnable(GL_CULL_FACE);
//	glDisable(GL_CULL_FACE);
	checkGLError("glEnable");
	glFrontFace(GL_CCW);
	checkGLError("glFrontFace");

	if (toShadowMap)
	{
		glCullFace(GL_BACK);
//		glCullFace(GL_FRONT);
		checkGLError("glCullFace");
		glEnable(GL_DEPTH_TEST);
        checkGLError("glEnable");
	}
	else
	{
		glCullFace(GL_BACK);
		checkGLError("glCullFace");

		glEnable(GL_BLEND);
		checkGLError("ShapeRenderer glEnable");
#ifndef PLATFORM_ANDROID
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		checkGLError("ShapeRenderer glBlendFunc");
#endif
	}

	// Add program to OpenGL environment
	int curProgram = -1;

	if (toShadowMap)
		curProgram = programShadowMap;
	else
		curProgram = programMain;

	glUseProgram(curProgram);
	checkGLError("glUseProgram");

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
	// Bind the VAO
	glBindVertexArray(vao);
	checkGLError("glBindVertexArray");
#endif

	// Bind the VBO
	if (object->shapeType == SHAPE_SPRITE)
	{
		vbo = vbos[SHAPE_QUAD];
	}
	else if (object->shapeType == SHAPE_QUAD)
	{
		vbo = vbos[SHAPE_QUAD];
	}
	else if (object->shapeType == SHAPE_BLOCK)
	{
		vbo = vbos[SHAPE_BLOCK];
	}
	else
	{
		vbo = mesh->vbo;
	}
	
	if (vbo == -1)
		return;
	
//	char c[1024];
//	sprintf(c, "%s %d", object->name.c_str(), numCoords);
//	std::string s = c;
//	Log(s);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	checkGLError("glBindBuffer");

	// Set the matrices
	glm::mat4 mvMatrix;
	glm::mat4 projectionMatrix;

	glm::mat4 cameraRotate;
	glm::mat4 cameraTranslate;
	glm::mat4 rotate;
	
	glm::mat4 scaleToNDC;

#ifdef PLATFORM_OPENVR
	scaleToNDC = glm::scale(glm::mat4(), glm::vec3(VRSCALE, VRSCALE, VRSCALE));
#else
	scaleToNDC = glm::scale(glm::mat4(), glm::vec3(NDC_SCALE, NDC_SCALE, NDC_SCALE));
#endif

	if (object->alwaysFacePlayer)
		rotate = glm::rotate(glm::mat4(), glm::radians(-camera->yaw), glm::vec3(0, 1, 0)) // Model yaw
			* glm::rotate(glm::mat4(), glm::radians(camera->pitch), glm::vec3(1, 0, 0)); // Model pitch
	else
		rotate = glm::rotate(glm::mat4(), glm::radians(-object->yaw), glm::vec3(0, 1, 0)) // Model yaw
			* glm::rotate(glm::mat4(), glm::radians(-object->pitch), glm::vec3(1, 0, 0)); // Model pitch

	if (object->isPartOfVehicle)
		cameraRotate = glm::rotate(glm::mat4(), glm::radians(mouseLook->roll), glm::vec3(0, 0, 1)) // Camera roll
		* glm::rotate(glm::mat4(), -glm::radians(mouseLook->pitch), glm::vec3(1, 0, 0)) // Camera pitch
		* glm::rotate(glm::mat4(), glm::radians(mouseLook->yaw), glm::vec3(0, 1, 0)); // Camera yaw
	else
		cameraRotate = glm::rotate(glm::mat4(), glm::radians(camera->roll), glm::vec3(0, 0, 1)) // Camera roll
		* glm::rotate(glm::mat4(), -glm::radians(camera->pitch), glm::vec3(1, 0, 0)) // Camera pitch
		* glm::rotate(glm::mat4(), glm::radians(camera->yaw), glm::vec3(0, 1, 0)); // Camera yaw
		
	if (object->isPartOfVehicle)
		cameraTranslate = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, 0.0)); // Camera translate
	else
		cameraTranslate = glm::translate(glm::mat4(), glm::vec3(-camera->position.x, -camera->position.y, -camera->position.z)); // Camera translate

#ifdef PLATFORM_OPENVR
	mvMatrix =
		glm::make_mat4((const GLfloat *)g_poseEyeMatrix.get())
	    * scaleToNDC
		* cameraRotate
		* cameraTranslate
		* glm::translate(glm::mat4(), glm::vec3(object->position.x, object->position.y, object->position.z)) // World translate
		* rotate
		* glm::scale(glm::mat4(), object->scale / glm::vec3(2.0, 2.0, 2.0)); // Scale
#else
	mvMatrix =
		scaleToNDC
		* cameraRotate
		* cameraTranslate
		* glm::translate(glm::mat4(), glm::vec3(object->position.x, object->position.y, object->position.z)) // World translate
		* rotate
		* glm::scale(glm::mat4(), object->scale / glm::vec3(2.0, 2.0, 2.0)); // Scale
#endif

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

#if defined PLATFORM_OPENVR
		projMatrix = glm::make_mat4((const GLfloat *)g_projectionMatrix.get());
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
		setUniform4f(curProgram, "vColor", object->color.x, object->color.y, object->color.z, object->color.w);
		setUniform4f(curProgram, "globalColor", globalColor.x, globalColor.y, globalColor.z, globalColor.w);
		setUniform4f(curProgram, "ambientLight", g_common.ambientr, g_common.ambientg, g_common.ambientb, 1.0);

		if (shape != nullptr && (object->mainOfBatch))
		{
			glActiveTexture(GL_TEXTURE0);
			checkGLError("glActiveTexture");

            glBindTexture(GL_TEXTURE_2D, mesh->glTexID);

            setUniform2f(curProgram, "vTexSpan", 1.0, 1.0);
            setUniform1f(curProgram, "useTexture", 1.0);
		}
		else if (shape != nullptr && (shape->type == SHAPE_VOXELS))
		{
			glActiveTexture(GL_TEXTURE0);
			checkGLError("glActiveTexture");

			Texture *tex = texMan->find(mesh->texture);
			glBindTexture(GL_TEXTURE_2D, tex->glTexID);
//			glBindTexture(GL_TEXTURE_2D, shadowMap->getRenderedTexture());

            setUniform2f(curProgram, "vTexSpan", tex->texSpanX, tex->texSpanY);
			setUniform1f(curProgram, "useTexture", 1.0);
		}
		else
		{
			// Set texture
			if (object->glTexID != -1 || object->mapTexture)
			{
				glActiveTexture(GL_TEXTURE0);
				checkGLError("glActiveTexture");

				glBindTexture(GL_TEXTURE_2D, object->glTexID);
//				glBindTexture(GL_TEXTURE_2D, shadowMap->getRenderedTexture());

				checkGLError("glBindTexture");

				// Set texture unit number
				int uTexture = glGetUniformLocation(curProgram, "uTexture");
				checkGLError("glGetUniformLocation");
				glUniform1i(uTexture, 0);
				checkGLError("glUniform1i");

                setUniform2f(curProgram, "vTexSpan", 1.0, 1.0);
				setUniform1f(curProgram, "useTexture", 1.0);
			}
			else
			{
				setUniform1f(curProgram, "useTexture", 0.0);
			}
		}

		if (useShadowMap)
		{
			// Set shadow texture
			glActiveTexture(GL_TEXTURE1);
			checkGLError("glActiveTexture");

			int shadowGLTexID = shadowMap->getRenderedTexture();

			glBindTexture(GL_TEXTURE_2D, shadowGLTexID);
			checkGLError("glBindTexture");

			int uShadowMap = glGetUniformLocation(curProgram, "shadowMap");
			checkGLError("glGetUniformLocation");
			glUniform1i(uShadowMap, 1);
			checkGLError("glUniform1i");
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
	{
		setVertexAttrib(curProgram, "vTexCoords", 2, GL_FLOAT, false, stride, 4);
        if (g_common.doDynamicLights)
            setVertexAttrib(curProgram, "vNormal", 3, GL_FLOAT, false, stride, 6);
#ifdef DO_VERTEX_LIGHTS
		if (floatsPerCoord == 12)
			setVertexAttrib(curProgram, "vVertexLight", 3, GL_FLOAT, false, stride, 9);
#endif
	}
	
	// Dynamic lights
	if (g_common.doDynamicLights)
	{
		setDynamicLights(dynamicLights, object, curProgram, rotate);
	}
	
//  char cstr[1000];
//	sprintf(cstr, "numCoords %d", numCoords);
	
    // Draw
    glDrawArrays(GL_TRIANGLES, 0, numCoords / floatsPerCoord);
//	checkGLError(cstr);
    checkGLError("ShapeRenderer glDrawArrays");

    // Reset
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    glBindVertexArray(0);
#endif    
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLError("glBindTexture");
    glUseProgram(0);
    checkGLError("glUseProgram");
}

/*
========================================
loadVertices
========================================
*/

void ShapeRenderer::loadVertices()
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

	float quad[72];

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
	quad[5] = 1.0f;

	quad[6] = 0.0f;
	quad[7] = 0.0f;
	quad[8] = 1.0f;

    quad[9] = 1.0f;
    quad[10] = 1.0f;
    quad[11] = 1.0f;

	// Vertex 1
	quad[12] = -1.0f;
	quad[13] = -1.0f;
	quad[14] = 0.0f;
	quad[15] = 1.0f;

	quad[16] = 0.0f;
	quad[17] = 0.0f;

	quad[18] = 0.0f;
	quad[19] = 0.0f;
	quad[20] = 1.0f;

    quad[21] = 1.0f;
    quad[22] = 1.0f;
    quad[23] = 1.0f;

    // Vertex 2
	quad[24] = 1.0f;
	quad[25] = -1.0f;
	quad[26] = 0.0f;
	quad[27] = 1.0f;

	quad[28] = 1.0f;
	quad[29] = 0.0f;

	quad[30] = 0.0f;
	quad[31] = 0.0f;
	quad[32] = 1.0f;

    quad[33] = 1.0f;
    quad[34] = 1.0f;
    quad[35] = 1.0f;

    // Triangle 2

	// Vertex 3
	quad[36] = 1.0f;
	quad[37] = -1.0f;
	quad[38] = 0.0f;
	quad[39] = 1.0f;

	quad[40] = 1.0f;
	quad[41] = 0.0f;

	quad[42] = 0.0f;
	quad[43] = 0.0f;
	quad[44] = 1.0f;

    quad[45] = 1.0f;
    quad[46] = 1.0f;
    quad[47] = 1.0f;

    // Vertex 4
	quad[48] = 1.0f;
	quad[49] = 1.0f;
	quad[50] = 0.0f;
	quad[51] = 1.0f;

	quad[52] = 1.0f;
	quad[53] = 1.0f;

	quad[54] = 0.0f;
	quad[55] = 0.0f;
	quad[56] = 1.0f;

    quad[57] = 1.0f;
    quad[58] = 1.0f;
    quad[59] = 1.0f;

    // Vertex 5
	quad[60] = -1.0f;
	quad[61] = 1.0f;
	quad[62] = 0.0f;
	quad[63] = 1.0f;

	quad[64] = 0.0f;
	quad[65] = 1.0f;

	quad[66] = 0.0f;
	quad[67] = 0.0f;
	quad[68] = 1.0f;

    quad[69] = 1.0f;
    quad[70] = 1.0f;
    quad[71] = 1.0f;

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 72, quad, GL_STATIC_DRAW);
	checkGLError("glBufferData");

	//
	// SHAPE_BLOCK
	//

	glBindBuffer(GL_ARRAY_BUFFER, vbos[SHAPE_BLOCK]);
	checkGLError("glBindBuffer");

	float block[72 * 6];

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

    block[6] = 0.0f;
    block[7] = 0.0f;
    block[8] = 1.0f;

    block[9] = 1.0f;
    block[10] = 1.0f;
    block[11] = 1.0f;

    // Vertex 1
    block[12] = -1.0f;
    block[13] = -1.0f;
    block[14] = 1.0f;
    block[15] = 1.0f;

    block[16] = 0.0f;
    block[17] = 1.0f;

    block[18] = 0.0f;
    block[19] = 0.0f;
    block[20] = 1.0f;

    block[21] = 1.0f;
    block[22] = 1.0f;
    block[23] = 1.0f;

    // Vertex 2
    block[24] = 1.0f;
    block[25] = -1.0f;
    block[26] = 1.0f;
    block[27] = 1.0f;

    block[28] = 1.0f;
    block[29] = 1.0f;

    block[30] = 0.0f;
    block[31] = 0.0f;
    block[32] = 1.0f;

    block[33] = 1.0f;
    block[34] = 1.0f;
    block[35] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[36] = 1.0f;
    block[37] = -1.0f;
    block[38] = 1.0f;
    block[39] = 1.0f;

    block[40] = 1.0f;
    block[41] = 1.0f;

    block[42] = 0.0f;
    block[43] = 0.0f;
    block[44] = 1.0f;

    block[45] = 1.0f;
    block[46] = 1.0f;
    block[47] = 1.0f;

    // Vertex 4
    block[48] = 1.0f;
    block[49] = 1.0f;
    block[50] = 1.0f;
    block[51] = 1.0f;

    block[52] = 1.0f;
    block[53] = 0.0f;

    block[54] = 0.0f;
    block[55] = 0.0f;
    block[56] = 1.0f;

    block[57] = 1.0f;
    block[58] = 1.0f;
    block[59] = 1.0f;

    // Vertex 5
    block[60] = -1.0f;
    block[61] = 1.0f;
    block[62] = 1.0f;
    block[63] = 1.0f;

    block[64] = 0.0f;
    block[65] = 0.0f;

    block[66] = 0.0f;
    block[67] = 0.0f;
    block[68] = 1.0f;

    block[69] = 1.0f;
    block[70] = 1.0f;
    block[71] = 1.0f;

    // RIGHT SIDE

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1
    int o = 72;

    // Vertex 0
    block[o + 0] = 1.0f;
    block[o + 1] = 1.0f;
    block[o + 2] = 1.0f;
    block[o + 3] = 1.0f;

    block[o + 4] = 0.0f;
    block[o + 5] = 0.0f;

    block[o + 6] = 1.0f;
    block[o + 7] = 0.0f;
    block[o + 8] = 0.0f;

    block[o + 9] = 1.0f;
    block[o + 10] = 1.0f;
    block[o + 11] = 1.0f;

    // Vertex 1
    block[o + 12] = 1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = 1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 0.0f;
    block[o + 17] = 1.0f;

    block[o + 18] = 1.0f;
    block[o + 19] = 0.0f;
    block[o + 20] = 0.0f;

    block[o + 21] = 1.0f;
    block[o + 22] = 1.0f;
    block[o + 23] = 1.0f;

    // Vertex 2
    block[o + 24] = 1.0f;
    block[o + 25] = -1.0f;
    block[o + 26] = -1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 1.0f;

    block[o + 30] = 1.0f;
    block[o + 31] = 0.0f;
    block[o + 32] = 0.0f;

    block[o + 33] = 1.0f;
    block[o + 34] = 1.0f;
    block[o + 35] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[o + 36] = 1.0f;
    block[o + 37] = -1.0f;
    block[o + 38] = -1.0f;
    block[o + 39] = 1.0f;

    block[o + 40] = 1.0f;
    block[o + 41] = 1.0f;

    block[o + 42] = 1.0f;
    block[o + 43] = 0.0f;
    block[o + 44] = 0.0f;

    block[o + 45] = 1.0f;
    block[o + 46] = 1.0f;
    block[o + 47] = 1.0f;

    // Vertex 4
    block[o + 48] = 1.0f;
    block[o + 49] = 1.0f;
    block[o + 50] = -1.0f;
    block[o + 51] = 1.0f;

    block[o + 52] = 1.0f;
    block[o + 53] = 0.0f;

    block[o + 54] = 1.0f;
    block[o + 55] = 0.0f;
    block[o + 56] = 0.0f;

    block[o + 57] = 1.0f;
    block[o + 58] = 1.0f;
    block[o + 59] = 1.0f;

    // Vertex 5
    block[o + 60] = 1.0f;
    block[o + 61] = 1.0f;
    block[o + 62] = 1.0f;
    block[o + 63] = 1.0f;

    block[o + 64] = 0.0f;
    block[o + 65] = 0.0f;

    block[o + 66] = 1.0f;
    block[o + 67] = 0.0f;
    block[o + 68] = 0.0f;

    block[o + 69] = 1.0f;
    block[o + 70] = 1.0f;
    block[o + 71] = 1.0f;

    // BACK SIDE

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1
    o = 72 * 2;

    // Vertex 0
    block[o + 0] = 1.0f;
    block[o + 1] = 1.0f;
    block[o + 2] = -1.0f;
    block[o + 3] = 1.0f;

    block[o + 4] = 0.0f;
    block[o + 5] = 0.0f;

    block[o + 6] = 0.0f;
    block[o + 7] = 0.0f;
    block[o + 8] = -1.0f;

    block[o + 9] = 1.0f;
    block[o + 10] = 1.0f;
    block[o + 11] = 1.0f;

    // Vertex 1
    block[o + 12] = 1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = -1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 0.0f;
    block[o + 17] = 1.0f;

    block[o + 18] = 0.0f;
    block[o + 19] = 0.0f;
    block[o + 20] = -1.0f;

    block[o + 21] = 1.0f;
    block[o + 22] = 1.0f;
    block[o + 23] = 1.0f;

    // Vertex 2
    block[o + 24] = -1.0f;
    block[o + 25] = -1.0f;
    block[o + 26] = -1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 1.0f;

    block[o + 30] = 0.0f;
    block[o + 31] = 0.0f;
    block[o + 32] = -1.0f;

    block[o + 33] = 1.0f;
    block[o + 34] = 1.0f;
    block[o + 35] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[o + 36] = -1.0f;
    block[o + 37] = -1.0f;
    block[o + 38] = -1.0f;
    block[o + 39] = 1.0f;

    block[o + 40] = 1.0f;
    block[o + 41] = 1.0f;

    block[o + 42] = 0.0f;
    block[o + 43] = 0.0f;
    block[o + 44] = -1.0f;

    block[o + 45] = 1.0f;
    block[o + 46] = 1.0f;
    block[o + 47] = 1.0f;

    // Vertex 4
    block[o + 48] = -1.0f;
    block[o + 49] = 1.0f;
    block[o + 50] = -1.0f;
    block[o + 51] = 1.0f;

    block[o + 52] = 1.0f;
    block[o + 53] = 0.0f;

    block[o + 54] = 0.0f;
    block[o + 55] = 0.0f;
    block[o + 56] = -1.0f;

    block[o + 57] = 1.0f;
    block[o + 58] = 1.0f;
    block[o + 59] = 1.0f;

    // Vertex 5
    block[o + 60] = 1.0f;
    block[o + 61] = 1.0f;
    block[o + 62] = -1.0f;
    block[o + 63] = 1.0f;

    block[o + 64] = 0.0f;
    block[o + 65] = 0.0f;

    block[o + 66] = 0.0f;
    block[o + 67] = 0.0f;
    block[o + 68] = -1.0f;

    block[o + 69] = 1.0f;
    block[o + 70] = 1.0f;
    block[o + 71] = 1.0f;

    // LEFT SIDE

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1
    o = 72 * 3;

    // Vertex 0
    block[o + 0] = -1.0f;
    block[o + 1] = 1.0f;
    block[o + 2] = -1.0f;
    block[o + 3] = 1.0f;

    block[o + 4] = 0.0f;
    block[o + 5] = 0.0f;

    block[o + 6] = -1.0f;
    block[o + 7] = 0.0f;
    block[o + 8] = 0.0f;

    block[o + 9] = 1.0f;
    block[o + 10] = 1.0f;
    block[o + 11] = 1.0f;

    // Vertex 1
    block[o + 12] = -1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = -1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 0.0f;
    block[o + 17] = 1.0f;

    block[o + 18] = -1.0f;
    block[o + 19] = 0.0f;
    block[o + 20] = 0.0f;

    block[o + 21] = 1.0f;
    block[o + 22] = 1.0f;
    block[o + 23] = 1.0f;

    // Vertex 2
    block[o + 24] = -1.0f;
    block[o + 25] = -1.0f;
    block[o + 26] = 1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 1.0f;

    block[o + 30] = -1.0f;
    block[o + 31] = 0.0f;
    block[o + 32] = 0.0f;

    block[o + 33] = 1.0f;
    block[o + 34] = 1.0f;
    block[o + 35] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[o + 36] = -1.0f;
    block[o + 37] = -1.0f;
    block[o + 38] = 1.0f;
    block[o + 39] = 1.0f;

    block[o + 40] = 1.0f;
    block[o + 41] = 1.0f;

    block[o + 42] = -1.0f;
    block[o + 43] = 0.0f;
    block[o + 44] = 0.0f;

    block[o + 45] = 1.0f;
    block[o + 46] = 0.0f;
    block[o + 47] = 0.0f;

    // Vertex 4
    block[o + 48] = -1.0f;
    block[o + 49] = 1.0f;
    block[o + 50] = 1.0f;
    block[o + 51] = 1.0f;

    block[o + 52] = 1.0f;
    block[o + 53] = 0.0f;

    block[o + 54] = -1.0f;
    block[o + 55] = 0.0f;
    block[o + 56] = 0.0f;

    block[o + 57] = 1.0f;
    block[o + 58] = 1.0f;
    block[o + 59] = 1.0f;

    // Vertex 5
    block[o + 60] = -1.0f;
    block[o + 61] = 1.0f;
    block[o + 62] = -1.0f;
    block[o + 63] = 1.0f;

    block[o + 64] = 0.0f;
    block[o + 65] = 0.0f;

    block[o + 66] = -1.0f;
    block[o + 67] = 0.0f;
    block[o + 68] = 0.0f;

    block[o + 69] = 1.0f;
    block[o + 70] = 1.0f;
    block[o + 71] = 1.0f;

    // BOTTOM SIDE

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1
    o = 72 * 4;

    // Vertex 0
    block[o + 0] = -1.0f;
    block[o + 1] = -1.0f;
    block[o + 2] = 1.0f;
    block[o + 3] = 1.0f;

    block[o + 4] = 0.0f;
    block[o + 5] = 0.0f;

    block[o + 6] = 0.0f;
    block[o + 7] = -1.0f;
    block[o + 8] = 0.0f;

    block[o + 9] = 1.0f;
    block[o + 10] = 1.0f;
    block[o + 11] = 1.0f;

    // Vertex 1
    block[o + 12] = -1.0f;
    block[o + 13] = -1.0f;
    block[o + 14] = -1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 0.0f;
    block[o + 17] = 1.0f;

    block[o + 18] = 0.0f;
    block[o + 19] = -1.0f;
    block[o + 20] = 0.0f;

    block[o + 21] = 1.0f;
    block[o + 22] = 1.0f;
    block[o + 23] = 1.0f;

    // Vertex 2
    block[o + 24] = 1.0f;
    block[o + 25] = -1.0f;
    block[o + 26] = -1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 1.0f;

    block[o + 30] = 0.0f;
    block[o + 31] = -1.0f;
    block[o + 32] = 0.0f;

    block[o + 33] = 1.0f;
    block[o + 34] = 1.0f;
    block[o + 35] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[o + 36] = 1.0f;
    block[o + 37] = -1.0f;
    block[o + 38] = -1.0f;
    block[o + 39] = 1.0f;

    block[o + 40] = 1.0f;
    block[o + 41] = 1.0f;

    block[o + 42] = 0.0f;
    block[o + 43] = -1.0f;
    block[o + 44] = 0.0f;

    block[o + 45] = 1.0f;
    block[o + 46] = 1.0f;
    block[o + 47] = 1.0f;

    // Vertex 4
    block[o + 48] = 1.0f;
    block[o + 49] = -1.0f;
    block[o + 50] = 1.0f;
    block[o + 51] = 1.0f;

    block[o + 52] = 1.0f;
    block[o + 53] = 0.0f;

    block[o + 54] = 0.0f;
    block[o + 55] = -1.0f;
    block[o + 56] = 0.0f;

    block[o + 57] = 1.0f;
    block[o + 58] = 1.0f;
    block[o + 59] = 1.0f;

    // Vertex 5
    block[o + 60] = -1.0f;
    block[o + 61] = -1.0f;
    block[o + 62] = 1.0f;
    block[o + 63] = 1.0f;

    block[o + 64] = 0.0f;
    block[o + 65] = 0.0f;

    block[o + 66] = 0.0f;
    block[o + 67] = -1.0f;
    block[o + 68] = 0.0f;

    block[o + 69] = 1.0f;
    block[o + 70] = 1.0f;
    block[o + 71] = 1.0f;

    // TOP SIDE

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Triangle 1
    o = 72 * 5;

    // Vertex 0
    block[o + 0] = -1.0f;
    block[o + 1] = 1.0f;
    block[o + 2] = -1.0f;
    block[o + 3] = 1.0f;

    block[o + 4] = 0.0f;
    block[o + 5] = 0.0f;

    block[o + 6] = 0.0f;
    block[o + 7] = 1.0f;
    block[o + 8] = 0.0f;

    block[o + 9] = 0.0f;
    block[o + 10] = 1.0f;
    block[o + 11] = 0.0f;

    // Vertex 1
    block[o + 12] = -1.0f;
    block[o + 13] = 1.0f;
    block[o + 14] = 1.0f;
    block[o + 15] = 1.0f;

    block[o + 16] = 0.0f;
    block[o + 17] = 1.0f;

    block[o + 18] = 0.0f;
    block[o + 19] = 1.0f;
    block[o + 20] = 0.0f;

    block[o + 21] = 1.0f;
    block[o + 22] = 1.0f;
    block[o + 23] = 1.0f;

    // Vertex 2
    block[o + 24] = 1.0f;
    block[o + 25] = 1.0f;
    block[o + 26] = 1.0f;
    block[o + 27] = 1.0f;

    block[o + 28] = 1.0f;
    block[o + 29] = 1.0f;

    block[o + 30] = 0.0f;
    block[o + 31] = 1.0f;
    block[o + 32] = 0.0f;

    block[o + 33] = 1.0f;
    block[o + 34] = 1.0f;
    block[o + 35] = 1.0f;

    // Triangle 2

    // Vertex 3
    block[o + 36] = 1.0f;
    block[o + 37] = 1.0f;
    block[o + 38] = 1.0f;
    block[o + 39] = 1.0f;

    block[o + 40] = 1.0f;
    block[o + 41] = 1.0f;

    block[o + 42] = 0.0f;
    block[o + 43] = 1.0f;
    block[o + 44] = 0.0f;

    block[o + 45] = 1.0f;
    block[o + 46] = 1.0f;
    block[o + 47] = 1.0f;

    // Vertex 4
    block[o + 48] = 1.0f;
    block[o + 49] = 1.0f;
    block[o + 50] = -1.0f;
    block[o + 51] = 1.0f;

    block[o + 52] = 1.0f;
    block[o + 53] = 0.0f;

    block[o + 54] = 0.0f;
    block[o + 55] = 1.0f;
    block[o + 56] = 0.0f;

    block[o + 57] = 1.0f;
    block[o + 58] = 1.0f;
    block[o + 59] = 1.0f;

    // Vertex 5
    block[o + 60] = -1.0f;
    block[o + 61] = 1.0f;
    block[o + 62] = -1.0f;
    block[o + 63] = 1.0f;

    block[o + 64] = 0.0f;
    block[o + 65] = 0.0f;

    block[o + 66] = 0.0f;
    block[o + 67] = 1.0f;
    block[o + 68] = 0.0f;

    block[o + 69] = 1.0f;
    block[o + 70] = 1.0f;
    block[o + 71] = 1.0f;

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 72 * 6, block, GL_STATIC_DRAW);
	checkGLError("glBufferData");
}

/*
========================================
freeVertices
========================================
*/

void ShapeRenderer::freeVertices()
{
}
