/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CONNECTDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CConnectDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CONNECTDLG_HPP
#define CONNECTDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** This dialog is used to specify the database connection.
**
*******************************************************************************
*/

class CConnectDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CConnectDlg();
	
	//
	// Members.
	//
	size_t	m_nConnection;
	CString	m_strLogin;
	CString	m_strPassword;

protected:
	//
	// Members.
	//
	CComboBox	m_cbDatabase;
	CEditBox	m_ebLogin;
	CEditBox	m_ebPassword;
	
	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	void OnManage();
	void OnSelect();

	//
	// Internal methods.
	//
	void LoadDatabaseList();
	void UpdateUI();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //CONNECTDLG_HPP
