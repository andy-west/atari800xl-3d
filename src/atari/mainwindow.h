#pragma once

#include "EGL/egl.h"

class MainWindow
{
    private:

#ifdef __linux__
        Atom deleteMessage_;
        EGLNativeDisplayType nativeDisplay_;
#endif

    public:
        MainWindow(const char *title);
        ~MainWindow();

        long unsigned int width;
        long unsigned int height;
        EGLNativeWindowType nativeWindow;

#ifdef __linux__
        bool wasDeleteMessageReceived();
#endif

};
