/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ROWDATADLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CRowDataDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "RowDataDlg.hpp"

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

CRowDataDlg::CRowDataDlg(CRow& oRow)
	: CDialog(IDD_ROWDATA)
	, m_oRow(oRow)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GRID, &m_lvGrid)
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

void CRowDataDlg::OnInitDialog()
{
	// Set grid style.
	m_lvGrid.FullRowSelect(true);
	m_lvGrid.GridLines(true);

	// Set grid columns.
	m_lvGrid.InsertColumn(0, "Column", 150);
	m_lvGrid.InsertColumn(1, "Value",  250);

	// Get the parent table.
	CTable& oTable = m_oRow.Table();

	// Load column names and data.
	for (int i = 0; i < oTable.ColumnCount(); i++)
	{
		int nRow = m_lvGrid.InsertItem(i, oTable.Column(i).Name());

		if (m_oRow[i] == null)
			m_lvGrid.ItemText(nRow, 1, App.m_strNull);
		else
			m_lvGrid.ItemText(nRow, 1, m_oRow[i].Format());
	}
}
