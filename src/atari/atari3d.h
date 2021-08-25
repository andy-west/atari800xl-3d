#ifndef _atari3d_h
#define _atari3d_h

#include <circle/multicore.h>
#include <circle/screen.h>
#include <circle/memory.h>
#include <circle/sched/scheduler.h>
#include <circle/types.h>
#include "scene.h"

class CAtari3d
    : public CMultiCoreSupport
{
    private:
        CScreenDevice *m_pScreen;
        CScheduler *m_pScheduler;

        volatile unsigned char atariData_[65536];
        volatile unsigned short dataReadIndex_ = 0;
        volatile unsigned short dataWriteIndex_ = 0;
        volatile unsigned char isGeometryLoaded_ = 0;

        enum CommunicationByteType {
            commNone,
            commBgRed,
            commBgGreen,
            commBgBlue,
            commColorRed,
            commColorGreen,
            commColorBlue,
            commTriX0High,
            commTriX0Low,
            commTriY0High,
            commTriY0Low,
            commTriZ0High,
            commTriZ0Low,
            commTriX1High,
            commTriX1Low,
            commTriY1High,
            commTriY1Low,
            commTriZ1High,
            commTriZ1Low,
            commTriX2High,
            commTriX2Low,
            commTriY2High,
            commTriY2Low,
            commTriZ2High,
            commTriZ2Low,
            commCameraX,
            commCameraY,
            commCameraZ,
            commSetCameraPositionX,
            commSetCameraPositionY,
            commSetCameraPositionZ
        };

        CommunicationByteType nextByteType_ = commNone;

        GLfloat backgroundRed_ = 0.0f;
        GLfloat backgroundGreen_ = 0.0f;
        GLfloat backgroundBlue_ = 0.0f;

        GLfloat colorRed_ = 0.0f;
        GLfloat colorGreen_ = 0.0f;
        GLfloat colorBlue_ = 0.0f;

        unsigned short triVertexX0High_ = 0;
        unsigned short triVertexX0Low_ = 0;
        unsigned short triVertexY0High_ = 0;
        unsigned short triVertexY0Low_ = 0;
        unsigned short triVertexZ0High_ = 0;
        unsigned short triVertexZ0Low_ = 0;
        unsigned short triVertexX1High_ = 0;
        unsigned short triVertexX1Low_ = 0;
        unsigned short triVertexY1High_ = 0;
        unsigned short triVertexY1Low_ = 0;
        unsigned short triVertexZ1High_ = 0;
        unsigned short triVertexZ1Low_ = 0;
        unsigned short triVertexX2High_ = 0;
        unsigned short triVertexX2Low_ = 0;
        unsigned short triVertexY2High_ = 0;
        unsigned short triVertexY2Low_ = 0;
        unsigned short triVertexZ2High_ = 0;
        unsigned short triVertexZ2Low_ = 0;

        GLfloat cameraForwardSpeed_ = 0.0f;

        GLfloat cameraPositionX_ = 0.0f;
        GLfloat cameraPositionY_ = 0.0f;
        GLfloat cameraPositionZ_ = 0.0f;

        GLfloat cameraRotationXSpeed_ = 0.0f;
        GLfloat cameraRotationYSpeed_ = 0.0f;
        GLfloat cameraRotationZSpeed_ = 0.0f;

        GLfloat cameraRotationX_ = 0.0f;
        GLfloat cameraRotationY_ = 0.0f;
        GLfloat cameraRotationZ_ = 0.0f;

        void processData(unsigned char c, Scene *scene);

    public:
        CAtari3d(CScreenDevice *pScreen, CMemorySystem *pMemorySystem, CScheduler *pScheduler);
        ~CAtari3d(void);

        void Run (unsigned nCore);
};

#endif
