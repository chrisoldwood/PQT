/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SELDSNDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CSelDSNDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "SelDSNDlg.hpp"
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

CSelDSNDlg::CSelDSNDlg()
	: CDialog(IDD_DSNS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_DSNS, &m_lbDSNs)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_DSNS, LBN_DBLCLK, &CSelDSNDlg::OnDblClickDSNs)
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

void CSelDSNDlg::OnInitDialog()
{
	const size_t first = 0;

	CBusyCursor	oWaitCursor;
	CStrArray   astrSources;

	try
	{
		CODBCSource::InstalledSources(astrSources);

		for (size_t i = 0; i < astrSources.Size(); i++)
			m_lbDSNs.Add(astrSources[i]);
	}
	catch(const CSQLException& e)
	{
		AlertMsg(TXT("Failed to enumerate ODBC DSNs.\n\n%s"), e.twhat());
	}

	// Select first by default.
	m_lbDSNs.CurSel(first);

	// Enable OK button, if something to select.
	Control(IDOK).Enable(astrSources.Size());
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

bool CSelDSNDlg::OnOk()
{
	m_strDSN = m_lbDSNs.Text(m_lbDSNs.CurSel());

	return true;
}

/******************************************************************************
** Method:		OnDblClickDSNs()
**
** Description:	User has double-clicked to select a DSN.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSelDSNDlg::OnDblClickDSNs()
{
	if (OnOk())
		EndDialog(IDOK);
}
