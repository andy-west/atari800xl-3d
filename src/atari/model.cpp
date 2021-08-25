#include "EGL/egl.h"

#include "model.h"

Model::Model()
{
}

void Model::resetAllVertexGroups()
{
    vertexGroupCount_ = 0;
}

void Model::addVertexGroup(GLfloat red, GLfloat green, GLfloat blue)
{
    currentVertexGroup_ = vertexGroups_ + vertexGroupCount_;

    currentVertexGroup_->red = red;
    currentVertexGroup_->green = green;
    currentVertexGroup_->blue = blue;
    currentVertexGroup_->vertexCount = 0;
    currentVertexGroup_->normalCount = 0;

    vertexGroupCount_++;
}

void Model::createTriangle(
    GLfloat x0, GLfloat y0, GLfloat z0,
    GLfloat x1, GLfloat y1, GLfloat z1,
    GLfloat x2, GLfloat y2, GLfloat z2)
{
    GLfloat *currentVertex = currentVertexGroup_->vertices + currentVertexGroup_->vertexCount;

    *currentVertex = x0;
    currentVertex++;
    *currentVertex = y0;
    currentVertex++;
    *currentVertex = z0;
    currentVertex++;
    *currentVertex = x1;
    currentVertex++;
    *currentVertex = y1;
    currentVertex++;
    *currentVertex = z1;
    currentVertex++;
    *currentVertex = x2;
    currentVertex++;
    *currentVertex = y2;
    currentVertex++;
    *currentVertex = z2;
    currentVertex++;

    currentVertexGroup_->vertexCount += 9;

    GLfloat *currentNormal = currentVertexGroup_->normals + currentVertexGroup_->normalCount;

    GLfloat ux = x1 - x0;
    GLfloat uy = y1 - y0;
    GLfloat uz = z1 - z0;
    GLfloat vx = x2 - x0;
    GLfloat vy = y2 - y0;
    GLfloat vz = z2 - z0;

    GLfloat normalX = uy * vz - uz * vy;
    GLfloat normalY = uz * vx - ux * vz;
    GLfloat normalZ = ux * vy - uy * vx;

    *currentNormal = normalX;
    currentNormal++;
    *currentNormal = normalY;
    currentNormal++;
    *currentNormal = normalZ;
    currentNormal++;
    *currentNormal = normalX;
    currentNormal++;
    *currentNormal = normalY;
    currentNormal++;
    *currentNormal = normalZ;
    currentNormal++;
    *currentNormal = normalX;
    currentNormal++;
    *currentNormal = normalY;
    currentNormal++;
    *currentNormal = normalZ;
    currentNormal++;
    
    currentVertexGroup_->normalCount += 9;
}

void Model::draw()
{
    for (int i = 0; i < vertexGroupCount_; i++)
    {
        VertexGroup vertexGroup = vertexGroups_[i];
        glColor4f(vertexGroup.red, vertexGroup.green, vertexGroup.blue, 1.0f);

        for (int j = 0; j < vertexGroup.vertexCount / 9; j++)
        {
            glVertexPointer(3, GL_FLOAT, 0, vertexGroup.vertices);
            glNormalPointer(GL_FLOAT, 0, vertexGroup.normals);
            glDrawArrays(GL_TRIANGLES, j * 3, 3);
        }
    }
}
