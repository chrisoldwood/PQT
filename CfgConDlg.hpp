/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CFGCONDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CCfgConDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CFGCONDLG_HPP
#define CFGCONDLG_HPP

/******************************************************************************
** 
** This dialog is used to configure a database connection.
**
*******************************************************************************
*/

class CCfgConDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CCfgConDlg();
	
	//
	// Members.
	//
	CConConfig	m_oConfig;

protected:
	//
	// Members.
	//
	CEditBox	m_ebName;
	CEditBox	m_ebDSN;
	CEditBox	m_ebDriver;
	CEditBox	m_ebServer;
	CEditBox	m_ebDatabase;
	CEditBox	m_ebFile;
	CEditBox	m_ebLogin;
	CEditBox	m_ebSQLDir;
	
	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	void OnDSNs();
	void OnDrivers();
	void OnComputers();
	void OnFiles();
	void OnSQLDirs();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //CFGCONDLG_HPP
