#include "stdafx.h"
#include "ListView.h"
#include "resource.h"
#include <shlwapi.h>
#include "Converter.h"
#include <shellapi.h>

#define IDI_FOLDER 0
#define IDI_UNKNOWN_FILE 1
#define IDI_DESKTOP 2
#define IDI_MYCOMPUTER 3

#define IDI_FLOPPY 4
#define IDI_USB 5
#define IDI_HDD 6
#define IDI_CD  7 


LPWSTR ListView::_GetSize(const WIN32_FIND_DATA& fd)
{
	DWORD dwSize = fd.nFileSizeLow;
	return Converter::Convert(dwSize);
}

#define ENTIRE_STRING NULL

LPWSTR ListView::_GetType(const WIN32_FIND_DATA& fd)
{
	int iDotPos = StrRStrI(fd.cFileName, ENTIRE_STRING, _T(".")) - fd.cFileName;
	int len = wcslen(fd.cFileName);

	if (iDotPos < 0 || iDotPos >= len) return _T("Không biết");

	WCHAR* szExtension = new WCHAR[len - iDotPos + 1];
	int i;

	for (i = iDotPos; i < len; ++i) szExtension[i - iDotPos] = fd.cFileName[i];
	szExtension[i - iDotPos] = NULL; // End of string

	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
		return _T("Web page");

	WCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}
	RegCloseKey(hKey);

	TCHAR* pszPath = new TCHAR[1000];
	dwSize = 1000;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszPath, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}
	RegCloseKey(hKey);

	return pszPath;
}

LPWSTR ListView::_GetDateModified(const FILETIME& ftLastWrite)
{
	SYSTEMTIME stUTC, stLocal;
	FileTimeToSystemTime(&ftLastWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	WCHAR* buffer = new WCHAR[50];
	wsprintf(buffer, _T("%02d/%02d/%04d %02d:%02d %s"),
		stLocal.wDay, stLocal.wMonth, stLocal.wYear,
		(stLocal.wHour > 12) ? (stLocal.wHour / 12) : (stLocal.wHour),
		stLocal.wMinute,
		(stLocal.wHour > 12) ? (_T("Chiều")) : (_T("Sáng")));
	return buffer;
}

ListView::ListView()
{
	this->hInst = NULL;
	this->hwndListView = NULL;
	this->hwndParent = NULL;
	this->ID = 0;
}


ListView::~ListView()
{
	DestroyWindow(this->hwndListView);
}

void ListView::Create(HWND hwndParent, long ID, HINSTANCE hParentInst,
	int iWidth, int iHeight, int x, int y, long lExtStyle, long lStyle)
{
	InitCommonControls();

	this->hInst = hParentInst;
	this->hwndParent = hwndParent;
	this->hwndListView = CreateWindowEx(lExtStyle, WC_LISTVIEW, _T("List View"),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | lStyle,
		x, y, iWidth, iHeight, this->hwndParent, (HMENU)ID, this->hInst, NULL);
	this->ID = ID;

	this->Init4Cols();
}

void ListView::Init4Cols()
{
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.cx = 130;
	lvCol.pszText = _T("Tên");
	ListView_InsertColumn(this->hwndListView, 0, &lvCol);

	lvCol.fmt = LVCFMT_LEFT | LVCF_WIDTH;
	lvCol.cx = 100;
	lvCol.pszText = _T("Loại");
	ListView_InsertColumn(this->hwndListView, 1, &lvCol);

	lvCol.fmt = LVCFMT_RIGHT;
	lvCol.pszText = _T("Kích thước");
	ListView_InsertColumn(this->hwndListView, 2, &lvCol);

	lvCol.pszText = _T("Mô tả");
	ListView_InsertColumn(this->hwndListView, 3, &lvCol);
}

void ListView::InitDriveColumn()
{
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_TEXT | LVCF_FMT;

	lvCol.fmt = LVCFMT_LEFT | LVCF_WIDTH;
	lvCol.cx = 100;
	lvCol.pszText = _T("Loại");
	ListView_SetColumn(this->hwndListView, 1, &lvCol);

	lvCol.fmt = LVCFMT_RIGHT | LVCF_WIDTH;
	lvCol.cx = 80;
	lvCol.pszText = _T("Tổng dung lượng");
	ListView_SetColumn(this->hwndListView, 2, &lvCol);

	lvCol.cx = 80;
	lvCol.pszText = _T("Dung lượng trống");
	ListView_SetColumn(this->hwndListView, 3, &lvCol);
}

void ListView::InitFolderColumn()
{
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_WIDTH;
	lvCol.cx = 180;
	ListView_SetColumn(this->hwndListView, 0, &lvCol);

	lvCol.mask = LVCF_TEXT | LVCF_FMT;
	lvCol.fmt = LVCFMT_RIGHT;
	lvCol.pszText = _T("Kích thước");
	ListView_SetColumn(this->hwndListView, 1, &lvCol);

	lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 130;
	lvCol.pszText = _T("Loại tập tin");
	ListView_SetColumn(this->hwndListView, 2, &lvCol);

	lvCol.pszText = _T("Ngày chỉnh sửa");
	ListView_SetColumn(this->hwndListView, 3, &lvCol);
}

HWND ListView::GetHandle()
{
	return this->hwndListView;
}

LPWSTR ListView::GetPath(int iItem)
{
	LVITEM lv;
	lv.mask = LVIF_PARAM;
	lv.iItem = iItem;
	ListView_GetItem(this->hwndListView, &lv);
	return (LPWSTR)lv.lParam;
}

LPWSTR ListView::GetCurrentSelectionPath()
{
	return this->GetPath(ListView_GetSelectionMark(this->hwndListView));
}

void ListView::LoadMyComputer(Drive* drive)
{
	this->InitDriveColumn();
	this->DeleteAll();

	LV_ITEM lv;

	for (int i = 0; i < drive->GetCount(); ++i)
	{
		lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lv.iItem = i;

		lv.iSubItem = 0;
		lv.pszText = drive->GetDisplayName(i);
		lv.lParam = (LPARAM)drive->GetDriveName(i);
		ListView_InsertItem(this->hwndListView, &lv);

		// Load other columns
		lv.mask = LVIF_TEXT;

		// Type column
		lv.iSubItem = 1;
		lv.pszText = drive->GetType(i);
		ListView_SetItem(this->hwndListView, &lv);

		// Size column
		lv.iSubItem = 2;

		if (drive->GetIconIndex(i) != IDI_FLOPPY && drive->GetIconIndex(i) != IDI_CD)
			lv.pszText = drive->GetSizeStr(i);
		else lv.pszText = NULL;

		ListView_SetItem(this->hwndListView, &lv);

		// Free space column
		lv.iSubItem = 3;

		if (drive->GetIconIndex(i) != IDI_FLOPPY && drive->GetIconIndex(i) != IDI_CD)
			lv.pszText = drive->GetFreeSizeStr(i);
		else lv.pszText = NULL;

		ListView_SetItem(this->hwndListView, &lv);
	}
}

void ListView::LoadFileAndFolder(LPWSTR path)
{
	SetDlgItemText(GetDlgItem(this->hwndParent, IDC_ADDRESS), IDC_ADDRESS_EDIT, path);
	this->InitFolderColumn();
	this->DeleteAll();

	WCHAR buffer[10240];
	StrCpy(buffer, path);

	if (wcslen(path) == 3) StrCat(buffer, _T("*"));
	else StrCat(buffer, _T("\\*"));

	WIN32_FIND_DATA fd;
	HANDLE hFile;
	BOOL bFound = true;
	LV_ITEM lv;

	WCHAR* folderPath;
	int iItemCount = 0;

	hFile = FindFirstFileW(buffer, &fd);
	bFound = TRUE;

	if (hFile == INVALID_HANDLE_VALUE)
		bFound = FALSE;

	while (bFound)
	{
		if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN) &&
			(StrCmp(fd.cFileName, _T(".")) != 0) && (StrCmp(fd.cFileName, _T("..")) != 0))
		{
			folderPath = new WCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
			StrCpy(folderPath, path);

			if (wcslen(path) != 3)
				StrCat(folderPath, _T("\\"));

			StrCat(folderPath, fd.cFileName);

			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = iItemCount;
			lv.iSubItem = 0;
			lv.pszText = fd.cFileName;
			lv.lParam = (LPARAM)folderPath;
			lv.iImage = IDI_FOLDER;
			
			ListView_InsertItem(this->hwndListView, &lv);

			ListView_SetItemText(this->hwndListView, iItemCount, 2, _T("Thư mục"));

			ListView_SetItemText(this->hwndListView, iItemCount, 3, this->_GetDateModified(fd.ftLastWriteTime));

			++iItemCount;
		}

		bFound = FindNextFileW(hFile, &fd);
	}

	DWORD folderCount = iItemCount;

	WCHAR* filePath;
	DWORD fileSizeCount = 0;
	DWORD fileCount = 0;

	hFile = FindFirstFileW(buffer, &fd);
	bFound = TRUE;

	if (hFile == INVALID_HANDLE_VALUE) bFound = FALSE;

	while (bFound)
	{
		if (((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN))
		{
			filePath = new WCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
			StrCpy(filePath, path);

			if (wcslen(path) != 3) StrCat(filePath, _T("\\"));
			
			StrCat(filePath, fd.cFileName);

			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = iItemCount;
			lv.iSubItem = 0;
			lv.pszText = fd.cFileName;
			lv.lParam = (LPARAM)filePath;
			
			ListView_InsertItem(this->hwndListView, &lv);

			ListView_SetItemText(this->hwndListView, iItemCount, 1, this->_GetSize(fd));
			fileSizeCount += fd.nFileSizeLow;

			ListView_SetItemText(this->hwndListView, iItemCount, 2, this->_GetType(fd));

			ListView_SetItemText(this->hwndListView, iItemCount, 3, this->_GetDateModified(fd.ftLastWriteTime));

			++iItemCount;
			++fileCount;
		}

		bFound = FindNextFileW(hFile, &fd);
	}

	TVITEMEX tv;
	WCHAR* folder = new WCHAR[512];
	WCHAR* info = new WCHAR[256];

	tv.mask = TVIF_TEXT;
	tv.hItem = TreeView_GetNextItem(GetDlgItem(this->hwndParent, IDC_TREEVIEW), NULL, TVGN_CARET);
	tv.pszText = folder;
	tv.cchTextMax = 256;
	TreeView_GetItem(GetDlgItem(this->hwndParent, IDC_TREEVIEW), &tv);
}

void ListView::LoadCurrentSelection()
{
	LPWSTR path = this->GetCurrentSelectionPath();
	WIN32_FIND_DATA fd;
	GetFileAttributesEx(path, GetFileExInfoStandard, &fd);

	if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		ListView_DeleteAllItems(this->hwndListView);
		this->LoadFileAndFolder(path);
	}
	else
		ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
}

void ListView::DeleteAll()
{
	ListView_DeleteAllItems(this->hwndListView);
}
