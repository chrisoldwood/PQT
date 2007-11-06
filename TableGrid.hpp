/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TABLEGRID.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CTableGrid class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TABLEGRID_HPP
#define TABLEGRID_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/ListView.hpp>
#include <MDBL/FwdDecls.hpp>
#include <Legacy/TArray.hpp>

// Forward declarations.
class IRowHandler;

/******************************************************************************
** 
** This ListView derived class is used for displaying MDB tables.
**
*******************************************************************************
*/

class CTableGrid : public CListView
{
public:
	/////////////////////////////////////////////////////
	// 
	// The structure used to hold the column definitions.
	//
	/////////////////////////////////////////////////////

	struct Column
	{
		CString	m_strName;		// The name.
		int		m_nWidth;		// The width in pixels.
		int		m_nAlign;		// The alignment of the text.
		int		m_nField;		// The row field.
	};

	//
	// Constructors/Destructor.
	//
	CTableGrid(IRowHandler* pRowHandler = NULL);
	~CTableGrid();

	void  Columns(int nColumns, Column* pColumns);

	int   RowCount();
	CRow& Row(int nRow);

	void  Clear    ();
	void  AddRows  (const CTable& oTable,  bool bReSort = true, int nSel = 0);
	void  AddRows  (const CResultSet& oRS, bool bReSort = true, int nSel = 0);
	int   AddRow   (const CRow& oRow,      bool bReSort = true, bool bSelect = true);
	int   UpdateRow(int nRow,        bool bReSort = true, bool bSelect = true);
	void  DeleteRow(int nRow);

	void  Sort();

	void  NullValue(const CString& strNull);

protected:
	//
	// Members.
	//
	TRefArray<Column>	m_oColumns;
	IRowHandler*		m_pRowHandler;
	CString				m_strNull;

	//
	// Message processors.
	//
	virtual void OnCreate(const CRect& rcClient);

	//
	// Internal methods.
	//
	friend int  CALLBACK TableGridCmpRows(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	virtual int CompareRows(CRow& oRow1, CRow& oRow2);
	virtual CString FieldValue(int nColumn, const CRow& oRow);
};

/******************************************************************************
** 
** This interface class is used to handle row events from the CTableGrid class.
**
*******************************************************************************
*/

class IRowHandler
{
public:
	virtual int     CompareRows(const CRow& oRow1, const CRow& oRow2) = 0;
	virtual CString RowFieldValue(const CRow& oRow, int nField) = 0;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CTableGrid::RowCount()
{
	return ItemCount();
}

inline CRow& CTableGrid::Row(int nRow)
{
	ASSERT((nRow >= 0) && (nRow < RowCount()));

	return *((CRow*)ItemPtr(nRow));
}

inline void CTableGrid::Clear()
{
	DeleteAllItems();
	DeleteAllColumns();
}

inline void CTableGrid::NullValue(const CString& strNull)
{
	m_strNull = strNull;
}

#endif //TABLEGRID_HPP
