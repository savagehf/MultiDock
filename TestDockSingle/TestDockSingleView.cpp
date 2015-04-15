
// TestDockSingleView.cpp : implementation of the CTestDockSingleView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TestDockSingle.h"
#endif

#include "TestDockSingleDoc.h"
#include "TestDockSingleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestDockSingleView

IMPLEMENT_DYNCREATE(CTestDockSingleView, CView)

BEGIN_MESSAGE_MAP(CTestDockSingleView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestDockSingleView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTestDockSingleView construction/destruction

CTestDockSingleView::CTestDockSingleView()
{
	// TODO: add construction code here

}

CTestDockSingleView::~CTestDockSingleView()
{
}

BOOL CTestDockSingleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTestDockSingleView drawing

void CTestDockSingleView::OnDraw(CDC* /*pDC*/)
{
	CTestDockSingleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTestDockSingleView printing


void CTestDockSingleView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestDockSingleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestDockSingleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestDockSingleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTestDockSingleView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTestDockSingleView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTestDockSingleView diagnostics

#ifdef _DEBUG
void CTestDockSingleView::AssertValid() const
{
	CView::AssertValid();
}

void CTestDockSingleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDockSingleDoc* CTestDockSingleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDockSingleDoc)));
	return (CTestDockSingleDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestDockSingleView message handlers
