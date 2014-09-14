// BaboMapFile.h: interface for the CBaboMapFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BABOMAPFILE_H__57B372F2_C591_415A_A5A3_2A2042202C1E__INCLUDED_)
#define AFX_BABOMAPFILE_H__57B372F2_C591_415A_A5A3_2A2042202C1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define CHG_END2(c) (c<<8&0xf0)|(c>>8&0xf)
#define CHG_END2(c) (c<<8|c>>8)
#define CHG_END4(c) (c>>24&0xff)|(c>>8&0xff00)|(c<<8&0xff0000)|(c<<24&0xff000000)

class CBaboMapFile  
{
public:
	void Close();
	void Init();
	BOOL Open(LPCSTR path, int mode = PAGE_READONLY );
	CBaboMapFile();
	virtual ~CBaboMapFile();
	
	BYTE operator[](INT index) {return m_Buffer[index];}
	void Seek(LONG index) { m_Position = index;}
	BYTE GetBYTE(){
		BYTE v = m_Buffer[m_Position];
		m_Position++;
		return v;
	}
	SHORT GetSHORT(){
		USHORT v = (*(USHORT*)(m_Buffer+m_Position));
		m_Position+=2;
		v = CHG_END2(v);
		return v;
	}
	USHORT GetUSHORT(){
		USHORT v = (*(USHORT*)(m_Buffer+m_Position));
		m_Position+=2;
		return CHG_END2(v);
	}
	LONG GetLONG(){
		LONG v = (*(LONG*)(m_Buffer+m_Position));
		m_Position+=4;
		return CHG_END4(v);
	}
	LONGLONG GetLONGLONG(){
		LONGLONG v = (*(LONGLONG*)(m_Buffer+m_Position));
		m_Position+=8;
		return v;
	}
	LONG GetPosition(){ return m_Position; }

private:
	BOOL m_Open;		// 생성 여부
	CString m_FilePath;	// 파일 이름
	LONG m_Position;	// file position
	HANDLE m_hFile;		// file 핸들
	HANDLE m_hFMap;		// file map 핸들
	BYTE *m_Buffer;
};

#endif // !defined(AFX_BABOMAPFILE_H__57B372F2_C591_415A_A5A3_2A2042202C1E__INCLUDED_)
