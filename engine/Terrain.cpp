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
int Terrain::holeSizeX = 0;
int Terrain::holeSizeZ = 0;
const int Terrain::verticesSize = 0;
float *Terrain::vertices;

void Terrain::generate(int sizexz, int holeSizeX, int holeSizeZ)
{
    Terrain::holeSizeX = holeSizeX;
	Terrain::holeSizeZ = holeSizeZ;

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

    for (int z = sizexz / 2 - holeSizeZ / 2; z < sizexz / 2 + holeSizeZ / 2; z++)
    {
        for (int x = sizexz / 2 - holeSizeX / 2; x < sizexz / 2 + holeSizeX / 2; x++)
        {
			heightMap[z][x] = RandomFloat(0.00, 0.00);
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

    numCoords = 6 * 6 * sizexz * sizexz;
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

            // Vertex 1
            vertices[o + 6] = lLeftX;
            vertices[o + 7] = lLeftY;
            vertices[o + 8] = lLeftZ;
            vertices[o + 9] = 1.0f;

            vertices[o + 10] = 0.0f;
            vertices[o + 11] = 1.0f;

            // Vertex 2
            vertices[o + 12] = lRightX;
            vertices[o + 13] = lRightY;
            vertices[o + 14] = lRightZ;
            vertices[o + 15] = 1.0f;

            vertices[o + 16] = 1.0f;
            vertices[o + 17] = 1.0f;

            // Triangle 2

            // Vertex 3
            vertices[o + 18] = lRightX;
            vertices[o + 19] = lRightY;
            vertices[o + 20] = lRightZ;
            vertices[o + 21] = 1.0f;

            vertices[o + 22] = 1.0f;
            vertices[o + 23] = 1.0f;

            // Vertex 4
            vertices[o + 24] = uRightX;
            vertices[o + 25] = uRightY;
            vertices[o + 26] = uRightZ;
            vertices[o + 27] = 1.0f;

            vertices[o + 28] = 1.0f;
            vertices[o + 29] = 0.0f;

            // Vertex 5
            vertices[o + 30] = uLeftX;
            vertices[o + 31] = uLeftY;
            vertices[o + 32] = uLeftZ;
            vertices[o + 33] = 1.0f;

            vertices[o + 34] = 0.0f;
            vertices[o + 35] = 0.0f;

            o += 6 * 6;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numCoords, vertices, GL_STATIC_DRAW);
    checkGLError("glBufferData");

//    delete vertices;
}

void Terrain::checkGLError(char *tag)
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
