#include "math.h"
#include "EGL/egl.h"

#include "scene.h"

#define PI 3.14159265

Scene::Scene()
{
    mainWindow = new MainWindow("Rotating Cube");
    graphics3d_ = new Graphics3d(mainWindow);
    model_ = new Model();
}

Scene::~Scene()
{
    delete model_;
    delete graphics3d_;
    delete mainWindow;
}

void Scene::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();
    drawObjects();
    graphics3d_->swapBuffers();
}

void Scene::updateCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (cameraForwardSpeed_ != 0.0f)
    {
        cameraRotationX_ += cameraRotationXSpeed_;
        if (cameraRotationX_ == 360) cameraRotationX_ = 0;
        if (cameraRotationX_ == -1) cameraRotationX_ = 359;

        cameraRotationY_ += cameraRotationYSpeed_;
        if (cameraRotationY_ == 360) cameraRotationY_ = 0;
        if (cameraRotationY_ == -1) cameraRotationY_ = 359;

        cameraRotationZ_ += cameraRotationZSpeed_;
        if (cameraRotationZ_ == 360) cameraRotationZ_ = 0;
        if (cameraRotationZ_ == -1) cameraRotationZ_ = 359;
    }

    glRotatef(-cameraRotationZ_, 0.0f, 0.0f, 1.0f);
    glRotatef(-cameraRotationY_, 0.0f, 1.0f, 0.0f);
    glRotatef(-cameraRotationX_, 1.0f, 0.0f, 0.0f);

    cameraPositionX_ += cameraForwardSpeed_ * sin(-cameraRotationY_ * PI / 180.0f);
    cameraPositionZ_ += cameraForwardSpeed_ * -cos(-cameraRotationY_ * PI / 180.0f);
    glTranslatef(-cameraPositionX_, -cameraPositionY_, -cameraPositionZ_);
}

void Scene::drawObjects()
{
    model_->draw();
}

void Scene::setBackgroundColor(GLfloat red, GLfloat green, GLfloat blue)
{
    glClearColor(red, green, blue, 1.0f);
}

void Scene::setColor(GLfloat red, GLfloat green, GLfloat blue)
{
    model_->addVertexGroup(red, green, blue);
}

void Scene::deleteAllGeometry()
{
    model_->resetAllVertexGroups();
}

void Scene::createTriangle(
    GLfloat x0, GLfloat y0, GLfloat z0,
    GLfloat x1, GLfloat y1, GLfloat z1,
    GLfloat x2, GLfloat y2, GLfloat z2)
{
    model_->createTriangle(x0, y0, z0, x1, y1, z1, x2, y2, z2);
}

void Scene::setCameraPosition(GLfloat x, GLfloat y, GLfloat z)
{
    cameraPositionX_ = x;
    cameraPositionY_ = y;
    cameraPositionZ_ = z;
}

void Scene::setCameraRotationX(GLfloat x)
{
    cameraRotationX_ = x;
}

void Scene::setCameraRotationY(GLfloat y)
{
    cameraRotationY_ = y;
}

void Scene::setCameraRotationZ(GLfloat z)
{
    cameraRotationZ_ = z;
}

void Scene::setCameraRotationXSpeed(GLfloat speed)
{
    cameraRotationXSpeed_ = speed;
}

void Scene::setCameraRotationYSpeed(GLfloat speed)
{
    cameraRotationYSpeed_ = speed;
}

void Scene::setCameraRotationZSpeed(GLfloat speed)
{
    cameraRotationZSpeed_ = speed;
}

void Scene::setCameraForwardSpeed(GLfloat speed)
{
    cameraForwardSpeed_ = speed;
}
