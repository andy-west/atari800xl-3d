#include <circle/string.h>
#include "atari3d.h"

CAtari3d::CAtari3d(CScreenDevice *pScreen, CMemorySystem *pMemorySystem, CScheduler *pScheduler)
:   CMultiCoreSupport(pMemorySystem),
    m_pScreen(pScreen),
    m_pScheduler(pScheduler)
{
}

CAtari3d::~CAtari3d(void)
{
    m_pScheduler = 0;
    m_pScreen = 0;
}

void CAtari3d::Run(unsigned nCore)
{
    switch (nCore)
    {
        case 0:
            {
                CGPIOPin Clock(26, GPIOModeInput);

                while (true)
                {
                    while(Clock.Read() == HIGH);
                    while(Clock.Read() == LOW);

                    u32 pinValues = CGPIOPin::ReadAll();

                    u8 readWrite = (pinValues & (1 << 27)) >> 27;

                    CString message;

                    if ((readWrite == LOW) && ((pinValues & 0xFFFF) == 0x0680))
                    {
                        unsigned char c = (pinValues & (0xFF << 17)) >> 17;
                        atariData_[dataWriteIndex_] = c;
                        dataWriteIndex_++;

                        //if (isGeometryLoaded_ == 0)
                        {
                            message.Format("%02X ", c);
                            m_pScreen->Write((const char *)message, 3);
                        }
                    }
                }

                break;
            }

        case 1:
            {
                Scene scene;
                float const scale = 1600.0f;

                while (true)
                {
                    m_pScheduler->Yield();

                    if (isGeometryLoaded_ == 0)
                    {
                        while (dataReadIndex_ < dataWriteIndex_)
                        {
                            unsigned char c = atariData_[dataReadIndex_];

                            switch(nextByteType_)
                            {
                                case commNone:
                                    processData(c, &scene);
                                    break;
                                case commBgRed:
                                    backgroundRed_ = c / 255.0f;
                                    nextByteType_ = commBgGreen;
                                    break;
                                case commBgGreen:
                                    backgroundGreen_ = c / 255.0f;
                                    nextByteType_ = commBgBlue;
                                    break;
                                case commBgBlue:
                                    backgroundBlue_ = c / 255.0f;
                                    scene.setBackgroundColor(backgroundRed_, backgroundGreen_, backgroundBlue_);
                                    nextByteType_ = commNone;
                                    break;
                                case commColorRed:
                                    colorRed_ = c / 255.0f;
                                    nextByteType_ = commColorGreen;
                                    break;
                                case commColorGreen:
                                    colorGreen_ = c / 255.0f;
                                    nextByteType_ = commColorBlue;
                                    break;
                                case commColorBlue:
                                    colorBlue_ = c / 255.0f;
                                    scene.setColor(colorRed_, colorGreen_, colorBlue_);
                                    nextByteType_ = commNone;
                                    break;
                                case commTriX0High:
                                    triVertexX0High_ = c;
                                    nextByteType_ = commTriX0Low;
                                    break;
                                case commTriX0Low:
                                    triVertexX0Low_ = c;
                                    nextByteType_ = commTriY0High;
                                    break;
                                case commTriY0High:
                                    triVertexY0High_ = c;
                                    nextByteType_ = commTriY0Low;
                                    break;
                                case commTriY0Low:
                                    triVertexY0Low_ = c;
                                    nextByteType_ = commTriZ0High;
                                    break;
                                case commTriZ0High:
                                    triVertexZ0High_ = c;
                                    nextByteType_ = commTriZ0Low;
                                    break;
                                case commTriZ0Low:
                                    triVertexZ0Low_ = c;
                                    nextByteType_ = commTriX1High;
                                    break;
                                case commTriX1High:
                                    triVertexX1High_ = c;
                                    nextByteType_ = commTriX1Low;
                                    break;
                                case commTriX1Low:
                                    triVertexX1Low_ = c;
                                    nextByteType_ = commTriY1High;
                                    break;
                                case commTriY1High:
                                    triVertexY1High_ = c;
                                    nextByteType_ = commTriY1Low;
                                    break;
                                case commTriY1Low:
                                    triVertexY1Low_ = c;
                                    nextByteType_ = commTriZ1High;
                                    break;
                                case commTriZ1High:
                                    triVertexZ1High_ = c;
                                    nextByteType_ = commTriZ1Low;
                                    break;
                                case commTriZ1Low:
                                    triVertexZ1Low_ = c;
                                    nextByteType_ = commTriX2High;
                                    break;
                                case commTriX2High:
                                    triVertexX2High_ = c;
                                    nextByteType_ = commTriX2Low;
                                    break;
                                case commTriX2Low:
                                    triVertexX2Low_ = c;
                                    nextByteType_ = commTriY2High;
                                    break;
                                case commTriY2High:
                                    triVertexY2High_ = c;
                                    nextByteType_ = commTriY2Low;
                                    break;
                                case commTriY2Low:
                                    triVertexY2Low_ = c;
                                    nextByteType_ = commTriZ2High;
                                    break;
                                case commTriZ2High:
                                    triVertexZ2High_ = c;
                                    nextByteType_ = commTriZ2Low;
                                    break;
                                case commTriZ2Low:
                                    triVertexZ2Low_ = c;

                                    scene.createTriangle(((GLfloat)(triVertexX0High_ * 256 + triVertexX0Low_)) / scale,
                                        ((GLfloat)(triVertexY0High_ * 256 + triVertexY0Low_)) / scale,
                                        ((GLfloat)(triVertexZ0High_ * 256 + triVertexZ0Low_)) / scale,
                                        ((GLfloat)(triVertexX1High_ * 256 + triVertexX1Low_)) / scale,
                                        ((GLfloat)(triVertexY1High_ * 256 + triVertexY1Low_)) / scale,
                                        ((GLfloat)(triVertexZ1High_ * 256 + triVertexZ1Low_)) / scale,
                                        ((GLfloat)(triVertexX2High_ * 256 + triVertexX2Low_)) / scale,
                                        ((GLfloat)(triVertexY2High_ * 256 + triVertexY2Low_)) / scale,
                                        ((GLfloat)(triVertexZ2High_ * 256 + triVertexZ2Low_)) / scale);

                                    nextByteType_ = commNone;
                                    break;
                                default:
                                    break;
                            }

                            dataReadIndex_++;
                        }
                    }
                    else if (isGeometryLoaded_ == 1)
                    {
                        while (dataReadIndex_ < dataWriteIndex_)
                        {
                            unsigned char c = atariData_[dataReadIndex_];

                            switch(nextByteType_)
                            {
                                case commNone:
                                    processData(c, &scene);
                                    break;
                                case commSetCameraPositionX:
                                    cameraPositionX_ = ((GLfloat)c) - 128;
                                    nextByteType_ = commSetCameraPositionY;
                                    break;
                                case commSetCameraPositionY:
                                    cameraPositionY_ = ((GLfloat)c) - 128;
                                    nextByteType_ = commSetCameraPositionZ;
                                    break;
                                case commSetCameraPositionZ:
                                    cameraPositionZ_ = ((GLfloat)c) - 128;
                                    scene.setCameraPosition(cameraPositionX_, cameraPositionY_, cameraPositionZ_);
                                    nextByteType_ = commNone;
                                    break;
                                default:
                                    break;
                            }

                            dataReadIndex_++;
                        }

                        scene.draw();
                    }
                }

                break;
            }
        case 2:
            break;
        case 3:
            break;
    }
}

void CAtari3d::processData(unsigned char c, Scene *scene)
{
    if (isGeometryLoaded_ == 0)
    {
        switch(c)
        {
            case 'B':  // Set background color.
                nextByteType_ = commBgRed;
                break;
            case 'C':  // Set color.
                nextByteType_ = commColorRed;
                break;
            case 'D':  // Delete all geometry.
                scene->deleteAllGeometry();
                break;
            case 'T':  // Create triangle.
                nextByteType_ = commTriX0High;
                break;
            case 'E':  // End loading geometry.
                isGeometryLoaded_ = 1;
                break;
        }
    }
    else if (isGeometryLoaded_ == 1)
    {
        switch(c)
        {
            case 'M':  // Start moving camera forward.
                cameraForwardSpeed_ = 0.05f;
                scene->setCameraForwardSpeed(cameraForwardSpeed_);
                break;
            case 'S':  // Stop moving.
                cameraForwardSpeed_ = 0.0f;
                scene->setCameraForwardSpeed(cameraForwardSpeed_);
                break;
            case 'X':  // Increase camera X axis rotation.
                cameraRotationXSpeed_ += 0.1f;
                scene->setCameraRotationXSpeed(cameraRotationXSpeed_);
                break;
            case 'Y':  // Increase camera Y axis rotation.
                cameraRotationYSpeed_ += 0.1f;
                scene->setCameraRotationYSpeed(cameraRotationYSpeed_);
                break;
            case 'Z':  // Increase camera Z axis rotation.
                cameraRotationZSpeed_ += 0.1f;
                scene->setCameraRotationZSpeed(cameraRotationZSpeed_);
                break;
            case 'U':  // Decrease camera X axis rotation.
                cameraRotationXSpeed_ -= 0.1f;
                scene->setCameraRotationXSpeed(cameraRotationXSpeed_);
                break;
            case 'V':  // Decrease camera Y axis rotation.
                cameraRotationYSpeed_ -= 0.1f;
                scene->setCameraRotationYSpeed(cameraRotationYSpeed_);
                break;
            case 'W':  // Decrease camera Z axis rotation.
                cameraRotationZSpeed_ -= 0.1f;
                scene->setCameraRotationZSpeed(cameraRotationZSpeed_);
                break;
            case 'P':  // Set camera position.
                nextByteType_ = commSetCameraPositionX;
                break;
            case 'R':  // Reset
                cameraForwardSpeed_ = 0.0f;
                scene->setCameraForwardSpeed(cameraForwardSpeed_);

                cameraRotationXSpeed_ = 0.0f;
                scene->setCameraRotationXSpeed(cameraRotationXSpeed_);

                cameraRotationYSpeed_ = 0.0f;
                scene->setCameraRotationYSpeed(cameraRotationYSpeed_);

                cameraRotationZSpeed_ = 0.0f;
                scene->setCameraRotationZSpeed(cameraRotationZSpeed_);
                
                cameraPositionX_ = 5.0f;
                cameraPositionY_ = 1.0f;
                cameraPositionZ_ = 25.0f;
                scene->setCameraPosition(cameraPositionX_, cameraPositionY_, cameraPositionZ_);

                cameraRotationX_ = 0.0f;
                scene->setCameraRotationX(cameraRotationX_);

                cameraRotationY_ = 0.0f;
                scene->setCameraRotationY(cameraRotationY_);

                cameraRotationZ_ = 0.0f;
                scene->setCameraRotationZ(cameraRotationZ_);

                break;
        }
    }
}
