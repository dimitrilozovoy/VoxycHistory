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

#include "CustomTris.h"
#include "DDLutils.hpp"

void CustomTris::cquad1(std::string texture, float x, float y, float z, float sizex, float sizey, float pitch, float yaw, float roll)
{
	
}


/*
========================================
getMeshes
========================================
*/

std::vector<Mesh*> CustomTris::getMeshes()
{
	std::vector<Mesh*> meshes;

	for (const auto &pair: buffers)
	{
		VertexBuffer *buf = pair.second;

		Mesh *mesh = new Mesh();

		mesh->texture = pair.first;
		mesh->data = buf->vertices;
#ifdef DO_VERTEX_LIGHTS
		mesh->floatsPerCoord = 12;
#else
		mesh->floatsPerCoord = 9;
#endif

		// Generate VBO
		glGenBuffers(1, (GLuint *)&mesh->vbo);
		checkGLError("glGenBuffers");

		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		checkGLError("glBindBuffer");

		int numCoords = buf->cursor;
		mesh->numCoords = numCoords;

		// Send vertices to GPU memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numCoords, buf->vertices, GL_STATIC_DRAW);
		checkGLError("glBufferData");

		meshes.push_back(mesh);
	}

	return meshes;
}

/*
========================================
checkGLError
========================================
*/

void CustomTris::checkGLError(char *tag)
{
#ifdef USE_OPENGL
    GLenum err = glGetError();

    switch (err)
    {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            Log("GL error GL_INVALID_ENUM", tag);
            break;
        case GL_INVALID_VALUE:
            Log("GL error GL_INVALID_VALUE", tag);
            break;
        case GL_INVALID_OPERATION:
            Log("GL error GL_INVALID_OPERATION", tag);
            break;
#ifndef USE_OPENGL_3
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            Log("GL error GL_INVALID_FRAMEBUFFER_OPERATION", str);
            break;
#endif
        case GL_OUT_OF_MEMORY:
            Log("GL error GL_OUT_OF_MEMORY", tag);
            break;
    }
#endif
}

