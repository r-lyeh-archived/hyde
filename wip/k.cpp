#include <windows.h>
#include <iostream>
#pragma comment(lib, "user32.lib")

struct riMgr_ {
    int mxr = 0, myr = 0, mzr = 0;
} riMgr;

void riProcessMouseMessage( const RAWMOUSE* rmouse )
{
    //MessageBox(NULL, L"Game Found", L"SUCCESS", MB_OK);
    if ( MOUSE_MOVE_RELATIVE == rmouse->usFlags )
    {
        riMgr.mxr = rmouse->lLastX;
        riMgr.myr = rmouse->lLastY;
    }
    riMgr.mzr = (RI_MOUSE_WHEEL & rmouse->usButtonFlags) ? rmouse->usButtonData : 0;
}

HWND get_hwnd() {
CURSORINFO ci;
ci.cbSize = sizeof(CURSORINFO);

// disable clipping
ClipCursor( 0 );

if( GetCursorInfo( &ci ) )
{
    POINT pPoint = ci.ptScreenPos;
    HWND hWnd = WindowFromPoint( pPoint );
    return hWnd;
}

return 0;
}


int main() {

auto hWnd = get_hwnd();

static RAWINPUTDEVICE Rid[2];
                // Keyboard
                Rid[0].usUsagePage = 1;
                Rid[0].usUsage = 6;
                Rid[0].dwFlags = 0;
                Rid[0].hwndTarget=hWnd;

                // Mouse
                Rid[1].usUsagePage = 1;
                Rid[1].usUsage = 2;
                Rid[1].dwFlags = 0;
                Rid[1].hwndTarget=hWnd;
                if (!RegisterRawInputDevices(Rid,2,sizeof(RAWINPUTDEVICE)))
                {
                    MessageBoxA(NULL, "Failed to Register Input Devices!", "ALERT", MB_OK);
                    exit(1);
                }

    for(;;) {
        // Determine how big the buffer should be
        UINT iBuffer;

        GetRawInputData(Rid[1].hDevice, RID_INPUT, NULL, &iBuffer,
            sizeof(RAWINPUTHEADER));
        LPBYTE lpb = new BYTE[iBuffer];
        if (lpb == NULL)
        {
            return 0;
        }

        UINT readSize = GetRawInputData( Rid[1].hDevice, RID_INPUT, lpb, &iBuffer, sizeof(RAWINPUTHEADER) ) ;

        if( readSize != iBuffer )
            puts( "ERROR:  GetRawInputData didn't return correct size!" ) ;
        RAWINPUT *raw = (RAWINPUT*) lpb;

        if (raw->header.dwType== RIM_TYPEMOUSE)
        {
            riProcessMouseMessage(&raw->data.mouse);
        }
        if (raw->header.dwType== RIM_TYPEKEYBOARD)
        {
            //riProcessKeyboardMessage(&raw->data.keyboard);
        }
    }
}
