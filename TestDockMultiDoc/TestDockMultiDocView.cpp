
// TestDockMultiDocView.cpp : implementation of the CTestDockMultiDocView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TestDockMultiDoc.h"
#endif

#include "TestDockMultiDocDoc.h"
#include "TestDockMultiDocView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestDockMultiDocView

IMPLEMENT_DYNCREATE(CTestDockMultiDocView, CView)

BEGIN_MESSAGE_MAP(CTestDockMultiDocView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestDockMultiDocView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTestDockMultiDocView construction/destruction

CTestDockMultiDocView::CTestDockMultiDocView()
{
	// TODO: add construction code here

}

CTestDockMultiDocView::~CTestDockMultiDocView()
{
}

BOOL CTestDockMultiDocView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTestDockMultiDocView drawing

void CTestDockMultiDocView::OnDraw(CDC* /*pDC*/)
{
	CTestDockMultiDocDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTestDockMultiDocView printing


void CTestDockMultiDocView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestDockMultiDocView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestDockMultiDocView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestDockMultiDocView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTestDockMultiDocView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTestDockMultiDocView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTestDockMultiDocView diagnostics

#ifdef _DEBUG
void CTestDockMultiDocView::AssertValid() const
{
	CView::AssertValid();
}

void CTestDockMultiDocView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDockMultiDocDoc* CTestDockMultiDocView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDockMultiDocDoc)));
	return (CTestDockMultiDocDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestDockMultiDocView message handlers
