#pragma once


// CDlgTest1 dialog

class CDlgTest1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTest1)

public:
	CDlgTest1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest1();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
