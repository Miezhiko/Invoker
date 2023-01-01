#pragma optimize( "ty", on )

#include "stdafx.h"   // include system headers
#include "static.h"   // static variables and constants
#include "tray.h"     // tray icon (optional)

#include <time.h>

typedef struct Key {
  SHORT keyIn;
  SHORT keyOutSeq[4];
  BOOL keyDown;
} Key;

VOID HandleKey(Key* key) {
  if (GetAsyncKeyState(key->keyIn) && !key->keyDown) {
    for (int i = 0; i < 4; i++) {
      INPUT ip;

      ip.type           = INPUT_KEYBOARD;
      ip.ki.time        = 0;
      ip.ki.wVk         = 0;
      ip.ki.dwExtraInfo = 0;

      ip.ki.dwFlags     = KEYEVENTF_SCANCODE;
      ip.ki.wScan       = key->keyOutSeq[i];

      Sleep(10 + (rand() % 10));

      SendInput(1, &ip, SIZE_OF_INPUT);

      Sleep(10 + (rand() % 10));

      ip.ki.dwFlags     = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
      SendInput(1, &ip, SIZE_OF_INPUT);

      Sleep(20 + (rand() % 25));
    }
    key->keyDown = TRUE;
  }
  if (!GetAsyncKeyState(key->keyIn) && key->keyDown) {
    key->keyDown = FALSE;
  }
}

INT WINAPI WinMain( _In_ HINSTANCE hInstance
                  , _In_opt_ HINSTANCE hPrevInstance
                  , _In_ LPSTR lpCmdLine
                  , _In_ int nShowCmd ) {

  // Turn on Num Lock before mutex check so even if are running already enable Num Lock
  if (!(GetKeyState(VK_NUMLOCK) & 0x0001)) {
    keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
    keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
  }

  MUTEX_HANDLE = CreateMutexW(NULL, TRUE, MUTEX_NAME);
  if(ERROR_ALREADY_EXISTS == GetLastError()) return 1;

  FindResourceW(hInstance, MAKEINTRESOURCEW(IDR_ICO_MAIN), L"ICON");

  {
    WNDCLASSEXW wclx; 
    memset(&wclx, 0, sizeof(wclx));
    wclx.cbSize         = sizeof( wclx );
    wclx.style          = 0;
    wclx.lpfnWndProc    = &WndProc;
    wclx.cbClsExtra     = 0;
    wclx.cbWndExtra     = 0;
    wclx.hInstance      = hInstance;
    wclx.hCursor        = LoadCursorW( NULL, MAKEINTRESOURCEW(IDC_ARROW) );
    wclx.hbrBackground  = (HBRUSH)( COLOR_BTNFACE + 1 );   

    wclx.lpszMenuName   = NULL;
    wclx.lpszClassName  = MUTEX_NAME;

    RegisterClassExW( &wclx );
  }

  {
    WINDOW = CreateWindowExW( 0, MUTEX_NAME
                            , TEXT(L"Title"), WS_OVERLAPPEDWINDOW
                            , 0, 0, 0, 0, NULL, NULL, hInstance, NULL );
    if ( !WINDOW ) {
      MessageBoxW(NULL, L"Can't create window!", TEXT(L"Warning!"), MB_ICONERROR | MB_OK | MB_TOPMOST);
      return 1;
    }
  }

  srand(time(NULL));

  Key keys[10] = { { VK_NUMPAD7, { 0x10, 0x10, 0x10, 0x13 } }
                 , { VK_NUMPAD4, { 0x10, 0x10, 0x11, 0x13 } }
                 , { VK_NUMPAD1, { 0x10, 0x10, 0x12, 0x13 } }
                 , { VK_NUMPAD8, { 0x11, 0x11, 0x11, 0x13 } }
                 , { VK_NUMPAD5, { 0x11, 0x11, 0x10, 0x13 } }
                 , { VK_NUMPAD2, { 0x11, 0x11, 0x12, 0x13 } }
                 , { VK_NUMPAD9, { 0x12, 0x12, 0x12, 0x13 } }
                 , { VK_NUMPAD6, { 0x12, 0x12, 0x10, 0x13 } }
                 , { VK_NUMPAD3, { 0x12, 0x12, 0x11, 0x13 } }
                 , { VK_NUMPAD0, { 0x10, 0x11, 0x12, 0x13 } }
                 };

  while (1) {
    for (int i = 0; i < 10; i++) {
      HandleKey(keys + i);
    }
    if (GetAsyncKeyState(EXIT_KEY)) {
      if (GetAsyncKeyState( VK_CONTROL ) & 0x8000) {
        break;
      }
    }
  }

  if (MUTEX_HANDLE) {
    CloseHandle(MUTEX_HANDLE);
    ReleaseMutex(MUTEX_HANDLE);
  }

  return 0;
}
