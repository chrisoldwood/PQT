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
	m_tcTabCtrl.CurSel(0);

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
	m_lvGrid.CanRedraw(false);

	// Clear grid.
	m_lvGrid.DeleteAllItems();
	m_lvGrid.DeleteAllColumns();

	// Tell list view how many rows we're adding.
	m_lvGrid.Reserve(oTable.RowCount());

	// Add the row number column.
	m_lvGrid.InsertColumn(0, "#", m_lvGrid.StringWidth(3) + 15);

	// Setup the columns.
	for (int i = 0; i < oTable.ColumnCount(); i++)
	{
		const CColumn& oColumn = oTable.Column(i);

		const char* pszName = oColumn.Name();
		int         nWidth  = m_lvGrid.StringWidth(strlen(pszName)) + 15;

		m_lvGrid.InsertColumn(i+1, pszName, nWidth);
	}

	int nCharWidth = m_lvGrid.StringWidth(1);

	// Create column width buffers.
	int* aiWidths = (int*) calloc(sizeof(int), oTable.ColumnCount());

	// Add the rows.
	for (int r = 0; r < oTable.RowCount(); r++)
	{
		const CRow& oRow = oTable[r];

		// Add the row to the grid.
		char szRowNum[20];

		m_lvGrid.AddItem(itoa(r+1, szRowNum, 10));

		// For all columns.
		for (int c = 0; c < oTable.ColumnCount(); c++)
		{
			const CField& oField = oRow[c];

			// Nothing to display?
			if (oField == null)
				continue;

			char		szValue[100];
			CString		strValue;
			const char* pszField = NULL;

			// Convert values.
			switch (oTable.Column(c).ColType())
			{
				// 32-bit integer.
				case MDCT_INT:
				case MDCT_IDENTITY:
				{
					sprintf(szValue, "%d", oField.GetInt());
					pszField = szValue;
				}
				break;

				// 64-bit floating-point.
				case MDCT_DOUBLE:
				{
					sprintf(szValue, "%f", oField.GetDouble());
					pszField = szValue;
				}
				break;

				// Single character.
				case MDCT_CHAR:
				{
					sprintf(szValue, "%c", oField.GetChar());
					pszField = szValue;
				}
				break;

				// Character string.
				case MDCT_FXDSTR:
				case MDCT_VARSTR:
				{
					pszField = oField.GetString();
				}
				break;

				// Boolean (true/false).
				case MDCT_BOOL:
				{
					pszField = (oField.GetBool() == true) ? "Yes" : "No";
				}
				break;

				// Date & Time as time_t.
				case MDCT_DATETIME:
				{
					time_t tTime = oField;
					strftime(szValue, sizeof(szValue), "%d/%m/%Y %H:%M:%S", localtime(&tTime));
					pszField = szValue;
				}
				break;

				// Date & Time as CTimeStamp.
				case MDCT_TIMESTAMP:
				{
					const CTimeStamp& tTime = oField;
					strValue = tTime.ToString();
					pszField = strValue;
				}
				break;
			}

			int nWidth = 15;

			// Show it!
			if (pszField != NULL)
			{
				m_lvGrid.ItemText(r, c+1, pszField);

				if (strlen(pszField) < 30)
					nWidth += nCharWidth * strlen(pszField);
			}

			// Update column width.
			if (nWidth > aiWidths[c])
				aiWidths[c] = nWidth;
		}
	}

	// Set column widths.
	for (i = 0; i < oTable.ColumnCount(); i++)
		ListView_SetColumnWidth(m_lvGrid.Handle(), i+1, aiWidths[i]);

	free(aiWidths);

	// Turn painting back on.
	m_lvGrid.CanRedraw(true);
	m_lvGrid.Invalidate();
}
