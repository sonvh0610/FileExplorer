#include "stdafx.h"
#include "Converter.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#define RADIX 10

enum TYPE {	Byte, KB, MB, GB, TB };


LPWSTR Converter::Convert(__int64 iSize)
{
	int iType = 0;

	while (iSize >= 1048576) // 2^20
	{
		iSize /= 1024;
		++iType;
	}

	__int64 iRight;

	if (iSize >= 1024)
	{
		iRight = iSize % 1024;

		while (iRight > 99) iRight /= 10;
		iSize /= 1024;
		++iType;
	}
	else iRight = 0;

	WCHAR* buffer = new WCHAR[11];
	_itow_s(iSize, buffer, 11, RADIX);

	if (iRight != 0 && iType > KB)
	{
		StrCat(buffer, _T("."));
		WCHAR* right = new WCHAR[3];
		_itow_s(iRight, right, 3, RADIX);
		StrCat(buffer, right);
	}

	switch (iType)
	{
	case Byte:
		StrCat(buffer, _T(" bytes"));
		break;
	case KB:
		StrCat(buffer, _T(" KB"));
		break;
	case MB:
		StrCat(buffer, _T(" MB"));
		break;
	case GB:
		StrCat(buffer, _T(" GB"));
		break;
	case TB:
		StrCat(buffer, _T(" TB"));
		break;
	}

	return buffer;
}
