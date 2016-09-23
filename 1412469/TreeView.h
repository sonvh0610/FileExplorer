#ifndef __TREEVIEW_H__
#define __TREEVIEW_H__

#include <windows.h>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")
#include "Drive.h"

class TreeView
{
private:
	HINSTANCE hParentInst;
	HWND hwndParent;
	HWND hwndTreeView;
	int ID;
public:
	TreeView();
	~TreeView();
	void Create(HWND hwndParent, long ID, HINSTANCE hParentInst, int iWidth, int iHeight,
		long lExtStyle = 0, long lStyle = TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
		int x = CW_USEDEFAULT, int y = 0);
	void LoadMyComputer(Drive *drive);

	HWND GetHandle();
	int	 GetID();
	void GetFocus();
};

#endif