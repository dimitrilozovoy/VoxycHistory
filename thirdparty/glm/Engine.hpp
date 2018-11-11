//
//  Graphics.hpp
//  Drumstr
//
//  Created by Dimitri Lozovoy on 12/28/15.
//
//

#ifndef Engine_hpp
#define Engine_hpp

#include "enginestate.h"
#include "transformer.h"
#include "lodepng.hpp"
#include "Globals.hpp"

#if defined USE_OPENGL_2 || defined USE_OPENGL_3
#include "GL/glew.h"
#endif
#ifdef USE_GLFW
#include "GLFW/glfw3.h"
#endif
#ifdef USE_OPENGL
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#endif

#define DEFAULT_MAIN_TILE_SIZE 100
#define MAX_TEXTURES            64

class Engine
{
public:
    EngineState m_state;
    Transformer m_transformer;
    int m_mainTileSize = DEFAULT_MAIN_TILE_SIZE;
    int *m_glTexIDs;
    bool m_textureIDsGenerated = false;
    bool m_glTexIDsUsed[MAX_TILES];
    float m_tileColors[MAX_TILES][4];
    float m_tileReds[MAX_TILES];
    float m_tileGreens[MAX_TILES];
    float m_tileBlues[MAX_TILES];
    float m_tileAlphas[MAX_TILES];
    
    Engine();
    ~Engine();
    void Start();
    void RenderFrame();
    DrawData *GetDrawData();
    int GetGLTexID(int tileId);
    void Stop();
    Transformer &GetTransformer() { return m_transformer; };
    EngineState *GetState() { return &m_state; };
    void SetWorldSize(int size) { m_state.set_world_size(size); };
    float GetTileSize() { return m_mainTileSize; };
    void SetTileColor(int tileId, float r, float g, float b, float a);
    float *GetTileColor(int tileId);
    float GetTileColorR(int tileId);
    float GetTileColorG(int tileId);
    float GetTileColorB(int tileId);
    float GetTileColorA(int tileId);
    void LoadTexture(int tileId, char *filename);
    void GenerateTextureIDs();
    void ReleaseTextureIDs();
    int LoadTilemap(int entity, char *filename);
    int NewTilemap(int tilemapSize, int tileSize, int textureSpan, int island);
    int NewInvisible();
    void SetMain(int entity);
    int NewShape(Shapes shape);
    void SetTile(int entity, int x, int y, int z, int tile);
    void SetTileOffset(int entity, int x, int y, int z, float offsetX, float offsetY, float offsetZ);
    void SetPosition(int entity, float x, float y, float z, float yaw = 0.0f, float pitch = 0.0f, float roll = 0.0f);
    void SetSize(int entity, float size);
    void SetCamera(float x, float y, float z, float yaw, float pitch, float roll);
    void SetSkyboxSouth(char tileId) {m_state.m_skyBox[0] = tileId;};
    void SetSkyboxNorth(char tileId) {m_state.m_skyBox[1] = tileId;};
    void SetSkyboxWest(char tileId) {m_state.m_skyBox[2] = tileId;};
    void SetSkyboxEast(char tileId) {m_state.m_skyBox[3] = tileId;};
    void SetSkyboxDown(char tileId) {m_state.m_skyBox[4] = tileId;};
    void SetSkyboxUp(char tileId) {m_state.m_skyBox[5] = tileId;};
    Entity *GetCamera();
};

void checkError();

#endif /* Graphics_hpp */
