#pragma once

#include "mainwindow.h"

class Graphics3d
{
    private:
        MainWindow *mainWindow_;
        EGLDisplay display_;
        EGLContext renderingContext_;
        EGLSurface windowSurface_;

        void initializeProjection();
        void initializeLighting();

    public:
        Graphics3d(MainWindow *mainWindow);
        ~Graphics3d();

        void swapBuffers();
};
