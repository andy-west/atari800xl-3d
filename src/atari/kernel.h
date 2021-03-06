#ifndef _kernel_h
#define _kernel_h

#include <circle/memory.h>
#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <circle/sched/scheduler.h>
#include <vc4/vchiq/vchiqdevice.h>
#include <circle/types.h>
#include "atari3d.h"

enum TShutdownMode
{
    ShutdownNone,
    ShutdownHalt,
    ShutdownReboot
};

class CKernel
{
    public:
        CKernel(void);
        ~CKernel(void);

        boolean Initialize(void);

        TShutdownMode Run(void);

    private:
        // Do not change this order.
        CMemorySystem       m_Memory;
        CActLED             m_ActLED;
        CKernelOptions      m_Options;
        CDeviceNameService  m_DeviceNameService;
        CScreenDevice       m_Screen;
        CSerialDevice       m_Serial;
        CExceptionHandler   m_ExceptionHandler;
        CInterruptSystem    m_Interrupt;
        CTimer              m_Timer;
        CLogger             m_Logger;
        CScheduler          m_Scheduler;

        CVCHIQDevice        m_VCHIQ;
        CAtari3d            m_Atari3d;
};

#endif
