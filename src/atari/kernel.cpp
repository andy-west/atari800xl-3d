#include "kernel.h"

static const char FromKernel[] = "kernel";

CKernel::CKernel(void)
:   m_Screen(m_Options.GetWidth(), m_Options.GetHeight()),
    m_Timer(&m_Interrupt),
    m_Logger(m_Options.GetLogLevel(), &m_Timer),
    m_VCHIQ(&m_Memory, &m_Interrupt),
    m_Atari3d(&m_Screen, &m_Memory, &m_Scheduler)
{
    m_ActLED.Blink (5);  // Show that we are alive.
}

CKernel::~CKernel(void)
{
}

boolean CKernel::Initialize(void)
{
    boolean bOK = TRUE;

    if (bOK)
    {
        bOK = m_Screen.Initialize();
    }

    if (bOK)
    {
        bOK = m_Serial.Initialize(115200);
    }

    if (bOK)
    {
        CDevice *pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);

        if (pTarget == 0)
        {
            pTarget = &m_Screen;
        }

        bOK = m_Logger.Initialize(pTarget);
    }

    if (bOK)
    {
        bOK = m_Interrupt.Initialize();
    }

    if (bOK)
    {
        bOK = m_Timer.Initialize();
    }

    if (bOK)
    {
        bOK = m_VCHIQ.Initialize();
    }

    if (bOK)
    {
        bOK = m_Atari3d.Initialize();  // Must be initialized last.
    }

    return bOK;
}

TShutdownMode CKernel::Run(void)
{
    m_Logger.Write(FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);

    m_Atari3d.Run(0);

    return ShutdownHalt;
}
