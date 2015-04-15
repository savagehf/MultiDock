// DlgTest1.cpp : implementation file
//

#include "stdafx.h"
#include "TestDockMultiDoc.h"
#include "DlgTest1.h"
#include "afxdialogex.h"


// CDlgTest1 dialog

IMPLEMENT_DYNAMIC(CDlgTest1, CDialogEx)

CDlgTest1::CDlgTest1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest1::IDD, pParent)
{

}

CDlgTest1::~CDlgTest1()
{
}

void CDlgTest1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTest1, CDialogEx)
END_MESSAGE_MAP()


// CDlgTest1 message handlers
