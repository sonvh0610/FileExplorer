#include "stdafx.h"
#include "Drive.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#define BUFFER_LEN	105
#define IDI_FLOPPY	4
#define IDI_USB		5
#define IDI_HDD		6
#define IDI_CD		7


Drive::Drive()
{}


Drive::~Drive()
{
	for (int i = 0; i < this->GetCount(); ++i)
	{
		delete[] this->mDrive[i];
		delete[] this->mVolName[i];
		delete[] this->mDisplayName[i];
	}

	this->mDrive.clear();
	this->mVolName.clear();
	this->mDisplayName.clear();
	this->mIconIndex.clear();
}

void Drive::GetSystemDrives()
{
	TCHAR buffer[BUFFER_LEN];
	int i;

	GetLogicalDriveStrings(BUFFER_LEN, buffer);

	for (LPTSTR lpDrive = buffer; *lpDrive != 0; lpDrive += 4)
	{
		WCHAR* temp = new WCHAR[4];
		StrCpy(temp, lpDrive);
		this->mDrive.push_back(temp);
	}
	this->mVolName.resize(this->GetCount());
	this->mDisplayName.resize(this->GetCount());
	this->mIconIndex.resize(this->GetCount());

	int iType;
	for (i = 0; i < this->mDrive.size(); ++i)
	{
		iType = GetDriveType(this->mDrive[i]);
		StrCpy(buffer, _T(""));
		this->mVolName[i] = new WCHAR[30];
		this->mDisplayName[i] = new WCHAR[35];

		if ((iType == DRIVE_FIXED) || ((i > 1) && (iType == DRIVE_REMOVABLE)))
		{
			if (iType == DRIVE_FIXED) this->mIconIndex[i] = IDI_HDD;
			else this->mIconIndex[i] = IDI_USB;

			GetVolumeInformation(this->mDrive[i], buffer, BUFFER_LEN, NULL, NULL, NULL, NULL, 0);
			StrCpy(this->mVolName[i], buffer);
		}
		else
		{
			if (iType == DRIVE_CDROM)
			{
				GetVolumeInformation(this->mDrive[i], buffer, BUFFER_LEN, NULL, NULL, NULL, NULL, 0);
				if (wcslen(buffer) == 0) StrCpy(this->mVolName[i], _T("CD Rom"));
				else StrCpy(this->mVolName[i], buffer);

				this->mIconIndex[i] = IDI_CD;
			}
			else
			{
				if ((i == 0) || (i == 1 && iType == DRIVE_REMOVABLE))
				{
					StrCpy(this->mVolName[i], _T("3½ Floppy"));
					this->mIconIndex[i] = IDI_FLOPPY;
				}
			}
		}

		if (wcslen(this->mVolName[i]) == 0) StrCpy(this->mDisplayName[i], _T("Local Disk"));
		else this->mDisplayName[i] = this->mVolName[i];

		StrCat(this->mDisplayName[i], _T(" ("));
		StrNCat(this->mDisplayName[i], this->mDrive[i], 3);
		StrCat(this->mDisplayName[i], _T(")"));
	}
}

int Drive::GetCount() { return this->mDrive.size(); }
WCHAR* Drive::GetDriveName(const int i) { return this->mDrive.at(i); }
WCHAR* Drive::GetVolName(const int i) { return this->mVolName.at(i); }
WCHAR* Drive::GetDisplayName(const int i) { return this->mDisplayName.at(i); }
int Drive::GetIconIndex(const int i) { return this->mIconIndex.at(i); }
