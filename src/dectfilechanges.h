#pragma once

#ifdef WIN32

#include <stdio.h>

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

//下面NOMINMAX保证std::min不重复定义
#define NOMINMAX

#include <windows.h>

class DectFileChanges
{
public:
	DectFileChanges();
	~DectFileChanges();
	void AddFile(LPCTSTR szDirectory, DWORD dwNotifyFilter);
	BOOL DetectChanges();
	void Terminate();

private:
	LPCTSTR _szFile = nullptr;
	DWORD _dwNotifyFilter = 0;
	WIN32_FILE_ATTRIBUTE_DATA _lastFileInfo = {};

};

#endif
