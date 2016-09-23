#pragma once

#include "resource.h"
#include "Drive.h"
#include "TreeView.h"

void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

HINSTANCE g_hInst;
HWND g_hWnd;

TreeView *g_TreeView;
Drive *g_Drive;