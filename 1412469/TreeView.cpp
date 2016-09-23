#include "stdafx.h"
#include "TreeView.h"

TreeView::TreeView()
{
	this->hwndParent = NULL;
	this->hParentInst = NULL;
	this->hwndTreeView = NULL;
	this->ID = 0;
}

TreeView::~TreeView()
{
	DestroyWindow(this->hwndTreeView);
}

void TreeView::Create(HWND hwndParent, long ID, HINSTANCE hParentInst, int iWidth, int iHeight,
	long lExtStyle, long lStyle, int x, int y)
{
	InitCommonControls();
	
	this->hwndParent = hwndParent;
	this->hParentInst = hParentInst;
	this->hwndTreeView = CreateWindowEx(lExtStyle, WC_TREEVIEW, _T("Tree View"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_SIZEBOX | WS_VSCROLL | WS_TABSTOP | lStyle, 
		x, y, iWidth, iHeight, hwndParent, (HMENU) ID, hParentInst, NULL);
	this->ID = ID;
}

void TreeView::LoadMyComputer(Drive * drive)
{
	TV_INSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	// Desktop
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = TVI_ROOT;
	tvInsert.item.iImage = 0;
	tvInsert.item.iSelectedImage = 0;
	tvInsert.item.pszText = _T("Desktop");
	tvInsert.item.lParam = (LPARAM)_T("Desktop");
	HTREEITEM hDesktop = TreeView_InsertItem(this->hwndTreeView, &tvInsert);
	
	// My Computer
	tvInsert.hParent = hDesktop;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.pszText = _T("My Computer");
	tvInsert.item.lParam = (LPARAM)_T("MyComputer");
	HTREEITEM hMyComputer = TreeView_InsertItem(this->hwndTreeView, &tvInsert);

	// Load drives
	for (int i = 0; i < drive->GetCount(); ++i)
	{
		tvInsert.hParent = hMyComputer;
		tvInsert.item.pszText = drive->GetDriveName(i);
		tvInsert.item.lParam = (LPARAM) drive->GetDriveName(i);
		HTREEITEM hDrive = TreeView_InsertItem(this->hwndTreeView, &tvInsert);

		
	}

	TreeView_Expand(this->hwndTreeView, hMyComputer, TVE_EXPAND);
	TreeView_SelectItem(this->hwndTreeView, hMyComputer);
}

HWND TreeView::GetHandle() { return this->hwndTreeView; }
int TreeView::GetID() {	return this->ID; }

void TreeView::GetFocus()
{
	SetFocus(this->hwndTreeView);
}
