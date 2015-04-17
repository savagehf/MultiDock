
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
//#include "FileView.h"
//#include "ClassView.h"
//#include "OutputWnd.h"
//#include "PropertiesWnd.h"
#include "BasePane.h"
#include <vector>
#include <string>

using namespace std;

struct stBasePane
{
	CString strPaneTitle;
	EDLGTYPE eType;
	CBaseDlg* pBaseDlg;

	stBasePane()
	{
		pBaseDlg = NULL;
	}
};
typedef vector<stBasePane> VecBasePanes;

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); if (!bNameValid) strName.Empty(); }
};

/////////////////////////////////////////////////////////////////////////////
// COutlookOptionsDlg dialog

//class COutlookOptionsDlg : public CDialog
//{
//	// Construction
//public:
//	COutlookOptionsDlg(CMFCOutlookBarTabCtrl& parentBar);   // standard constructor
//
//	// Dialog Data
//	//{{AFX_DATA(COutlookOptionsDlg)
//	enum { IDD = IDD_AFXBARRES_OUTLOOKBAR_OPTIONS };
//	CButton m_btnMoveUp;
//	CButton m_wndMoveDown;
//	CButton m_wndReset;
//	CMFCToolBarsListCheckBox m_wndList;
//	//}}AFX_DATA
//
//
//	// Overrides
//	// ClassWizard generated virtual function overrides
//	//{{AFX_VIRTUAL(COutlookOptionsDlg)
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	//}}AFX_VIRTUAL
//
//	// Implementation
//protected:
//
//	// Generated message map functions
//	//{{AFX_MSG(COutlookOptionsDlg)
//	afx_msg void OnSelchange();
//	afx_msg void OnDblclkList();
//	afx_msg void OnMoveDown();
//	afx_msg void OnMoveUp();
//	virtual BOOL OnInitDialog();
//	virtual void OnOK();
//	afx_msg void OnReset();
//	//}}AFX_MSG
//	DECLARE_MESSAGE_MAP()
//
//	CMFCOutlookBarTabCtrl& m_parentBar;
//
//	void MoveItem(BOOL bMoveUp);
//};


class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

	//add rabin button
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;

	COutlookBar       m_wndNavigationBar;
	CMFCShellTreeCtrl m_wndTree;
	VecBasePanes m_vecUserDlgs;
	

	//CFileView         m_wndFileView;
	//CClassView        m_wndClassView;
	//COutputWnd        m_wndOutput;
	//CPropertiesWnd    m_wndProperties;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRibbonPane1Cmd1();
	afx_msg void OnRibbonPane1Cmd2();
	afx_msg void OnRibbonPane1Cmd3();
	afx_msg void OnRibbonPane1Cmd4();
	afx_msg void OnRibbonPane2Cmd1();
	afx_msg void OnRibbonPane2Cmd2();
	afx_msg void OnRibbonPane2Cmd3();
	afx_msg void OnRibbonPane2Cmd4();

	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, int nInitialWidth);

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


