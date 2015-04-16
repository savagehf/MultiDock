// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "stdafx.h"
#include "BasePane.h"
#include "DlgTest1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 10;

/////////////////////////////////////////////////////////////////////////////
// CBaseDlg

CBaseDlg::CBaseDlg()
{
	m_pBaseDlg = NULL;
	m_nMyCalendarsY = 0;
	m_eDlgType = enmDlgType_Test1;
}

CBaseDlg::~CBaseDlg()
{
	if(NULL != m_pBaseDlg && NULL != m_pBaseDlg->GetSafeHwnd())
	{
		m_pBaseDlg->DestroyWindow();
		delete m_pBaseDlg;
		m_pBaseDlg = NULL;
	}
}

BEGIN_MESSAGE_MAP(CBaseDlg, CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseDlg message handlers

int CBaseDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy(0, 0, 0, 0);
	switch(m_eDlgType)
	{
	case enmDlgType_Test1:
		{
			m_pBaseDlg = new CDlgTest1;
			m_pBaseDlg->Create(CDlgTest1::IDD, this);
		}
		break;
	case enmDlgType_Test2:
		{
			m_pBaseDlg = new CDlgTest2;
			m_pBaseDlg->Create(CDlgTest2::IDD, this);
		}
		break;
	default:
		break;
	}
	//m_dlgTest1.Create(CDlgTest1::IDD, this);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_PAGES_SMALL_HC);

	m_Images.Create(16, 16, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_Images.Add(&bmp, RGB(255, 0, 255));

	return 0;
}

BOOL CBaseDlg::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CBaseDlg::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	int nMyCalendarsHeight = 70;

	if(NULL != m_pBaseDlg && m_pBaseDlg->GetSafeHwnd() != NULL)
	{
		m_pBaseDlg->SetWindowPos(NULL, nBorderSize, nBorderSize, cx - 2 * nBorderSize, 
			cy - 2 * nBorderSize - nMyCalendarsHeight - 10, 
			SWP_NOZORDER | SWP_NOACTIVATE);
	}

	m_nMyCalendarsY = cy - nMyCalendarsHeight;
}

BOOL CBaseDlg::Create(const RECT& rect, CWnd* pParentWnd, EDLGTYPE eType, UINT nID)
{
	m_eDlgType = eType;

	return CWnd::Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID);
}

void CBaseDlg::OnPaint()
{
	CPaintDC dc(this);

	CRect rectClient;
	GetClientRect(rectClient);

	dc.FillRect(rectClient, &afxGlobalData.brWindow);

	if (rectClient.bottom - m_nMyCalendarsY > 0)
	{
		CRect rectMyCalendarsCaption = rectClient;
		rectMyCalendarsCaption.top = m_nMyCalendarsY;
		rectMyCalendarsCaption.bottom = rectMyCalendarsCaption.top + afxGlobalData.GetTextHeight(TRUE) * 3 / 2;

		COLORREF clrText = CMFCVisualManager::GetInstance()->OnDrawPaneCaption(&dc, NULL, FALSE, rectMyCalendarsCaption, CRect(0, 0, 0, 0));

		CPen* pOldPen = dc.SelectObject(&afxGlobalData.penBarShadow);

		dc.MoveTo(rectMyCalendarsCaption.left - 1, rectMyCalendarsCaption.top);
		dc.LineTo(rectMyCalendarsCaption.right, rectMyCalendarsCaption.top);

		dc.SelectStockObject(BLACK_PEN);

		dc.MoveTo(rectMyCalendarsCaption.left - 1, rectMyCalendarsCaption.bottom);
		dc.LineTo(rectMyCalendarsCaption.right, rectMyCalendarsCaption.bottom);

		dc.SelectObject(pOldPen);

		CRect rectText = rectMyCalendarsCaption;
		rectText.DeflateRect(10, 0);

		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(clrText);

		CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

		BOOL bNameValid;
		CString str;

		bNameValid = str.LoadString(IDS_MYCALENDARS);
		ASSERT(bNameValid);
		dc.DrawText(str, rectText, DT_VCENTER | DT_LEFT | DT_SINGLELINE);

		CRect rectCalendar = rectClient;
		rectCalendar.top = rectMyCalendarsCaption.bottom + 5;
		rectCalendar.bottom = rectCalendar.top + afxGlobalData.GetTextHeight(TRUE) * 3 / 2 - 5;

		dc.FillSolidRect(rectCalendar, RGB(255, 255, 213));

		rectCalendar.DeflateRect(20, 0);
		m_Images.Draw(&dc, 3, rectCalendar.TopLeft(), 0);

		rectCalendar.left += 20;

		bNameValid = str.LoadString(IDS_CALENDAR);
		ASSERT(bNameValid);

		dc.SetTextColor(afxGlobalData.clrHotLinkNormalText);
		dc.DrawText(str, rectCalendar, DT_VCENTER | DT_LEFT | DT_SINGLELINE);

		dc.SelectObject(pOldFont);
	}
}


