#include <assert.h>
#include <string.h>

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#endif

#ifdef __circle__
#include <linux/kernel.h>
#include "bcm_host.h"
#endif

#include "mainwindow.h"

MainWindow::MainWindow(const char *title)
{

#ifdef __circle__
    bcm_host_init();

    int32_t wasSuccessful = graphics_get_display_size(0, &width, &height);
    assert(wasSuccessful >= 0);

    VC_RECT_T destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.width = width;
    destRect.height = height;

    VC_RECT_T sourceRect;
    sourceRect.x = 0;
    sourceRect.y = 0;
    sourceRect.width = width << 16;
    sourceRect.height = height << 16;

    DISPMANX_DISPLAY_HANDLE_T display = vc_dispmanx_display_open(0);
    DISPMANX_UPDATE_HANDLE_T update = vc_dispmanx_update_start(0);

    DISPMANX_ELEMENT_HANDLE_T element =
        vc_dispmanx_element_add (update, display, 0, &destRect, 0,
        &sourceRect, DISPMANX_PROTECTION_NONE, 0, 0, DISPMANX_NO_ROTATE);

    static EGL_DISPMANX_WINDOW_T window;
    window.element = element;
    window.width = width;
    window.height = height;

    vc_dispmanx_update_submit_sync(update);

    nativeWindow = &window;
#endif

#ifdef __linux__
    this->width = 640;
    this->height = 480;

    Display *display = XOpenDisplay(NULL);
    assert(display != NULL);
    Window rootWindow = DefaultRootWindow(display);

    XSetWindowAttributes windowAttributes;
    windowAttributes.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;

    Window window = XCreateWindow(display, rootWindow, 0, 0, width, height, 0,
        CopyFromParent, InputOutput, CopyFromParent, CWEventMask,
        &windowAttributes);

    deleteMessage_ = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, window, &deleteMessage_, 1);

    XSetWindowAttributes newWindowAttributes;
    newWindowAttributes.override_redirect = false;
    XChangeWindowAttributes(display, window, CWOverrideRedirect, &newWindowAttributes);

    XWMHints windowManagerHints;
    windowManagerHints.input = true;
    windowManagerHints.flags = InputHint;
    XSetWMHints(display, window, &windowManagerHints);

    XMapWindow(display, window);
    XStoreName(display, window, title);

    Atom windowState = XInternAtom(display, "_NET_WM_STATE", false);

    XEvent xEvent;
    memset(&xEvent, 0, sizeof(xEvent));
    xEvent.type = ClientMessage;
    xEvent.xclient.window = window;
    xEvent.xclient.message_type = windowState;
    xEvent.xclient.format = 32;
    xEvent.xclient.data.l[0] = 1;
    xEvent.xclient.data.l[1] = false;

    XSendEvent(display, DefaultRootWindow(display), false,
        SubstructureNotifyMask, &xEvent);

    nativeWindow = (EGLNativeWindowType)window;
    nativeDisplay_ = (EGLNativeDisplayType)display;
#endif

}

MainWindow::~MainWindow()
{

#ifdef __linux__
    XCloseDisplay(nativeDisplay_);
#endif

}

#ifdef __linux__
bool MainWindow::wasDeleteMessageReceived()
{
    bool wasMessageReceived = false;

    while(XPending(nativeDisplay_))
    {
        XEvent event;
        XNextEvent(nativeDisplay_, &event);

        if (event.type == ClientMessage && (unsigned)event.xclient.data.l[0] == deleteMessage_)
        {
            wasMessageReceived = true;
        }
    }

    return wasMessageReceived;
}
#endif
