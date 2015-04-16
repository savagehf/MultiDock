// DlgTest2.cpp : implementation file
//

#include "stdafx.h"
#include "TestDockMultiDoc.h"
#include "DlgTest2.h"
#include "afxdialogex.h"


// CDlgTest2 dialog
const int nBorderSize = 10;


IMPLEMENT_DYNAMIC(CDlgTest2, CDialogEx)

CDlgTest2::CDlgTest2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest2::IDD, pParent)
{
}

CDlgTest2::~CDlgTest2()
{
}

void CDlgTest2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTest2, CDialogEx)

END_MESSAGE_MAP()
