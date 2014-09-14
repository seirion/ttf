// ControlPoint.h: interface for the CControlPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLPOINT_H__1DABD542_0B2A_4992_8698_DEFFF4E2B706__INCLUDED_)
#define AFX_CONTROLPOINT_H__1DABD542_0B2A_4992_8698_DEFFF4E2B706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////
// point type
#define TAG_ON	1
#define TAG_OFF	0
#define POINT_SIZE	2


struct MyPoint
{
	int x;	
	int y;
	short tag;		// contour 상에 있으면 1
};

class CControlPoint
{
public:	
	void WriteText(CStdioFile& file);

	int FindRegion( CPoint start, CPoint end, int selected[] );
	void SetPointAt(int index, int x, int y, int tag);
	MyPoint* GetPointPtr(int x, int y);
	int Add(int x, int y, int tag);
	
	void Init();
	CControlPoint();
	CControlPoint( int size );
	virtual ~CControlPoint();

	int Find(CPoint point);
	void DrawControlPoints(CDC *pDC);
	void DrawBezierCurve(CDC *pDC);
	void DrawGlyph(CDC *pDC);

	///////////////////////////////////////////////////////////////////////
	// data
	MyPoint *m_Buffer;		// 데이터의 list
	int m_Size;				// 실제 data의 수
	int m_ArraySize;		// 할당된 array의 크기
	int m_ContourNum;		// contour 
	int m_Contour[64];

private:
	void ArrayDouble();

	//////////////////////////////////////////////////////////////////////////
	// 그리기 관련
	CPen *m_pen, *m_oldPen;
	CBrush *m_brush, *m_oldBrush;
	void ReleaseBrush(CDC *pDC);
	void ReleasePen(CDC *pDC);
	void SetPen( CDC *pDC, int width, COLORREF color);
	void SetBrush(CDC *pDC, COLORREF color);

	//////////////////////////////////////////////////////////////////////////
	// drawing bezier curve
	void DrawBezier(CDC *pDC);
	CPoint GetBezierPoint( double u);
	double Power(double u, int k);
	int GetCoefficient(int n, int k);
	int Factorial(int value);

	int m_BezierSize;
	CPoint m_BezierPoint[8];
	COLORREF m_Color;
};

#endif // !defined(AFX_CONTROLPOINT_H__1DABD542_0B2A_4992_8698_DEFFF4E2B706__INCLUDED_)
