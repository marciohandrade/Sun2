#pragma once

#ifdef _DEV_VER
#if (WZENGINEVER >= 480)

namespace LogManager
{
    void Initialize();
    void Release();

    void ToggleConsoleView();
    void ProcessCommand( LPCTSTR command );
};

#endif // (WZENGINEVER >= 480)
#endif // _DEV_VER
