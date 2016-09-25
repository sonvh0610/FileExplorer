#ifndef __LISTVIEW_H__
#define __LISTVIEW_H__

#include "Drive.h"
#include <commctrl.h>

class ListView
{
private:
	HINSTANCE	hInst;
	HWND		hwndParent;
	HWND		hwndListView;
	int			ID;

	LPWSTR _GetSize(const WIN32_FIND_DATA &fd);
	LPWSTR _GetType(const WIN32_FIND_DATA &fd);
	LPWSTR _GetDateModified(const FILETIME &ftLastWrite);
public:
	ListView();
	~ListView();

	void Create(HWND hwndParent, long ID, HINSTANCE hParentInst, int iWidth,
		int iHeight, int x = CW_USEDEFAULT, int y = 0,
		long lExtStyle = WS_EX_CLIENTEDGE,
		long lStyle = LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS);
	void Init4Cols();
	void InitDriveColumn();
	void InitFolderColumn();

	HWND GetHandle();
	LPWSTR GetPath(int iItem);
	LPWSTR GetCurrentSelectionPath();

	void LoadMyComputer(Drive* drive);
	void LoadFileAndFolder(LPWSTR path);
	void LoadCurrentSelection();

	void DeleteAll();
};

#endif