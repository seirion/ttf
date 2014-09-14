  // BaboMapFile.cpp: implementation of the CBaboMapFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "B2TTF.h"
#include "BaboMapFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaboMapFile::CBaboMapFile()
{
	Init();
}
void CBaboMapFile::Init()
{
	m_Open = FALSE;	
	m_FilePath.Empty();
	m_Position = 0;
}
CBaboMapFile::~CBaboMapFile()
{

}

//
// 파일 열기
BOOL CBaboMapFile::Open(LPCSTR path, int mode )
{
	m_hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( m_hFile == INVALID_HANDLE_VALUE ) return FALSE;

	m_Open = TRUE;
	m_FilePath = path;

	m_hFMap = CreateFileMapping( m_hFile, NULL, mode, 0, 0, NULL);
	m_Buffer = (BYTE *)MapViewOfFile(m_hFMap, FILE_MAP_READ, 0, 0, 0);
	
	return TRUE;
}


void CBaboMapFile::Close()
{
	if( !m_Open ) return;
	m_Open = FALSE;
	CloseHandle(m_hFMap);
	CloseHandle(m_hFile);
}

