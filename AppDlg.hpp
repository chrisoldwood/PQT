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
	virtual void    OnInitDialog();
	        LRESULT OnGridDblClick(NMHDR&);
	        LRESULT OnGridKeyDown(NMHDR&);

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
