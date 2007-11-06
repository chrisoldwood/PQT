/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FINDDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CFindDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FINDDLG_HPP
#define FINDDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** This is the dialog used to enter the value to search for.
**
*******************************************************************************
*/

class CFindDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CFindDlg();
	
	//
	// Members.
	//
	CString	m_strValue;

protected:
	//
	// Members.
	//
	CEditBox	m_ebValue;
	
	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //FINDDLG_HPP
