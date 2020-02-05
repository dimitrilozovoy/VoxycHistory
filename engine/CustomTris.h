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

#pragma once

#include <string>
#include <vector>
#include "../thirdparty/glm/glm.hpp"
#include "Common.h"
#include "Mesh.h"
#include "GLIncludes.h"

typedef struct Triangle
{
	float ax = 0;
	float ay = 0;
	float az = 0;
	float bx = 0;
	float by = 0;
	float bz = 0;
	float cx = 0;
	float cy = 0;
	float cz = 0;
	float au = 0;
	float av = 0;
	float bu = 0;
	float bv = 0;
	float cu = 0;
	float cv = 0;
} Triangle;

class CustomTris
{
	public:
	    void cquad1(std::string texture, float x, float y, float z, float sizex, float sizey, float pitch, float yaw, float roll);
	    void addVertex(std::string texture, float x, float y, float z, float w, float u, float v, float nx, float ny, float nz, float r, float g, float b);
	    std::vector<Mesh*> getMeshes();
	    void checkGLError(char *tag);
		
	private:
		std::map<std::string, VertexBuffer*> buffers;
};
