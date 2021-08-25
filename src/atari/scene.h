#pragma once

#include "mainwindow.h"
#include "graphics3d.h"
#include "model.h"

class Scene
{
    private:
        Graphics3d *graphics3d_;
        Model *model_;

        GLfloat cameraPositionX_ = 5.0f;
        GLfloat cameraPositionY_ = 1.0f;
        GLfloat cameraPositionZ_ = 25.0;

        GLfloat cameraRotationX_ = 0.0f;
        GLfloat cameraRotationY_ = 0.0f;
        GLfloat cameraRotationZ_ = 0.0f;

        GLfloat cameraRotationXSpeed_ = 0.0f;
        GLfloat cameraRotationYSpeed_ = 0.0f;
        GLfloat cameraRotationZSpeed_ = 0.0f;

        GLfloat cameraForwardSpeed_ = 0.0f;

        void updateCamera();
        void drawObjects();

    public:
        Scene();
        ~Scene();

        MainWindow *mainWindow;

        void draw();
        void setBackgroundColor(GLfloat red, GLfloat green, GLfloat blue);
        void setColor(GLfloat red, GLfloat green, GLfloat blue);
        void deleteAllGeometry();

        void createTriangle(
            GLfloat x0, GLfloat y0, GLfloat z0,
            GLfloat x1, GLfloat y1, GLfloat z1,
            GLfloat x2, GLfloat y2, GLfloat z2);

        void setCameraPosition(GLfloat x, GLfloat y, GLfloat z);

        void setCameraRotationX(GLfloat x);
        void setCameraRotationY(GLfloat y);
        void setCameraRotationZ(GLfloat z);

        void setCameraRotationXSpeed(GLfloat speed);
        void setCameraRotationYSpeed(GLfloat speed);
        void setCameraRotationZSpeed(GLfloat speed);

        void setCameraForwardSpeed(GLfloat speed);
};
