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
#include "SpriteRenderer.h"
#include "platform.h"
#include "DDLUtils.hpp"

void SpriteRenderer::init(TextureManager2 *texMan, SpriteRenderer2D *spr2d)
{
	texAtlas.init(texMan, spr2d);
	
    const int len = 10000;

    char vertexShaderStr[len];
    char fragmentShaderStr[len];

    // Compile regular program

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
		snprintf(vertexShaderStr, len, "#version 330 core\n%s", vertexShaderCodeDesktop);
		snprintf(fragmentShaderStr, len, "#version 330 core\n%s", fragmentShaderCodeDesktop);
#endif
#if defined PLATFORM_OPENVR
    snprintf(vertexShaderStr, len, "#version 330 core\n%s", vertexShaderCodeOpenVR);
	snprintf(fragmentShaderStr, len, "#version 330 core\n%s", fragmentShaderCodeOpenVR);
#endif
#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
    snprintf(vertexShaderStr, len, "%s", vertexShaderCodeES20);
    snprintf(fragmentShaderStr, len, "%s", fragmentShaderCodeES20);
#endif

    programMain = loadProgram(vertexShaderStr, fragmentShaderStr, false);

#ifdef DO_FAST_SPRITES
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Generate VAO
    glGenVertexArrays(1, (GLuint *)&vao);
    checkGLError("glGenVertexArrays");
#endif
    
    // Generate VBO
    glGenBuffers(1, (GLuint *)&vbo);
    checkGLError("glGenBuffers");
#endif
}

void SpriteRenderer::draw(int eye, std::map<std::string, Object*> objects, Object *camera)
{
	frameDump = "";

	int numVerts = 0;
	int numQuads = 0;
	
	// Alloc enough data for all sprites
    for(const auto &pair: objects)
    {
        Object *obj = pair.second;
  
        if (obj != nullptr && obj->visible && obj->type == OBJTYPE_SPRITE)
        {
			numVerts += 6;
			numQuads += 1;
        }
    }
	
	int floatsPerVert = 26;
	
	float *data = new float[numVerts * floatsPerVert];
	
	int cursor = 0;

    // Quad/sprite index
    int q = 0;

    // Fill data for all sprites
    for(const auto &pair: objects)
    {		
        Object *obj = pair.second;

        if (obj != nullptr && obj->visible && obj->type == OBJTYPE_SPRITE) {
            
			// Add sprite
            texAtlas.add(obj->textureName);

            if (texAtlas.getNeedsRefresh())
			{
//				Log("texAtlas.refresh();");
                texAtlas.refresh();
			}
				
			// Set modelview matrix
            glm::mat4 mvMatrix;
	        glm::mat4 scaleToNDC;
            glm::mat4 cameraRotate;
            glm::mat4 cameraTranslate;
            glm::mat4 rotate;

#ifdef PLATFORM_OPENVR
            scaleToNDC = glm::scale(glm::mat4(), glm::vec3(VRSCALE, VRSCALE, VRSCALE));
#else
            scaleToNDC = glm::scale(glm::mat4(), glm::vec3(NDC_SCALE, NDC_SCALE, NDC_SCALE));
#endif

	        if (obj->alwaysFacePlayer)
		        rotate = glm::rotate(glm::mat4(), glm::radians(-camera->yaw), glm::vec3(0, 1, 0)) // Model yaw
			    * glm::rotate(glm::mat4(), glm::radians(camera->pitch), glm::vec3(1, 0, 0)); // Model pitch
	        else
		        rotate = glm::rotate(glm::mat4(), glm::radians(-obj->yaw), glm::vec3(0, 1, 0)) // Model yaw
			    * glm::rotate(glm::mat4(), glm::radians(-obj->pitch), glm::vec3(1, 0, 0)); // Model pitch

            cameraRotate = glm::rotate(glm::mat4(), glm::radians(camera->roll), glm::vec3(0, 0, 1)) // Camera roll
                       * glm::rotate(glm::mat4(), -glm::radians(camera->pitch), glm::vec3(1, 0, 0)) // Camera pitch
                       * glm::rotate(glm::mat4(), glm::radians(camera->yaw), glm::vec3(0, 1, 0)); // Camera yaw

            cameraTranslate = glm::translate(glm::mat4(), glm::vec3(-camera->position.x, -camera->position.y, -camera->position.z)); // Camera translate

#ifdef PLATFORM_OPENVR
            mvMatrix =
		        glm::make_mat4((const GLfloat *)g_poseEyeMatrix.get())
	            * scaleToNDC
		        * cameraRotate
		        * cameraTranslate
		        * glm::translate(glm::mat4(), glm::vec3(obj->position.x, obj->position.y, obj->position.z)) // World translate
		        * rotate
		        * glm::scale(glm::mat4(), obj->scale / glm::vec3(2.0, 2.0, 2.0)); // Scale
#else
            mvMatrix =
                scaleToNDC
                * cameraRotate
                * cameraTranslate
                * glm::translate(glm::mat4(), glm::vec3(obj->position.x, obj->position.y, obj->position.z)) // World translate
                * rotate
                * glm::scale(glm::mat4(), obj->scale / glm::vec3(2.0, 2.0, 2.0)); // Scale
#endif

            //   ______
            // |\\5   4|
            // |0\\    |
            // |  \\   |
            // |   \\  |
            // |    \\3|
            // |1__2_\\|

            // Triangle 1

            // Vertex 0
            data[cursor + 0] = -1.0f;
            data[cursor + 1] = 1.0f;
            data[cursor + 2] = 0.0f;
            data[cursor + 3] = 1.0f;

            UV input;
            input.u = 0.0f;
            input.v = 1.0f;
            UV output = texAtlas.getUV(obj->textureName, input);

            data[cursor + 4] = output.u;
            data[cursor + 5] = output.v;

            data[cursor + 6] = mvMatrix[0][0];
			data[cursor + 7] = mvMatrix[0][1];
			data[cursor + 8] = mvMatrix[0][2];
			data[cursor + 9] = mvMatrix[0][3];

            data[cursor + 10] = mvMatrix[1][0];
			data[cursor + 11] = mvMatrix[1][1];
			data[cursor + 12] = mvMatrix[1][2];
			data[cursor + 13] = mvMatrix[1][3];

            data[cursor + 14] = mvMatrix[2][0];
			data[cursor + 15] = mvMatrix[2][1];
			data[cursor + 16] = mvMatrix[2][2];
			data[cursor + 17] = mvMatrix[2][3];

            data[cursor + 18] = mvMatrix[3][0];
			data[cursor + 19] = mvMatrix[3][1];
			data[cursor + 20] = mvMatrix[3][2];
			data[cursor + 21] = mvMatrix[3][3];

            data[cursor + 22] = obj->color.r;
			data[cursor + 23] = obj->color.g;
			data[cursor + 24] = obj->color.b;
			data[cursor + 25] = obj->color.a;

            cursor += floatsPerVert;

            // Vertex 1
            data[cursor + 0] = -1.0f;
            data[cursor + 1] = -1.0f;
            data[cursor + 2] = 0.0f;
            data[cursor + 3] = 1.0f;

            input.u = 0.0f;
            input.v = 0.0f;
            output = texAtlas.getUV(obj->textureName, input);

            data[cursor + 4] = output.u;
            data[cursor + 5] = output.v;

            data[cursor + 6] = mvMatrix[0][0];
			data[cursor + 7] = mvMatrix[0][1];
			data[cursor + 8] = mvMatrix[0][2];
			data[cursor + 9] = mvMatrix[0][3];

            data[cursor + 10] = mvMatrix[1][0];
			data[cursor + 11] = mvMatrix[1][1];
			data[cursor + 12] = mvMatrix[1][2];
			data[cursor + 13] = mvMatrix[1][3];

            data[cursor + 14] = mvMatrix[2][0];
			data[cursor + 15] = mvMatrix[2][1];
			data[cursor + 16] = mvMatrix[2][2];
			data[cursor + 17] = mvMatrix[2][3];

            data[cursor + 18] = mvMatrix[3][0];
			data[cursor + 19] = mvMatrix[3][1];
			data[cursor + 20] = mvMatrix[3][2];
			data[cursor + 21] = mvMatrix[3][3];

            data[cursor + 22] = obj->color.r;
			data[cursor + 23] = obj->color.g;
			data[cursor + 24] = obj->color.b;
			data[cursor + 25] = obj->color.a;

            cursor += floatsPerVert;

            // Vertex 2
            data[cursor + 0] = 1.0f;
            data[cursor + 1] = -1.0f;
            data[cursor + 2] = 0.0f;
            data[cursor + 3] = 1.0f;

            input.u = 1.0f;
            input.v = 0.0f;
            output = texAtlas.getUV(obj->textureName, input);

            data[cursor + 4] = output.u;
            data[cursor + 5] = output.v;

            data[cursor + 6] = mvMatrix[0][0];
			data[cursor + 7] = mvMatrix[0][1];
			data[cursor + 8] = mvMatrix[0][2];
			data[cursor + 9] = mvMatrix[0][3];

            data[cursor + 10] = mvMatrix[1][0];
			data[cursor + 11] = mvMatrix[1][1];
			data[cursor + 12] = mvMatrix[1][2];
			data[cursor + 13] = mvMatrix[1][3];

            data[cursor + 14] = mvMatrix[2][0];
			data[cursor + 15] = mvMatrix[2][1];
			data[cursor + 16] = mvMatrix[2][2];
			data[cursor + 17] = mvMatrix[2][3];

            data[cursor + 18] = mvMatrix[3][0];
			data[cursor + 19] = mvMatrix[3][1];
			data[cursor + 20] = mvMatrix[3][2];
			data[cursor + 21] = mvMatrix[3][3];

            data[cursor + 22] = obj->color.r;
			data[cursor + 23] = obj->color.g;
			data[cursor + 24] = obj->color.b;
			data[cursor + 25] = obj->color.a;

            cursor += floatsPerVert;

            // Triangle 2

            // Vertex 3
            data[cursor + 0] = 1.0f;
            data[cursor + 1] = -1.0f;
            data[cursor + 2] = 0.0f;
            data[cursor + 3] = 1.0f;

            input.u = 1.0f;
            input.v = 0.0f;
            output = texAtlas.getUV(obj->textureName, input);

            data[cursor + 4] = output.u;
            data[cursor + 5] = output.v;

            data[cursor + 6] = mvMatrix[0][0];
			data[cursor + 7] = mvMatrix[0][1];
			data[cursor + 8] = mvMatrix[0][2];
			data[cursor + 9] = mvMatrix[0][3];

            data[cursor + 10] = mvMatrix[1][0];
			data[cursor + 11] = mvMatrix[1][1];
			data[cursor + 12] = mvMatrix[1][2];
			data[cursor + 13] = mvMatrix[1][3];

            data[cursor + 14] = mvMatrix[2][0];
			data[cursor + 15] = mvMatrix[2][1];
			data[cursor + 16] = mvMatrix[2][2];
			data[cursor + 17] = mvMatrix[2][3];

            data[cursor + 18] = mvMatrix[3][0];
			data[cursor + 19] = mvMatrix[3][1];
			data[cursor + 20] = mvMatrix[3][2];
			data[cursor + 21] = mvMatrix[3][3];

		    data[cursor + 22] = obj->color.r;
			data[cursor + 23] = obj->color.g;
			data[cursor + 24] = obj->color.b;
			data[cursor + 25] = obj->color.a;
	
            cursor += floatsPerVert;

            // Vertex 4
            data[cursor + 0] = 1.0f;
            data[cursor + 1] = 1.0f;
            data[cursor + 2] = 0.0f;
            data[cursor + 3] = 1.0f;

            input.u = 1.0f;
            input.v = 1.0f;
            output = texAtlas.getUV(obj->textureName, input);

            data[cursor + 4] = output.u;
            data[cursor + 5] = output.v;

            data[cursor + 6] = mvMatrix[0][0];
			data[cursor + 7] = mvMatrix[0][1];
			data[cursor + 8] = mvMatrix[0][2];
			data[cursor + 9] = mvMatrix[0][3];

            data[cursor + 10] = mvMatrix[1][0];
			data[cursor + 11] = mvMatrix[1][1];
			data[cursor + 12] = mvMatrix[1][2];
			data[cursor + 13] = mvMatrix[1][3];

            data[cursor + 14] = mvMatrix[2][0];
			data[cursor + 15] = mvMatrix[2][1];
			data[cursor + 16] = mvMatrix[2][2];
			data[cursor + 17] = mvMatrix[2][3];

            data[cursor + 18] = mvMatrix[3][0];
			data[cursor + 19] = mvMatrix[3][1];
			data[cursor + 20] = mvMatrix[3][2];
			data[cursor + 21] = mvMatrix[3][3];

	        data[cursor + 22] = obj->color.r;
			data[cursor + 23] = obj->color.g;
			data[cursor + 24] = obj->color.b;
			data[cursor + 25] = obj->color.a;
		
            cursor += floatsPerVert;

            // Vertex 5
            data[cursor + 0] = -1.0f;
            data[cursor + 1] = 1.0f;
            data[cursor + 2] = 0.0f;
            data[cursor + 3] = 1.0f;

            input.u = 0.0f;
            input.v = 1.0f;
            output = texAtlas.getUV(obj->textureName, input);

            data[cursor + 4] = output.u;
            data[cursor + 5] = output.v;

            data[cursor + 6] = mvMatrix[0][0];
			data[cursor + 7] = mvMatrix[0][1];
			data[cursor + 8] = mvMatrix[0][2];
			data[cursor + 9] = mvMatrix[0][3];

            data[cursor + 10] = mvMatrix[1][0];
			data[cursor + 11] = mvMatrix[1][1];
			data[cursor + 12] = mvMatrix[1][2];
			data[cursor + 13] = mvMatrix[1][3];

            data[cursor + 14] = mvMatrix[2][0];
			data[cursor + 15] = mvMatrix[2][1];
			data[cursor + 16] = mvMatrix[2][2];
			data[cursor + 17] = mvMatrix[2][3];

            data[cursor + 18] = mvMatrix[3][0];
			data[cursor + 19] = mvMatrix[3][1];
			data[cursor + 20] = mvMatrix[3][2];
			data[cursor + 21] = mvMatrix[3][3];

	        data[cursor + 22] = obj->color.r;
			data[cursor + 23] = obj->color.g;
			data[cursor + 24] = obj->color.b;
			data[cursor + 25] = obj->color.a;
			
            cursor += floatsPerVert;

            q++;
        }
    }
	
#ifndef DO_FAST_SPRITES
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Generate VAO
    glGenVertexArrays(1, (GLuint *)&vao);
    checkGLError("glGenVertexArrays");
#endif
    
    // Generate VBO
    glGenBuffers(1, (GLuint *)&vbo);
    checkGLError("glGenBuffers");
#endif

#if defined PLATFORM_WINDOWS or defined PLATFORM_OSX
    glBindVertexArray(vao);
    checkGLError("glBindVertexArray");
#endif

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLError("glBindBuffer");
    
    // Load data into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * floatsPerVert * q, data, GL_STATIC_DRAW);
    checkGLError("glBufferData");

    // Delete data
    delete data;

    // Get aspect
    float width = PLAT_GetWindowWidth();
    float height = PLAT_GetWindowHeight();
#ifdef PLATFORM_OPENVR
    float aspect = 1.0;
#else
    float aspect = width / height;
#endif

    // DRAW
	glDisable(GL_CULL_FACE);
	
/*    glEnable(GL_CULL_FACE);
    checkGLError("glEnable");
    glFrontFace(GL_CCW);
    checkGLError("glFrontFace");

    glCullFace(GL_BACK);
    checkGLError("glCullFace");*/

    glEnable(GL_BLEND);
    checkGLError("ShapeRenderer glEnable");
#ifndef PLATFORM_ANDROID
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		checkGLError("ShapeRenderer glBlendFunc");
#endif

    // Add program to OpenGL environment
    int curProgram = -1;
    curProgram = programMain;
	
    glUseProgram(curProgram);
    checkGLError("SpriteRenderer glUseProgram");
	
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Bind the VAO
    glBindVertexArray(vao);
    checkGLError("glBindVertexArray");
#endif

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLError("glBindBuffer");

    // Set the projection matrix
	glm::mat4 projMatrix;
	
	#if defined PLATFORM_OPENVR
        projMatrix = glm::make_mat4((const GLfloat *)g_projectionMatrix.get());
    #else
        projMatrix = glm::perspective(VIEW_FOV, aspect, 0.001f, 1000.0f);
    #endif
	
    setMatrix(curProgram, "projectionMatrix", projMatrix);

    // Set variables
    setUniform4f(curProgram, "globalColor", globalColor.x, globalColor.y, globalColor.z, globalColor.w);

	int t = texAtlas.getGlTexId();
//    Texture * tex = texAtlas.getTexMan()->find("brick3.png");
//    int t = tex->glTexID;

	glActiveTexture(GL_TEXTURE0);
    checkGLError("glActiveTexture");

    glBindTexture(GL_TEXTURE_2D, t);

    setUniform2f(curProgram, "vTexSpan", 1.0, 1.0);
	
	if (g_common.useTexturesOnSprites)
        setUniform1f(curProgram, "useTexture", 1.0);
	else
		setUniform1f(curProgram, "useTexture", 0.0);
		
    setUniform1f(curProgram, "fadeNear", 600.0 * NDC_SCALE);
    setUniform1f(curProgram, "fadeFar", 900.0 * NDC_SCALE);

    // Set attributes
    setVertexAttrib(curProgram, "vPosition", 4, GL_FLOAT, false, floatsPerVert * sizeof(float), 0);
    setVertexAttrib(curProgram, "vTexCoords", 2, GL_FLOAT, false, floatsPerVert * sizeof(float), 4);
    
	setVertexAttrib(curProgram, "mvMatrixPt1", 4, GL_FLOAT, false, floatsPerVert * sizeof(float), 6);
    setVertexAttrib(curProgram, "mvMatrixPt2", 4, GL_FLOAT, false, floatsPerVert * sizeof(float), 10);
    setVertexAttrib(curProgram, "mvMatrixPt3", 4, GL_FLOAT, false, floatsPerVert * sizeof(float), 14);
    setVertexAttrib(curProgram, "mvMatrixPt4", 4, GL_FLOAT, false, floatsPerVert * sizeof(float), 18);
	
    setVertexAttrib(curProgram, "vColor", 4, GL_FLOAT, false, floatsPerVert * sizeof(float), 22);
	
    // Draw
    glDrawArrays(GL_TRIANGLES, 0, q * 6);
    checkGLError("glDrawArrays");

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Reset
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
#endif

    // Delete VAO and VBO
 //   glDeleteBuffers(1, (GLuint *)&vbo);
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
//    glDeleteVertexArrays(1, (GLuint *)&vao);
#endif
	
	dumpFrame = false;
}
