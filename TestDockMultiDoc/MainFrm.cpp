
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TestDockMultiDoc.h"
#include "MainFrm.h"
#include "XmlConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_SET_PANE1_CMD1, &CMainFrame::OnRibbonPane1Cmd1)
	ON_COMMAND(ID_SET_PANE1_CMD2, &CMainFrame::OnRibbonPane1Cmd2)
	ON_COMMAND(ID_SET_PANE1_CMD3, &CMainFrame::OnRibbonPane1Cmd3)
	ON_COMMAND(ID_SET_PANE1_CMD4, &CMainFrame::OnRibbonPane1Cmd4)

	ON_COMMAND(ID_SET_PANE2_CMD1, &CMainFrame::OnRibbonPane2Cmd1)
	ON_COMMAND(ID_SET_PANE2_CMD2, &CMainFrame::OnRibbonPane2Cmd2)
	ON_COMMAND(ID_SET_PANE2_CMD3, &CMainFrame::OnRibbonPane2Cmd3)
	ON_COMMAND(ID_SET_PANE2_CMD4, &CMainFrame::OnRibbonPane2Cmd4)
	
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_OFF_2007_BLUE, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_OFF_2007_BLUE, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	//ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);


	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	//create rabin�����д����������Menu������ɲſ��ԡ�
	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	//if (!CreateDockingWindows())
	m_vecUserDlgs.clear();
	int nBasePaneCount = AppXml()->GetAttributeInt(_T("BaseDlgNodeCount"), 0);
	if(nBasePaneCount > 0)
	{
		for(int index=1; index<=nBasePaneCount; index++)
		{
			stBasePane oneItem;
			oneItem.pBaseDlg = new CBaseDlg;
			
			CString strNode;
			strNode.Format(_T("BaseDlgNode\\Index_%d"), index);
			oneItem.eType = (EDLGTYPE)(AppXml()->GetAttributeInt(strNode, 0));

			strNode.Empty();
			strNode.Format(_T("BaseDlgNode\\Title_%d"), index);
			std::wstring strTitle= AppXml()->GetAttributeText(strNode, _T(""));
			oneItem.strPaneTitle = strTitle.c_str();
			
			m_vecUserDlgs.push_back(oneItem);
		}
	}
	else
	{
		stBasePane oneItem;
		oneItem.pBaseDlg = new CBaseDlg;
		oneItem.strPaneTitle = _T("�������һ");
		oneItem.eType = enmDlgType_Test1;
		m_vecUserDlgs.push_back(oneItem);

		stBasePane oneItem2;
		oneItem2.pBaseDlg = new CBaseDlg;
		oneItem2.strPaneTitle = _T("��������");
		oneItem2.eType = enmDlgType_Test2;
		m_vecUserDlgs.push_back(oneItem2);
	
		stBasePane oneItem3;
		oneItem3.pBaseDlg = new CBaseDlg;
		oneItem3.strPaneTitle = _T("���������");
		oneItem3.eType = enmDlgType_Test3;
		m_vecUserDlgs.push_back(oneItem3);

		stBasePane oneItem4;
		oneItem4.pBaseDlg = new CBaseDlg;
		oneItem4.strPaneTitle = _T("���������");
		oneItem4.eType = enmDlgType_Test4;
		m_vecUserDlgs.push_back(oneItem4);
	}
	

	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION,250))
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	/*m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);*/
	m_wndNavigationBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndNavigationBar);

	//EnableDocking(CBRS_ALIGN_LEFT);
	//EnableAutoHidePanes(CBRS_ALIGN_RIGHT);


	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}
BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp("Short Cut");
	//bNameValid = strTemp.LoadStringW(
	//ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID,
		WS_CHILD 
		| WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI|CBRS_FLOATING/*WS_CHILD | WS_VISIBLE | CBRS_LEFT*/))
	{
		return FALSE; // fail to create
	}

	//lee���õ���ǰbar�����basetabctrl��
	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;
	static UINT uDlgID = 1200;

	// can float, can autohide, can resize, CAN NOT CLOSE
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	//lee������tree �ؼ�, hard code id=1200
	CRect rectDummy(0, 0, 0, 0);
	strTemp = _T("test1");
	int nDlgIndex = 1;
	for(VecBasePanes::iterator it = m_vecUserDlgs.begin();
		it != m_vecUserDlgs.end(); ++it)
	{
		CBaseDlg* pBaseDlg = it->pBaseDlg;
		if(NULL != pBaseDlg)
		{
			CString strNode;
			strNode.Format(_T("BaseDlgNode\\Index_%d"), nDlgIndex);
			AppXml()->SetAttributeInt(strNode, (UINT)it->eType);
			AppXml()->FlushData();

			strNode.Empty();
			strNode.Format(_T("BaseDlgNode\\Title_%d"), nDlgIndex);
			AppXml()->SetAttribute(strNode, it->strPaneTitle);
			AppXml()->FlushData();
			
			pBaseDlg->Create(rectDummy, &bar,  it->eType, uDlgID++);
			pOutlookBar->AddControl(pBaseDlg, it->strPaneTitle, ++uiPageID, TRUE, dwStyle);
		}
		nDlgIndex++;
	}
	AppXml()->SetAttributeInt(_T("BaseDlgNodeCount"), m_vecUserDlgs.size());
	AppXml()->FlushData();


	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	
	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}
BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	//// Create class view
	//CString strClassView;
	//bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Class View window\n");
	//	return FALSE; // failed to create
	//}

	//// Create file view
	//CString strFileView;
	//bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create File View window\n");
	//	return FALSE; // failed to create
	//}

	//// Create output window
	//CString strOutputWnd;
	//bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	//ASSERT(bNameValid);
	//if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Output window\n");
	//	return FALSE; // failed to create
	//}

	//// Create properties window
	//CString strPropertiesWnd;
	//bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	//ASSERT(bNameValid);
	//if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), 
	// TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS 
	//| WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Properties window\n");
	//	return FALSE; // failed to create
	//}

	//SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	/*HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();*/
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers
void CMainFrame::OnRibbonPane1Cmd1()
{
	//lee���õ���ǰbar�����basetabctrl��
	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)m_wndNavigationBar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return /*FALSE*/;
	}

	pOutlookBar->OnShowOptions();
}
void CMainFrame::OnRibbonPane1Cmd2()
{
	AfxMessageBox(_T("�������Լ����û�����1��"));
}
void CMainFrame::OnRibbonPane1Cmd3()
{
	AfxMessageBox(_T("�������Լ����û�����2��"));
}
void CMainFrame::OnRibbonPane1Cmd4()
{
	AfxMessageBox(_T("�������Լ����û�����3��"));
}
void CMainFrame::OnRibbonPane2Cmd1()
{
	AfxMessageBox(_T("�������Լ����û�����4��"));
}
void CMainFrame::OnRibbonPane2Cmd2()
{
	AfxMessageBox(_T("�������Լ����û�����5��"));
}
void CMainFrame::OnRibbonPane2Cmd3()
{
	AfxMessageBox(_T("�������Լ����û�����6��"));
}
void CMainFrame::OnRibbonPane2Cmd4()
{
	AfxMessageBox(_T("�������Լ����û�����7��"));
}

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	/*case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;*/

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}


//COutlookOptionsDlg::COutlookOptionsDlg(CMFCOutlookBarTabCtrl& parentBar)
//	: CDialog(COutlookOptionsDlg::IDD, &parentBar), m_parentBar(parentBar)
//{
//}
//
//void COutlookOptionsDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(COutlookOptionsDlg)
//	DDX_Control(pDX, IDC_AFXBARRES_MOVEUP, m_btnMoveUp);
//	DDX_Control(pDX, IDC_AFXBARRES_MOVEDOWN, m_wndMoveDown);
//	DDX_Control(pDX, IDC_AFXBARRES_LIST, m_wndList);
//	DDX_Control(pDX, IDC_AFXBARRES_RESET, m_wndReset);
//	//}}AFX_DATA_MAP
//}
//
//BEGIN_MESSAGE_MAP(COutlookOptionsDlg, CDialog)
//	//{{AFX_MSG_MAP(COutlookOptionsDlg)
//	ON_LBN_SELCHANGE(IDC_AFXBARRES_LIST, &COutlookOptionsDlg::OnSelchange)
//	ON_LBN_DBLCLK(IDC_AFXBARRES_LIST, &COutlookOptionsDlg::OnDblclkList)
//	ON_BN_CLICKED(IDC_AFXBARRES_MOVEDOWN, &COutlookOptionsDlg::OnMoveDown)
//	ON_BN_CLICKED(IDC_AFXBARRES_MOVEUP, &COutlookOptionsDlg::OnMoveUp)
//	ON_BN_CLICKED(IDC_AFXBARRES_RESET, &COutlookOptionsDlg::OnReset)
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()
//
///////////////////////////////////////////////////////////////////////////////
//// COutlookOptionsDlg message handlers
//
//void COutlookOptionsDlg::OnSelchange()
//{
//	m_btnMoveUp.EnableWindow(m_wndList.GetCurSel() > 0);
//	m_wndMoveDown.EnableWindow(m_wndList.GetCurSel() < m_wndList.GetCount() - 1);
//}
//
//void COutlookOptionsDlg::OnDblclkList()
//{
//	int nSel = m_wndList.GetCurSel();
//	if (nSel >= 0)
//	{
//		m_wndList.SetCheck(nSel, !m_wndList.GetCheck(nSel));
//	}
//}
//
//void COutlookOptionsDlg::OnMoveDown()
//{
//	MoveItem(FALSE);
//}
//
//void COutlookOptionsDlg::OnMoveUp()
//{
//	MoveItem(TRUE);
//}
//
//BOOL COutlookOptionsDlg::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	if (AfxGetMainWnd() != NULL && (AfxGetMainWnd()->GetExStyle() & WS_EX_LAYOUTRTL))
//	{
//		ModifyStyleEx(0, WS_EX_LAYOUTRTL);
//	}
//
//	for (int i = 0; i < m_parentBar.m_iTabsNum; i ++)
//	{
//		CString str;
//		m_parentBar.GetTabLabel(i, str);
//
//		int nIndex = m_wndList.AddString(str);
//
//		m_wndList.SetItemData(nIndex, (DWORD_PTR) i);
//		m_wndList.SetCheck(nIndex, m_parentBar.IsTabVisible(i));
//	}
//
//	m_wndList.SetCurSel(0);
//	OnSelchange();
//
//	CMFCOutlookBar* pOutlookBar = DYNAMIC_DOWNCAST(CMFCOutlookBar, m_parentBar.GetParent());
//	if (pOutlookBar == NULL)
//	{
//		m_wndReset.EnableWindow(FALSE);
//		m_wndReset.ShowWindow(SW_HIDE);
//	}
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//}
//
//void COutlookOptionsDlg::OnOK()
//{
//	CArray<int, int> arTabsOrder;
//
//	for (int nIndex = 0; nIndex < m_wndList.GetCount(); nIndex++)
//	{
//		int i = (int) m_wndList.GetItemData(nIndex);
//
//		BOOL bVisible = m_wndList.GetCheck(nIndex);
//
//		if (bVisible != m_parentBar.IsTabVisible(i))
//		{
//			m_parentBar.ShowTab(i, bVisible, FALSE);
//		}
//
//		arTabsOrder.Add(i);
//	}
//
//	m_parentBar.SetTabsOrder(arTabsOrder);
//
//	CDialog::OnOK();
//}
//
//void COutlookOptionsDlg::OnReset()
//{
//	CMFCOutlookBar* pOutlookBar = DYNAMIC_DOWNCAST(CMFCOutlookBar, m_parentBar.GetParent());
//	if (pOutlookBar == NULL)
//	{
//		ASSERT(FALSE);
//		return;
//	}
//
//	CArray<int, int> arTabsOrder;
//	int i = 0;
//
//	for (i = 0; i < pOutlookBar->GetDefaultTabsOrder().GetSize(); i++)
//	{
//		int iTabID = pOutlookBar->GetDefaultTabsOrder() [i];
//		int iTab = m_parentBar.GetTabByID(iTabID);
//
//		if (iTab < 0)
//		{
//			ASSERT(FALSE);
//			return;
//		}
//
//		arTabsOrder.Add(iTab);
//	}
//
//	m_wndList.ResetContent();
//
//	for (i = 0; i < arTabsOrder.GetSize(); i ++)
//	{
//		int iTabNum = arTabsOrder [i];
//
//		CString str;
//		m_parentBar.GetTabLabel(iTabNum, str);
//
//		int nIndex = m_wndList.AddString(str);
//
//		m_wndList.SetItemData(nIndex, (DWORD_PTR) iTabNum);
//		m_wndList.SetCheck(nIndex, TRUE);
//	}
//
//	m_wndList.SetCurSel(0);
//	OnSelchange();
//}
//
//void COutlookOptionsDlg::MoveItem(BOOL bMoveUp)
//{
//	int nSel = m_wndList.GetCurSel();
//
//	CString str;
//	m_wndList.GetText(nSel, str);
//	DWORD_PTR dwData = m_wndList.GetItemData(nSel);
//	BOOL bCheck = m_wndList.GetCheck(nSel);
//
//	m_wndList.DeleteString(nSel);
//
//	int nNewIndex = bMoveUp ? nSel - 1 : nSel + 1;
//
//	int nIndex = m_wndList.InsertString(nNewIndex, str);
//
//	m_wndList.SetItemData(nIndex, dwData);
//	m_wndList.SetCheck(nIndex, bCheck);
//
//	m_wndList.SetCurSel(nIndex);
//	OnSelchange();
//}