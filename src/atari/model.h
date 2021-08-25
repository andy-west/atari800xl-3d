#pragma once

#include "GLES/gl.h"
#include "vertexgroup.h"

class Model
{
    private:
        VertexGroup vertexGroups_[256];
        int vertexGroupCount_ = 0;
        VertexGroup *currentVertexGroup_;

    public:
        Model();

        void resetAllVertexGroups();
        void addVertexGroup(GLfloat red, GLfloat green, GLfloat blue);

        void createTriangle(
            GLfloat x0, GLfloat y0, GLfloat z0,
            GLfloat x1, GLfloat y1, GLfloat z1,
            GLfloat x2, GLfloat y2, GLfloat z2);

        void draw();
};
