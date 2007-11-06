/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SELDSNDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSelDSNDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SELDSNDLG_HPP
#define SELDSNDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** This dialog is used to select a DSN.
**
*******************************************************************************
*/

class CSelDSNDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CSelDSNDlg();
	
	//
	// Members.
	//
	CString	m_strDSN;

protected:
	//
	// Members.
	//
	CListBox m_lbDSNs;
	
	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
			void OnDblClickDSNs();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //SELDSNDLG_HPP
