#include "stdafx.h"
#include "static.h"

#include <shellapi.h>
#include <winuser.h>

#define IDR_ICO_MAIN        101
#define IDS_DRAGON          102

#define ID_ABOUT            2000
#define ID_EXIT             2002

static POINT CURSOR_POSITION;

static BOOL MODAL_STATE     = FALSE;

VOID DragonBox(HWND hWnd, LPCWSTR title, UINT flags) {
  MODAL_STATE = TRUE;

  WCHAR* dragon = malloc(MAX_PATH * sizeof(WCHAR));
  LoadStringW(GetModuleHandle(NULL), IDS_DRAGON, dragon, MAX_PATH);
  MessageBoxW( hWnd, dragon, title, flags );
  free( dragon );

  MODAL_STATE = FALSE;
}

VOID RemoveTrayIcon( HWND hWnd, UINT uID ) {
  NOTIFYICONDATAW nid;
                  nid.hWnd = hWnd;
                  nid.uID  = uID;

  Shell_NotifyIconW( NIM_DELETE, &nid );
}

VOID AddTrayIcon( HWND hWnd, UINT uID, UINT uCallbackMsg ) {
  NOTIFYICONDATAW nid;
                  nid.hWnd             = hWnd;
                  nid.uID              = uID;
                  nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
                  nid.uCallbackMessage = uCallbackMsg;

  WCHAR buffer[MAX_PATH];
  GetModuleFileNameW( NULL, buffer, MAX_PATH );
  ExtractIconExW    ( buffer, 0, NULL, &(nid.hIcon), 1 );
  wcscpy_s          ( nid.szTip, _countof(nid.szTip), MUTEX_NAME );

  Shell_NotifyIconW( NIM_ADD, &nid );
}

BOOL ShowPopupMenu( HWND hWnd, POINT *curpos ) {
  HMENU hPop = CreatePopupMenu();
  if ( MODAL_STATE ) return FALSE;

  InsertMenuW( hPop, 0, MF_BYPOSITION | MF_STRING, ID_ABOUT,  L"About" );
  InsertMenuW( hPop, 2, MF_BYPOSITION | MF_STRING, ID_EXIT,   L"Exit" );

  SetMenuDefaultItem( hPop, ID_ABOUT, FALSE );
  SetFocus          ( hWnd );
  SendMessageW      ( hWnd, WM_INITMENUPOPUP, (WPARAM)hPop, 0 );

  {
    if (!curpos) {
      GetCursorPos( &CURSOR_POSITION );
      curpos = &CURSOR_POSITION;
    }

    {
      BOOL cmd = TrackPopupMenu( hPop, TPM_LEFTALIGN
                                     | TPM_RIGHTBUTTON
                                     | TPM_RETURNCMD
                                     | TPM_NONOTIFY
                                     , curpos->x, curpos->y, 0, hWnd, NULL );
      SendMessageW( hWnd, WM_COMMAND, cmd, 0 );
    }
  }

  DestroyMenu( hPop );
  return 0;
}

static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) { 
  switch (uMsg) {
    case WM_CREATE:
      AddTrayIcon( hWnd, 1, WM_APP );
      return 0;
    case WM_CLOSE:
      RemoveTrayIcon( hWnd, 1 );
      PostQuitMessage( 0 );

      return DefWindowProcW( hWnd, uMsg, wParam, lParam );

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case ID_ABOUT:
          DragonBox( hWnd, MUTEX_NAME, MB_ICONINFORMATION | MB_OK );
          return 0;

        case ID_EXIT:
          PostMessageW( hWnd, WM_CLOSE, 0, 0 );
          return 0;
      }
      return 0;

    case WM_APP:
      switch (lParam) {
        case WM_LBUTTONDBLCLK:
          DragonBox( hWnd, MUTEX_NAME, MB_ICONINFORMATION | MB_OK );
          return 0;

        case WM_RBUTTONUP:
          SetForegroundWindow( hWnd );
          ShowPopupMenu( hWnd, NULL );
          PostMessageW( hWnd, WM_APP + 1, 0, 0 );
          return 0;
      }

      return 0;
  }

  return DefWindowProcW( hWnd, uMsg, wParam, lParam );
}
