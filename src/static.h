#pragma once

#include "stdafx.h"

static HWND WINDOW          = NULL;

#define MUTEX_NAME          L"Invoker"

const INT SIZE_OF_INPUT     = sizeof(INPUT);

static HANDLE MUTEX_HANDLE  = 0;

static BOOL HOTKEYS_ON      = FALSE;
