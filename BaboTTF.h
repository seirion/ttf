// BaboTTF.h: interface for the BaboTTF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BABOTTF_H__6E5DFE46_E6F8_4588_AE95_985F14BDE912__INCLUDED_)
#define AFX_BABOTTF_H__6E5DFE46_E6F8_4588_AE95_985F14BDE912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include "BaboMapFile.h"
#include "ControlPoint.h"

//////////////////////////////////////////////////////////////////////////
// macro functions
#define CHAR2ULONG(c) *((ULONG*)c)	// char pointer -> ULONG
#define CHANGE_ENDIAN2(c) htons(c)
#define CHANGE_ENDIAN4(c) htonl(c)
/*#define CHANGE_ENDIAN2(c) (c<<8|c>>8)
#define CHANGE_ENDIAN4(c) (c>>24&0xff)|(c>>8&0xff00)|(c<<8&0xff0000)|(c<<24&0xff000000)*/
#define CHANGE_ENDIAN(c) (sizeof(c) == 2 ? CHANGE_ENDIAN2(c) : sizeof(c) == 4 ? CHANGE_ENDIAN4(c) : c)

//////////////////////////////////////////////////////////////////////////
// data type definitions
#define B2_F2DOT14 unsigned short
#define B2_Fixed ULONG

//////////////////////////////////////////////////////////////////////////
// flags
#define ARG_1_AND_2_ARE_WORDS		0x01
#define ARGS_ARE_XY_VALUES			0x02
#define ROUND_XY_TO_GRID			0x04
#define WE_HAVE_A_SCALE				0x08
#define MORE_COMPONENTS				0x20
#define WE_HAVE_AN_X_AND_Y_SCALE	0x40
#define WE_HAVE_A_TWO_BY_TWO		0x80
#define WE_HAVE_INSTRUCTION			0x100
#define USE_MY_METRICS				0x200

//////////////////////////////////////////////////////////////////////////
// struct definitions

/* ttc header */
struct B2_TTC_Header
{
	BYTE TTCTag[4];			// TrueType Collection ID string: 'ttcf'
	ULONG version;			// Version of the TTC Header (1.0), 0x00010000
	ULONG numFont;			// Number of fonts in TTC
	ULONG *OffsetTable;		/* Array of offsets to the OffsetTable 
								for each font from the beginning of the file
								the size of array is numFont */

	// only version 2.0 
	ULONG ulDsigTag;		/* Tag indicating that a DSIG table exists, 0x44534947
								('DSIG') (null if no signature) */
	ULONG ulDsigLength;		// the length (in byte) of the DSIG table (null if no signature)
	ULONG ulDsigOffset;		/* The offset (in byte) of the DSIG table from the beginning of the TTC file
								(null if no signature) */

};

/* table level 0, 1 */

// Data Table Directory (level 0)
struct B2_Data_Table_Directory
{
	B2_Fixed sfnt_version;	// 0x00010000 for version 1.0.
	USHORT numTables;		// Number of tables
	USHORT searchRange;		// (Maximum power of 2 <= numTables) * 16
	USHORT entrySelector;	// Log2(Maximum power of 2 <= numTables)
	USHORT rangeShift;		// NumTables * 16 - serachRange
};

// Table Directory (level 0_1)
struct B2_Table_Directory
{
	ULONG tag;			// 4-byte identifier
	ULONG checkSum;		// CheckSum for this table
	ULONG offset;		// Offset from beginning of TrueType font file
	ULONG length;		// Length of this table
};

//////////////////////////////////////////////////////////////////////////
/* table level 1 */

// head - Font Header
struct B2_head
{
	B2_Fixed version;		// 0x00010000 for version 1.0
	B2_Fixed fontRevision;	// Set by font manufacturer
	ULONG checkSum;		/* set it to 0. sum the entire font as ULONG, 
						then store 0xB1B0AFBA - sum */
	ULONG magicNumber;	// Set to 0x5F0F3cF5
	USHORT flags;

	USHORT unitsPerEm;	// valid range is from 16 to 16384
	LONGLONG created;	// international date (8-byte field)
	LONGLONG modified;	// international date (8-byte field)
	
	SHORT xMin;			// For all glyph bounding boxes
	SHORT yMin;
	USHORT xMax;
	USHORT yMax;

	USHORT macStyle;	// Bit 0 bold, Bit 1 italic, others are reserved
	USHORT lowestRecPPEM;	// Smallest readable size in pixels
	SHORT fontDirectionHint;// 

	SHORT indexToLocFormat;	// 0 for short offset, 1 for long
	SHORT glyphDataFormat;	// 0 for current format 
};
//////////////////////////////////////////////////////////////////////////

// cmap - Character To Clyph Index Mapping Table (level 1)
struct B2_cmap 
{
	USHORT table_version;	// tables version number (0)
	USHORT numTables;		// Number of encoding tables, n
};

// cmap sub tables (level 1_1)
struct B2_cmap_Sub
{
	USHORT platformID;	// Platform ID
	USHORT encodingID;	// Platform-specific encoding ID
	ULONG offset;		// Byte offset from beginning of tables to the subtable for this encoding
};

// (cmap) Format 4: Segmentation mapping to delta values
struct B2_cmap_Mapping_table
{
	USHORT format;			// Format number is set to 4
	USHORT length;			// Length in bytes
	USHORT version;			// Version number (start at 0)
	USHORT segCountX2;		// 2 * segCount
	USHORT searchRange;		// 2 * (2**floor(log2(segCount)))
	USHORT entrySelector;	// log2(searchRange/2)
	USHORT rangeShift;		// 2 * segCount - serachRange
	USHORT *endCount;		// End characterCode for each segment, last = 0xffff (size is segCount)
	USHORT reservedPad;		// Set to 0
	USHORT *startCount;		// Start character code for each segment (size is segCount)
	USHORT *idDelta;			// Delta for all character codes in segment
	USHORT *idRangeOffset;	// Offsets into glyphIdArray or 0
	USHORT *glyphIdArray;	// Glyph index array (arbitrary length)
};

//////////////////////////////////////////////////////////////////////////

// glyf - Glyph Data
struct B2_glyf
{
	SHORT numberOfContours;		/* if the number of contours is greater than or equal to zero,
								this is a single glyph; if negative, this is a composite glyph */
	SHORT xMin;		// Minimum x for coordinate data
	SHORT yMin;		// Minimum y for coordinate data
	SHORT xMax;		// Maximum x for coordinate data
	SHORT yMax;		// Maximum y for coordinate data

	/* for data processing - composite glyphs */
	SHORT oldNumberOfContours;
};

struct B2_glyf_Description
{
	USHORT *endPtsOfContours;	/* Array of last points of each contour;
								the size is the number of contours */
	USHORT instructionLength;	// Total number of bytes for instructions
	BYTE *instruction;			/* Array of instructions for each flyph;
								the size is the number of instructions */
	BYTE *flags;				/* Array of flags for each coordinate in ouline;
								the size is the number of flags */
	SHORT *xCoordinates;
	SHORT *yCoordinates;
};
//////////////////////////////////////////////////////////////////////////

// loca - index to Location
struct B2_loca
{
	/*USHORT *offset;*/		/* The actual local offset divided by 2 is stored
						The size is numGlyphs + 1,
						which is found in the 'maxp' table */
	/*ULONG *offset;*/		/* The actual local offset is stored
						the size is numGlyphs + 1,
						which is found in the 'maxp' table */
	ULONG *offset;

};


//////////////////////////////////////////////////////////////////////////

// maxp - Maximum Profile

struct B2_maxp
{
	B2_Fixed version;				// 0x00010000 for version 1.0
	USHORT numGlyphs;				// the number of glyphs in the font
	USHORT maxPoints;				// Maximum points in a non-composite glyph
	USHORT maxContour;				// Maximum contours in a non-composite glyph
	USHORT maxCompositePoints;		// Maximum points in a composite glyph
	USHORT maxCompositeContours;	// Maximum countours in a composite glyph
	USHORT maxZones;			/* 1 if instructions do not use the twilight zone (Z0),
								or 2 if instrcutions do use Z0; 
								should be set to 2 in most case */
	USHORT maxTwilightPoints;		// Maximum points used in Z0
	USHORT maxStorage;				// Number of Storage Area locations
	USHORT maxFunctionDefs;			// Number of FDEFs
	USHORT maxInstructionDefs;		// Number of IDEFs
	USHORT maxStackElements;		// Maximum stack depth
	USHORT maxSizeOfInstructions;	// Maximum byte count for glyph instructions
	USHORT maxComponentElements;	/* Maximum number of components referenced at 
									"top level" for any composite glyph */
	USHORT maxComponentDepth;		// Maximum levels of recursion; 1 for simple components

};
//////////////////////////////////////////////////////////////////////////

// OS/2 - OS/2 and Windows Metrics
struct B2_os_2
{
	USHORT version;
	SHORT xAvgCharWidth;
	USHORT usWeightClass;
	USHORT usWidthClass;
	SHORT fsType;
	
	SHORT ySubscriptXSize;
	SHORT ySubscriptYSize;
	SHORT ySubscriptXOffset;
	SHORT ySubscriptYOffset;
	SHORT ySuperscriptXSize;
	SHORT ySuperscriptYSize;
	SHORT ySuperscriptXOffset;
	SHORT ySuperscriptYOffset;

	SHORT yStrikeoutSize;
	SHORT yStrikeoutPosition;
	SHORT sFamilyClass;
	/* PANOSE - 10_byte */ 
	BYTE pn_bFamilyType;
	BYTE pn_bSerifStyle;
	BYTE pn_bWeight;
	BYTE pn_bProportion;
	BYTE pn_bContrast;
	BYTE pn_bStrokeVariation;
	BYTE pn_bArmStyle;
	BYTE pn_bLetterform;
	BYTE pn_bMidline;
	BYTE pn_bXHeight;

	//
	ULONG ulUnicodeRange1;
	ULONG ulUnicodeRange2;
	ULONG ulUnicodeRange3;
	ULONG ulUnicodeRange4;
	CHAR achVendID[4];

	USHORT fsSelection;
	USHORT usFirstCharIndex;
	USHORT usLastCharIndex;
	USHORT sTypoAscender;
	USHORT sTypoDescender;
	USHORT sTypoLineGap;
	USHORT usWinAscent;
	USHORT usWinDescent;

	ULONG ulCodePageRange1;
	ULONG ulCodePageRange2;

};
//////////////////////////////////////////////////////////////////////////
// composite glyph
struct B2_Composite_Glyph
{
	USHORT glyphIndex;
	SHORT argument1;
	SHORT argument2;
	B2_F2DOT14 xscale;
	B2_F2DOT14 scale01;
	B2_F2DOT14 scale10;
	B2_F2DOT14 yscale; 
};

//////////////////////////////////////////////////////////////////////////
class BaboTTF  
{
public:
	BOOL GetControlPoint(CControlPoint & control_point, int width, int height);
	BOOL LoadFont(LPCSTR path);
	BOOL LoadGlyph(USHORT code);
	
	/* constructor & deconstructor */
	BaboTTF();
	virtual ~BaboTTF();
	void ReleaseGlyphTable();
	void Release();

private:
	/* tables in level 1 */
	void Get_head(CBaboMapFile& file);
	void Get_cmap(CBaboMapFile& file);
	void Get_maxp(CBaboMapFile& file);
	void Get_loca(CBaboMapFile& file);
	void Get_os_2(CBaboMapFile& file);

	/* loading */
	void Get_glyf(USHORT index, int count);
	void Glyph_Simple_des(int count);
	void Glyph_Composite_des();
	void Composite_Data_Set();

	/* flags */
	BOOL m_LoadFont;
	BOOL m_LoadGlyph;

	/* file */
	CBaboMapFile m_File;

	/* ttc file header */
	B2_TTC_Header ttc_header;

	/* directory header */
	B2_Data_Table_Directory dtd;

	/* directories */
	B2_Table_Directory dir_cmap;
	B2_Table_Directory dir_glyf; 
	B2_Table_Directory dir_head;
	B2_Table_Directory dir_loca;
	B2_Table_Directory dir_maxp;
	B2_Table_Directory dir_os_2;

	/* head */
	B2_head tb_head;

	/* cmap */
	B2_cmap tb_cmap;
	B2_cmap_Sub tb_cmap_sub;
	B2_cmap_Mapping_table tb_cmap_mapping;

	/* glyf */
	B2_glyf tb_glyf;
	B2_glyf_Description tb_glyf_des;

	/* maxp */
	B2_maxp tb_maxp;

	/* loca */
	B2_loca tb_loca;

	/* OS/2 */
	B2_os_2 tb_os_2;

	// composite glyphs
	BOOL m_Composite;
	int m_CompositeGlyphNum;
	B2_Composite_Glyph m_CompositeGlyph[16];

};

#endif // !defined(AFX_BABOTTF_H__6E5DFE46_E6F8_4588_AE95_985F14BDE912__INCLUDED_)
