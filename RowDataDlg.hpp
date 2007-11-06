/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ROWDATADLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CRowDataDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ROWDATADLG_HPP
#define ROWDATADLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include <MDBL/FwdDecls.hpp>

/******************************************************************************
** 
** This is the dialog used to display a single rows' data.
**
*******************************************************************************
*/

class CRowDataDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CRowDataDlg(CRow& oRow);
	
protected:
	//
	// Members.
	//
	CRow&		m_oRow;
	CTable&		m_oTable;
	CListView 	m_lvGrid;
	uint		m_nSortColumn;
	int			m_nSortOrder;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	LRESULT OnRightClick(NMHDR& rMsgHdr);
	LRESULT OnClickColumn(NMHDR& rMsgHdr);

	// Listview sort function.
	static int CALLBACK CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //ROWDATADLG_HPP
