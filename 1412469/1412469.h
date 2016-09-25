#pragma once

#include "resource.h"
#include "Drive.h"
#include "TreeView.h"
#include "ListView.h"

void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);

HINSTANCE g_hInst;
HWND g_hWnd;

TreeView *g_TreeView;
ListView *g_ListView;
Drive *g_Drive;