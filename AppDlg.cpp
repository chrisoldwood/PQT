/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include <stdio.h>

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

CAppDlg::CAppDlg() : CMainDlg(IDD_MAIN)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_TAB,	&m_tcTabCtrl)
		CTRL(IDC_QUERY,	&m_ebQuery)
		CTRL(IDC_GRID,	&m_lvGrid)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_TAB, LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE
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

void CAppDlg::OnInitDialog()
{
	// Add the tabs.
	m_tcTabCtrl.AddTab("Query",   m_ebQuery);
	m_tcTabCtrl.AddTab("Results", m_lvGrid);
	m_tcTabCtrl.CurSel(QUERY_TAB);

	// Use a fixed pitch font for the query/results.
	m_ebQuery.Font(CFont(ANSI_FIXED_FONT));
	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));

	// Set grid style.
	m_lvGrid.FullRowSelect(true);
	m_lvGrid.GridLines(true);
}

/******************************************************************************
** Method:		DisplayTable()
**
** Description:	Load the table into the grid.
**
** Parameters:	oTable	The table.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::DisplayTable(const CTable& oTable)
{
	// Turn off painting.
	m_lvGrid.Redraw(false);

	// Clear grid.
	m_lvGrid.DeleteAllItems();
	m_lvGrid.DeleteAllColumns();

	// Tell list view how many rows we're adding.
	m_lvGrid.Reserve(oTable.RowCount());

	// Allocate the columns array.
	CTableGrid::Column* pColumns = new CTableGrid::Column[oTable.ColumnCount()];

	// Setup the columns.
	for (int i = 0; i < oTable.ColumnCount(); i++)
	{
		const CColumn& oColumn = oTable.Column(i);

		pColumns[i].m_pszName   = oColumn.Name();
		pColumns[i].m_nWidth    = m_lvGrid.StringWidth(oColumn.DisplayWidth()) + 15;
		pColumns[i].m_nAlign    = LVCFMT_LEFT;
		pColumns[i].m_nField    = i;
		pColumns[i].m_pszFormat = NULL;
	}

	// Load the grid.
	m_lvGrid.Columns(oTable.ColumnCount(), pColumns);
	m_lvGrid.AddRows(oTable, false, 0);

	// Turn painting back on.
	m_lvGrid.Redraw(true);
	m_lvGrid.Invalidate();
}
