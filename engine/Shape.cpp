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

#include "Shape.h"
#include "GLIncludes.h"
#include "DDLUtils.hpp"
#include "Terrain.h"
#include "Grid.h"
#include "VertexLights.h"

void Shape::generate(std::map<std::string, std::string> *stringExtras)
{
	//    if (vertices != nullptr)
    //        return;

	if (type == SHAPE_TERRAIN)
	{
		Terrain::generate((int)sizeA, (int)sizeB, (int)sizeC, (int)sizeD, (int)sizeE, (int)sizeF, (int)sizeG);
		vbo = Terrain::vbo;
		numCoords = Terrain::numCoords;

		// New mesh
		Mesh *mesh = new Mesh();
		mesh->vbo = Terrain::vbo;
		mesh->numCoords = Terrain::numCoords;
		mesh->data = Terrain::vertices;
		mesh->floatsPerCoord = 9;

		// Add mesh to shape
		meshes.push_back(mesh);
	}
	else if (type == SHAPE_BLOCK)
	{
		//
		// SHAPE_BLOCK
		//

        // Generate VBO
		glGenBuffers(1, (GLuint *)&vbo);
		checkGLError("glGenBuffers");

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		checkGLError("glBindBuffer");

		float block[54 * 6];
		numCoords = 54 * 6;

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

		// Vertex 1
		block[9] = -1.0f;
		block[10] = -1.0f;
		block[11] = 1.0f;
		block[12] = 1.0f;

		block[13] = 0.0f;
		block[14] = 1.0f;

		block[15] = 0.0f;
		block[16] = 0.0f;
		block[17] = 1.0f;

		// Vertex 2
		block[18] = 1.0f;
		block[19] = -1.0f;
		block[20] = 1.0f;
		block[21] = 1.0f;

		block[22] = 1.0f;
		block[23] = 1.0f;

		block[24] = 0.0f;
		block[25] = 0.0f;
		block[26] = 1.0f;

		// Triangle 2

		// Vertex 3
		block[27] = 1.0f;
		block[28] = -1.0f;
		block[29] = 1.0f;
		block[30] = 1.0f;

		block[31] = 1.0f;
		block[32] = 1.0f;

		block[33] = 0.0f;
		block[34] = 0.0f;
		block[35] = 1.0f;

		// Vertex 4
		block[36] = 1.0f;
		block[37] = 1.0f;
		block[38] = 1.0f;
		block[39] = 1.0f;

		block[40] = 1.0f;
		block[41] = 0.0f;

		block[42] = 0.0f;
		block[43] = 0.0f;
		block[44] = 1.0f;

		// Vertex 5
		block[45] = -1.0f;
		block[46] = 1.0f;
		block[47] = 1.0f;
		block[48] = 1.0f;

		block[49] = 0.0f;
		block[50] = 0.0f;

		block[51] = 0.0f;
		block[52] = 0.0f;
		block[53] = 1.0f;

		// RIGHT SIDE

		//   ______
		// |\\5   4|
		// |0\\    |
		// |  \\   |
		// |   \\  |
		// |    \\3|
		// |1__2_\\|

		// Triangle 1
		int o = 54;

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

		// Vertex 1
		block[o + 9] = 1.0f;
		block[o + 10] = -1.0f;
		block[o + 11] = 1.0f;
		block[o + 12] = 1.0f;

		block[o + 13] = 0.0f;
		block[o + 14] = 1.0f;

		block[o + 15] = 1.0f;
		block[o + 16] = 0.0f;
		block[o + 17] = 0.0f;

		// Vertex 2
		block[o + 18] = 1.0f;
		block[o + 19] = -1.0f;
		block[o + 20] = -1.0f;
		block[o + 21] = 1.0f;

		block[o + 22] = 1.0f;
		block[o + 23] = 1.0f;

		block[o + 24] = 1.0f;
		block[o + 25] = 0.0f;
		block[o + 26] = 0.0f;

		// Triangle 2

		// Vertex 3
		block[o + 27] = 1.0f;
		block[o + 28] = -1.0f;
		block[o + 29] = -1.0f;
		block[o + 30] = 1.0f;

		block[o + 31] = 1.0f;
		block[o + 32] = 1.0f;

		block[o + 33] = 1.0f;
		block[o + 34] = 0.0f;
		block[o + 35] = 0.0f;

		// Vertex 4
		block[o + 36] = 1.0f;
		block[o + 37] = 1.0f;
		block[o + 38] = -1.0f;
		block[o + 39] = 1.0f;

		block[o + 40] = 1.0f;
		block[o + 41] = 0.0f;

		block[o + 42] = 1.0f;
		block[o + 43] = 0.0f;
		block[o + 44] = 0.0f;

		// Vertex 5
		block[o + 45] = 1.0f;
		block[o + 46] = 1.0f;
		block[o + 47] = 1.0f;
		block[o + 48] = 1.0f;

		block[o + 49] = 0.0f;
		block[o + 50] = 0.0f;

		block[o + 51] = 1.0f;
		block[o + 52] = 0.0f;
		block[o + 53] = 0.0f;

		// BACK SIDE

		//   ______
		// |\\5   4|
		// |0\\    |
		// |  \\   |
		// |   \\  |
		// |    \\3|
		// |1__2_\\|

		// Triangle 1
		o = 54 * 2;

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

		// Vertex 1
		block[o + 9] = 1.0f;
		block[o + 10] = -1.0f;
		block[o + 11] = -1.0f;
		block[o + 12] = 1.0f;

		block[o + 13] = 0.0f;
		block[o + 14] = 1.0f;

		block[o + 15] = 0.0f;
		block[o + 16] = 0.0f;
		block[o + 17] = -1.0f;

		// Vertex 2
		block[o + 18] = -1.0f;
		block[o + 19] = -1.0f;
		block[o + 20] = -1.0f;
		block[o + 21] = 1.0f;

		block[o + 22] = 1.0f;
		block[o + 23] = 1.0f;

		block[o + 24] = 0.0f;
		block[o + 25] = 0.0f;
		block[o + 26] = -1.0f;

		// Triangle 2

		// Vertex 3
		block[o + 27] = -1.0f;
		block[o + 28] = -1.0f;
		block[o + 29] = -1.0f;
		block[o + 30] = 1.0f;

		block[o + 31] = 1.0f;
		block[o + 32] = 1.0f;

		block[o + 33] = 0.0f;
		block[o + 34] = 0.0f;
		block[o + 35] = -1.0f;

		// Vertex 4
		block[o + 36] = -1.0f;
		block[o + 37] = 1.0f;
		block[o + 38] = -1.0f;
		block[o + 39] = 1.0f;

		block[o + 40] = 1.0f;
		block[o + 41] = 0.0f;

		block[o + 42] = 0.0f;
		block[o + 43] = 0.0f;
		block[o + 44] = -1.0f;

		// Vertex 5
		block[o + 45] = 1.0f;
		block[o + 46] = 1.0f;
		block[o + 47] = -1.0f;
		block[o + 48] = 1.0f;

		block[o + 49] = 0.0f;
		block[o + 50] = 0.0f;

		block[o + 51] = 0.0f;
		block[o + 52] = 0.0f;
		block[o + 53] = -1.0f;

		// LEFT SIDE

		//   ______
		// |\\5   4|
		// |0\\    |
		// |  \\   |
		// |   \\  |
		// |    \\3|
		// |1__2_\\|

		// Triangle 1
		o = 54 * 3;

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

		// Vertex 1
		block[o + 9] = -1.0f;
		block[o + 10] = -1.0f;
		block[o + 11] = -1.0f;
		block[o + 12] = 1.0f;

		block[o + 13] = 0.0f;
		block[o + 14] = 1.0f;

		block[o + 15] = -1.0f;
		block[o + 16] = 0.0f;
		block[o + 17] = 0.0f;

		// Vertex 2
		block[o + 18] = -1.0f;
		block[o + 19] = -1.0f;
		block[o + 20] = 1.0f;
		block[o + 21] = 1.0f;

		block[o + 22] = 1.0f;
		block[o + 23] = 1.0f;

		block[o + 24] = -1.0f;
		block[o + 25] = 0.0f;
		block[o + 26] = 0.0f;

		// Triangle 2

		// Vertex 3
		block[o + 27] = -1.0f;
		block[o + 28] = -1.0f;
		block[o + 29] = 1.0f;
		block[o + 30] = 1.0f;

		block[o + 31] = 1.0f;
		block[o + 32] = 1.0f;

		block[o + 33] = -1.0f;
		block[o + 34] = 0.0f;
		block[o + 35] = 0.0f;

		// Vertex 4
		block[o + 36] = -1.0f;
		block[o + 37] = 1.0f;
		block[o + 38] = 1.0f;
		block[o + 39] = 1.0f;

		block[o + 40] = 1.0f;
		block[o + 41] = 0.0f;

		block[o + 42] = -1.0f;
		block[o + 43] = 0.0f;
		block[o + 44] = 0.0f;

		// Vertex 5
		block[o + 45] = -1.0f;
		block[o + 46] = 1.0f;
		block[o + 47] = -1.0f;
		block[o + 48] = 1.0f;

		block[o + 49] = 0.0f;
		block[o + 50] = 0.0f;

		block[o + 51] = -1.0f;
		block[o + 52] = 0.0f;
		block[o + 53] = 0.0f;

		// BOTTOM SIDE

		//   ______
		// |\\5   4|
		// |0\\    |
		// |  \\   |
		// |   \\  |
		// |    \\3|
		// |1__2_\\|

		// Triangle 1
		o = 54 * 4;

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

		// Vertex 1
		block[o + 9] = -1.0f;
		block[o + 10] = -1.0f;
		block[o + 11] = -1.0f;
		block[o + 12] = 1.0f;

		block[o + 13] = 0.0f;
		block[o + 14] = 1.0f;

		block[o + 15] = 0.0f;
		block[o + 16] = -1.0f;
		block[o + 17] = 0.0f;

		// Vertex 2
		block[o + 18] = 1.0f;
		block[o + 19] = -1.0f;
		block[o + 20] = -1.0f;
		block[o + 21] = 1.0f;

		block[o + 22] = 1.0f;
		block[o + 23] = 1.0f;

		block[o + 24] = 0.0f;
		block[o + 25] = -1.0f;
		block[o + 26] = 0.0f;

		// Triangle 2

		// Vertex 3
		block[o + 27] = 1.0f;
		block[o + 28] = -1.0f;
		block[o + 29] = -1.0f;
		block[o + 30] = 1.0f;

		block[o + 31] = 1.0f;
		block[o + 32] = 1.0f;

		block[o + 33] = 0.0f;
		block[o + 34] = -1.0f;
		block[o + 35] = 0.0f;

		// Vertex 4
		block[o + 36] = 1.0f;
		block[o + 37] = -1.0f;
		block[o + 38] = 1.0f;
		block[o + 39] = 1.0f;

		block[o + 40] = 1.0f;
		block[o + 41] = 0.0f;

		block[o + 42] = 0.0f;
		block[o + 43] = -1.0f;
		block[o + 44] = 0.0f;

		// Vertex 5
		block[o + 45] = -1.0f;
		block[o + 46] = -1.0f;
		block[o + 47] = 1.0f;
		block[o + 48] = 1.0f;

		block[o + 49] = 0.0f;
		block[o + 50] = 0.0f;

		block[o + 51] = 0.0f;
		block[o + 52] = -1.0f;
		block[o + 53] = 0.0f;

		// TOP SIDE

		//   ______
		// |\\5   4|
		// |0\\    |
		// |  \\   |
		// |   \\  |
		// |    \\3|
		// |1__2_\\|

		// Triangle 1
		o = 54 * 5;

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

		// Vertex 1
		block[o + 9] = -1.0f;
		block[o + 10] = 1.0f;
		block[o + 11] = 1.0f;
		block[o + 12] = 1.0f;

		block[o + 13] = 0.0f;
		block[o + 14] = 1.0f;

		block[o + 15] = 0.0f;
		block[o + 16] = 1.0f;
		block[o + 17] = 0.0f;

		// Vertex 2
		block[o + 18] = 1.0f;
		block[o + 19] = 1.0f;
		block[o + 20] = 1.0f;
		block[o + 21] = 1.0f;

		block[o + 22] = 1.0f;
		block[o + 23] = 1.0f;

		block[o + 24] = 0.0f;
		block[o + 25] = 1.0f;
		block[o + 26] = 0.0f;

		// Triangle 2

		// Vertex 3
		block[o + 27] = 1.0f;
		block[o + 28] = 1.0f;
		block[o + 29] = 1.0f;
		block[o + 30] = 1.0f;

		block[o + 31] = 1.0f;
		block[o + 32] = 1.0f;

		block[o + 33] = 0.0f;
		block[o + 34] = 1.0f;
		block[o + 35] = 0.0f;

		// Vertex 4
		block[o + 36] = 1.0f;
		block[o + 37] = 1.0f;
		block[o + 38] = -1.0f;
		block[o + 39] = 1.0f;

		block[o + 40] = 1.0f;
		block[o + 41] = 0.0f;

		block[o + 42] = 0.0f;
		block[o + 43] = 1.0f;
		block[o + 44] = 0.0f;

		// Vertex 5
		block[o + 45] = -1.0f;
		block[o + 46] = 1.0f;
		block[o + 47] = -1.0f;
		block[o + 48] = 1.0f;

		block[o + 49] = 0.0f;
		block[o + 50] = 0.0f;

		block[o + 51] = 0.0f;
		block[o + 52] = 1.0f;
		block[o + 53] = 0.0f;

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 54 * 6, block, GL_STATIC_DRAW);
		checkGLError("glBufferData");

		// New mesh
		Mesh *mesh = new Mesh();
		mesh->vbo = vbo;
		mesh->numCoords = 54 * 6;
		mesh->floatsPerCoord = 9;

		// Add mesh to shape
		meshes.push_back(mesh);
	}
	else if (type == SHAPE_VOXELS)
	{
		voxels = new Voxels();
		voxels->init(sizeA, stringExtras);
		built = false;
		needsRebuild = true;
	}
}

void Shape::rebuild(TextureManager2 *texMan)
{
	if (type == SHAPE_VOXELS && voxels != nullptr)
	{
		// Clear meshes
		for (int i = 0; i < meshes.size(); i++)
		{
			glDeleteBuffers(1, (GLuint *)&meshes[i]->vbo);
			checkGLError("glDeleteBuffers");
		}

		meshes.clear();

		// Rebuild meshes
#ifdef DO_VERTEX_LIGHTS
//        Log("rebuild shape " + name);
		VertexLights vl;
		vl.process(voxels, texMan);
#endif
		voxels->build(texMan);

		meshes = voxels->getMeshes();
	}

	needsRebuild = false;
}

void Shape::free()
{
	if (numCoords > 0)
	{
	    glDeleteBuffers(1, (GLuint *)&vbo);
        checkGLError("glDeleteBuffers");
	}
	
	for (int i = 0; i < meshes.size(); i++)
    {
        glDeleteBuffers(1, (GLuint *)&meshes[i]->vbo);
        checkGLError("glDeleteBuffers");
    }
	
	meshes.clear();

	if (type == SHAPE_VOXELS && voxels != nullptr)
	{
		voxels->clear();
		voxels = nullptr;
	}
	
//    delete vertices;
}

void Shape::checkGLError(char *tag)
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

void Shape::save(FILE *f)
{
	setFile(f);

    write6Char('S', 'H', 'A', 'P', 'E', ' ');

    clearKVSet();
    addKV("name", name);
    addKV("type", type);
    addKV("sizeA", sizeA);
    addKV("sizeB", sizeB);
    addKV("sizeC", sizeC);
    addKV("sizeD", sizeD);
	addKV("sizeE", sizeE);
	addKV("sizeF", sizeF);
	addKV("sizeG", sizeG);
    writeKVSet();

    if (voxels != nullptr)
    {
        // Got voxels!
        writeInt(1);
        voxels->save ("", file);
    }
    else
    {
        // No voxels
        writeInt(0);
    }
}

void Shape::load(FILE *f)
{
	setFile(f);

    if (!read6CharEquals('S', 'H', 'A', 'P', 'E', ' '))
        return;

    readKVSet();
    name = getKVStr("name");
    type = (ObjShapeType)getKVInt("type");
    sizeA = getKVFloat("sizeA");
    sizeB = getKVFloat("sizeB");
    sizeC = getKVFloat("sizeC");
    sizeD = getKVFloat("sizeD");
	sizeE = getKVFloat("sizeE");
	sizeF = getKVFloat("sizeF");
	sizeG = getKVFloat("sizeG");

    // Do we have voxels to load?
    int gotVoxels = readInt();

    if (gotVoxels == 1)
    {
        // Load voxels
        voxels = new Voxels();
        voxels->load("", file);
    }
    else
    {
        generate(nullptr);
    }
}
