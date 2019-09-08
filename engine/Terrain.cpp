;/*w
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
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAI2$"fM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE U=$nSE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "GLIncludes.h"
#include "Terrain.h"
#include "DDLUtils.hpp"
#include <vector>

int Terrain::vbo = 0;
int Terrain::numCoords = 0;
int Terrain::valleySizeX = 0;
int Terrain::valleySizeZ = 0;
int Terrain::dropSizeX = 0;
int Terrain::dropSizeZ = 0;
int Terrain::bottomSizeX = 0;
int Terrain::bottomSizeZ = 0;
const int Terrain::verticesSize = 0;
float *Terrain::vertices;

void Terrain::generate(int sizexz, int valleySizeX, int valleySizeZ, int dropSizeX, int dropSizeZ, int bottomSizeX, int bottomSizeZ)
{
    Terrain::valleySizeX = valleySizeX;
	Terrain::valleySizeZ = valleySizeZ;
	Terrain::dropSizeX = dropSizeX;
	Terrain::dropSizeZ = dropSizeZ;
	Terrain::bottomSizeX = bottomSizeX;
	Terrain::bottomSizeZ = bottomSizeZ;
	
	Log("valleySizeX", valleySizeX);
    Log("valleySizeZ", valleySizeZ);
	Log("dropSizeX", dropSizeX);
    Log("dropSizeZ", dropSizeZ);
	Log("bottomSizeX", bottomSizeX);
    Log("bottomSizeZ", bottomSizeZ);

    // Generate VBO
    glGenBuffers(1, (GLuint *)&vbo);
    checkGLError("glGenBuffers");

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLError("glBindBuffer");

    // Initialize height map

    std::vector<std::vector<float>> heightMap(sizexz + 1, std::vector<float>(sizexz + 1));

    for (int z = 0; z < sizexz + 1; z++)
    {
        for (int x = 0; x < sizexz + 1; x++)
        {
            heightMap[z][x] = RandomFloat(0.0, 1.0);
        }
    }

    for (int z = sizexz / 2 - valleySizeZ / 2; z < sizexz / 2 + valleySizeZ / 2; z++)
    {
        for (int x = sizexz / 2 - valleySizeX / 2; x < sizexz / 2 + valleySizeX / 2; x++)
        {
			heightMap[z][x] = RandomFloat(0.00, 0.00);
		}
    }

	for (int z = sizexz / 2 - dropSizeZ / 2; z < sizexz / 2 + dropSizeZ / 2; z++)
    {
        for (int x = sizexz / 2 - dropSizeX / 2; x < sizexz / 2 + dropSizeX / 2; x++)
        {
			heightMap[z][x] = RandomFloat(-0.50, -0.50);
		}
    }

	for (int z = sizexz / 2 - bottomSizeZ / 2; z < sizexz / 2 + bottomSizeZ / 2; z++)
    {
        for (int x = sizexz / 2 - bottomSizeX / 2; x < sizexz / 2 + bottomSizeX / 2; x++)
        {
			heightMap[z][x] = RandomFloat(-1.00, -1.00);
		}
    }
	
	for (int x = 0; x < sizexz + 1; x++)
	{
		heightMap[0][x] = 0.0;
		heightMap[sizexz][x] = 0.0;
	}

	for (int z = 0; z < sizexz + 1; z++)
	{
		heightMap[z][0] = 0.0;
		heightMap[z][sizexz] = 0.0;
	}

	// Allocate memory for x * z quads

    numCoords = 12 * 6 * sizexz * sizexz;
    vertices = new float[numCoords];
    int o = 0;
    float quadSizeXZ = 2.0 / (float)sizexz;

    // Create quads

    for (int z = 0; z < sizexz; z++)
    {
        for (int x = 0; x < sizexz; x++)
        {
/*            if ((x >= (sizexz / 2) - (holeSize / 2) && x <= (sizexz / 2) + (holeSize / 2)) &&
                    (z >= (sizexz / 2) - (holeSize / 2) && z <= (sizexz / 2) + (holeSize / 2)))
                break;*/

            // TOP SIDE

            //   ______
            // |\\5   4|
            // |0\\    |
            // |  \\   |
            // |   \\  |
            // |    \\3|
            // |1__2_\\|

            float uLeftX = -1.0 + quadSizeXZ * (float)x;
            float uLeftZ = -1.0 + quadSizeXZ * (float)z;
            float uLeftY = heightMap[z][x];

            float lLeftX = -1.0 + quadSizeXZ * (float)x;
            float lLeftZ = -1.0 + quadSizeXZ * (float)z + quadSizeXZ;
            float lLeftY = heightMap[z + 1][x];

            float lRightX = -1.0 + quadSizeXZ * (float)x + quadSizeXZ;
            float lRightZ = -1.0 + quadSizeXZ * (float)z + quadSizeXZ;
            float lRightY = heightMap[z + 1][x + 1];

            float uRightX = -1.0 + quadSizeXZ * (float)x + quadSizeXZ;
            float uRightZ = -1.0 + quadSizeXZ * (float)z;
            float uRightY = heightMap[z][x + 1];

            // Vertex 0
            vertices[o + 0] = uLeftX;
            vertices[o + 1] = uLeftY;
            vertices[o + 2] = uLeftZ;
            vertices[o + 3] = 1.0f;

            vertices[o + 4] = 0.0f;
            vertices[o + 5] = 0.0f;

			vertices[o + 6] = 0.0f;
			vertices[o + 7] = 0.0f;
			vertices[o + 8] = 0.0f;

            vertices[o + 9] = 1.0f;
            vertices[o + 10] = 1.0f;
            vertices[o + 11] = 1.0f;

			// Vertex 1
            vertices[o + 12] = lLeftX;
            vertices[o + 13] = lLeftY;
            vertices[o + 14] = lLeftZ;
            vertices[o + 15] = 1.0f;

            vertices[o + 16] = 0.0f;
            vertices[o + 17] = 1.0f;

			vertices[o + 18] = 0.0f;
			vertices[o + 19] = 0.0f;
			vertices[o + 20] = 0.0f;

            vertices[o + 21] = 1.0f;
            vertices[o + 22] = 1.0f;
            vertices[o + 23] = 1.0f;

			// Vertex 2
            vertices[o + 24] = lRightX;
            vertices[o + 25] = lRightY;
            vertices[o + 26] = lRightZ;
            vertices[o + 27] = 1.0f;

            vertices[o + 28] = 1.0f;
            vertices[o + 29] = 1.0f;

			vertices[o + 30] = 0.0f;
			vertices[o + 31] = 0.0f;
			vertices[o + 32] = 0.0f;

            vertices[o + 33] = 1.0f;
            vertices[o + 34] = 1.0f;
            vertices[o + 35] = 1.0f;

            // Triangle 2

            // Vertex 3
            vertices[o + 36] = lRightX;
            vertices[o + 37] = lRightY;
            vertices[o + 38] = lRightZ;
            vertices[o + 39] = 1.0f;

            vertices[o + 40] = 1.0f;
            vertices[o + 41] = 1.0f;

			vertices[o + 42] = 0.0f;
			vertices[o + 43] = 0.0f;
			vertices[o + 44] = 0.0f;

            vertices[o + 45] = 1.0f;
            vertices[o + 46] = 1.0f;
            vertices[o + 47] = 1.0f;

			// Vertex 4
            vertices[o + 48] = uRightX;
            vertices[o + 49] = uRightY;
            vertices[o + 50] = uRightZ;
            vertices[o + 51] = 1.0f;

            vertices[o + 52] = 1.0f;
            vertices[o + 53] = 0.0f;

			vertices[o + 54] = 0.0f;
			vertices[o + 55] = 0.0f;
			vertices[o + 56] = 0.0f;
			
			vertices[o + 57] = 1.0f;
			vertices[o + 58] = 1.0f;
			vertices[o + 59] = 1.0f;

			// Vertex 5
            vertices[o + 60] = uLeftX;
            vertices[o + 61] = uLeftY;
            vertices[o + 62] = uLeftZ;
            vertices[o + 63] = 1.0f;

            vertices[o + 64] = 0.0f;
            vertices[o + 65] = 0.0f;

			vertices[o + 66] = 0.0f;
			vertices[o + 67] = 0.0f;
			vertices[o + 68] = 0.0f;

            vertices[o + 69] = 1.0f;
            vertices[o + 70] = 1.0f;
            vertices[o + 71] = 1.0f;

			o += 12 * 6;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numCoords, vertices, GL_STATIC_DRAW);
    checkGLError("glBufferData");

//    delete vertices;
}

void Terrain::checkGLError(char *tag)
{
#ifdef DEBUG_BUILD
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
#endif
}
