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
	, m_oTable(m_oRow.Table())
	, m_nSortColumn(0)
	, m_nSortOrder(1)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GRID, &m_lvGrid)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_GRID, LEFT_EDGE,  TOP_EDGE,    RIGHT_EDGE, BOTTOM_EDGE)
		CTRLGRAV(IDCANCEL, RIGHT_EDGE, BOTTOM_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE

	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_GRID, NM_RCLICK,       OnRightClick)
		NFY_CTRLMSG(IDC_GRID, LVN_COLUMNCLICK, OnClickColumn)
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

void CRowDataDlg::OnInitDialog()
{
	// Set grid style.
	m_lvGrid.FullRowSelect(true);
	m_lvGrid.GridLines(true);
	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));

	// Set grid columns.
	m_lvGrid.InsertColumn(0, "#",       50);
	m_lvGrid.InsertColumn(1, "Column", 150);
	m_lvGrid.InsertColumn(2, "Value",  225);

	// Load column names and data.
	for (int i = 0; i < m_oTable.ColumnCount(); i++)
	{
		char szRow[10];

		// LPARAM is column index.
		int nRow = m_lvGrid.InsertItem(i, itoa(i, szRow, 10), i, -1);

		// Set the column name.
		m_lvGrid.ItemText(nRow, 1, m_oTable.Column(i).Name());

		// Set the column value.
		if (m_oRow[i] == null)
			m_lvGrid.ItemText(nRow, 2, App.m_strNull);
		else
			m_lvGrid.ItemText(nRow, 2, m_oRow[i].Format());
	}

	// Sort listview.
	m_lvGrid.Sort(CompareFunction, (LPARAM)this);
}

/******************************************************************************
** Method:		OnRightClick()
**
** Description:	Right click on the grid, show context menu.
**
** Parameters:	rMsgHdr		The WM_NOTIFY msg header.
**
** Returns:		0.
**
*******************************************************************************
*/

LRESULT CRowDataDlg::OnRightClick(NMHDR& rMsgHdr)
{
	NMITEMACTIVATE& oMsgHdr = reinterpret_cast<NMITEMACTIVATE&>(rMsgHdr);

	// Only show menu, if a selection.
	if (m_lvGrid.IsSelection())
	{
		// Get the selected row column.
		uint nRowCol = m_lvGrid.ItemData(m_lvGrid.Selection());

		CPopupMenu oMenu(IDR_ROWDATAMENU);

		// Calculate popup menu position.
		CPoint ptMenu = oMsgHdr.ptAction;

		::ClientToScreen(m_lvGrid.Handle(), &ptMenu);

		// Show contect menu.
		uint nCmdID = oMenu.TrackMenu(m_lvGrid, ptMenu);

		// Handle command.
		if (nCmdID == ID_ROWDATA_COPYNAME)
		{
			CClipboard::CopyText(m_hWnd, m_oTable.Column(nRowCol).Name());
		}
		else if (nCmdID == ID_ROWDATA_COPYVALUE)
		{
			CClipboard::CopyText(m_hWnd, m_oRow[nRowCol].Format());
		}
	}

	return 0;
}

/******************************************************************************
** Method:		OnClickColumn()
**
** Description:	Grid column clicked, change sort column or reverse sort order.
**
** Parameters:	rMsgHdr		The WM_NOTIFY msg header.
**
** Returns:		0.
**
*******************************************************************************
*/

LRESULT CRowDataDlg::OnClickColumn(NMHDR& rMsgHdr)
{
	NMLISTVIEW& oMsgHdr = reinterpret_cast<NMLISTVIEW&>(rMsgHdr);

	// Get the column clicked.
	uint nColumn = oMsgHdr.iSubItem;

	// Reverse sort order?
	if (nColumn == m_nSortColumn)
	{
		m_nSortOrder = -m_nSortOrder;
	}
	// Change sort column.
	else
	{
		// Default to ascending sort.
		m_nSortColumn = nColumn;
		m_nSortOrder  = 1;
	}

	// Sort listview.
	m_lvGrid.Sort(CompareFunction, (LPARAM)this);

	return 0;
}

/******************************************************************************
** Method:		CompareFunction()
**
** Description:	Listview compare function used when sorting.
**
** Parameters:	See message LVM_SORTITEMS.
**
** Returns:		As strcmp.
**
*******************************************************************************
*/

int CALLBACK CRowDataDlg::CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CRowDataDlg* pDialog  = reinterpret_cast<CRowDataDlg*>(lParamSort);
	uint         nRowCol1 = lParam1;
	uint         nRowCol2 = lParam2;
	int          nResult  = 0;

	// Sort by column index?
	if (pDialog->m_nSortColumn == 0)
	{
		nResult = nRowCol1 - nRowCol2;
	}
	// Sort by column name?
	else if (pDialog->m_nSortColumn == 1)
	{
		nResult = strcmp(pDialog->m_oTable.Column(nRowCol1).Name(), pDialog->m_oTable.Column(nRowCol2).Name());
	}
	// Sort by column value?
	else if (pDialog->m_nSortColumn == 2)
	{
		CString strValue1 = App.m_strNull;
		CString strValue2 = App.m_strNull;

		if (pDialog->m_oRow[nRowCol1] != null)
			strValue1 = pDialog->m_oRow[nRowCol1].Format();

		if (pDialog->m_oRow[nRowCol2] != null)
			strValue2 = pDialog->m_oRow[nRowCol2].Format();

		nResult = strcmp(strValue1, strValue2);
	}

	return (nResult * pDialog->m_nSortOrder);
}
