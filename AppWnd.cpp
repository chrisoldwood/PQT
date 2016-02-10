/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPWND.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppWnd class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppWnd.hpp"
#include "PQTApp.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppWnd::CAppWnd()
	: CDlgFrame(IDR_APPICON, m_AppDlg, false)
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppWnd::~CAppWnd()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Creates the toolbar and status bars.
**
** Parameters:	rcClient	The client rectangle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnCreate(const CRect& rcClient)
{
	// Register for drag'n'drop support.
	::DragAcceptFiles(m_hWnd, TRUE);

	//
	// Create and attach the components.
	//
	m_Accel.LoadRsc(IDR_APPACCEL);
	Accel(&m_Accel);

	m_Menu.LoadRsc(IDR_APPMENU);
	Menu(&m_Menu);

	m_StatusBar.Create(*this, IDC_STATUS_BAR, rcClient);
	StatusBar(&m_StatusBar);

	m_AppDlg.RunModeless(*this);

	UpdateTitle();

	// Call base class.
	CDlgFrame::OnCreate(rcClient);
}

/******************************************************************************
** Method:		OnDestroy()
**
** Description:	The window is being destroyed.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnDestroy()
{
	// Unregister for drag'n'drop support.
	::DragAcceptFiles(m_hWnd, FALSE);

	CDlgFrame::OnDestroy();
}

/******************************************************************************
** Method:		OnFocus()
**
** Description:	Divert the focus to the active view window.
**				
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnFocus()
{
	App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurTabWnd().Focus();
}

/******************************************************************************
** Method:		OnQueryClose()
**
** Description:	Check if the app can close.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppWnd::OnQueryClose()
{
	// If query modified, query to save.
	if (App.m_bModified)
	{
		int nResult = QueryMsg(CAppCmds::QUERY_MODIFIED_MSG);

		if (nResult == IDCANCEL)
			return false;

		if (nResult == IDYES)
		{
			App.m_AppCmds.OnQuerySave();
			return false;
		}
	}

	// Fetch windows final placement.
	App.m_rcLastPos = Placement();

	return true;
}

/******************************************************************************
** Method:		UpdateTitle()
**
** Description:	Updates the title bar by using the connection and query file
**				strings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::UpdateTitle()
{
	// Start with app name.
	CString strTitle = App.m_strTitle;

	bool bConnected = (App.m_pCurrConn.get() != nullptr);

	// Append connection, if one.
	if (bConnected)
	{
		strTitle += TXT(" [");
		strTitle += App.m_pCurrConn->m_strName;
		strTitle += TXT("]");
	}

	if ( (bConnected) && (App.m_strQueryFile != TXT("")) )
		strTitle += TXT(" -");

	// Append query filename, if one.
	if (App.m_strQueryFile != TXT(""))
	{
		strTitle += TXT(" [");
		strTitle += App.m_strQueryFile.FileName();

		// Add modified flag, if set.
		if (App.m_bModified)
			strTitle += TXT(" *");

		strTitle += TXT("]");
	}

	Title(strTitle);
}

/******************************************************************************
** Method:		OnDropFile()
**
** Description:	The user has drag'n'dropped one or more files onto the window.
**
** Parameters:	nFile		The index of the dropped file.
**				pszPath		The files' path.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnDropFile(int nFile, const tchar* pszPath)
{
	ASSERT(nFile   >= 0);
	ASSERT(pszPath != nullptr);

	// Only load the first file.
	if (nFile == 0)
		App.m_AppCmds.OnQueryOpen(pszPath);
}
