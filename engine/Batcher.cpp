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

#include "Batcher.h"
#include "DDLUtils.hpp"
#include "GLIncludes.h"

void Batcher::batch(std::map<std::string, Batch*> batches, TextureAtlas *ta, TextureManager2 *texMan)
{
	texAtlas = ta;
	
	for(const auto &pair: batches)
    {
        Batch *batch = pair.second;

        if (batch->builtWithAtlasSize != texAtlas->getSize())
            batch->needsRebuild = true;

		if (batch != nullptr && batch->needsRebuild)
		{
			batch->needsRebuild = false;
			
			Object *mainObj = batch->mainObj;
			mainObj->mainOfBatch = true;
			
			// Calculate size of merged mesh
			int numMergedCoords = 0;

			// Process main object
			Shape *mainShape = mainObj->shape;
			if (mainShape->needsRebuild)
				mainShape->rebuild(texMan);

			for (int m = 0; m < mainShape->meshes.size(); m++)
			{
				Mesh *oldMesh = mainShape->meshes[m];

				if (oldMesh != nullptr && oldMesh->data != nullptr)
					numMergedCoords += oldMesh->numCoords;

				// Add mesh's texture
				if (oldMesh->texture != "")
				    texAtlas->add(oldMesh->texture);
			}

			// Add object's texture
			if (mainObj->textureName != "")
				texAtlas->add(mainObj->textureName);

			// Process sub-objects
			for (int o = 0; o < batch->objects.size(); o++)
			{
				Object *obj = batch->objects[o];
				Shape *shape = obj->shape;

				if (shape->needsRebuild)
					shape->rebuild(texMan);

				for (int m = 0; m < shape->meshes.size(); m++)
		        {
			        Mesh *oldMesh = shape->meshes[m];
					numMergedCoords += oldMesh->numCoords;

					// Add mesh's texture
					if (oldMesh->texture != "")
						texAtlas->add(oldMesh->texture);
                }

				// Add object's texture
				if (obj->textureName != "")
					texAtlas->add(obj->textureName);
			}

            texAtlas->refresh();
						
			// Alloc merged mesh
		    Mesh *newMesh = new Mesh();
						
			newMesh->numCoords = numMergedCoords;
			newMesh->data = (float*)malloc(sizeof(float) * numMergedCoords);
						
			// Merge meshes
			int cursor = 0;

            const int vlen = 12;

            // Merge main object meshes
			if (mainObj->visible && mainObj->type == OBJTYPE_SHAPE && mainObj->shapeType != SHAPE_QUAD)
			{
				Shape *mainShape = mainObj->shape;

				if (mainShape != nullptr)
				{
					for (int m = 0; m < mainShape->meshes.size(); m++)
					{
						Mesh *oldMesh = mainShape->meshes[m];

						if (oldMesh != nullptr && oldMesh->data != nullptr)
						{
							int numVerts = oldMesh->numCoords / vlen;

							for (int v = 0; v < numVerts && cursor < newMesh->numCoords; v++)
							{
								newMesh->data[cursor] = oldMesh->data[v * vlen];
								newMesh->data[cursor + 1] = oldMesh->data[v * vlen + 1];
								newMesh->data[cursor + 2] = oldMesh->data[v * vlen + 2];
								newMesh->data[cursor + 3] = oldMesh->data[v * vlen + 3];

                                UV uv;

                                uv.u = oldMesh->data[v * vlen + 4];
                                uv.v = oldMesh->data[v * vlen + 5];

                                UV newuv;

                                if (oldMesh->texture != "")
                                    newuv = texAtlas->getUV(oldMesh->texture, uv);
                                else if (mainObj->textureName != "")
                                    newuv = texAtlas->getUV(mainObj->textureName, uv);

                                newMesh->data[cursor + 4] = newuv.u;
                                newMesh->data[cursor + 5] = newuv.v;

								newMesh->data[cursor + 6] = oldMesh->data[v * vlen + 6];
								newMesh->data[cursor + 7] = oldMesh->data[v * vlen + 7];
								newMesh->data[cursor + 8] = oldMesh->data[v * vlen + 8];

								newMesh->data[cursor + 9] = oldMesh->data[v * vlen + 9];
								newMesh->data[cursor + 10] = oldMesh->data[v * vlen + 10];
								newMesh->data[cursor + 11] = oldMesh->data[v * vlen + 11];

								cursor += vlen;
							}
						}
					}
				}
			}

			// Merge sub-objects meshes
			for (int i = 0; i < batch->objects.size(); i++)
			{
				Object *obj = batch->objects[i];
				obj->partOfBatch = true;
				
				float scalex = obj->scale.x / mainObj->scale.x;
				float scaley = obj->scale.y / mainObj->scale.y;
			    float scalez = obj->scale.z / mainObj->scale.z;
				
				float translatex = (obj->position.x - mainObj->position.x) / (mainObj->scale.x / 2.0);
			    float translatey = (obj->position.y - mainObj->position.y) / (mainObj->scale.y / 2.0);
				float translatez = (obj->position.z - mainObj->position.z) / (mainObj->scale.z / 2.0);
		
			    if (obj->visible && obj->type == OBJTYPE_SHAPE && obj->shapeType != SHAPE_QUAD)
			    {
		            Shape *shape = obj->shape;

		            if (shape != nullptr)
			        {
		                for (int m = 0; m < shape->meshes.size(); m++)
		                {
			                Mesh *oldMesh = shape->meshes[m];

							if (oldMesh != nullptr && oldMesh->data != nullptr)
                            {
								int numVerts = oldMesh->numCoords / vlen;

								for (int v = 0; v < numVerts && cursor < newMesh->numCoords; v++) {
									newMesh->data[cursor] = (oldMesh->data[v * vlen] * scalex) + translatex;
									newMesh->data[cursor + 1] = (oldMesh->data[v * vlen + 1] * scaley) + translatey;
									newMesh->data[cursor + 2] = (oldMesh->data[v * vlen + 2] * scalez) + translatez;
									newMesh->data[cursor + 3] = oldMesh->data[v * vlen + 3];
									
									UV uv;
									
									uv.u = oldMesh->data[v * vlen + 4];
									uv.v = oldMesh->data[v * vlen + 5];

                                    UV newuv;

                                    if (oldMesh->texture != "")
                                        newuv = texAtlas->getUV(oldMesh->texture, uv);
                                    else if (obj->textureName != "")
                                        newuv = texAtlas->getUV(obj->textureName, uv);

                                    newMesh->data[cursor + 4] = newuv.u;
									newMesh->data[cursor + 5] = newuv.v;

									newMesh->data[cursor + 6] = oldMesh->data[v * vlen + 6];
									newMesh->data[cursor + 7] = oldMesh->data[v * vlen + 7];
									newMesh->data[cursor + 8] = oldMesh->data[v * vlen + 8];

									newMesh->data[cursor + 9] = oldMesh->data[v * vlen + 9];
									newMesh->data[cursor + 10] = oldMesh->data[v * vlen + 10];
									newMesh->data[cursor + 11] = oldMesh->data[v * vlen + 11];

									cursor += vlen;
								}
							}
		                }					
		            }
			    }
			}
			
			newMesh->glTexID = texAtlas->getGlTexId();
//			newMesh->texture = "tile.png";

			// Generate VBO
		    glGenBuffers(1, (GLuint *)&newMesh->vbo);
		    checkGLError("glGenBuffers");

		    glBindBuffer(GL_ARRAY_BUFFER, newMesh->vbo);
		    checkGLError("glBindBuffer");

		    // Send vertices to GPU memory
		    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * newMesh->numCoords, newMesh->data, GL_STATIC_DRAW);
		    checkGLError("glBufferData");

		    mainObj->batchedMesh = newMesh;

            batch->builtWithAtlasSize = texAtlas->getSize();
        }
	}
}

void Batcher::checkGLError(char *tag)
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

