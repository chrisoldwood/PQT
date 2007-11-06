/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		QUERYPREFS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CQueryPrefsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef QUERYPREFS_HPP
#define QUERYPREFS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** This is the dialog used to edit the query settings.
**
*******************************************************************************
*/

class CQueryPrefsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CQueryPrefsDlg();
	
	//
	// Members.
	//
	int		m_nMinWidth;
	int		m_nMaxWidth;
	CString	m_strNullVal;

protected:
	//
	// Members.
	//
	CEditBox	m_ebMinWidth;
	CEditBox	m_ebMaxWidth;
	CEditBox	m_ebNullVal;
	
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

#endif //QUERYPREFS_HPP
