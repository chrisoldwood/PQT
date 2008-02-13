/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		QUERYPREFSDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CQueryPrefsDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "QueryPrefsDlg.hpp"
#include <WCL/StrCvt.hpp>

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

CQueryPrefsDlg::CQueryPrefsDlg()
	: CDialog(IDD_QUERYPREFS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_MINWIDTH,	&m_ebMinWidth)
		CTRL(IDC_MAXWIDTH,	&m_ebMaxWidth)
		CTRL(IDC_NULLVAL,	&m_ebNullVal )
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

void CQueryPrefsDlg::OnInitDialog()
{
	// Initialise the controls
	m_ebMinWidth.Text(CStrCvt::FormatUInt(m_nMinWidth));
	m_ebMinWidth.TextLimit(3);

	m_ebMaxWidth.Text(CStrCvt::FormatUInt(m_nMaxWidth));
	m_ebMaxWidth.TextLimit(3);

	m_ebNullVal.Text(m_strNullVal);
	m_ebNullVal.TextLimit(10);
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

bool CQueryPrefsDlg::OnOk()
{
	// Fetch data from the controls.
	m_nMinWidth  = CStrCvt::ParseUInt(m_ebMinWidth.Text());
	m_nMaxWidth  = CStrCvt::ParseUInt(m_ebMaxWidth.Text());
	m_strNullVal = m_ebNullVal.Text();

	// Validate.
	if ( (m_nMinWidth < 1) || (m_nMinWidth > 255) )
	{
		AlertMsg(TXT("The min column width must between 1 and 255"));
		return false;
	}

	if ( (m_nMaxWidth < 1) || (m_nMaxWidth > 255) )
	{
		AlertMsg(TXT("The max column width must between 1 and 255"));
		return false;
	}

	return true;
}
