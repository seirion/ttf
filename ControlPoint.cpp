  // ControlPoint.cpp: implementation of the CControlPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "B2TTF.h"
#include "ControlPoint.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlPoint::CControlPoint()
{
	m_Size = 0;
	m_ArraySize = 64;
	m_Buffer = new MyPoint[m_ArraySize];
	m_pen = NULL;
	m_brush = NULL;
}

CControlPoint::CControlPoint( int size )
{
	m_Size = 0;
	m_ArraySize = size;
	m_Buffer = new MyPoint[m_ArraySize];
	m_pen = NULL;
	m_brush = NULL;
}

// destructor
CControlPoint::~CControlPoint()
{
	delete [] m_Buffer;
}

void CControlPoint::Init()
{
	delete[] m_Buffer;	

	m_Size = 0;
	m_ArraySize = 64;
	m_Buffer = new MyPoint[m_ArraySize];
	m_ContourNum = 0;
	m_pen = NULL;
	m_brush = NULL;
}

void CControlPoint::ArrayDouble()
{
	MyPoint *newArray;
	newArray = new MyPoint[m_ArraySize*2];

	m_ArraySize = m_ArraySize * 2;
	
	for( int i = 0; i < m_Size; i++)
		newArray[i] = m_Buffer[i];

	// 메모리 해제		
	delete [] m_Buffer;

	// 새로운 array 받음
	m_Buffer = newArray;	
}


// return last index
int CControlPoint::Add( int x, int y, int tag)
{
	if( m_ArraySize == m_Size ) ArrayDouble();

	m_Buffer[m_Size].x = x;
	m_Buffer[m_Size].y = y;
	m_Buffer[m_Size].tag = tag%2;

	m_Size++;

	return m_Size;
}

////////////////////////////////////////////////////////////////////////////

MyPoint* CControlPoint::GetPointPtr(int x, int y)
{
    int i;
	for (i=0; i<m_Size; i++)
		if (m_Buffer[i].x == x && m_Buffer[i].y == y)
			return &m_Buffer[i];

	return &m_Buffer[i];
}

void CControlPoint::SetPointAt(int index, int x, int y, int tag)
{
	m_Buffer[index].x = x;
	m_Buffer[index].y = y;
	m_Buffer[index].tag = tag;
}

// 컨트롤 포인트 출력하기
void CControlPoint::DrawControlPoints(CDC *pDC)
{
	int i;

	CBrush nullBrush, *oldBrush;
	nullBrush.CreateStockObject(NULL_BRUSH);
	oldBrush = (CBrush *)pDC->SelectObject(&nullBrush);
	
	for( i = 0; i < m_Size; i++)
	{
		if( m_Buffer[i].tag )	// 1 : fixed - points on boundary
		{
			SetPen(pDC, 2, RGB(0,0,255));
			pDC->Rectangle( m_Buffer[i].x - POINT_SIZE-1, m_Buffer[i].y - POINT_SIZE-1,
				m_Buffer[i].x + POINT_SIZE+1, m_Buffer[i].y + POINT_SIZE+1 );		
		}
		else
		{
			SetPen(pDC, 2, RGB(255,0,0));
			pDC->Ellipse(m_Buffer[i].x - POINT_SIZE-1, m_Buffer[i].y - POINT_SIZE-1,
				m_Buffer[i].x + POINT_SIZE+1, m_Buffer[i].y + POINT_SIZE+1 );
		}
	}

	pDC->SelectObject(oldBrush);
	nullBrush.DeleteObject();
	ReleasePen( pDC);
}

void CControlPoint::DrawGlyph(CDC *pDC)
{
}
/////////////////////////////////////////////////////////////////////////////////
// 그리기 관련 함수들


void CControlPoint::ReleasePen(CDC *pDC)
{
	if( m_pen == NULL)return;
	pDC->SelectObject(m_oldPen);
	delete m_pen;
	m_pen = NULL;
}

void CControlPoint::ReleaseBrush(CDC *pDC)
{
	if( m_brush == NULL)return;
	pDC->SelectObject(m_oldBrush);
	delete m_brush;
	m_brush = NULL;
}

void CControlPoint::SetBrush(CDC *pDC, COLORREF color)
{
	if( m_brush != NULL) delete m_brush;
	m_brush = new CBrush;
	m_brush->CreateSolidBrush(color);
	m_oldBrush = pDC->SelectObject(m_brush);
}

void CControlPoint::SetPen(CDC *pDC, int width, COLORREF color)
{
	if( m_pen != NULL) delete m_pen;
	m_pen = new CPen;
	m_pen->CreatePen(PS_SOLID, width, color);
	m_oldPen = pDC->SelectObject(m_pen);
}

//////////////////////////////////////////////////////////////////////////
// drawing bezier curves
void CControlPoint::DrawBezierCurve(CDC *pDC)
{
	m_Color = RGB(0,0,0);

	int contour = 0, current = 1;
	
	m_BezierSize = 1;
	m_BezierPoint[0].x = m_Buffer[0].x;
	m_BezierPoint[0].y = m_Buffer[0].y;
	while(current < m_Size) // for all points
	{
		m_BezierPoint[m_BezierSize].x = m_Buffer[current].x; // data
		m_BezierPoint[m_BezierSize].y = m_Buffer[current].y;
		m_BezierSize++;
		current++;
		if( m_Buffer[current-1].tag ) // check
		{
			DrawBezier(pDC);
			m_BezierPoint[0] = m_BezierPoint[m_BezierSize-1];
			m_BezierSize = 1;
		}

		if( current-1 == m_Contour[contour])
		{
			m_BezierPoint[m_BezierSize].x = m_Buffer[contour==0?0:m_Contour[contour-1]+1].x; // data
			m_BezierPoint[m_BezierSize].y = m_Buffer[contour==0?0:m_Contour[contour-1]+1].y;
			m_BezierSize++;
			DrawBezier(pDC);
			contour++;
			m_BezierPoint[0].x = m_Buffer[m_Contour[contour-1]+1].x;
			m_BezierPoint[0].y = m_Buffer[m_Contour[contour-1]+1].y;
			current++;
			m_BezierSize = 1;
		}

	} // while
}

void CControlPoint::DrawBezier(CDC *pDC)
{
	int i;
	const int size = 100;
	for( i = 0; i <= size; i++)
	{
		pDC->SetPixel(GetBezierPoint((double)i/size), m_Color);
	}
}

CPoint CControlPoint::GetBezierPoint( double u)
{
	CPoint point;
	double x, y;
	int k;
	int n = m_BezierSize-1;
	int coeff;
	
	x = 0;
	y = 0;
	for( k = 0; k < m_BezierSize; k++)
	{
		coeff = GetCoefficient(n,k);
		x += m_BezierPoint[k].x      *coeff*        Power(u,k)*      Power(1-u, n-k);
		y += m_BezierPoint[k].y      *coeff*        Power(u,k)*      Power(1-u, n-k);
	}

	point.x = (int)(x+0.5);
	point.y = (int)(y+0.5);
	return point;
}

double CControlPoint::Power(double u, int k)
{
	if( k <= 0 ) return 1;
	if( u == 0 ) return 0;
	double result = 1;
	for( int i = 0; i < k; i++)
	{
		result = result * u;
	}

	return result;
}

int CControlPoint::GetCoefficient(int n, int k)
{
	int r = 1;
	for( int i = n; i > k; i--)
	{
		r = r*i;
	}

	return r/Factorial(n-k);
}

int CControlPoint::Factorial(int value)
{
	if( value <= 1 ) return 1;
	int i;
	int result = 1;
	for( i = value; i >= 2; i--)
		result = result * i;
	return result;
}

//////////////////////////////////////////////////////////////////////////
// store data at files

// 
// save text mode
// parameter :
//				file : objective file instance
void CControlPoint::WriteText(CStdioFile &file)
{
	CString str;

	int i, j, count;
	CPoint point[3];

	i = 0; // index for points
	j = 0; // index for contours
	count = 0;
	
	str.Format("/* INSTRUCTIONS\tINDICES\tX\tY */\n");
	file.WriteString(str);

	while(i < m_Size)
	{
		if( i == 0 || (j > 0 && i == m_Contour[j-1]+1))
		{
			str.Format("MOVE_TO\t\t%d\t%d\t%d\n", 0, m_Buffer[i].x, m_Buffer[i].y);
			file.WriteString(str);
			count = 0;
			i++;
			continue;
		}

		while(1)
		{
			point[count].x = m_Buffer[i].x; 
			point[count].y = m_Buffer[i].y;
			count++;
			if( m_Buffer[i].tag ) break;

			if( i == m_Contour[j] ) // end of contour
			{
				if( j == 0 )
				{
					point[count].x = m_Buffer[0].x;
					point[count].y = m_Buffer[0].y;	
				}else
				{
					point[count].x = m_Buffer[m_Contour[j-1]+1].x;
					point[count].y = m_Buffer[m_Contour[j-1]+1].y;
				}
				count++;
				break;
			}
			else i++;
				
		}

		if( count == 1 ) // LINE_TO
		{
			str.Format("LINE_TO\t\t%d\t%d\t%d\n", 0, point[0].x, point[0].y);
			file.WriteString(str);
		}else if( count == 2 ) // QUAD_TO
		{
			str.Format("QUAD_TO\t\t%d\t%d\t%d\n", 0, point[0].x, point[0].y);
			file.WriteString(str);
			str.Format("QUAD_TO\t\t%d\t%d\t%d\n", 1, point[1].x, point[1].y);
			file.WriteString(str);
		}else // CUBIC_TO
		{
			str.Format("CUBIC_TO\t%d\t%d\t%d\n", 0, point[0].x, point[0].y);
			file.WriteString(str);
			str.Format("CUBIC_TO\t%d\t%d\t%d\n", 1, point[1].x, point[1].y);
			file.WriteString(str);
			str.Format("CUBIC_TO\t%d\t%d\t%d\n", 2, point[2].x, point[2].y);
			file.WriteString(str);
		}
		
		count = 0;

		if( i == m_Contour[j]) // end of countour
		{
			j++;
			str.Format("CLOSE_PATH\t%d\n", 0);
			file.WriteString(str);
		}
		i++;
		
	} // while(i < size)
}
