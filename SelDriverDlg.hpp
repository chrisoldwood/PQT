/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SELDRIVERDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSelDriverDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SELDRIVERDLG_HPP
#define SELDRIVERDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** This dialog is used to select a driver.
**
*******************************************************************************
*/

class CSelDriverDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CSelDriverDlg();
	
	//
	// Members.
	//
	CString	m_strDriver;

protected:
	//
	// Members.
	//
	CListBox m_lbDrivers;
	
	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
			void OnDblClickDrivers();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //SELDRIVERDLG_HPP
