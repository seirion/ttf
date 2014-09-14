                             // B2TTFDlg.cpp : implementation file
//

#include "stdafx.h"
#include "B2TTF.h"
#include "B2TTFDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB2TTFDlg dialog

CB2TTFDlg::CB2TTFDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CB2TTFDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CB2TTFDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CB2TTFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CB2TTFDlg)
	DDX_Control(pDX, IDC_EDIT_IN, m_CtrlIn);
	DDX_Control(pDX, IDC_STATIC_POINTS, m_CtrlPoints);
	DDX_Control(pDX, IDC_STATIC_CONTOURS, m_CtrlContours);
	DDX_Control(pDX, IDC_STATIC_UNICODE, m_CtrlUnicode);
	DDX_Control(pDX, IDC_STATIC_FONT_NAME, m_CtrlFontName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CB2TTFDlg, CDialog)
	//{{AFX_MSG_MAP(CB2TTFDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FONT_OPEN, OnButtonFontOpen)
	ON_BN_CLICKED(IDC_BUTTON_IN, OnButtonIn)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_TEXT, OnButtonExportText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB2TTFDlg message handlers

BOOL CB2TTFDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	Init();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CB2TTFDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CB2TTFDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect win_size;
	this->GetClientRect(win_size);
	
	dc.SetMapMode(MM_ISOTROPIC);
//	dc.SetWindowOrg(0, win_size.bottom);
	dc.SetWindowExt(win_size.right, -win_size.bottom);
	dc.SetViewportOrg(20, win_size.bottom-120);
	dc.SetViewportExt(win_size.right, win_size.bottom);

	CPen greenPen, brightPen, *pOldPen;
	greenPen.CreatePen(PS_SOLID, 1, RGB(200,127,255));
	brightPen.CreatePen(PS_SOLID, 1, RGB(230,196,125));
/*	CBrush nullBrush, *pOldBrush;
	nullBrush.CreateStockObject(NULL_BRUSH);
	pOldBrush = (CBrush *)dc.SelectObject(&nullBrush);*/

	pOldPen = (CPen *)dc.SelectObject(&brightPen);
	int i;
	const int size = 50;
	for( i = 0; i < 11; i++)
	{
		dc.MoveTo(0, i*size-100);
		dc.LineTo(500, i*size-100);
		dc.MoveTo(i*size, -100);
		dc.LineTo(i*size, 400);
	}
/*	dc.Rectangle(0,0,400,400); // erase bgr
	dc.SelectObject(pOldBrush);
	nullBrush.DeleteObject();*/

//	dc.SetViewportOrg(100,100);
	pOldPen = (CPen *)dc.SelectObject(&greenPen);
	dc.MoveTo(0, 0);
	dc.LineTo(400, 0);
	dc.MoveTo(0, 0);
	dc.LineTo(0, 400);

	dc.SelectObject(pOldPen);
	greenPen.DeleteObject();
	brightPen.DeleteObject();
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if( !m_LoadFont )
		{
			CDialog::OnPaint();
			return;
		}
		if( !m_LoadGlyph )
		{
			CDialog::OnPaint();
			return;
		}
		m_ControlPoint.DrawControlPoints(&dc);
		m_ControlPoint.DrawBezierCurve(&dc);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CB2TTFDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////////////////////////
// system functions

//
// initializing all vairalbes/status
void CB2TTFDlg::Init()
{
	m_LoadFont = FALSE;
	m_LoadGlyph = FALSE;
	m_ControlPoint.Init();
	m_CtrlFontName.SetWindowText("");
	m_CtrlUnicode.SetWindowText("");
	m_CtrlPoints.SetWindowText("");
	m_CtrlContours.SetWindowText("");
	m_TTF = NULL;
}

//////////////////////////////////////////////////////////////////////////
// implementation button events

//
// TTF file open
void CB2TTFDlg::OnButtonFontOpen() 
{
	char szOpenFilter[] = "TrueType Font Files(.ttf;.ttc)|*.ttf;*.ttc|TrueType Font Files(.ttf)|*.ttf|TrueType Collection Files(.ttc)|*.ttc|";

	CFileDialog dlg( TRUE,"*","",OFN_HIDEREADONLY,szOpenFilter,AfxGetMainWnd());

	if( dlg.DoModal() == IDOK )
	{
		if( LoadFont(dlg.GetPathName()) )
		{
			m_CtrlFontName.SetWindowText(dlg.GetFileName());
		}
		else
		{
			CString msg;
			msg.Format("Cannot open : %s", dlg.GetFileName());
			AfxMessageBox(msg);
		}
	}
} 

//
// load ttf tables
// parameters : 
//		LPCSTR path ; ttf file path
BOOL CB2TTFDlg::LoadFont(LPCSTR path)
{
	if( m_LoadFont )
	{
		if( m_LoadGlyph ) m_ControlPoint.Init();
		
		delete m_TTF;
		Init();
	}
	m_TTF = new BaboTTF();
	m_LoadFont = m_TTF->LoadFont(path);
	Invalidate();
	return m_LoadFont;
}

//
// getting glyph
void CB2TTFDlg::OnButtonIn() 
{
	if( !m_LoadFont ) return;
	
	//////////////////////////////////////////////////////////////////////////
	
	CString str;
	unsigned short *wcsCode = new unsigned short[32];

	m_CtrlIn.GetWindowText(str);
	str.TrimLeft();
	str.TrimRight();
	if( str.GetLength() <= 0 ) return;
	if( m_LoadGlyph )
	{
		m_TTF->ReleaseGlyphTable();
		m_ControlPoint.Init();
	}

	int size = str.GetLength();
	if( size > 2) size = 2;

	MultiByteToWideChar(
		CP_ACP,                 // code page
		MB_PRECOMPOSED,         // character-type options
		str.GetBuffer(32),		// address of string to map
		size,                   // number of bytes in string
		(LPWSTR)wcsCode,        // address of wide-character buffer
		32                      // size of buffer
	);

	m_TTF->LoadGlyph(wcsCode[0]);
	m_LoadGlyph = TRUE;
	
	if( m_TTF->GetControlPoint(m_ControlPoint,400,400) ) m_LoadGlyph = TRUE;
	else
	{
		m_TTF->ReleaseGlyphTable();
		m_LoadGlyph = FALSE;
		m_ControlPoint.Init();
	}

	//////////////////////////////////////////////////////////////////////////
	// controls
	str.Format("0x%4x", wcsCode[0]);
	m_CtrlUnicode.SetWindowText(str);
	str.Format("%d", m_ControlPoint.m_Size);
	m_CtrlPoints.SetWindowText(str);
	str.Format("%d", m_ControlPoint.m_ContourNum);
	m_CtrlContours.SetWindowText(str);
	
	delete [] wcsCode;
	Invalidate();
	
}

//
// Export text files
void CB2TTFDlg::OnButtonExportText() 
{
	if( !m_LoadFont ) return;
	if( !m_LoadGlyph ) return;
	//////////////////////////////////////////////////////////////////////////
	char szOpenFilter[] = "Text Files(.txt)|*.txt|";

	CFileDialog dlg( FALSE,"*","",OFN_HIDEREADONLY,szOpenFilter,AfxGetMainWnd());
	if( dlg.DoModal() != IDOK ) return;
	
	CStdioFile file;
	CString str;
	if(!file.Open(dlg.GetPathName(), CFile::modeWrite|CFile::modeCreate ))
	{
		str.Format("Cannot open file : %s", dlg.GetFileName());
		AfxMessageBox(str);
	}

	// write data on text file
	CString ch, uni;
	m_CtrlIn.GetWindowText(ch);
	m_CtrlUnicode.GetWindowText(uni);
	str.Format("/* Character : %s */\n/* Unicode : %s */\n\n", ch, uni);
	file.WriteString(str);
	
	/* write result on text file (ascii mode) */
	m_ControlPoint.WriteText(file);
	//WriteBinary();
	file.Close();
	
}

