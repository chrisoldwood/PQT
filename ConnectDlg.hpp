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
	CString	m_strConnection;

protected:
	//
	// Members.
	//
	CEditBox	m_ebConnection;
	
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

#endif //CONNECTDLG_HPP
