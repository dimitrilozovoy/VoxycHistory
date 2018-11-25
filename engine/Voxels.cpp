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

#include "Voxels.h"
#include <cstdlib>
#include "GLIncludes.h"
#include "DDLUtils.hpp"
#include "TextureManager2.h"

#define STACK_HEIGHT_ALLOC_PAD	1
#define BUFFER_ALLOC_INCREMENT 1000000

//int Voxels::cursor = 0;

void Voxels::init(int size, std::map<std::string, std::string> *extraStrings)
{
	if (initialized)
		clear();
		
//	Log("init size", size);

	this->size = size;
	this->extraStrings = extraStrings;

	stacks = new VoxelStack[size * size];

	for (int z = 0; z < size; z++)
	{
		for (int x = 0; x < size; x++)
		{
			stacks[z * size + x].height = 0;
			stacks[z * size + x].voxels = nullptr;
		}
	}
	
	minx = size / 2.0;
	maxx = size / 2.0;
	miny = 0;
	maxy = 0;
	minz = size / 2.0;
	maxz = size / 2.0;
	
	voxelTextures[0] = "";
}

void Voxels::clear()
{
	for (int z = 0; z < size; z++)
	{
		for (int x = 0; x < size; x++)
		{
			VoxelStack *stack = getStack(x, z);

			if (/*stack->height > 0 &&*/ stack->voxels != nullptr)
			{
				delete stack->voxels;
				stack->voxels = NULL;
				stack->height = 0;
			}
		}
	}
	
	minx = size / 2.0;
	maxx = size / 2.0;
	miny = 0;
	maxy = 0;
	minz = size / 2.0;
	maxz = size / 2.0;
	
	initialized = false;
}

void Voxels::set(int x, int y, int z, char value)
{
	VoxelStack *stack = getStack(x, z);

    if (g_simpleMode)
    {
        // HACK
    	if (x == 0 && y == 0 && z == 0)
	    	return;
    }

	if (stack == NULL)
		return;

	if (stack->height <= y)
	{
		setStackHeight(x, z, y + STACK_HEIGHT_ALLOC_PAD);
	}

	stack->voxels[y].texture = value;
	
	if (x < minx)
		minx = x;
	if (x >= maxx)
		maxx = x + 2;
	if (z < minz)
		minz = z;
	if (z >= maxx)
		maxz = z + 2;
	if (y < miny)
		miny = y;
	if (y >= maxy)
		maxy = y + 2;
}

char Voxels::get(int x, int y, int z)
{
	VoxelStack *stack = getStack(x, z);

	if (stack == NULL)
		return 0;

	if (x < 0 || y < 0 || z < 0 || stack->height <= y)
	{
		return 0;
	}

	return stack->voxels[y].texture;
}

VoxelStack *Voxels::getStack(int x, int z)
{
	if (x < 0 || z < 0 || x >= size || z >= size)
		return NULL;

	return &stacks[z * size + x];
}

void Voxels::setStackHeight(int x, int z, int height)
{
	VoxelStack *stack = getStack(x, z);

	if (stack == NULL)
		return;

	Voxel *newVoxels = new Voxel[height];

	// Clear the new stack
	for (int y = 0; y < height; y++)
	{
		newVoxels[y].texture = 0;
/*		newVoxels[y].xofs = 0;
		newVoxels[y].yofs = 0;
		newVoxels[y].zofs = 0;
		newVoxels[y].r = 1.0;
		newVoxels[y].g = 1.0;
		newVoxels[y].b = 1.0;*/
	}

	// Copy old voxel stack into new one
	for (int y = 0; y < stack->height; y++)
	{
		if (stack->voxels != nullptr)
		{
			newVoxels[y].texture = stack->voxels[y].texture;
			/*		newVoxels[y].xofs = stack->voxels[y].xofs;
					newVoxels[y].yofs = stack->voxels[y].yofs;
					newVoxels[y].zofs = stack->voxels[y].zofs;
					newVoxels[y].r = stack->voxels[y].r;
					newVoxels[y].g = stack->voxels[y].g;
					newVoxels[y].b = stack->voxels[y].b;*/
		}
	}

	if (/*stack->height > 0 && */stack->voxels != NULL)
	{
		delete stack->voxels;
	}

	stack->voxels = newVoxels;
	stack->height = height;
}

void Voxels::build(TextureManager2 *texMan)
{
//	Log("Build(): Start");

	calcMinMax();
	
/*	minx = 0;
	maxx = size;
	miny = 0;
	maxy = size;
	minz = 0;
	maxz = size;*/

	// Clear buffers
	for (const auto &pair : buffers)
	{
		VertexBuffer *buf = pair.second;

		if (buf->vertices != nullptr)
		{
			free(buf->vertices);
			buf->vertices = nullptr;
		}
	}

	buffers.clear();

	std::string texture = "";

	char *doneMap;
	doneMap = (char *)malloc(size * size);

	// Find max Y
//	int maxY = maxy;

/*	for (int s = 0; s < size * size; s++)
	{
		int sizeY = stacks[s].height;

		if (sizeY > maxY)
			maxY = sizeY;
	}*/

	//
	// Y PLANE FROM TOP
	//

	for (int y = miny; y < maxy; y++)
	{
		// Reset the done map; nothing is done on this Y plane yet
		for (int z = 0; z < size; z++)
		{
			for (int x = 0; x < size; x++)
			{
				doneMap[z * size + x] = 0;
			}
		}

		// Go through the Y plane and construct necessary quads
		for (int z = minz; z < maxz; z++)
		{
			for (int x = minx; x < maxx; x++)
			{
				if (stacks[z * size + x].height == 0)
					continue;

				if (doneMap[z * size + x] == 0)
				{
					// Figure out how far we can expand the quad from this point
					int qstartz = z;
					int qstartx = x;
					int qendz = z;
					int qendx = x;

					char tex = get(x, y, z);

					texture = voxelTextures[tex];
                    Texture *t = texMan->find(texture);

					if (tex != 0)
					{
						// Expand quad along X
						while (qendx < size && get(qendx, y, z) == tex && get(qendx, y + 1, z) == 0 && doneMap[qendz * size + qendx] == 0)
						{
							qendx++;
						}

						// Expand quad along Z
						bool allTexsMatchAlongX = true;

						while (qendz < size && allTexsMatchAlongX)
						{
							int qendx2 = x;

							while (qendx2 < size && qendx2 < qendx && get(qendx2, y, qendz) == tex && get(qendx2, y + 1, qendz) == 0 && doneMap[qendz * size + qendx2] == 0)
							{
								qendx2++;
							}

							if (qendx2 == qendx)
							{
								allTexsMatchAlongX = true;
								qendz++;
							}
							else
								allTexsMatchAlongX = false;
						}

						// We calculated the quad; now create vertices for it
						float voxSize = 2.0 / (float)size;

						Vertex ll, lr, ur, ul;

						ll.x = -1.0 + (float)qstartx * voxSize;
						ll.y = -1.0 + (float)y * voxSize + voxSize;
						ll.z = -1.0 + (float)qendz * voxSize;
						ll.w = 1.0;

						lr.x = -1.0 + (float)qendx * voxSize;
						lr.y = -1.0 + (float)y * voxSize + voxSize;
						lr.z = -1.0 + (float)qendz * voxSize;
						lr.w = 1.0;

						ur.x = -1.0 + (float)qendx * voxSize;
						ur.y = -1.0 + (float)y * voxSize + voxSize;
						ur.z = -1.0 + (float)qstartz * voxSize;
						ur.w = 1.0;

						ul.x = -1.0 + (float)qstartx * voxSize;
						ul.y = -1.0 + (float)y * voxSize + voxSize;
						ul.z = -1.0 + (float)qstartz * voxSize;
						ul.w = 1.0;

//						Log("addQuad y from top");

						addQuad(texture, ll, lr, ur, ul, 1, t->texSpanX, t->texSpanY);

						// Mark the voxels for this quad as done
						for (int z = qstartz; z < qendz; z++)
						{
							for (int x = qstartx; x < qendx; x++)
							{
								doneMap[z * size + x] = (char)1;
							}
						}
					}
				}
			}
		}
	}

	//
	// Z PLANE FROM FRONT
	//

	for (int z = minz; z < maxz; z++)
	{
		// Reset the done map; nothing is done on this Y plane yet
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				doneMap[y * size + x] = 0;
			}
		}

		// Go through the Z plane and construct necessary quads
		for (int y = miny; y < maxy; y++)
		{
			for (int x = minx; x < maxx; x++)
			{
				if (stacks[z * size + x].height == 0)
					continue;

				if (doneMap[y * size + x] == 0)
				{
					// Figure out how far we can expand the quad from this point
					int qstarty = y;
					int qstartx = x;
					int qendy = y;
					int qendx = x;

					char tex = get(x, y, z);

					texture = voxelTextures[tex];
                    Texture *t = texMan->find(texture);

					if (tex != 0)
					{
						// Expand quad along X
						while (qendx < size && get(qendx, qendy, z) == tex && get(qendx, qendy, z + 1) == 0 && doneMap[qendy * size + qendx] == 0)
						{
							qendx++;
						}

						// Expand quad along Y
						bool allTexsMatchAlongX = true;

						while (qendy < size && allTexsMatchAlongX)
						{
							int qendx2 = x;

							while (qendx2 < size && qendx2 < qendx && get(qendx2, qendy, z) == tex && get(qendx2, qendy, z + 1) == 0&& doneMap[qendy * size + qendx2] == 0)
							{
								qendx2++;
							}

							if (qendx2 == qendx)
							{
								allTexsMatchAlongX = true;
								qendy++;
							}
							else
								allTexsMatchAlongX = false;
						}

						// We calculated the quad; now create vertices for it
						float voxSize = 2.0 / (float)size;

						Vertex ll, lr, ur, ul;

						ll.x = -1.0 + (float)qstartx * voxSize;
						ll.y = -1.0 + (float)qstarty * voxSize;
						ll.z = -1.0 + (float)z * voxSize + voxSize;
						ll.w = 1.0;

						lr.x = -1.0 + (float)qendx * voxSize;
						lr.y = -1.0 + (float)qstarty * voxSize;
						lr.z = -1.0 + (float)z * voxSize + voxSize;
						lr.w = 1.0;

						ur.x = -1.0 + (float)qendx * voxSize;
						ur.y = -1.0 + (float)qendy * voxSize;
						ur.z = -1.0 + (float)z * voxSize + voxSize;
						ur.w = 1.0;

						ul.x = -1.0 + (float)qstartx * voxSize;
						ul.y = -1.0 + (float)qendy * voxSize;
						ul.z = -1.0 + (float)z * voxSize + voxSize;
						ul.w = 1.0;

//						Log("addQuad z from front");

						addQuad(texture, ll, lr, ur, ul, 2, t->texSpanX, t->texSpanY);

						// Mark the voxels for this quad as done
						for (int y = qstarty; y < qendy; y++)
						{
							for (int x = qstartx; x < qendx; x++)
							{
								doneMap[y * size + x] = (char)1;
							}
						}
					}
				}
			}
		}
	}

	//
	// X PLANE FROM THE RIGHT
	//

	for (int x = minx; x < maxx; x++)
	{
		// Reset the done map; nothing is done on this Y plane yet
		for (int z = 0; z < size; z++)
		{
			for (int y = 0; y < size; y++)
			{
				doneMap[z * size + y] = 0;
			}
		}

		// Go through the X plane and construct necessary quads
		for (int z = minz; z < maxz; z++)
		{
			for (int y = miny; y < maxy; y++)
			{
				if (stacks[z * size + x].height == 0)
					continue;

				if (doneMap[z * size + y] == 0)
				{
					// Figure out how far we can expand the quad from this point
					int qstartz = z;
					int qstarty = y;
					int qendz = z;
					int qendy = y;

					char tex = get(x, y, z);

					texture = voxelTextures[tex];
                    Texture *t = texMan->find(texture);

					if (tex != 0)
					{
						// Expand quad along Z
						while (qendz < size && get(x, qendy, qendz) == tex && get(x + 1, qendy, qendz) == 0 && doneMap[qendz * size + qendy] == 0)
						{
							qendz++;
						}

						// Expand quad along Y
						bool allTexsMatchAlongZ = true;

						while (qendy < size && allTexsMatchAlongZ)
						{
							int qendz2 = z;

							while (qendz2 < size && qendz2 < qendz && get(x, qendy, qendz2) == tex && get(x + 1, qendy, qendz2) == 0 && doneMap[qendz2 * size + qendy] == 0)
							{
								qendz2++;
							}

							if (qendz2 == qendz)
							{
								allTexsMatchAlongZ = true;
								qendy++;
							}
							else
								allTexsMatchAlongZ = false;
						}

						// We calculated the quad; now create vertices for it
						float voxSize = 2.0 / (float)size;

						Vertex ll, lr, ur, ul;

						ll.x = -1.0 + (float)x * voxSize + voxSize;
						ll.y = -1.0 + (float)qstarty * voxSize;
						ll.z = -1.0 + (float)qendz * voxSize;
						ll.w = 1.0;

						lr.x = -1.0 + (float)x * voxSize + voxSize;
						lr.y = -1.0 + (float)qstarty * voxSize;
						lr.z = -1.0 + (float)qstartz * voxSize;
						lr.w = 1.0;

						ur.x = -1.0 + (float)x * voxSize + voxSize;
						ur.y = -1.0 + (float)qendy * voxSize;
						ur.z = -1.0 + (float)qstartz * voxSize;
						ur.w = 1.0;

						ul.x = -1.0 + (float)x * voxSize + voxSize;
						ul.y = -1.0 + (float)qendy * voxSize;
						ul.z = -1.0 + (float)qendz * voxSize;
						ul.w = 1.0;

//						Log("addQuad x from right");

						addQuad(texture, ll, lr, ur, ul, 3, t->texSpanX, t->texSpanY);

						// Mark the voxels for this quad as done
						for (int z = qstartz; z < qendz; z++)
						{
							for (int y = qstarty; y < qendy; y++)
							{
								doneMap[z * size + y] = (char)1;
							}
						}
					}
				}
			}
		}
	}
	
	//
	// Y PLANE FROM BOTTOM
	//

	for (int y = miny; y < maxy; y++)
	{
		// Reset the done map; nothing is done on this Y plane yet
		for (int z = 0; z < size; z++)
		{
			for (int x = 0; x < size; x++)
			{
				doneMap[z * size + x] = 0;
			}
		}

		// Go through the Y plane and construct necessary quads
		for (int z = minz; z < maxz; z++)
		{
			for (int x = minx; x < maxx; x++)
			{
				if (stacks[z * size + x].height == 0)
					continue;

				if (doneMap[z * size + x] == 0)
				{
					// Figure out how far we can expand the quad from this point
					int qstartz = z;
					int qstartx = x;
					int qendz = z;
					int qendx = x;

					char tex = get(x, y, z);

					texture = voxelTextures[tex];
                    Texture *t = texMan->find(texture);

					if (tex != 0)
					{
						// Expand quad along X
						while (qendx < size && get(qendx, y, z) == tex && get(qendx, y - 1, z) == 0 && doneMap[qendz * size + qendx] == 0)
						{
							qendx++;
						}

						// Expand quad along Z
						bool allTexsMatchAlongX = true;

						while (qendz < size && allTexsMatchAlongX)
						{
							int qendx2 = x;

							while (qendx2 < size && qendx2 < qendx && get(qendx2, y, qendz) == tex && get(qendx2, y - 1, qendz) == 0 && doneMap[qendz * size + qendx2] == 0)
							{
								qendx2++;
							}

							if (qendx2 == qendx)
							{
								allTexsMatchAlongX = true;
								qendz++;
							}
							else
								allTexsMatchAlongX = false;
						}

						// We calculated the quad; now create vertices for it
						float voxSize = 2.0 / (float)size;

						Vertex ll, lr, ur, ul;

						ll.x = -1.0 + (float)qstartx * voxSize;
						ll.y = -1.0 + (float)y * voxSize;
						ll.z = -1.0 + (float)qstartz * voxSize;
						ll.w = 1.0;

						lr.x = -1.0 + (float)qendx * voxSize;
						lr.y = -1.0 + (float)y * voxSize;
						lr.z = -1.0 + (float)qstartz * voxSize;
						lr.w = 1.0;

						ur.x = -1.0 + (float)qendx * voxSize;
						ur.y = -1.0 + (float)y * voxSize;
						ur.z = -1.0 + (float)qendz * voxSize;
						ur.w = 1.0;

						ul.x = -1.0 + (float)qstartx * voxSize;
						ul.y = -1.0 + (float)y * voxSize;
						ul.z = -1.0 + (float)qendz * voxSize;
						ul.w = 1.0;

//						Log("addQuad y from bottom");

						addQuad(texture, ll, lr, ur, ul, 4, t->texSpanX, t->texSpanY);

						// Mark the voxels for this quad as done
						for (int z = qstartz; z < qendz; z++)
						{
							for (int x = qstartx; x < qendx; x++)
							{
								doneMap[z * size + x] = (char)1;
							}
						}
					}
				}
			}
		}
	}

	//
	// X PLANE FROM THE LEFT
	//

	for (int x = minx; x < maxx ; x++)
	{
		// Reset the done map; nothing is done on this Y plane yet
		for (int z = 0; z < size; z++)
		{
			for (int y = 0; y < size; y++)
			{
				doneMap[z * size + y] = 0;
			}
		}

		// Go through the X plane and construct necessary quads
		for (int z = minz; z < maxz; z++)
		{
			for (int y = miny; y < maxy; y++)
			{
				if (stacks[z * size + x].height == 0)
					continue;

				if (doneMap[z * size + y] == 0)
				{
					// Figure out how far we can expand the quad from this point
					int qstartz = z;
					int qstarty = y;
					int qendz = z;
					int qendy = y;

					char tex = get(x, y, z);

					texture = voxelTextures[tex];
                    Texture *t = texMan->find(texture);

					if (tex != 0)
					{
						// Expand quad along Z
						while (qendz < size && get(x, qendy, qendz) == tex && get(x - 1, qendy, qendz) == 0 && doneMap[qendz * size + qendy] == 0)
						{
							qendz++;
						}

						// Expand quad along Y
						bool allTexsMatchAlongZ = true;

						while (qendy < size && allTexsMatchAlongZ)
						{
							int qendz2 = z;

							while (qendz2 < size && qendz2 < qendz && get(x, qendy, qendz2) == tex && get(x - 1, qendy, qendz2) == 0 && doneMap[qendz2 * size + qendy] == 0)
							{
								qendz2++;
							}

							if (qendz2 == qendz)
							{
								allTexsMatchAlongZ = true;
								qendy++;
							}
							else
								allTexsMatchAlongZ = false;
						}

						// We calculated the quad; now create vertices for it
						float voxSize = 2.0 / (float)size;

						Vertex ll, lr, ur, ul;

						ll.x = -1.0 + (float)x * voxSize;
						ll.y = -1.0 + (float)qstarty * voxSize;
						ll.z = -1.0 + (float)qstartz * voxSize;
						ll.w = 1.0;

						lr.x = -1.0 + (float)x * voxSize;
						lr.y = -1.0 + (float)qstarty * voxSize;
						lr.z = -1.0 + (float)qendz * voxSize;
						lr.w = 1.0;

						ur.x = -1.0 + (float)x * voxSize;
						ur.y = -1.0 + (float)qendy * voxSize;
						ur.z = -1.0 + (float)qendz * voxSize;
						ur.w = 1.0;

						ul.x = -1.0 + (float)x * voxSize;
						ul.y = -1.0 + (float)qendy * voxSize;
						ul.z = -1.0 + (float)qstartz * voxSize;
						ul.w = 1.0;

//						Log("addQuad x from left");

						addQuad(texture, ll, lr, ur, ul, 5, t->texSpanX, t->texSpanY);

						// Mark the voxels for this quad as done
						for (int z = qstartz; z < qendz; z++)
						{
							for (int y = qstarty; y < qendy; y++)
							{
								doneMap[z * size + y] = (char)1;
							}
						}
					}
				}
			}
		}
	}

	//
	// Z PLANE FROM BACK
	//

	for (int z = minz; z < maxz; z++)
	{
		// Reset the done map; nothing is done on this Y plane yet
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				doneMap[y * size + x] = 0;
			}
		}

		// Go through the Z plane and construct necessary quads
		for (int y = miny; y < maxy; y++)
		{
			for (int x = minx; x < maxx; x++)
			{
				if (stacks[z * size + x].height == 0)
					continue;

				if (doneMap[y * size + x] == 0)
				{
					// Figure out how far we can expand the quad from this point
					int qstarty = y;
					int qstartx = x;
					int qendy = y;
					int qendx = x;

					char tex = get(x, y, z);

					texture = voxelTextures[tex];
                    Texture *t = texMan->find(texture);

					if (tex != 0)
					{
						// Expand quad along X
						while (qendx < size && get(qendx, qendy, z) == tex && get(qendx, qendy, z - 1) == 0 && doneMap[qendy * size + qendx] == 0)
						{
							qendx++;
						}

						// Expand quad along Y
						bool allTexsMatchAlongX = true;

						while (qendy < size && allTexsMatchAlongX)
						{
							int qendx2 = x;

							while (qendx2 < size && qendx2 < qendx && get(qendx2, qendy, z) == tex && get(qendx2, qendy, z - 1) == 0 && doneMap[qendy * size + qendx2] == 0)
							{
								qendx2++;
							}

							if (qendx2 == qendx)
							{
								allTexsMatchAlongX = true;
								qendy++;
							}
							else
								allTexsMatchAlongX = false;
						}

						// We calculated the quad; now create vertices for it
						float voxSize = 2.0 / (float)size;

						Vertex ll, lr, ur, ul;

						ll.x = -1.0 + (float)qendx * voxSize;
						ll.y = -1.0 + (float)qstarty * voxSize;
						ll.z = -1.0 + (float)z * voxSize;
						ll.w = 1.0;

						lr.x = -1.0 + (float)qstartx * voxSize;
						lr.y = -1.0 + (float)qstarty * voxSize;
						lr.z = -1.0 + (float)z * voxSize;
						lr.w = 1.0;

						ur.x = -1.0 + (float)qstartx * voxSize;
						ur.y = -1.0 + (float)qendy * voxSize;
						ur.z = -1.0 + (float)z * voxSize;
						ur.w = 1.0;

						ul.x = -1.0 + (float)qendx * voxSize;
						ul.y = -1.0 + (float)qendy * voxSize;
						ul.z = -1.0 + (float)z * voxSize;
						ul.w = 1.0;

//						Log("addQuad z from back");

						addQuad(texture, ll, lr, ur, ul, 6, t->texSpanX, t->texSpanY);

						// Mark the voxels for this quad as done
						for (int y = qstarty; y < qendy; y++)
						{
							for (int x = qstartx; x < qendx; x++)
							{
								doneMap[y * size + x] = (char)1;
							}
						}
					}
				}
			}
		}
	}

	free(doneMap);
}

void Voxels::addQuad(std::string texture, Vertex ll, Vertex lr, Vertex ur, Vertex ul, int dir, float texSpanX, float texSpanY)
{
	// HACK
/*	if (ll.x == 1 || ll.y == 1 || ll.z == 1
			|| lr.x == 1 || lr.y == 1 || lr.z == 1
			|| ur.x == 1 || ur.y == 1 || ur.z == 1
			|| ul.x == 1 || ul.y == 1 || ul.z == 1
			)
		return;*/
		
//	Log("addQuad");

	// addVertex(texture, ll.x, ll.y, ll.z, ll.w, (ll.x + 1.0) / 2.0, 0.0);

//	addVertex(texture, ll.x, ll.y, ll.z, ll.w, (ll.x + 1.0) / 2.0, (ll.y + 1.0) / 2.0);
	
/*	float texspan = 1.0;
	
	if (g_useLegacyTextureSpan)
		texspan = 6.0;*/

	if (dir == 1)
	{
		// Y PLANE FROM TOP
		
	    addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.x + 1.0) / 2.0) * texSpanX, ((ll.z + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, lr.x, lr.y, lr.z, lr.w, ((lr.x + 1.0) / 2.0) * texSpanX, ((lr.z + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.x + 1.0) / 2.0) * texSpanX, ((ur.z + 1.0) / 2.0) * texSpanY);

	    addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.x + 1.0) / 2.0) * texSpanX, ((ll.z + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.x + 1.0) / 2.0) * texSpanX, ((ur.z + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ul.x, ul.y, ul.z, ul.w, ((ul.x + 1.0) / 2.0) * texSpanX, ((ul.z + 1.0) / 2.0) * texSpanY);
	}
	else if (dir == 2)
	{
		// Z PLANE FROM FRONT
		
        addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.x + 1.0) / 2.0) * texSpanX, ((ll.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, lr.x, lr.y, lr.z, lr.w, ((lr.x + 1.0) / 2.0) * texSpanX, ((lr.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.x + 1.0) / 2.0) * texSpanX, ((ur.y + 1.0) / 2.0) * texSpanY);

	    addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.x + 1.0) / 2.0) * texSpanX, ((ll.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.x + 1.0) / 2.0) * texSpanX, ((ur.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ul.x, ul.y, ul.z, ul.w, ((ul.x + 1.0) / 2.0) * texSpanX, ((ul.y + 1.0) / 2.0) * texSpanY);
	}
	else if (dir == 3)
	{
		// X PLANE FROM THE RIGHT
		
        addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.z + 1.0) / 2.0) * texSpanX, ((ll.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, lr.x, lr.y, lr.z, lr.w, ((lr.z + 1.0) / 2.0) * texSpanX, ((lr.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.z + 1.0) / 2.0) * texSpanX, ((ur.y + 1.0) / 2.0) * texSpanY);

	    addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.z + 1.0) / 2.0) * texSpanX, ((ll.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.z + 1.0) / 2.0) * texSpanX, ((ur.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ul.x, ul.y, ul.z, ul.w, ((ul.z + 1.0) / 2.0) * texSpanX, ((ul.y + 1.0) / 2.0) * texSpanY);
	}
	else if (dir == 4)
	{
		// Y PLANE FROM BOTTOM
		
	    addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.x + 1.0) / 2.0) * texSpanX, ((ll.z + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, lr.x, lr.y, lr.z, lr.w, ((lr.x + 1.0) / 2.0) * texSpanX, ((lr.z + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.x + 1.0) / 2.0) * texSpanX, ((ur.z + 1.0) / 2.0) * texSpanY);

	    addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.x + 1.0) / 2.0) * texSpanX, ((ll.z + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.x + 1.0) / 2.0) * texSpanX, ((ur.z + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ul.x, ul.y, ul.z, ul.w, ((ul.x + 1.0) / 2.0) * texSpanX, ((ul.z + 1.0) / 2.0) * texSpanY);
	}
	else if (dir == 5)
	{
		// X PLANE FROM THE LEFT
		
        addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.z + 1.0) / 2.0) * texSpanX, ((ll.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, lr.x, lr.y, lr.z, lr.w, ((lr.z + 1.0) / 2.0) * texSpanX, ((lr.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.z + 1.0) / 2.0) * texSpanX, ((ur.y + 1.0) / 2.0) * texSpanY);

	    addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.z + 1.0) / 2.0) * texSpanX, ((ll.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.z + 1.0) / 2.0) * texSpanX, ((ur.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ul.x, ul.y, ul.z, ul.w, ((ul.z + 1.0) / 2.0) * texSpanX, ((ul.y + 1.0) / 2.0) * texSpanY);
	}
	else if (dir == 6)
	{
		// Z PLANE FROM BACK
		
        addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.x + 1.0) / 2.0) * texSpanX, ((ll.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, lr.x, lr.y, lr.z, lr.w, ((lr.x + 1.0) / 2.0) * texSpanX, ((lr.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.x + 1.0) / 2.0) * texSpanX, ((ur.y + 1.0) / 2.0) * texSpanY);

	    addVertex(texture, ll.x, ll.y, ll.z, ll.w, ((ll.x + 1.0) / 2.0) * texSpanX, ((ll.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ur.x, ur.y, ur.z, ur.w, ((ur.x + 1.0) / 2.0) * texSpanX, ((ur.y + 1.0) / 2.0) * texSpanY);
	    addVertex(texture, ul.x, ul.y, ul.z, ul.w, ((ul.x + 1.0) / 2.0) * texSpanX, ((ul.y + 1.0) / 2.0) * texSpanY);
	}
	
	/*addVertex(texture, ll.x, ll.y, ll.z, ll.w, 0.0, 0.0);
	addVertex(texture, lr.x, lr.y, lr.z, lr.w, 1.0, 0.0);
	addVertex(texture, ur.x, ur.y, ur.z, ur.w, 1.0, 1.0);

	addVertex(texture, ll.x, ll.y, ll.z, ll.w, 0.0, 0.0);
	addVertex(texture, ur.x, ur.y, ur.z, ur.w, 1.0, 1.0);
	addVertex(texture, ul.x, ul.y, ul.z, ul.w, 0.0, 1.0);*/
}

void Voxels::addVertex(std::string texture, float x, float y, float z, float w, float u, float v)
{
	VertexBuffer *buffer;

	if (buffers[texture] == nullptr)
	{
		buffer = new VertexBuffer();
		buffer->vertices = nullptr;
		buffer->size = 0;
		buffer->allocatedSize = 0;
		buffer->cursor = 0;
		buffers[texture] = buffer;
	}
	else
	{
		buffer = buffers[texture];
	}

	if (buffer->cursor >= buffer->allocatedSize)
	{
//		Log("cursor ", buffer->cursor, "allocatedSize ", buffer->allocatedSize);
		
		float *tempVertices = buffer->vertices;

//		Log("vertex buffer realloc", buffer->size + BUFFER_ALLOC_INCREMENT);
		buffer->vertices = (float *)malloc(sizeof(float) * (buffer->size + BUFFER_ALLOC_INCREMENT));

		if (buffer->allocatedSize != 0)
		{
			memcpy(buffer->vertices, tempVertices, buffer->size * sizeof(float));
		}

		if (tempVertices != nullptr)
			free(tempVertices);

		buffer->allocatedSize = buffer->size + BUFFER_ALLOC_INCREMENT;
	}

    buffer->vertices[buffer->cursor + 0] = x;
	buffer->vertices[buffer->cursor + 1] = y;
	buffer->vertices[buffer->cursor + 2] = z;
	buffer->vertices[buffer->cursor + 3] = 1.0f;

	buffer->vertices[buffer->cursor + 4] = u;
	buffer->vertices[buffer->cursor + 5] = v;

	buffer->cursor += 6;
	
	buffer->size = buffer->cursor;
}

std::vector<Mesh*> Voxels::getMeshes()
{
	std::vector<Mesh*> meshes;

	for (const auto &pair : buffers)
	{
		VertexBuffer *buf = pair.second;

		Mesh *mesh = new Mesh();

		mesh->texture = pair.first;
		mesh->data = buf->vertices;

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

void Voxels::checkGLError(char *tag)
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

int Voxels::save(std::string fname, FILE *f = nullptr)
{
	FILE *file;

	if (f == nullptr) {
#ifndef PLATFORM_ANDROID
		int err = fopen_s(&file, fname.c_str(), "wb+");
#else
		file = fopen(fname.c_str(), "wb+");
#endif
	} else
		file = f;

	if (file == nullptr)
		return 0;

	char vxSig[] = { 'V', 'X', ' ' };
	fwrite(vxSig, sizeof(char), 3, file);

	char version[] = { '1', '0', '0' };
	fwrite(version, sizeof(char), 3, file);

	int sizeArr[] = { size };
	fwrite(sizeArr, sizeof(int), 1, file);

	for (int z = 0; z < size; z++)
	{
		for (int x = 0; x < size; x++)
		{
			VoxelStack *stack = getStack(x, z);

			char *voxels = (char *)malloc(stack->height);

			for (int i = 0; i < stack->height; i++)
			{
				voxels[i] = stack->voxels[i].texture;
			}

			// Write stack height
			int heightArr[] = { stack->height };
			fwrite(heightArr, sizeof(int), 1, file);

			// Write stack
			fwrite(voxels, sizeof(char), stack->height, file);

			free(voxels);
		}
	}

	// Save texture names
	char texSig[] = { 'T', 'E', 'X' };
	fwrite(texSig, sizeof(char), 3, file);

	// Write total number of texture names recorded
	const unsigned int numTexturesArr[1] = {voxelTextures.size()};
	fwrite(numTexturesArr, sizeof(int), 1, file);

	int t = 0;
	
	for(const auto &pair: voxelTextures)
	{
		std::string texFilename = pair.second;

		// Write length of texture filename
		const unsigned int texFilenameLengthArr[] = { texFilename.length() };
		fwrite(texFilenameLengthArr, sizeof(int), 1, file);

		// Write texture filename
		const char *cTexFilename = texFilename.c_str();
		fwrite(cTexFilename, sizeof(char), texFilename.length(), file);
		
//		Log(texFilename.c_str(), t);
		t++;
	}

	if (g_simpleMode) {
		// Save terrain texture
		std::string tertex = (*extraStrings)["terraintexture"];

		char tertexSig[] = {'T', 'E', 'R', 'T', 'E', 'X'};
		fwrite(tertexSig, sizeof(char), 6, file);

		// Write length of texture filename
		const unsigned int tertexFilenameLengthArr[] = {tertex.length()};
		fwrite(tertexFilenameLengthArr, sizeof(int), 1, file);

		// Write texture filename
		const char *ctertex = tertex.c_str();
		fwrite(ctertex, sizeof(char), tertex.length(), file);

		Log(tertex);

		// Save sky texture
		std::string skytex = (*extraStrings)["skytexture"];

		char skytexSig[] = {'S', 'K', 'Y', 'T', 'E', 'X'};
		fwrite(skytexSig, sizeof(char), 6, file);

		// Write length of texture filename
		const unsigned int skytexFilenameLengthArr[] = {skytex.length()};
		fwrite(skytexFilenameLengthArr, sizeof(int), 1, file);

		// Write texture filename
		const char *cskytex = skytex.c_str();
		fwrite(cskytex, sizeof(char), skytex.length(), file);

		Log(skytex);
	}

	// Close file
	if (f == nullptr)
		fclose(file);
	
	return 1;
//#endif
}

int Voxels::load(std::string fname, FILE *f = nullptr)
{
	FILE *file;

	if (f == nullptr) {
#ifndef PLATFORM_ANDROID
		int err = fopen_s(&file, fname.c_str(), "rb+");
#else
		file = fopen(fname.c_str(), "rb+");
#endif
	} else
		file = f;

//    Log(fname);

	if (file == nullptr)
		return 0;	

	// Read signature
	char tlmSig[] = {' ', ' ', ' '};
	fread(tlmSig, sizeof(char), 3, file);

	if (tlmSig[0] != 'V' && tlmSig[1] != 'X' && tlmSig[2] != ' ')
	{
		Log("VX file signature not recognized");
		return 0;
	}

	// Read version
	char version[] = {' ', ' ', ' '};
	fread(version, sizeof(char), 3, file);

	if (version[0] != '1' && version[1] != '0' && version[2] != '0')
	{
		Log("VX file version not recognized");
		return 0;
	}

	// Read size
	int sizeArr[] = { 0 };
	fread(sizeArr, sizeof(int), 1, file);

	int size = sizeArr[0];
	init(size, extraStrings);
	
//	Log(fname);
//	Log("init size", size);

	for (int z = 0; z < size; z++)
	{
		for (int x = 0; x < size; x++)
		{
			VoxelStack *stack = getStack(x, z);

			// Read stack height
			int heightArr[1] = { 0 };
			fread(heightArr, sizeof(int), 1, file);
			stack->height = heightArr[0];

			setStackHeight(x, z, stack->height);

			// Alloc voxels byte array to read this stack
			char *voxels = (char *)malloc(sizeof(Voxel) * stack->height);

			// Read stack
			fread(voxels, sizeof(char), stack->height, file);

			// Fill stack
			for (int i = 0; i < stack->height; i++)
			{
				stack->voxels[i].texture = voxels[i];
			}

			free(voxels);
		}
	}

	// Read texture filenames if they are there
	char texSig[] = {' ', ' ', ' '};
	fread(texSig, sizeof(char), 3, file);

	if (texSig[0] == 'T' && texSig[1] == 'E' && texSig[2] == 'X')
	{
		// File has texture name information; read it

		// Read number of texture filenames
		int numTexturesArr[] = { 0 };
		fread(numTexturesArr, sizeof(int), 1, file);
		int numTextures = numTexturesArr[0];

		for (int t = 0; t < numTextures; t++)
		{
			// Read length of texture filename
			int texFilenameLengthArr[] = { 0 };
			fread(texFilenameLengthArr, sizeof(int), 1, file);
			int texFilenameLength = texFilenameLengthArr[0];

			char *cTexFilename = (char *)malloc(texFilenameLength + 1);
			fread((void *)cTexFilename, sizeof(char), texFilenameLength, file);
			cTexFilename[texFilenameLength] = 0;

			std::string texFilename = cTexFilename;
			
//			Log(texFilename.c_str(), t);

            (voxelTextures)[t] = texFilename;

			free((void *)cTexFilename);
		}
	}

	if (g_simpleMode) {
		// Read terrain texture
		char tertexSig[] = {' ', ' ', ' ', ' ', ' ', ' '};
		fread(tertexSig, sizeof(char), 6, file);

		if (tertexSig[0] == 'T' && tertexSig[1] == 'E' && tertexSig[2] == 'R' &&
			tertexSig[3] == 'T' && tertexSig[4] == 'E' && tertexSig[5] == 'X') {
			// Read length of texture filename
			int tertexFilenameLengthArr[] = {0};
			fread(tertexFilenameLengthArr, sizeof(int), 1, file);
			int tertexFilenameLength = tertexFilenameLengthArr[0];

			char *cTerTexFilename = (char *) malloc(tertexFilenameLength + 1);
			fread((void *) cTerTexFilename, sizeof(char), tertexFilenameLength, file);
			cTerTexFilename[tertexFilenameLength] = 0;

			(*extraStrings)["terraintexture"] = cTerTexFilename;

			Log((*extraStrings)["terraintexture"]);
		}

		// Read sky texture
		char skytexSig[] = {' ', ' ', ' ', ' ', ' ', ' '};
		fread(skytexSig, sizeof(char), 6, file);

		if (skytexSig[0] == 'S' && skytexSig[1] == 'K' && skytexSig[2] == 'Y' &&
			skytexSig[3] == 'T' && skytexSig[4] == 'E' && skytexSig[5] == 'X') {
			// Read length of texture filename
			int skytexFilenameLengthArr[] = {0};
			fread(skytexFilenameLengthArr, sizeof(int), 1, file);
			int skytexFilenameLength = skytexFilenameLengthArr[0];

			char *cSkyTexFilename = (char *) malloc(skytexFilenameLength + 1);
			fread((void *) cSkyTexFilename, sizeof(char), skytexFilenameLength, file);
			cSkyTexFilename[skytexFilenameLength] = 0;

			(*extraStrings)["skytexture"] = cSkyTexFilename;

			Log((*extraStrings)["skytexture"]);
		}
	}

	// Close file
	if (f == nullptr)
		fclose(file);
	
	calcMinMax();
	
//	Log("minz", minz);
//	Log("maxz", maxz);
	
	return 1;
//#endif
}

void Voxels::calcMinMax()
{
    if (!g_simpleMode)
    {
        minx = 0;
        maxx = size;
        miny = 0;
        maxy = size;
        minz = 0;
        maxz = size;
    }
    else
    {
        minx = size / 2.0;
        maxx = size / 2.0;
        miny = 0;
        maxy = 0;
        minz = size / 2.0;
        maxz = size / 2.0;

        //	Log("size", size);

        for (int z = 0; z < size; z++) {
            for (int x = 0; x < size; x++) {
                VoxelStack *stack = getStack(x, z);

                if (stack->height > 0 && stack->voxels != NULL) {
                    if (x < minx)
                        minx = x;
                    if (x > maxx)
                        maxx = x + 8;
                    if (z < minz)
                        minz = z;
                    if (z > maxz)
                        maxz = z + 4;
                    if (stack->height > maxy)
                        maxy = stack->height;
                }
            }
        }
    }
}

void Voxels::crop()
{
	calcMinMax();
	
	int diffx = maxx - minx;
//	int diffy = maxy - miny;
	int diffz = maxz - minz;
	
	int maxdiff = diffx;
//	if (diffy > maxdiff)
//		maxdiff = diffy;
	if (diffz > maxdiff)
	    maxdiff = diffz;
	
	int newsize = maxdiff + 2;
	
//	Log("minx", minx);
//	Log("maxx", maxx);
	
	Voxels cropped;
	
	cropped.init(newsize, extraStrings);
	
	int centerx = minx + diffx / 2;
//	int centery = miny + diffy / 2;
	int centerz = minz + diffz / 2;
	
	int half = maxdiff / 2;

	int newx = 1;
	
	for (int oldx = centerx - half; oldx < centerx + half; oldx++)
	{
		newx++;
		int newy = 1;
		
	    for (int oldy = 0; oldy < newsize; oldy++)
		{
			newy++;
			int newz = 1;
			
            for (int oldz = centerz - half; oldz < centerz + half; oldz++)
			{
				newz++;
				
				int v = get(oldx, oldy, oldz);
				if (v != 0)
				    cropped.set(newx, newy, newz, v);
			}
		}
	}
	
	clear();
	size = newsize;
	stacks = cropped.stacks;
}

void Voxels::setVoxelTexture(int voxel, std::string texture)
{
	voxelTextures[voxel] = texture;
}

std::string Voxels::getVoxelTexture(int voxel)
{
	return voxelTextures[voxel];
}
