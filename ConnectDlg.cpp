/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CONNECTDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CConnectDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ConnectDlg.hpp"
#include "CfgConDlg.hpp"

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

CConnectDlg::CConnectDlg() : CDialog(IDD_CONNECT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_DATABASE,   &m_cbDatabase  )
		CTRL(IDC_LOGIN,      &m_ebLogin     )
		CTRL(IDC_PASSWORD,   &m_ebPassword  )
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ADD,      BN_CLICKED,    OnAdd   )
		CMD_CTRLMSG(IDC_EDIT,     BN_CLICKED,    OnEdit  )
		CMD_CTRLMSG(IDC_DATABASE, CBN_SELCHANGE, OnSelect)
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
	// Initialise the controls.
	m_ebLogin.TextLimit(100);
	m_ebPassword.TextLimit(100);

	// Load the database combo.
	for (int i = 0; i < App.m_aConConfigs.Size(); i++)
	{
		CConConfig& oConn = App.m_aConConfigs[i];

		int nIndex = m_cbDatabase.Add(oConn.m_strName);
		m_cbDatabase.ItemData(nIndex, i);
	}

	// Any connections setup?
	if (m_cbDatabase.Count())
	{
		// Select 1st connection by default.
		m_cbDatabase.CurSel(0);
		OnSelect();
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
		int nArrIndex = App.m_aConConfigs.Add(*new CConConfig(Dlg.m_oConfig));

		// Add to the combo box.
		int nCBIndex = m_cbDatabase.Add(Dlg.m_oConfig.m_strName);
		m_cbDatabase.ItemData(nCBIndex, nArrIndex);

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

	Dlg.m_oConfig = App.m_aConConfigs[nConn];

	if (Dlg.RunModal(*this) == IDOK)
	{
		// Update the master list.
		App.m_aConConfigs[nConn] = Dlg.m_oConfig;

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
	CConConfig&	oConn = App.m_aConConfigs[nConn];

	// Set the default login.
	m_ebLogin.Text(oConn.m_strLogin);
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
