/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SELDRIVERDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CSelDriverDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "SelDriverDlg.hpp"
#include <WCL/BusyCursor.hpp>
#include <MDBL/ODBCSource.hpp>
#include <WCL/StrArray.hpp>
#include <MDBL/SQLException.hpp>

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

CSelDriverDlg::CSelDriverDlg()
	: CDialog(IDD_DRIVERS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_DRIVERS, &m_lbDrivers)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_DRIVERS, LBN_DBLCLK, &CSelDriverDlg::OnDblClickDrivers)
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

void CSelDriverDlg::OnInitDialog()
{
	const size_t first = 0;

	CBusyCursor	oWaitCursor;
	CStrArray   astrDrivers;

	try
	{
		CODBCSource::InstalledDrivers(astrDrivers);

		for (size_t i = 0; i < astrDrivers.Size(); i++)
			m_lbDrivers.Add(astrDrivers[i]);
	}
	catch(CSQLException e)
	{
		AlertMsg(TXT("Failed to enumerate ODBC drivers.\n\n%s"), e.twhat());
	}

	// Select first by default.
	m_lbDrivers.CurSel(first);

	// Enable OK button, if something to select.
	Control(IDOK).Enable(astrDrivers.Size());
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

bool CSelDriverDlg::OnOk()
{
	m_strDriver = m_lbDrivers.Text(m_lbDrivers.CurSel());

	return true;
}

/******************************************************************************
** Method:		OnDblClickDrivers()
**
** Description:	User has double-clicked to select a driver.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSelDriverDlg::OnDblClickDrivers()
{
	if (OnOk())
		EndDialog(IDOK);
}
