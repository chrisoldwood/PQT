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
		CMD_CTRLMSG(IDC_ADD,      BN_CLICKED,    &CConnectDlg::OnAdd   )
		CMD_CTRLMSG(IDC_EDIT,     BN_CLICKED,    &CConnectDlg::OnEdit  )
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
	CString strDefault;

	// Initialise the controls.
	m_ebLogin.TextLimit(100);
	m_ebPassword.TextLimit(100);

	// Load the database combo.
	for (size_t i = 0; i < App.m_apConConfigs.size(); i++)
	{
		CConConfig* pConn = App.m_apConConfigs[i];

		m_cbDatabase.Add(pConn->m_strName, i);

		if (m_nConnection == i)
			strDefault = pConn->m_strName;
	}

	// Any connections setup?
	if (m_cbDatabase.Count())
	{
		// Find default.
		int nDefault = m_cbDatabase.FindExact(strDefault);

		// Select 1st, if no default.
		if (nDefault == CB_ERR)
			nDefault = 0;
			
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

void CConnectDlg::OnAdd()
{
	CCfgConDlg Dlg;

	if (Dlg.RunModal(*this) == IDOK)
	{
		// Add to the App collection.
		App.m_apConConfigs.push_back(new CConConfig(Dlg.m_oConfig));

		// Add to the combo box.
		int nCBIndex = m_cbDatabase.Add(Dlg.m_oConfig.m_strName);
		m_cbDatabase.ItemData(nCBIndex, App.m_apConConfigs.size()-1);

		// Select by default.
		m_cbDatabase.CurSel(nCBIndex);
		OnSelect();
	}

	UpdateUI();
}

/******************************************************************************
** Method:		OnEdit()
**
** Description:	Show the dialog to edit the current database connection.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CConnectDlg::OnEdit()
{
	// Ignore, if nothing editable.
	if (m_cbDatabase.CurSel() == CB_ERR)
		return;

	// Get the current selection.
	int nSel  = m_cbDatabase.CurSel();
	int nConn = m_cbDatabase.ItemData(nSel);

	// Initialise the dialog.
	CCfgConDlg Dlg;

	Dlg.m_oConfig = *App.m_apConConfigs[nConn];

	if (Dlg.RunModal(*this) == IDOK)
	{
		// Update the master list.
		*App.m_apConConfigs[nConn] = Dlg.m_oConfig;

		// Update to the combo box.
		m_cbDatabase.Delete(nSel);
		nSel = m_cbDatabase.Add(Dlg.m_oConfig.m_strName);
		m_cbDatabase.ItemData(nSel, nConn);

		// Reselect.
		m_cbDatabase.CurSel(nSel);
		OnSelect();
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
	int         nSel  = m_cbDatabase.CurSel();
	int         nConn = m_cbDatabase.ItemData(nSel);
	CConConfig*	pConn = App.m_apConConfigs[nConn];

	// Set the default login.
	m_ebLogin.Text(pConn->m_strLogin);
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
	Control(IDC_EDIT).Enable(bConns);
}
