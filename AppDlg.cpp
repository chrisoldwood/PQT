/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppDlg.hpp"
#include "PQTApp.hpp"
#include <stdio.h>
#include "RowDataDlg.hpp"
#include <WCL/Clipboard.hpp>
#include <Core/ArrayPtr.hpp>
#include <algorithm>

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

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_QUERY, EN_CHANGE,   &CAppDlg::OnQueryChanged  )
		NFY_CTRLMSG(IDC_GRID,  NM_DBLCLK,   &CAppDlg::OnGridDblClick  )
		NFY_CTRLMSG(IDC_GRID,  LVN_KEYDOWN, &CAppDlg::OnGridKeyDown   )
		NFY_CTRLMSG(IDC_GRID,  NM_RCLICK,   &CAppDlg::OnGridRightClick)
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

void CAppDlg::OnInitDialog()
{
	// Add the tabs.
	m_tcTabCtrl.AddTab(TXT("Query"),   m_ebQuery);
	m_tcTabCtrl.AddTab(TXT("Results"), m_lvGrid);
	m_tcTabCtrl.CurSel(QUERY_TAB);

	// Use a fixed pitch font for the query/results.
	m_ebQuery.Font(CFont(ANSI_FIXED_FONT));
	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));

	// Set grid style.
	m_lvGrid.FullRowSelect(true);
	m_lvGrid.GridLines(App.m_bGridlines);
	m_lvGrid.NullValue(App.m_strNull);
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
	typedef Core::ArrayPtr<CTableGrid::Column> ColumnArrayPtr;

	// Turn off painting.
	m_lvGrid.Redraw(false);

	// Clear grid.
	m_lvGrid.DeleteAllItems();
	m_lvGrid.DeleteAllColumns();

	// Tell list view how many rows we're adding.
	m_lvGrid.Reserve(oTable.RowCount());

	// Allocate the columns array.
	ColumnArrayPtr pColumns(new CTableGrid::Column[oTable.ColumnCount()]);

	// Setup the columns.
	for (size_t i = 0; i < oTable.ColumnCount(); i++)
	{
		const CColumn& oColumn = oTable.Column(i);

		// Compute col width in chars, clipping to app settings.
		size_t nColWidth = std::min<size_t>(std::max<size_t>(oColumn.DisplayWidth(), App.m_nMinWidth), App.m_nMaxWidth);

		pColumns[i].m_strName = oColumn.Name();
		pColumns[i].m_nWidth  = m_lvGrid.StringWidth(nColWidth) + 15;
		pColumns[i].m_nAlign  = LVCFMT_LEFT;
		pColumns[i].m_nField  = i;
	}

	// Load the grid.
	m_lvGrid.Columns(oTable.ColumnCount(), pColumns.get());
	m_lvGrid.AddRows(oTable, false, 0);

	// Turn painting back on.
	m_lvGrid.Redraw(true);
	m_lvGrid.Invalidate();
}

/******************************************************************************
** Method:		OnQueryChanged()
**
** Description:	Query text changed, update modified flag.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::OnQueryChanged()
{
	// 1st modification?
	if (!App.m_bModified)
	{
		// Update flag and UI.
		App.m_bModified = true;
		App.m_AppWnd.UpdateTitle();
	}
}

/******************************************************************************
** Method:		OnGridDblClick()
**
** Description:	Grid row double-clicked.
**
** Parameters:	rMsgHdr		The message details.
**
** Returns:		0.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridDblClick(NMHDR&)
{
	if (m_lvGrid.IsSelection())
		ShowRowDetails();

	return 0;
}

/******************************************************************************
** Method:		OnGridKeyDown()
**
** Description:	Key pressed on grid.
**
** Parameters:	rMsgHdr		The message details.
**
** Returns:		0.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridKeyDown(NMHDR& oNMHdr)
{
	NMLVKEYDOWN& oMsg = reinterpret_cast<NMLVKEYDOWN&>(oNMHdr);

	// User pressed ENTER key?
	if (m_lvGrid.IsSelection() && (oMsg.wVKey == VK_RETURN))
		ShowRowDetails();

	return 0;
}

/******************************************************************************
** Method:		OnGridRightClick()
**
** Description:	Right click on the grid, show context menu.
**
** Parameters:	rMsgHdr		The WM_NOTIFY msg header.
**
** Returns:		0.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridRightClick(NMHDR& rMsgHdr)
{
	NMITEMACTIVATE& oMsgHdr = reinterpret_cast<NMITEMACTIVATE&>(rMsgHdr);

	// If a row is selected AND on clicked a table column.
	if ( (m_lvGrid.IsSelection()) && (oMsgHdr.iSubItem != 0) )
	{
		// Get the current selection.
		int   nCol = oMsgHdr.iSubItem - 1;
		size_t nRow = m_lvGrid.Selection();
		CRow& oRow = m_lvGrid.Row(nRow);

		CPopupMenu oMenu(IDR_TABLEMENU);

		// Calculate popup menu position.
		CPoint ptMenu = oMsgHdr.ptAction;

		::ClientToScreen(m_lvGrid.Handle(), &ptMenu);

		// Show context menu.
		uint nCmdID = oMenu.TrackMenu(m_lvGrid, ptMenu);

		// Handle command.
		if (nCmdID == ID_TABLE_COPY_VALUE)
		{
			CClipboard::CopyText(m_hWnd, oRow[nCol].Format());
		}

		if (nCmdID == ID_TABLE_COPY_NAME)
		{
			CClipboard::CopyText(m_hWnd, oRow.Table().Column(nCol).Name());
		}
	}

	return 0;
}

/******************************************************************************
** Method:		ShowRowDetails()
**
** Description:	Shows a dialog which displays the row details in full for the
**				selected row.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::ShowRowDetails()
{
	// Is a selection?
	if (m_lvGrid.IsSelection())
	{
		// Get the current selection.
		size_t nRow = m_lvGrid.Selection();
		CRow& oRow = m_lvGrid.Row(nRow);

		CRowDataDlg Dlg(oRow);

		// Show dialog.
		Dlg.RunModal(*this);
	}
}
