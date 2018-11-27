//
// Created by dimitri on 11/25/2018.
//

#include "Billboarder.h"
#include "Object.h"

void Billboarder::init() {
    // Load shaders
    const int len = 10000;

    char vertexShaderStr[len];
    char fragmentShaderStr[len];

    // Compile program
#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
    snprintf(vertexShaderStr, len, "%s", vertexShaderCodeES20);
    snprintf(fragmentShaderStr, len, "%s", fragmentShaderCodeES20);
#endif
    program = loadProgram(vertexShaderStr, fragmentShaderStr, false);

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Generate VAO
    glGenVertexArrays(1, (GLuint *) &vao);
    checkGLError("glGenVertexArrays");
#endif
}

void Billboarder::refresh(std::map<std::string, Object*> objects)
{
/*    for(const auto &pair: objects)
    {
        Object *obj = pair.second;

        if (obj != nullptr && obj->type == OBJTYPE_MODEL && obj->model != nullptr && !obj->model->billboarded)
        {
            process(obj->model, obj->glTexID);
            obj->model->billboarded = true;
        }
    }*/
}

void Billboarder::process(Model2 *model, int glTexID)
{
    if (model == nullptr)
        return;

    for (int m = 0; m < model->meshes.size(); m++)
    {
        Mesh *mesh = model->meshes[m];

/*        for (int r = 0; r < NUM_BILLBOARD_ROTATIONS; r++) {
            float yaw = 360.0f / (float)r;
            mesh->billboards[r] = draw(model, mesh, yaw, glTexID);
        }*/
    }
}

int Billboarder::draw(Model2 *model, Mesh *mesh, float yaw, int glTexID) {
    if (model == nullptr || mesh == nullptr)
        return -1;

    fb.bind();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    checkGLError("glEnable");
    glFrontFace(GL_CCW);
    checkGLError("glFrontFace");

    // Use program
    glUseProgram(program);
    checkGLError("ShapeRenderer glUseProgram");

#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    // Bind VAO
    glBindVertexArray(vao);
    checkGLError("glBindVertexArray");
#endif

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    checkGLError("glBindBuffer");

    // Set the matrices
    glm::mat4 mvMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 rotate;
    glm::mat4 scale;
    glm::mat4 scaleToNDC;

#ifdef PLATFORM_OPENVR
    scaleToNDC = glm::scale(glm::mat4(), glm::vec3(VRSCALE, VRSCALE, VRSCALE));
#else
    scaleToNDC = glm::scale(glm::mat4(), glm::vec3(NDC_SCALE, NDC_SCALE, NDC_SCALE));
#endif

    // Calculate scale of model based on min and max
    float scaleX = 1.0 / (model->maxX - model->minX) * model->proportionScale.x;
    float scaleY = 1.0 / (model->maxY - model->minY) * model->proportionScale.y;
    float scaleZ = 1.0 / (model->maxZ - model->minZ) * model->proportionScale.z;

    scale = glm::scale(glm::mat4(), glm::vec3(scaleX, scaleY, scaleZ)); // Normalization scale

    // Rotate
    rotate = glm::rotate(glm::mat4(), glm::radians(-yaw - model->yaw),
                             glm::vec3(0, 1, 0)) // Model yaw
                 * glm::rotate(glm::mat4(), glm::radians(-0.0f - model->pitch),
                               glm::vec3(1, 0, 0)); // Model pitch

    mvMatrix =
            scaleToNDC
            * rotate
            * scale;

    glm::mat4 projMatrix;
    glm::mat4 depthMVP;

    setMatrix(program, "mvMatrix", mvMatrix);

    projMatrix = glm::perspective(VIEW_FOV, 1.0f, 0.001f, 1000.0f);

    setMatrix(program, "projectionMatrix", projMatrix);

    // Set variables
    setUniform4f(program, "vColor", 1.0f, 1.0f, 1.0f, 1.0f);

    // Set texture
    if (glTexID != -1) {
        glActiveTexture(GL_TEXTURE0);
        checkGLError("glActiveTexture");
        glBindTexture(GL_TEXTURE_2D, glTexID);
        checkGLError("glBindTexture");

        // Set texture unit number
        int uTexture = glGetUniformLocation(program, "uTexture");
        checkGLError("glGetUniformLocation");
        glUniform1i(uTexture, 0);
        checkGLError("glUniform1i");

        setUniform1f(program, "useTexture", 1.0);
    } else {
        setUniform1f(program, "useTexture", 0.0);
    }

    // Set attributes
    int stride = 9 * sizeof(float);
    setVertexAttrib(program, "vPosition", 4, GL_FLOAT, false, stride, 0);
    setVertexAttrib(program, "vTexCoords", 2, GL_FLOAT, false, stride, 4);
//	setVertexAttrib("vNormal", 2, GL_FLOAT, false, stride, 6);

    // Draw
    glDrawElements(GL_TRIANGLES, mesh->numPolys * 3, GL_UNSIGNED_INT, 0);
    checkGLError("glDrawElements");

    // Reset
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    glBindVertexArray(0);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    fb.unbind();

    return fb.getGlTexId();
}
