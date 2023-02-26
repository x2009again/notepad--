#include "dectfilechanges.h"

#ifdef WIN32

DectFileChanges::DectFileChanges()
{
	_szFile = NULL;
	_dwNotifyFilter = 0;
}


DectFileChanges::~DectFileChanges()
{
}


BOOL DectFileChanges::DetectChanges() {

	WIN32_FILE_ATTRIBUTE_DATA fInfo;
	BOOL rValue = FALSE;
	::GetFileAttributesEx(_szFile, GetFileExInfoStandard, &fInfo);

	if ((_dwNotifyFilter & FILE_NOTIFY_CHANGE_SIZE) && (fInfo.nFileSizeHigh != _lastFileInfo.nFileSizeHigh || fInfo.nFileSizeLow != _lastFileInfo.nFileSizeLow)) {
		rValue = TRUE;
	}

	if ((_dwNotifyFilter & FILE_NOTIFY_CHANGE_LAST_WRITE) && (fInfo.ftLastWriteTime.dwHighDateTime != _lastFileInfo.ftLastWriteTime.dwHighDateTime || fInfo.ftLastWriteTime.dwLowDateTime != _lastFileInfo.ftLastWriteTime.dwLowDateTime)) {
		rValue = TRUE;
	}

	_lastFileInfo = fInfo;
	return rValue;
}

void DectFileChanges::AddFile(LPCTSTR szFile, DWORD dwNotifyFilter)
{
	_szFile = szFile;
	_dwNotifyFilter = dwNotifyFilter;
	::GetFileAttributesEx(szFile, GetFileExInfoStandard, &_lastFileInfo);
}


void DectFileChanges::Terminate()
{
	_szFile = NULL;
	_dwNotifyFilter = 0;
}
#endif