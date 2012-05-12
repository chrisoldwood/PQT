/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CONNECTDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CConnectDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ConnectDlg.hpp"
#include "CfgConDlg.hpp"
#include "PQTApp.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CConnectDlg::CConnectDlg()
	: CDialog(IDD_CONNECT)
	, m_nConnection(Core::npos)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_DATABASE,   &m_cbDatabase  )
		CTRL(IDC_LOGIN,      &m_ebLogin     )
		CTRL(IDC_PASSWORD,   &m_ebPassword  )
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_MANAGE,   BN_CLICKED,    &CConnectDlg::OnManage)
		CMD_CTRLMSG(IDC_DATABASE, CBN_SELCHANGE, &CConnectDlg::OnSelect)
	END_CTRLMSG_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CConnectDlg::OnInitDialog()
{
	ASSERT( (m_nConnection == Core::npos) || (m_nConnection < App.m_apConConfigs.size()) );

	// Initialise the controls.
	m_ebLogin.TextLimit(100);
	m_ebLogin.Enable(false);
	m_ebPassword.TextLimit(100);
	m_ebPassword.Enable(false);

	LoadDatabaseList();

	CConConfigPtr pDefault;

	// Find default connection, if available.
	if (m_nConnection != Core::npos)
		pDefault = App.m_apConConfigs[m_nConnection];

	// Any connections setup?
	if (m_cbDatabase.Count() != 0)
	{
		CString strDefault;

		if (pDefault.get() != nullptr)
			strDefault = pDefault->m_strName;

		// Find default.
		size_t nDefault = m_cbDatabase.FindExact(strDefault);

		// Select 1st, if no default.
		if (nDefault == Core::npos)
			nDefault = 0u;
			
		m_cbDatabase.CurSel(nDefault);
		OnSelect();

		// Set focus to password field.
		m_ebPassword.Focus();
		MsgHandled(false);
	}

	UpdateUI();
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	Validate the data and close the dialog.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CConnectDlg::OnOk()
{
	ASSERT(m_cbDatabase.Count() > 0);
	ASSERT(m_cbDatabase.CurSel() != Core::npos);

	// Fetch data from the controls.
	m_nConnection = m_cbDatabase.ItemData(m_cbDatabase.CurSel());
	m_strLogin    = m_ebLogin.Text();
	m_strPassword = m_ebPassword.Text();

	return true;
}

/******************************************************************************
** Method:		OnAdd()
**
** Description:	Show the dialog to add a new database connection.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CConnectDlg::OnManage()
{
	const size_t first = 0;

	if (App.m_AppCmds.ManageDatabases())
	{
		LoadDatabaseList();

		// Select by default.
		if (m_cbDatabase.Count() != 0)
		{
			m_cbDatabase.CurSel(first);
			OnSelect();
		}

		UpdateUI();
	}
}

/******************************************************************************
** Method:		OnSelect()
**
** Description:	The user has selected a pre-configured connection.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CConnectDlg::OnSelect()
{
	// Get the current selection.
	size_t        nSel  = m_cbDatabase.CurSel();
	size_t        nConn = m_cbDatabase.ItemData(nSel);
	CConConfigPtr pConn = App.m_apConConfigs[nConn];

	// Set the default login.
	m_ebLogin.Text(pConn->m_strLogin);
	m_ebLogin.Enable(pConn->m_eSecurity == LOGIN);
	m_ebPassword.Text(TXT(""));
	m_ebPassword.Enable(pConn->m_eSecurity == LOGIN);
}

////////////////////////////////////////////////////////////////////////////////
//! Load the list of database connections in the view.

void CConnectDlg::LoadDatabaseList()
{
	m_cbDatabase.Reset();

	// Load the database combo.
	for (size_t i = 0; i < App.m_apConConfigs.size(); i++)
	{
		CConConfigPtr pConn = App.m_apConConfigs[i];

		m_cbDatabase.Add(pConn->m_strName, i);
	}
}

/******************************************************************************
** Method:		UpdateUI()
**
** Description:	Updates the UI by enabling/disabling buttons.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CConnectDlg::UpdateUI()
{
	bool bConns = (m_cbDatabase.Count() > 0);

	Control(IDOK).Enable(bConns);
}
