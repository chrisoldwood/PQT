/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TABLEGRID.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CTableGrid class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "TableGrid.hpp"
#include <MDBL/Table.hpp>
#include <MDBL/ResultSet.hpp>

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

CTableGrid::CTableGrid(IRowHandler* pRowHandler)
	: m_pRowHandler(pRowHandler)
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CTableGrid::~CTableGrid()
{
	m_oColumns.DeleteAll();
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Initialises the grid.
**
** Parameters:	rcClient	The client rectangle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTableGrid::OnCreate(const CRect& /*rcClient*/)
{
	// Set the grid style.
	GridLines(true);
	FullRowSelect(true);

	// Setup the grid columns, if supplied.
	if (m_oColumns.Size())
	{
		for (int c = 0; c < m_oColumns.Size(); c++)
		{
			Column& oColumn = m_oColumns[c];

			InsertColumn(c, oColumn.m_strName, oColumn.m_nWidth, oColumn.m_nAlign);
		}
	}
}

/******************************************************************************
** Method:		Columns()
**
** Description:	Sets the columns for the grid.
**
** Parameters:	nColumns	The number of columns.
**				pColumns	The column definitions.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTableGrid::Columns(int nColumns, Column* pColumns)
{
	ASSERT(nColumns >= 0);
	ASSERT(pColumns != NULL);

	// Delete exsiting columns.
	m_oColumns.DeleteAll();

	// Window created?
	if (Handle() != NULL)
	{
		DeleteAllColumns();

		// Add row number colun.
		InsertColumn(0, "#", 50);

		// Setup the grid columns.
		for (int c = 0; c < nColumns; c++)
		{
			Column* pColumn = new Column(pColumns[c]);

			m_oColumns.Add(*pColumn);
			InsertColumn(c+1, pColumn->m_strName, pColumn->m_nWidth, pColumn->m_nAlign);
		}
	}
}

/******************************************************************************
** Method:		AddRows()
**
** Description:	Adds all the rows from a table.
**
** Parameters:	oTable		The table whose rows to add.
**				bReSort		Sort after adding rows.
**				nSel		The row to make the selection.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTableGrid::AddRows(const CTable& oTable, bool bReSort, int nSel)
{
	// Add the rows.
	for (int i = 0; i < oTable.RowCount(); i++)
		AddRow(oTable[i], false);

	// Needs sorting?
	if (bReSort)
		Sort();

	// Set selection.
	Select(nSel);
}

/******************************************************************************
** Method:		AddRows()
**
** Description:	Add all the rows from a result set.
**
** Parameters:	oRow		The result set whose rows to add.
**				bReSort		Sort after adding rows.
**				nSel		The row to make the selection.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTableGrid::AddRows(const CResultSet& oRS, bool bReSort, int nSel)
{
	// Add the rows.
	for (int i = 0; i < oRS.Count(); i++)
		AddRow(oRS[i], false);

	// Needs sorting?
	if (bReSort)
		Sort();

	// Set selection.
	Select(nSel);
}

/******************************************************************************
** Method:		AddRow()
**
** Description:	Add a row to the grid.
**
** Parameters:	oRow		The row to add.
**				bReSort		Re-sort the grid after?
**				bSelect		Select the row after adding?
**
** Returns:		The grid row index.
**
*******************************************************************************
*/

int CTableGrid::AddRow(const CRow& oRow, bool bReSort, bool bSelect)
{
	char szRow[100];

	int nRow = AppendItem("");

	// Set the row number.
	ItemText(nRow, 0, _itoa(nRow+1, szRow, 10));
	ItemPtr (nRow, &oRow);

	return UpdateRow(nRow, bReSort, bSelect);
}

/******************************************************************************
** Method:		UpdateRow()
**
** Description:	Update a row in the grid.
**
** Parameters:	nRow		The grid row to update.
**				bReSort		Re-sort the grid after?
**				bSelect		Reselect the row after updating?
**
** Returns:		The row index, if resorted.
**
*******************************************************************************
*/

int CTableGrid::UpdateRow(int nRow, bool bReSort, bool bSelect)
{
	CRow& oRow = Row(nRow);

	// For all grid columns.
	for (int i = 0; i < m_oColumns.Size(); i++)
		ItemText(nRow, i+1, FieldValue(i, oRow));

	// Re-sort the row?
	if (bReSort)
	{
		Sort();
		nRow = FindItem(&oRow);
	}

	// Reselect it?
	if (bSelect)
		Select(nRow);

	return nRow;
}

/******************************************************************************
** Method:		DeleteRow()
**
** Description:	Delete a row from the grid.
**
** Parameters:	nRow	The grid row to delete.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTableGrid::DeleteRow(int nRow)
{
	// Is selected row?
	bool bSelected = (Selection() == nRow);

	DeleteItem(nRow);

	// Update selection?
	if (bSelected)
	{
		if (nRow >= RowCount())
			nRow--;

		Select(nRow);
	}
}

/******************************************************************************
** Method:		Sort()
**
** Description:	Sorts the rows in the grid.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTableGrid::Sort()
{
	CListView::Sort(TableGridCmpRows, (LPARAM)this);
}

/******************************************************************************
** Method:		TableGridCmpRows()
**
** Description:	Callback function to compare two rows.
**
** Parameters:	lParam1			Row 1.
**				lParam2			Row 2.
**				lParamSort		The CTableGrid object.
**
** Returns:		As strcmp.
**
*******************************************************************************
*/

int CALLBACK TableGridCmpRows(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CTableGrid*	pGrid = (CTableGrid*) lParamSort;
	CRow*		pRow1 = (CRow*) lParam1;
	CRow*		pRow2 = (CRow*) lParam2;

	ASSERT(pGrid != NULL);
	ASSERT(pRow1 != NULL);
	ASSERT(pRow2 != NULL);

	return pGrid->CompareRows(*pRow1, *pRow2);
}

/******************************************************************************
** Method:		CompareRows()
**
** Description:	Template method to compare two rows.
**
** Parameters:	oRow1	Row 1.
**				oRow2	Row 2.
**
** Returns:		As strcmp.
**
*******************************************************************************
*/

int CTableGrid::CompareRows(CRow& oRow1, CRow& oRow2)
{
	// Use external handler?
	if (m_pRowHandler != NULL)
		return m_pRowHandler->CompareRows(oRow1, oRow2);

	// Sort by column 0's field.
	int nSortCol = m_oColumns[0].m_nField;

	return oRow1[nSortCol].Compare(oRow2[nSortCol]);
}

/******************************************************************************
** Method:		FieldValue()
**
** Description:	Template method to get the value for a field.
**
** Parameters:	nColumn	The column index.
**				oRow	The row.
**
** Returns:		The string value.
**
*******************************************************************************
*/

CString CTableGrid::FieldValue(int nColumn, const CRow& oRow)
{
	int nField = m_oColumns[nColumn].m_nField;

	// Use external handler?
	if (m_pRowHandler != NULL)
		return m_pRowHandler->RowFieldValue(oRow, nField);

	// Use default formatter.
	if (oRow[nField] == null)
		return m_strNull;

	return oRow[nField].Format();
}
