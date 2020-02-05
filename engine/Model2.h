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

#ifndef FATELESS_MODEL2_H
#define FATELESS_MODEL2_H

#include <vector>
#include <string>
#include "../thirdparty/glm/glm.hpp"
#include "Mesh.h"

#define CALC_MODEL_NORMALS

typedef enum ModelState
{
	MODEL_NOT_LOADED,
	MODEL_LOADING,
	MODEL_LOADED,
	MODEL_READY
} ModelState;

class Model2 {
public:
    void load(std::string fileName, int vao);
    void load_process();
	void release();
    void checkGLError(char *tag);

    std::string name;
    bool loaded = false;
    int numMeshes = 0;
    std::vector<Mesh*> meshes;
    float minX = 0.0;
    float maxX = 0.0;
    float minY = 0.0;
    float maxY = 0.0;
    float minZ = 0.0;
    float maxZ = 0.0;

	glm::vec4 proportionScale;
	glm::vec3 offset;
	float yaw = 0;
	float pitch = 0;
	float roll = 0;
	
	ModelState state = MODEL_NOT_LOADED;
	std::string filename;
	int vao;
};


#endif //FATELESS_MODEL2_H
