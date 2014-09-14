                                                // BaboTTF.cpp: implementation of the BaboTTF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "B2TTF.h"
#include "BaboTTF.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BaboTTF::BaboTTF()
{

	m_LoadFont = FALSE;
	m_LoadGlyph = FALSE;
	m_CompositeGlyphNum = 0;
	m_Composite = FALSE;
	ttc_header.numFont = 0;
}

BaboTTF::~BaboTTF()
{
	Release();
}

BOOL BaboTTF::LoadFont(LPCSTR path)
{
	if( !m_File.Open(path) )
	{
		return FALSE;
	}

	USHORT i;
	int j;

	// *.ttc 
	CString title = path;
	title = title.Right(3);
	if( title.CompareNoCase("ttc") == 0)
	{
		ttc_header.TTCTag[0] = m_File.GetBYTE();
		ttc_header.TTCTag[1] = m_File.GetBYTE();
		ttc_header.TTCTag[2] = m_File.GetBYTE();
		ttc_header.TTCTag[3] = m_File.GetBYTE();
		title.Format("%c%c%c%c", ttc_header.TTCTag[0], ttc_header.TTCTag[1], ttc_header.TTCTag[2], ttc_header.TTCTag[3]);
		if( title.Compare("ttcf") != 0 ) return FALSE;

		ttc_header.version = m_File.GetLONG();
		ttc_header.numFont = m_File.GetLONG();

		ttc_header.OffsetTable = new ULONG [ttc_header.numFont];
		for( i = 0; i < ttc_header.numFont; i++)
			ttc_header.OffsetTable[i] = m_File.GetLONG();

		// only version 2.0
		// if( ttc_header.version == 0x00020000 )
		// {
		// }

		m_File.Seek(ttc_header.OffsetTable[0]);
	}

	//////////////////////////////////////////////////////////////////////////
	dtd.sfnt_version = m_File.GetLONG();
	dtd.numTables = m_File.GetUSHORT();
	dtd.searchRange = m_File.GetUSHORT();
	dtd.entrySelector = m_File.GetUSHORT();
	dtd.rangeShift = m_File.GetUSHORT();
	
	
	CString tag;
	B2_Table_Directory dir;
	for( i = 0; i < dtd.numTables; i++)
	{
		j = m_File.GetPosition();
		tag.Format("%c%c%c%c", m_File[j],m_File[j+1],m_File[j+2],m_File[j+3]); // getting tag
		
		dir.tag = m_File.GetLONG();
		dir.checkSum = m_File.GetLONG();
		dir.offset = m_File.GetLONG();
		dir.length = m_File.GetLONG();

		if( tag.Compare("cmap") == 0 ) dir_cmap = dir;
		else if( tag.Compare("glyf") == 0 ) dir_glyf = dir;
		else if( tag.Compare("head") == 0 ) dir_head = dir;
		else if( tag.Compare("loca") == 0 ) dir_loca = dir;
		else if( tag.Compare("maxp") == 0 ) dir_maxp = dir;
		else if( tag.Compare("OS/2") == 0 ) dir_os_2 = dir;
		
	}

	//////////////////////////////////////////////////////////////////////////
	// level 1
	Get_cmap(m_File);
	Get_head(m_File);
	Get_maxp(m_File);
	Get_loca(m_File);
	Get_os_2(m_File);
//	Get_glyf(m_File); // this is invoked at last time . (!)
	m_LoadFont = TRUE;
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// (level 1)
void BaboTTF::Get_head(CBaboMapFile& file)
{
	// head
	file.Seek( dir_head.offset);

	tb_head.version = file.GetLONG();
	tb_head.fontRevision = file.GetLONG();
	tb_head.checkSum = file.GetLONG();
	tb_head.magicNumber = file.GetLONG();

	tb_head.flags = file.GetUSHORT();
	tb_head.unitsPerEm = file.GetUSHORT();
	tb_head.created = file.GetLONGLONG();
	tb_head.modified = file.GetLONGLONG();

	tb_head.xMin = file.GetSHORT();
	tb_head.yMin = file.GetSHORT();
	tb_head.xMax = file.GetSHORT();
	tb_head.yMax = file.GetSHORT();
	tb_head.macStyle = file.GetUSHORT();
	tb_head.lowestRecPPEM = file.GetUSHORT();
	tb_head.fontDirectionHint = file.GetSHORT();
	tb_head.indexToLocFormat = file.GetSHORT();
	tb_head.glyphDataFormat = file.GetSHORT();

}

void BaboTTF::Get_cmap(CBaboMapFile& file)
{
	int i, j;
	// cmap
	file.Seek( dir_cmap.offset);
	
	tb_cmap.table_version = file.GetUSHORT();
	tb_cmap.numTables = file.GetUSHORT();

	B2_cmap_Sub cmap_sub; 
	for( i = 0; i < tb_cmap.numTables; i++)
	{
		cmap_sub.platformID = file.GetUSHORT();
		cmap_sub.encodingID = file.GetUSHORT();
		cmap_sub.offset = file.GetLONG()+dir_cmap.offset;

		if( cmap_sub.platformID == 3 && cmap_sub.encodingID == 1)
		{
			tb_cmap_sub = cmap_sub;
			break;
		}
	}
	
	file.Seek(tb_cmap_sub.offset);

	tb_cmap_mapping.format = file.GetUSHORT();
	tb_cmap_mapping.length = file.GetUSHORT();
	tb_cmap_mapping.version = file.GetUSHORT();
	tb_cmap_mapping.segCountX2 = file.GetUSHORT();
	tb_cmap_mapping.searchRange = file.GetUSHORT();
	tb_cmap_mapping.entrySelector = file.GetUSHORT();
	tb_cmap_mapping.rangeShift = file.GetUSHORT();


	// array (memory allocation) the size = segCount
	tb_cmap_mapping.startCount = new USHORT[tb_cmap_mapping.segCountX2/2];
	tb_cmap_mapping.endCount = new USHORT[tb_cmap_mapping.segCountX2/2];
	tb_cmap_mapping.idDelta = new USHORT[tb_cmap_mapping.segCountX2/2];
	tb_cmap_mapping.idRangeOffset = new USHORT[tb_cmap_mapping.segCountX2/2];

	j = tb_cmap_mapping.length - (tb_cmap_mapping.segCountX2*4);
	tb_cmap_mapping.glyphIdArray = new USHORT[j/2];
	//////////////////////////////////////////////////////////////////////////
	for( i = 0; i < tb_cmap_mapping.segCountX2/2; i++)
		tb_cmap_mapping.endCount[i] = file.GetUSHORT();

	tb_cmap_mapping.reservedPad = file.GetUSHORT(); // reserved pad

	for( i = 0; i < tb_cmap_mapping.segCountX2/2; i++)
		tb_cmap_mapping.startCount[i] = file.GetUSHORT();

	for( i = 0; i < tb_cmap_mapping.segCountX2/2; i++)
		tb_cmap_mapping.idDelta[i] = file.GetUSHORT();
	
	for( i = 0; i < tb_cmap_mapping.segCountX2/2; i++)
		tb_cmap_mapping.idRangeOffset[i] = file.GetUSHORT();

	for( i = 0; i < j/2; i++)
		tb_cmap_mapping.glyphIdArray[i] = file.GetUSHORT();
}

void BaboTTF::Get_glyf(USHORT index, int count)
{
	//if( count == -1)
	// glyf
	ULONG offset;
	m_File.Seek( dir_loca.offset);
	offset = tb_loca.offset[index];
	m_File.Seek( dir_glyf.offset+offset);

	if( count > 0 )
	{
		tb_glyf.oldNumberOfContours = tb_glyf.numberOfContours;
		tb_glyf.numberOfContours += m_File.GetSHORT();
	}
	else tb_glyf.numberOfContours = m_File.GetSHORT();

	tb_glyf.xMin = m_File.GetSHORT();
	tb_glyf.yMin = m_File.GetSHORT();
	tb_glyf.xMax = m_File.GetSHORT();
	tb_glyf.yMax = m_File.GetSHORT();


	if( tb_glyf.numberOfContours > 0 ) Glyph_Simple_des( count);
	else Glyph_Composite_des(); //return;//
}

void BaboTTF::Glyph_Simple_des(int count)
{
	int i, j;
	BYTE repeat;
	int flag_num, start = 0;
	
	/* simple or composite glyph */
	if( count > 0 ) // composite glyph
	{
		Composite_Data_Set();

		tb_glyf_des.instructionLength = m_File.GetUSHORT();
		tb_glyf_des.instruction = new BYTE[tb_glyf_des.instructionLength];

		start = tb_glyf_des.endPtsOfContours[tb_glyf.oldNumberOfContours-1]+1;
		flag_num = tb_glyf_des.endPtsOfContours[tb_glyf.numberOfContours-1]+1;
	}
	else // simple glyph
	{
		tb_glyf_des.endPtsOfContours = new USHORT[tb_glyf.numberOfContours];
		for( i = 0; i < tb_glyf.numberOfContours; i++ )
			tb_glyf_des.endPtsOfContours[i] = m_File.GetUSHORT();
		flag_num = tb_glyf_des.endPtsOfContours[i-1]+1;
		
		tb_glyf_des.instructionLength = m_File.GetUSHORT();
		tb_glyf_des.instruction = new BYTE[tb_glyf_des.instructionLength];

		tb_glyf_des.flags = new BYTE[flag_num];
		tb_glyf_des.xCoordinates = new SHORT[flag_num];
		tb_glyf_des.yCoordinates = new SHORT[flag_num];
	}

	//////////////////////////////////////////////////////////////////////////
	// data
	for( i = 0; i < tb_glyf_des.instructionLength; i++ )
		tb_glyf_des.instruction[i] = m_File.GetBYTE();

	
	for( i = start; i < flag_num; i++)
	{
		tb_glyf_des.flags[i] = m_File.GetBYTE();
		if( tb_glyf_des.flags[i] & 0x08 ) // repeat
		{
			repeat = m_File.GetBYTE();
			for( j = 0; j < repeat; j++, i++ )
				tb_glyf_des.flags[i+1] = tb_glyf_des.flags[i];
		
		}
	}
	//////////////////////////////////////////////////////////////////////////
	SHORT in_before, in_now;
	in_before = 0;
	for( i = start; i < flag_num; i++)
	{
	
		if( tb_glyf_des.flags[i] & 0x02 && tb_glyf_des.flags[i] & 0x10 ) // 1 & 1
		{
			in_now = (USHORT)m_File.GetBYTE();
			tb_glyf_des.xCoordinates[i] = in_before+in_now;
		}
		else if ( tb_glyf_des.flags[i] & 0x02 && !(tb_glyf_des.flags[i] & 0x10) ) // 1 & 0
		{
			in_now = (USHORT)m_File.GetBYTE();
			tb_glyf_des.xCoordinates[i] = in_before-in_now;
		}
		else if ( !(tb_glyf_des.flags[i] & 0x02) && tb_glyf_des.flags[i] & 0x10 ) // 0 & 1
		{
			//in_now = m_File.GetSHORT();
			tb_glyf_des.xCoordinates[i] = in_before;
		}
		else // else if ( !(tb_glyf_des.flags[i] & 0x02) && !(tb_glyf_des.flags[i] & 0x10) ) // 0 & 0
		{
			in_now = m_File.GetUSHORT();
			tb_glyf_des.xCoordinates[i] = in_before+in_now;
		}
		
		in_before = tb_glyf_des.xCoordinates[i];
	}


	in_before = 0;
	for( i = start; i < flag_num; i++)
	{
		if( tb_glyf_des.flags[i] & 0x04 && tb_glyf_des.flags[i] & 0x20 ) // 1 & 1
		{
			in_now = (USHORT)m_File.GetBYTE();
			tb_glyf_des.yCoordinates[i] = in_before+in_now;
		}
		else if ( tb_glyf_des.flags[i] & 0x04 && !(tb_glyf_des.flags[i] & 0x20) ) // 1 & 0
		{
			in_now = (USHORT)m_File.GetBYTE();
			tb_glyf_des.yCoordinates[i] = in_before-in_now;
		}
		else if ( !(tb_glyf_des.flags[i] & 0x04) && tb_glyf_des.flags[i] & 0x20 ) // 0 & 1
		{
			tb_glyf_des.yCoordinates[i] = in_before;
		}
		else // else if ( !(tb_glyf_des.flags[i] & 0x04) && !(tb_glyf_des.flags[i] & 0x20) ) // 0 & 0
		{
			in_now = m_File.GetUSHORT();
			tb_glyf_des.yCoordinates[i] = in_before+in_now;
		}

		in_before = tb_glyf_des.yCoordinates[i];
	}
}

void BaboTTF::Composite_Data_Set()
{
	int i, size1, size2;

	USHORT *t_countours;
	t_countours = new USHORT[tb_glyf.numberOfContours];

	// back-up
	for(i = 0; i < tb_glyf.oldNumberOfContours; i++ ) 
		t_countours[i] = tb_glyf_des.endPtsOfContours[i];
	
	size1 = tb_glyf_des.endPtsOfContours[i-1]+1;

	// new data
	for( ; i < tb_glyf.numberOfContours; i++ ) 
		t_countours[i] = size1+m_File.GetUSHORT();
	
	size2 = t_countours[i-1]+1;

	delete [] tb_glyf_des.endPtsOfContours;
	tb_glyf_des.endPtsOfContours = t_countours;
	//////////////////////////////////////////////////////////////////////////
	// x, y-coordinates data
	BYTE *t_flag;
	SHORT *t_x, *t_y;
	t_flag = new BYTE[size2];
	t_x = new SHORT[size2];
	t_y = new SHORT[size2];

	for( i = 0; i < size1; i++)
	{
		t_flag[i] = tb_glyf_des.flags[i];
		t_x[i] = tb_glyf_des.xCoordinates[i];
		t_y[i] = tb_glyf_des.yCoordinates[i];
	}

	delete [] tb_glyf_des.flags;
	delete [] tb_glyf_des.xCoordinates;
	delete [] tb_glyf_des.yCoordinates;

	tb_glyf_des.flags = t_flag;
	tb_glyf_des.xCoordinates = t_x;
	tb_glyf_des.yCoordinates = t_y;
}

void BaboTTF::Glyph_Composite_des()
{
/*	#define ARG_1_AND_2_ARE_WORDS		0x01
	#define ARGS_ARE_XY_VALUES			0x02
	#define ROUND_XY_TO_GRID			0x04
	#define WE_HAVE_A_SCALE				0x08
	#define MORE_COMPONENTS				0x20
	#define WE_HAVE_AN_X_AND_Y_SCALE	0x40
	#define WE_HAVE_A_TWO_BY_TWO		0x80
	#define WE_HAVE_INSTRUCTION			0x100
	#define USE_MY_METRICS				0x200    */
	tb_glyf_des.endPtsOfContours = new USHORT[32];
	tb_glyf.numberOfContours = 1;
	tb_glyf_des.xCoordinates = new SHORT[128];
	tb_glyf_des.yCoordinates = new SHORT[128];

	USHORT flags;
	int size = 0;
	BOOL bContinue = TRUE;
	//SHORT arg1, arg2;
	//B2_F2DOT14 xscale, scale01, scale10, yscale; 
	while(bContinue)
	{
		flags = m_File.GetUSHORT();
		m_CompositeGlyph[m_CompositeGlyphNum].glyphIndex = m_File.GetUSHORT();

		if( flags & ARG_1_AND_2_ARE_WORDS ) // SHORT
		{
			m_CompositeGlyph[m_CompositeGlyphNum].argument1 = m_File.GetSHORT();
			m_CompositeGlyph[m_CompositeGlyphNum].argument2 = m_File.GetSHORT();
		}
		else // BYTE, BYTE (USHORT == BYTE<<8 | BYTE)
		{
			m_CompositeGlyph[m_CompositeGlyphNum].argument1 = m_File.GetBYTE();
			m_CompositeGlyph[m_CompositeGlyphNum].argument2 = m_File.GetBYTE();
		}

		m_CompositeGlyph[m_CompositeGlyphNum].scale01 = 0;
		m_CompositeGlyph[m_CompositeGlyphNum].scale10 = 0;
		m_CompositeGlyph[m_CompositeGlyphNum].xscale = 0x40; /* set 1 */
		m_CompositeGlyph[m_CompositeGlyphNum].xscale = 0x40; /* set 1 */

		if( flags & WE_HAVE_A_SCALE ) // B2_F2DOT14
		{
			m_CompositeGlyph[m_CompositeGlyphNum].xscale 
				= m_CompositeGlyph[m_CompositeGlyphNum].yscale 
				= m_File.GetUSHORT();
		}
		else if( flags & WE_HAVE_AN_X_AND_Y_SCALE)
		{
			m_CompositeGlyph[m_CompositeGlyphNum].xscale = m_File.GetUSHORT();
			m_CompositeGlyph[m_CompositeGlyphNum].yscale = m_File.GetUSHORT();
		}
		else if(flags & WE_HAVE_A_TWO_BY_TWO )
		{
			m_CompositeGlyph[m_CompositeGlyphNum].xscale = m_File.GetUSHORT();
			m_CompositeGlyph[m_CompositeGlyphNum].scale01 = m_File.GetUSHORT();
			m_CompositeGlyph[m_CompositeGlyphNum].scale10 = m_File.GetUSHORT();
			m_CompositeGlyph[m_CompositeGlyphNum].yscale = m_File.GetUSHORT();
		}
		m_CompositeGlyphNum++;
		if( !(flags & MORE_COMPONENTS) ) bContinue = FALSE;
	}

	for( int i = 0; i < m_CompositeGlyphNum; i++)
		Get_glyf( m_CompositeGlyph[i].glyphIndex, i );
	

}

void BaboTTF::Get_maxp(CBaboMapFile& file)
{
	file.Seek( dir_maxp.offset);

	tb_maxp.version = file.GetLONG();
	tb_maxp.numGlyphs = file.GetUSHORT();
	tb_maxp.maxPoints = file.GetUSHORT();
	tb_maxp.maxContour = file.GetUSHORT();
		
	tb_maxp.maxCompositePoints = file.GetUSHORT();
	tb_maxp.maxCompositeContours = file.GetUSHORT();
	tb_maxp.maxZones = file.GetUSHORT();
				
	tb_maxp.maxTwilightPoints = file.GetUSHORT();	
	tb_maxp.maxStorage = file.GetUSHORT();			
	tb_maxp.maxFunctionDefs = file.GetUSHORT();			
	tb_maxp.maxInstructionDefs = file.GetUSHORT();
	tb_maxp.maxStackElements = file.GetUSHORT();	
	tb_maxp.maxSizeOfInstructions = file.GetUSHORT();
	tb_maxp.maxComponentElements = file.GetUSHORT();
							
	tb_maxp.maxComponentDepth = file.GetUSHORT();
}

void BaboTTF::Get_loca(CBaboMapFile& file)
{
	int i;
	file.Seek( dir_loca.offset);

	int size = tb_maxp.numGlyphs+1;
	
	// 0 is short, 1 is long
	tb_loca.offset = new ULONG[size];
	if( tb_head.indexToLocFormat )
	{	
		for( i = 0; i < size; i++)
			tb_loca.offset[i] = (ULONG)file.GetLONG();
	}
	else
	{
		for( i = 0; i < size; i++)
			tb_loca.offset[i] = (ULONG)file.GetUSHORT()*2;
	}
}

void BaboTTF::Get_os_2(CBaboMapFile& file)
{
	file.Seek( dir_os_2.offset);
	
	tb_os_2.version = file.GetUSHORT();
	tb_os_2.xAvgCharWidth = file.GetSHORT();
	tb_os_2.usWeightClass = file.GetUSHORT();
	tb_os_2.usWidthClass = file.GetUSHORT();
	tb_os_2.fsType = file.GetSHORT();
	
	tb_os_2.ySubscriptXSize = file.GetSHORT();
	tb_os_2.ySubscriptYSize = file.GetSHORT();
	tb_os_2.ySubscriptXOffset = file.GetSHORT();
	tb_os_2.ySubscriptYOffset = file.GetSHORT();
	tb_os_2.ySuperscriptXSize = file.GetSHORT();
	tb_os_2.ySuperscriptYSize = file.GetSHORT();
	tb_os_2.ySuperscriptXOffset = file.GetSHORT();
	tb_os_2.ySuperscriptYOffset = file.GetSHORT();

	tb_os_2.yStrikeoutSize = file.GetSHORT();
	tb_os_2.yStrikeoutPosition = file.GetSHORT();
	tb_os_2.sFamilyClass = file.GetSHORT();
	/* PANOSE - 10_byte */ 
	tb_os_2.pn_bFamilyType = file.GetBYTE();
	tb_os_2.pn_bSerifStyle = file.GetBYTE();
	tb_os_2.pn_bWeight = file.GetBYTE();
	tb_os_2.pn_bProportion = file.GetBYTE();
	tb_os_2.pn_bContrast = file.GetBYTE();
	tb_os_2.pn_bStrokeVariation = file.GetBYTE();
	tb_os_2.pn_bArmStyle = file.GetBYTE();
	tb_os_2.pn_bLetterform = file.GetBYTE();
	tb_os_2.pn_bMidline = file.GetBYTE();
	tb_os_2.pn_bXHeight = file.GetBYTE();

	//
	tb_os_2.ulUnicodeRange1 = file.GetLONG();
	tb_os_2.ulUnicodeRange2 = file.GetLONG();
	tb_os_2.ulUnicodeRange3 = file.GetLONG();
	tb_os_2.ulUnicodeRange4 = file.GetLONG();

	tb_os_2.achVendID[0] = file.GetBYTE();
	tb_os_2.achVendID[1] = file.GetBYTE();
	tb_os_2.achVendID[2] = file.GetBYTE();
	tb_os_2.achVendID[3] = file.GetBYTE();

	tb_os_2.fsSelection = file.GetUSHORT();
	tb_os_2.usFirstCharIndex = file.GetUSHORT();
	tb_os_2.usLastCharIndex = file.GetUSHORT();
	tb_os_2.sTypoAscender = file.GetUSHORT();
	tb_os_2.sTypoDescender = file.GetUSHORT();
	tb_os_2.sTypoLineGap = file.GetUSHORT();
	tb_os_2.usWinAscent = file.GetUSHORT();
	tb_os_2.usWinDescent = file.GetUSHORT();

	tb_os_2.ulCodePageRange1 = file.GetLONG();
	tb_os_2.ulCodePageRange2 = file.GetLONG();
}

void BaboTTF::Release()
{
	if( !m_LoadFont ) return;
	
	//////////////////////////////////////////////////////////////////////////
	// release heap memories

	/* ttc header */
	if( ttc_header.numFont > 0 )
	{
		delete [] ttc_header.OffsetTable;
		ttc_header.numFont = 0;
	}

	/* B2_cmap_Mapping_table */
	delete [] tb_cmap_mapping.endCount;
	delete [] tb_cmap_mapping.startCount;
	delete [] tb_cmap_mapping.idDelta;
	delete [] tb_cmap_mapping.idRangeOffset;
	delete [] tb_cmap_mapping.glyphIdArray;
	
	/* B2_loca */
	delete [] tb_loca.offset;
	if( m_LoadGlyph ) ReleaseGlyphTable();

	/* variables */
	m_LoadFont = FALSE;
	m_File.Close();
}

void BaboTTF::ReleaseGlyphTable()
{
	/* B2_glyf_Descriptions */
	delete [] tb_glyf_des.endPtsOfContours;	
	delete [] tb_glyf_des.instruction;								
	delete [] tb_glyf_des.flags;
	delete [] tb_glyf_des.xCoordinates;
	delete [] tb_glyf_des.yCoordinates;

	m_LoadGlyph = FALSE;
	m_Composite = FALSE;
	m_CompositeGlyphNum = 0;
}


//
// LoadGlyph(USHORT code)
// code is character 2-byte code (UNICODE)
// return TRUE, if loading is successful
// reutrn FALSE, otherwise
BOOL BaboTTF::LoadGlyph(USHORT code)
{
	if( !m_LoadFont ) return FALSE;
	if( m_LoadGlyph ) ReleaseGlyphTable();

	int i;
	USHORT index = 0;
	// find glyph index using cmap table
	for( i = 0; i < tb_cmap_mapping.segCountX2/2; i++)
	{
		if( tb_cmap_mapping.endCount[i] >= code ) break;
	}

	if( i == tb_cmap_mapping.segCountX2/2 ) index = 0;
	else if( code < tb_cmap_mapping.startCount[i] ) index = 0;
	else
	{
		if( tb_cmap_mapping.idRangeOffset[i] == 0) index = tb_cmap_mapping.idDelta[i] + code;
		else
		{
			//index = tb_cmap_mapping.idRangeOffset[i+tb_cmap_mapping.idRangeOffset[i]/2+(code-tb_cmap_mapping.startCount[i])]
			//	+ tb_cmap_mapping.idDelta[i];
			int sub = i+tb_cmap_mapping.idRangeOffset[i]/2+(code-tb_cmap_mapping.startCount[i]);
			index = tb_cmap_mapping.glyphIdArray[sub-tb_cmap_mapping.segCountX2/2];
		}
	}
	
//	int xx = *(tb_cmap_mapping.idRangeOffset[i]/2+(code-tb_cmap_mapping.startCount[i]) +&tb_cmap_mapping.idRangeOffset[i]);
	if( tb_cmap_mapping.idRangeOffset[i] == 0)Get_glyf(index, -1);
	else Get_glyf( index, -1);
	m_LoadGlyph = TRUE;
	return TRUE;
}

BOOL BaboTTF::GetControlPoint(CControlPoint& control_point, int width, int height)
{

	if( !m_LoadFont) return FALSE;
	if( !m_LoadGlyph ) return FALSE;

	int i, j, k, size;

	size = tb_glyf_des.endPtsOfContours[tb_glyf.numberOfContours-1]+1;
	control_point.m_ContourNum = tb_glyf.numberOfContours;


	int x_t[6], y_t[6], num;
	int x[16],y[16],tag[16];
	int firstX, firstY;
	CPoint last;

	FLOAT magX, magY; // magnify ratio
	magX = (FLOAT)(tb_os_2.ySuperscriptXSize)/width*3/2;
	magY = (FLOAT)(tb_os_2.ySuperscriptYSize)/height*3/2;

	i = 0;
	j = 0;
	firstX = tb_glyf_des.xCoordinates[0];
	firstY = tb_glyf_des.yCoordinates[0];
	while(i < size)
	{
		num = 0;
		do 
		{
			x[num] = tb_glyf_des.xCoordinates[i];
			y[num] = tb_glyf_des.yCoordinates[i];
			tag[num] = tb_glyf_des.flags[i]%2;
			num++;
			i++;
		} while((tb_glyf_des.flags[i-1]%2==0) && i-1 < tb_glyf_des.endPtsOfContours[j] );

		if( num < 4) 
		{
			if( num == 3 && tb_glyf_des.flags[i-1]%2 == 0 )
			{ // devide ..
				x_t[0] = last.x;
				y_t[0] = last.y;
				for( k = 0; k < 3; k++)
				{ 
					x_t[k+1] = x[k];
					y_t[k+1] = y[k];
				}
				x_t[4] = firstX;
				y_t[4] = firstY;
				//////////////////////////////////////////////////////////////////////////
				x[1] = (-x_t[0] + 4*x_t[1])/3;
				y[1] = (-y_t[0] + 4*y_t[1])/3;
				x[2] = (11*x_t[0] +28*x_t[1] +18*x_t[2] -4*x_t[3] -5*x_t[4])/48;
				y[2] = (11*y_t[0] +28*y_t[1] +18*y_t[2] -4*y_t[3] -5*y_t[4])/48;
				x[3] = (x_t[0] +4*x_t[1] +6*x_t[2] +4*x_t[3] +x_t[4])/16;
				y[3] = (y_t[0] +4*y_t[1] +6*y_t[2] +4*y_t[3] +y_t[4])/16;

				x[5] = (-x_t[4] + 4*x_t[3])/3;
				y[5] = (-y_t[4] + 4*y_t[3])/3;
				x[4] = (11*x_t[4] +28*x_t[3] +18*x_t[2] -4*x_t[1] -5*x_t[0])/48;
				y[4] = (11*y_t[4] +28*y_t[3] +18*y_t[2] -4*y_t[1] -5*y_t[0])/48;

				x[1] = (x[1]+x_t[0]+1)/2; // mid
				y[1] = (y[1]+y_t[0]+1)/2; // mid
				x[2] = (x[2]+x[3]+1)/2; // mid
				y[2] = (y[2]+y[3]+1)/2; // mid
				x[5] = (x[5]+x_t[4]+1)/2; // mid
				y[5] = (y[5]+y_t[4]+1)/2; // mid
				x[4] = (x[3]+x[4]+1)/2; // mid
				y[4] = (y[3]+y[4]+1)/2; // mid

				for( k = 1; k < 6; k++) control_point.Add((int)(x[k]/magX),(int)(y[k]/magY),k==3?1:0);
				
				control_point.m_Contour[j++] = control_point.m_Size-1; // end of contour
			} 
			else
			{
				for(k = 0; k < num; k++) control_point.Add((int)(x[k]/magX),(int)(y[k]/magY),tag[k]);
				last.x = x[k-1];
				last.y = y[k-1];
				
				if( i-1 == tb_glyf_des.endPtsOfContours[j] ) // end of contour
				{
					control_point.m_Contour[j++] = control_point.m_Size-1;
					firstX = tb_glyf_des.xCoordinates[i];
					firstY = tb_glyf_des.xCoordinates[i];
				}
				
			}
			
		} // if( num < 4) 
		else if( num == 4 && tb_glyf_des.flags[i-1]%2 != 0)
		{
			x_t[0] = last.x;
			y_t[0] = last.y;
			for( k = 0; k < 4; k++)
			{ 
				x_t[k+1] = x[k];
				y_t[k+1] = y[k];
			}
			//////////////////////////////////////////////////////////////////////////
			x[1] = (-x_t[0] + 4*x_t[1])/3;
			y[1] = (-y_t[0] + 4*y_t[1])/3;
			x[2] = (11*x_t[0] +28*x_t[1] +18*x_t[2] -4*x_t[3] -5*x_t[4])/48;
			y[2] = (11*y_t[0] +28*y_t[1] +18*y_t[2] -4*y_t[3] -5*y_t[4])/48;
			x[3] = (x_t[0] +4*x_t[1] +6*x_t[2] +4*x_t[3] +x_t[4])/16;
			y[3] = (y_t[0] +4*y_t[1] +6*y_t[2] +4*y_t[3] +y_t[4])/16;

			x[5] = (-x_t[4] + 4*x_t[3])/3;
			y[5] = (-y_t[4] + 4*y_t[3])/3;
			x[4] = (11*x_t[4] +28*x_t[3] +18*x_t[2] -4*x_t[1] -5*x_t[0])/48;
			y[4] = (11*y_t[4] +28*y_t[3] +18*y_t[2] -4*y_t[1] -5*y_t[0])/48;

			x[1] = (x[1]+x_t[0]+1)/2; // mid
			y[1] = (y[1]+y_t[0]+1)/2; // mid
			x[2] = (x[2]+x[3]+1)/2; // mid
			y[2] = (y[2]+y[3]+1)/2; // mid
			x[5] = (x[5]+x_t[4]+1)/2; // mid
			y[5] = (y[5]+y_t[4]+1)/2; // mid
			x[4] = (x[3]+x[4]+1)/2; // mid
			y[4] = (y[3]+y[4]+1)/2; // mid
			
			for( k = 1; k < 6; k++) control_point.Add((int)(x[k]/magX),(int)(y[k]/magY),k==3?1:0);
			control_point.Add((int)(x_t[4]/magX),(int)(y_t[4]/magY),1);
			last.x = x_t[4];
			last.y = y_t[4];
			
			if( i-1 == tb_glyf_des.endPtsOfContours[j] ) // end of contour
			{
				control_point.m_Contour[j++] = control_point.m_Size-1;
				firstX = tb_glyf_des.xCoordinates[i];
				firstY = tb_glyf_des.xCoordinates[i];
			}
		}else //////////////////////////////////////////////////////////////////////////
		{
			if( num == 4 && tb_glyf_des.flags[i-1]%2 == 0)
			{
				x_t[5] = firstX;
				y_t[5] = firstY;
			}
			else if( num == 5 && tb_glyf_des.flags[i-1]%2 == 1)
			{
				x_t[5] = x[4];
				y_t[5] = y[4];
			}
			else  return FALSE;
			
			x_t[0] = last.x;
			y_t[0] = last.y;
			for( k = 0; k < 4; k++)
			{ 
				x_t[k+1] = x[k];
				y_t[k+1] = y[k];
			}
			//////////////////////////////////////////////////////////////////////////
			x[1] = (-2*x_t[0] + 5*x_t[1])/3;
			y[1] = (-2*y_t[0] + 5*y_t[1])/3;
			x[2] = (-7*x_t[0] -15*x_t[1] +10*x_t[2] +50*x_t[3] +45*x_t[4] +13*x_t[5])/96;
			y[2] = (-7*y_t[0] -15*y_t[1] +10*y_t[2] +50*y_t[3] +45*y_t[4] +13*y_t[5])/96;
			x[3] = (x_t[0] +5*x_t[1] +10*x_t[2] +10*x_t[3] +5*x_t[4] +x_t[5])/32;
			y[3] = (y_t[0] +5*y_t[1] +10*y_t[2] +10*y_t[3] +5*y_t[4] +y_t[5])/32;

			x[5] = (-2*x_t[5] + 5*x_t[4])/3;
			y[5] = (-2*y_t[5] + 5*y_t[4])/3;
			x[4] = (-7*x_t[5] -15*x_t[4] +10*x_t[3] +50*x_t[2] +45*x_t[1] +13*x_t[0])/96;
			y[4] = (-7*y_t[5] -15*y_t[4] +10*y_t[3] +50*y_t[2] +45*y_t[1] +13*y_t[0])/96;

			x[1] = (x[1]+x_t[0]+1)/2; // mid
			y[1] = (y[1]+y_t[0]+1)/2; // mid
			x[2] = (x[2]+x[3]+1)/2; // mid
			y[2] = (y[2]+y[3]+1)/2; // mid
			x[5] = (x[5]+x_t[5]+1)/2; // mid
			y[5] = (y[5]+y_t[5]+1)/2; // mid
			x[4] = (x[3]+x[4]+1)/2; // mid
			y[4] = (y[3]+y[4]+1)/2; // mid

			/* compensation */
			x[2] = x[3] - (x[2]-x[3]);
			y[2] = y[3] - (y[2]-y[3]);
			x[4] = x[3] - (x[4]-x[3]);
			y[4] = y[3] - (y[4]-y[3]);
			
			for( k = 1; k < 6; k++) control_point.Add((int)(x[k]/magX),(int)(y[k]/magY),k==3?1:0);
			if( num == 5) control_point.Add((int)(x_t[5]/magX),(int)(y_t[5]/magY),1);
			last.x = x_t[5];
			last.y = y_t[5];
			
			if( i-1 == tb_glyf_des.endPtsOfContours[j] ) // end of contour
			{
				control_point.m_Contour[j++] = control_point.m_Size-1;
				firstX = tb_glyf_des.xCoordinates[i];
				firstY = tb_glyf_des.xCoordinates[i];
			}
		}
	
	} // while(i < size)

	return TRUE;
}
