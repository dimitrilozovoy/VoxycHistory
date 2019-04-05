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

#include "SpriteRenderer2.h"
#include "platform.h"
#include "DDLUtils.hpp"

/*
========================================
init
========================================
*/

void SpriteRenderer2::init(TextureManager2 *texMan, SpriteRenderer2D *spr2d)
{
	texAtlas.init(texMan, spr2d);
	
    // Compile regular program
    const int len = 10000;
    char vertexShaderStr[len];
    char fragmentShaderStr[len];

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

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Generate VAO
    glGenVertexArrays(1, (GLuint *)&vao);
    checkGLError("glGenVertexArrays");
#endif
    
    // Generate VBO
    glGenBuffers(1, (GLuint *)&vbo);
    checkGLError("glGenBuffers");

	// Alloc data for max number of sprites
	int numQuads = maxSprites;
	int numVerts = numQuads * 6;
	int floatsPerVert = 6;
	
	float *data = new float[numVerts * floatsPerVert];
	
    // Fill data for all possible sprites
	int cursor = 0;
	
    for(int i = 0; i < maxSprites; i++)
    {		
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

        data[cursor + 4] = (float)i;
	    data[cursor + 5] = (float)(i * floatsPerVert);

        cursor += floatsPerVert;

        // Vertex 1
        data[cursor + 0] = -1.0f;
        data[cursor + 1] = -1.0f;
        data[cursor + 2] = 0.0f;
        data[cursor + 3] = 1.0f;
		
	    data[cursor + 4] = (float)i;
	    data[cursor + 5] = (float)(i * floatsPerVert + 1);

        cursor += floatsPerVert;

        // Vertex 2
        data[cursor + 0] = 1.0f;
        data[cursor + 1] = -1.0f;
        data[cursor + 2] = 0.0f;
        data[cursor + 3] = 1.0f;

	    data[cursor + 4] = (float)i;
		data[cursor + 5] = (float)(i * floatsPerVert + 2);

        cursor += floatsPerVert;

        // Triangle 2

        // Vertex 3
        data[cursor + 0] = 1.0f;
        data[cursor + 1] = -1.0f;
        data[cursor + 2] = 0.0f;
        data[cursor + 3] = 1.0f;

        data[cursor + 4] = (float)i;
	    data[cursor + 5] = (float)(i * floatsPerVert + 3);

        cursor += floatsPerVert;

        // Vertex 4
        data[cursor + 0] = 1.0f;
        data[cursor + 1] = 1.0f;
        data[cursor + 2] = 0.0f;
        data[cursor + 3] = 1.0f;

	    data[cursor + 4] = (float)i;
	    data[cursor + 5] = (float)(i * floatsPerVert + 4);

        cursor += floatsPerVert;

        // Vertex 5
        data[cursor + 0] = -1.0f;
        data[cursor + 1] = 1.0f;
        data[cursor + 2] = 0.0f;
        data[cursor + 3] = 1.0f;

	    data[cursor + 4] = (float)i;
	    data[cursor + 5] = (float)(i * floatsPerVert + 5);

        cursor += floatsPerVert;
    }
	
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    glBindVertexArray(vao);
    checkGLError("glBindVertexArray");
#endif

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLError("glBindBuffer");
    
    // Load data into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * floatsPerVert * numQuads, data, GL_STATIC_DRAW);
    checkGLError("glBufferData");

    // Delete data
    delete data;
}

/*
========================================
draw
========================================
*/

void SpriteRenderer2::draw(int eye, std::map<std::string, Object*> objects, Object *camera)
{
	int floatsPerVert = 6;
	
    // Get aspect
    float width = PLAT_GetWindowWidth();
    float height = PLAT_GetWindowHeight();
	
#ifdef PLATFORM_OPENVR
    float aspect = 1.0;
#else
    float aspect = width / height;
#endif

    int numVerts = 0;
    int numQuads = 0;
	
	// Declare arrays for uniform data
	glm::mat4 mvMatrices[maxSprites];
	float texCoords[maxSprites * 6 * 2];

	// Calculate number of sprites to draw
	// and fill the mv matrix and tex coord arrays
    for(const auto &pair: objects)
    {
        Object *obj = pair.second;
		
	    if (numQuads >= maxSprites)
			break;
  
        if (obj != nullptr && obj->visible && obj->type == OBJTYPE_SPRITE)
        {	
			// Add sprite
            texAtlas.add(obj->textureName);

            if (texAtlas.getNeedsRefresh())
			{
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
			
			mvMatrices[numQuads] = mvMatrix;
			
			// Set texture coords
			UV input;
            input.u = 0.0f;
            input.v = 1.0f;
            UV output = texAtlas.getUV(obj->textureName, input);
			texCoords[numQuads * 12] = output.u;
			texCoords[numQuads * 12 + 1] = output.v;
			
            input.u = 0.0f;
            input.v = 0.0f;
            output = texAtlas.getUV(obj->textureName, input);
			texCoords[numQuads * 12 + 2] = output.u;
			texCoords[numQuads * 12 + 3] = output.v;
			
            input.u = 1.0f;
            input.v = 0.0f;
            output = texAtlas.getUV(obj->textureName, input);
			texCoords[numQuads * 12 + 4] = output.u;
			texCoords[numQuads * 12 + 5] = output.v;
			
            input.u = 1.0f;
            input.v = 0.0f;
            output = texAtlas.getUV(obj->textureName, input);
			texCoords[numQuads * 12 + 6] = output.u;
			texCoords[numQuads * 12 + 7] = output.v;

            input.u = 1.0f;
            input.v = 1.0f;
            output = texAtlas.getUV(obj->textureName, input);
			texCoords[numQuads * 12 + 8] = output.u;
			texCoords[numQuads * 12 + 9] = output.v;
			
			input.u = 0.0f;
            input.v = 1.0f;
            output = texAtlas.getUV(obj->textureName, input);
			texCoords[numQuads * 12 + 10] = output.u;
			texCoords[numQuads * 12 + 11] = output.v;
			
			numVerts += 6;
			numQuads += 1;
        }
    }
	
	// Add program to OpenGL environment
    int curProgram = -1;
    curProgram = programMain;
	
    glUseProgram(curProgram);
    checkGLError("glUseProgram");

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    glBindVertexArray(vao);
    checkGLError("glBindVertexArray");
#endif

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLError("glBindBuffer");
	
	// Set mv matrix array uniform
	setMatrixArray(curProgram, "mvMatrices", numQuads, mvMatrices);
	
	// Set texture coordinates uniform
	setUniform2fv(curProgram, "texCoords", numVerts, texCoords);
	
	// Set the projection matrix
	glm::mat4 projMatrix;
	
	#if defined PLATFORM_OPENVR
        projMatrix = glm::make_mat4((const GLfloat *)g_projectionMatrix.get());
    #else
        projMatrix = glm::perspective(VIEW_FOV, aspect, 0.001f, 1000.0f);
    #endif
	
	setMatrix(curProgram, "projectionMatrix", projMatrix);

    // Set misc variables
    setUniform4f(curProgram, "globalColor", globalColor.x, globalColor.y, globalColor.z, globalColor.w);
    setUniform1f(curProgram, "useTexture", 1.0);
    setUniform1f(curProgram, "fadeNear", 600.0 * NDC_SCALE);
    setUniform1f(curProgram, "fadeFar", 900.0 * NDC_SCALE);

    // Set attributes
    setVertexAttrib(curProgram, "vPosition", 4, GL_FLOAT, false, floatsPerVert * sizeof(float), 0);
    setVertexAttrib(curProgram, "quadIdx", 1, GL_FLOAT, false, floatsPerVert * sizeof(float), 4);
    setVertexAttrib(curProgram, "vertIdx", 1, GL_FLOAT, false, floatsPerVert * sizeof(float), 5);

	// Set texture
	int t = texAtlas.getGlTexId();
	
	glActiveTexture(GL_TEXTURE0);
    checkGLError("glActiveTexture");
    glBindTexture(GL_TEXTURE_2D, t);
    checkGLError("glBindTexture");
	
	// Set culling
    glEnable(GL_CULL_FACE);
    checkGLError("glEnable");
    glFrontFace(GL_CCW);
    checkGLError("glFrontFace");
    glCullFace(GL_BACK);
    checkGLError("glCullFace");

	// Set blend
    glEnable(GL_BLEND);
    checkGLError("glEnable");
	
#ifndef PLATFORM_ANDROID
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    checkGLError("glBlendFunc");
#endif
	
    // Draw
    glDrawArrays(GL_TRIANGLES, 0, numQuads * 6);
    checkGLError("glDrawArrays");

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Reset
    glBindVertexArray(0);
#endif

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

/*
========================================
free
========================================
*/

void SpriteRenderer2::free()
{
	// Delete VAO and VBO
    glDeleteBuffers(1, (GLuint *)&vbo);
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    glDeleteVertexArrays(1, (GLuint *)&vao);
#endif
}
