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

#ifndef FATELESS_MESH_H
#define FATELESS_MESH_H

#include "../thirdparty/glm/glm.hpp"
#include <vector>
#include "FileIO.h"

#define BONES_PER_VEREX 4

struct VertexBoneData
{
    int ids[BONES_PER_VEREX];
    float weights[BONES_PER_VEREX];
};

class Mesh: FileIO
{
public:
    void clear();
	void save(std::string fname, FILE *f);
	void load(std::string fname, FILE *f);

	int index = 0;
    int numVertices = 0;
    int vbo = -1;
    int indexVBO = 0;
    bool hasTexCoords = false;
    float *data;
    int dataLength = 0;
    int *indexData;
    int indexDataLength = 0;
    int numIndexes = 0;
    int numPolys = 0;
	int numCoords = 0;
	int floatsPerCoord = 0;
	glm::vec4 color;
	std::string texture;
	int glTexID = -1;
	std::vector<glm::vec3> vertices;
};

#endif //FATELESS_MODEL2_H
