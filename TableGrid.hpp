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
#include <vector>

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
		size_t	m_nWidth;		// The width in pixels.
		uint	m_nAlign;		// The alignment of the text.
		size_t	m_nField;		// The row field.
	};

	//
	// Constructors/Destructor.
	//
	CTableGrid(IRowHandler* pRowHandler = nullptr);
	~CTableGrid();

	void  Columns(size_t nColumns, Column* pColumns);

	size_t RowCount();
	CRow& Row(size_t nRow);

	void  Clear    ();
	void  AddRows  (const CTable& oTable,  bool bReSort = true, int nSel = 0);
	void  AddRows  (const CResultSet& oRS, bool bReSort = true, int nSel = 0);
	size_t AddRow   (const CRow& oRow,      bool bReSort = true, bool bSelect = true);
	size_t UpdateRow(size_t nRow,           bool bReSort = true, bool bSelect = true);
	void  DeleteRow(size_t nRow);

	void  Sort();

	void  NullValue(const CString& strNull);

protected:
	//! The column collection type.
	typedef std::vector<Column> ColumnList;

	//
	// Members.
	//
	ColumnList			m_oColumns;
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
	virtual CString FieldValue(size_t nColumn, const CRow& oRow);
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
	virtual CString RowFieldValue(const CRow& oRow, size_t nField) = 0;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CTableGrid::RowCount()
{
	return ItemCount();
}

inline CRow& CTableGrid::Row(size_t nRow)
{
	ASSERT((nRow >= 0) && (nRow < RowCount()));

	return *(static_cast<CRow*>(ItemPtr(nRow)));
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
