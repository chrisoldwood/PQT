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
	int		m_nConnection;
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

	void OnAdd();
	void OnEdit();
	void OnSelect();

	//
	// Internal methods.
	//
	void UpdateUI();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //CONNECTDLG_HPP
