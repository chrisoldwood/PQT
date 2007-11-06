/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPDLG_HPP
#define APPDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/MainDlg.hpp>
#include <WCL/TabWndHost.hpp> 
#include <WCL/EditBox.hpp> 
#include "TableGrid.hpp"

/******************************************************************************
** 
** This is the main application dialog.
**
*******************************************************************************
*/

class CAppDlg : public CMainDlg
{
public:
	//
	// Constructors/Destructor.
	//
	CAppDlg();
	
	//
	// Methods.
	//
	void DisplayTable(const CTable& oTable);

	// Tabs
	enum Tab
	{
		QUERY_TAB   = 0,
		RESULTS_TAB = 1,
	};

	//
	// Members.
	//
	CTabWndHost	m_tcTabCtrl;
	CEditBox	m_ebQuery;
	CTableGrid	m_lvGrid;

protected:
	//
	// Message processors.
	//
	virtual void OnInitDialog();

	void    OnQueryChanged();
	LRESULT OnGridDblClick(NMHDR&);
	LRESULT OnGridKeyDown(NMHDR& oNMHdr);
	LRESULT OnGridRightClick(NMHDR& rMsgHdr);

	//
	// Internal methods.
	//
	void ShowRowDetails();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPDLG_HPP
