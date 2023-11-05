#pragma optimize( "t", on )

#include "stdafx.h"   // include system headers
#include "static.h"   // static variables and constants
#include "tray.h"     // tray icon (optional)

#include <time.h>

typedef struct Key {
  SHORT keyIn;
  SHORT keyOutSeq[4];
  BOOL  keyDown;
} Key;

VOID HandleKey(Key* key) {
  if (GetAsyncKeyState(key->keyIn) && !key->keyDown) {
    if (!HOTKEYS_ON) {
      Sleep(100);
    } else for (int i = 0; i < 4; i++) {
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

DWORD WINAPI HandleInvokerKeys(LPVOID _lpParameter) {
  #pragma warning(push)
  #pragma warning(disable: 4100)
  UNREFERENCED_PARAMETER(_lpParameter);
  #pragma warning(pop)

  srand((unsigned int)time(NULL));

  Key keys[10] =
    { { VK_NUMPAD7, { 0x10, 0x10, 0x10, 0x13 } }
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

  int i;
  while (1) {
    for (i = 0; i < 10; ++i) {
      HandleKey(keys + i);
    }
    if (GetAsyncKeyState(VK_SCROLL)) {
      HOTKEYS_ON = !HOTKEYS_ON;
      Sleep(100);
    }
  }

  return 0;
}

INT WINAPI WinMain( _In_ HINSTANCE hInstance
                  , _In_opt_ HINSTANCE _hPrevInstance
                  , _In_ LPSTR _lpCmdLine
                  , _In_ int _nShowCmd ) {
  #pragma warning(push)
  #pragma warning(disable: 4100)
  UNREFERENCED_PARAMETER(_hPrevInstance);
  UNREFERENCED_PARAMETER(_lpCmdLine);
  UNREFERENCED_PARAMETER(_nShowCmd);
  #pragma warning(pop)

  // Turn on Num Lock before mutex check so even if are running already enable Num Lock
  if (!(GetKeyState(VK_NUMLOCK) & 0x0001)) {
    keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
    keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
  }

  // Turn on Scroll lock
  if (!(GetKeyState(VK_SCROLL) & 0x0001)) {
    keybd_event(VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
    keybd_event(VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
  } else HOTKEYS_ON = TRUE;

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
    wclx.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wclx.hbrBackground  = (HBRUSH)( COLOR_BTNFACE + 1 );   

    wclx.lpszMenuName   = NULL;
    wclx.lpszClassName  = MUTEX_NAME;

    RegisterClassExW( &wclx );
  }

  {
    WINDOW = CreateWindowExW( 0, MUTEX_NAME
                            , TEXT(MUTEX_NAME), WS_OVERLAPPEDWINDOW
                            , 0, 0, 0, 0, NULL, NULL, hInstance, NULL );
    if ( !WINDOW ) {
      MessageBoxW( NULL, L"Can't create window!", TEXT(MUTEX_NAME)
                 , MB_ICONERROR | MB_OK | MB_TOPMOST );
      return 1;
    }
  }

  HANDLE hThread = CreateThread(
      NULL,
      0,
      HandleInvokerKeys,
      NULL,
      0,
      NULL);
  if (hThread == NULL) {
      MessageBoxW( NULL, L"Can't create thread!", TEXT(MUTEX_NAME)
                 , MB_ICONERROR | MB_OK | MB_TOPMOST );
      return 1;
  }

  BOOL bRet; 
  MSG msg;
  while( ( bRet = GetMessageW(&msg, NULL, 0, 0) ) != 0 ) {
    if (bRet != -1)  {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }

  CloseHandle(hThread);

  // Turn off Scroll Lock
  if (GetKeyState(VK_SCROLL) & 0x0001) {
    keybd_event(VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
    keybd_event(VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
  }

  if (WINDOW) {
    RemoveTrayIcon(WINDOW, 1);
    DestroyWindow(WINDOW);
  }

  if (MUTEX_HANDLE) {
    CloseHandle(MUTEX_HANDLE);
    ReleaseMutex(MUTEX_HANDLE);
  }

  return 0;
}
