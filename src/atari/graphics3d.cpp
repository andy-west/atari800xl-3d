#include <assert.h>
#include <math.h>

#include "GLES/gl.h"
#include "EGL/egl.h"

#include "graphics3d.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

Graphics3d::Graphics3d(MainWindow *mainWindow)
{
    mainWindow_ = mainWindow;

    EGLBoolean wasSuccessful;

    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(display_ != EGL_NO_DISPLAY);

    wasSuccessful = eglInitialize(display_, NULL, NULL);
    assert(wasSuccessful);

    static const EGLint attributes[] =
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    EGLConfig config;
    EGLint configCount;
    wasSuccessful = eglChooseConfig(display_, attributes, &config, 1, &configCount);
    assert(wasSuccessful);

    renderingContext_ = eglCreateContext(display_, config, EGL_NO_CONTEXT, NULL);
    assert(renderingContext_ != EGL_NO_CONTEXT);

    windowSurface_ = eglCreateWindowSurface(display_, config, mainWindow->nativeWindow, NULL);
    assert(windowSurface_ != EGL_NO_SURFACE);

    wasSuccessful = eglMakeCurrent(display_, windowSurface_, windowSurface_, renderingContext_);
    assert(wasSuccessful);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);

    initializeProjection();
    initializeLighting();
}

Graphics3d::~Graphics3d()
{
    eglDestroySurface(display_, windowSurface_);
    eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display_, renderingContext_);
    eglTerminate(display_);
}

void Graphics3d::initializeProjection()
{
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glViewport(0, 0, (GLsizei)mainWindow_->width, (GLsizei)mainWindow_->height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float clipNear = 1.0f;
    float clipFar = 500.0f;
    float halfClipHeight = clipNear * (float)tan(45.0f / 2.0f / 180.0f * M_PI);
    float halfClipWidth = halfClipHeight * (float)mainWindow_->width / (float)mainWindow_->height;

    glFrustumf(-halfClipWidth, halfClipWidth,
        -halfClipHeight, halfClipHeight,
        clipNear, clipFar);
}

void Graphics3d::initializeLighting()
{
    glEnable(GL_LIGHTING);

    GLfloat ambientParameters[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientParameters);

    GLfloat diffuseParameters[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseParameters);

    //GLfloat lightDirection[] = { -0.5f, 1.0f, 1.0f, 0.0f };
    //glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);

    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
}

void Graphics3d::swapBuffers()
{
    eglSwapBuffers(display_, windowSurface_);
}
