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
		CTRL(IDC_CONNECTION,	&m_ebConnection)
	END_CTRL_TABLE
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
	// Initialise the control.
	m_ebConnection.Text(m_strConnection);
	m_ebConnection.TextLimit(1024);
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
	m_strConnection = m_ebConnection.Text();

	// Validate.
	if (m_strConnection == "")
	{
		AlertMsg("You must supply a connection string.");
		return false;
	}

	return true;
}
