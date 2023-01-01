#include "stdafx.h"
#include "static.h"

#include <shellapi.h>
#include <winuser.h>

void RemoveTrayIcon( HWND hWnd, UINT uID ) {
  NOTIFYICONDATAW nid;
                  nid.hWnd = hWnd;
                  nid.uID  = uID;

  Shell_NotifyIconW( NIM_DELETE, &nid );
}

void AddTrayIcon( HWND hWnd, UINT uID, UINT uCallbackMsg, UINT uIcon ) {
  NOTIFYICONDATAW nid;
                  nid.hWnd             = hWnd;
                  nid.uID              = uID;
                  nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
                  nid.uCallbackMessage = uCallbackMsg;

  WCHAR buffer[MAX_PATH];
  GetModuleFileNameW( NULL, buffer, MAX_PATH );
  ExtractIconExW( buffer, 0, NULL, &(nid.hIcon), 1 );
  wcscpy        ( nid.szTip, MUTEX_NAME );

  Shell_NotifyIconW( NIM_ADD, &nid );
}

static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) { 
  if (uMsg == WM_CREATE) {
    AddTrayIcon( hWnd, 1, WM_APP, 0 );
    return 0;
  }

  return DefWindowProcW( hWnd, uMsg, wParam, lParam );
}
