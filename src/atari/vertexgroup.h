#pragma once

#include "GLES/gl.h"

struct VertexGroup
{
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat vertices[2048];
    int vertexCount = 0;
    GLfloat normals[2048];
    int normalCount = 0;
};
