/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FINDDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CFindDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "FindDlg.hpp"

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

CFindDlg::CFindDlg()
	: CDialog(IDD_FIND)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_VALUE,	&m_ebValue)
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

void CFindDlg::OnInitDialog()
{
	// Initialise the controls
	m_ebValue.Text(m_strValue);
	m_ebValue.TextLimit(100);
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

bool CFindDlg::OnOk()
{
	// Fetch data from the controls.
	m_strValue = m_ebValue.Text();

	return true;
}
