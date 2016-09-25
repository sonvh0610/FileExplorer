#pragma once

#include <vector>

using namespace std;
class Drive
{
private:
	vector<WCHAR*> mDrive;
	vector<WCHAR*> mVolName;
	vector<WCHAR*> mDisplayName;
	vector<int> mIconIndex;
public:
	Drive();
	~Drive();

	void GetSystemDrives();
	int GetCount();
	WCHAR* GetDriveName(const int i);
	WCHAR* GetVolName(const int i);
	WCHAR* GetDisplayName(const int i);
	int GetIconIndex(const int i);

	LPWSTR GetType(const int i);
	__int64 GetSize(const int i);
	LPWSTR GetSizeStr(const int i);
	__int64 GetFreeSize(const int i);
	LPWSTR GetFreeSizeStr(const int i);
};

