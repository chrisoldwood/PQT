/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PQTAPP.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPQTApp class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PQTAPP_HPP
#define PQTAPP_HPP

/******************************************************************************
** 
** The application class.
**
*******************************************************************************
*/

class CPQTApp : public CApp
{
public:
	//
	// Constructors/Destructor.
	//
	CPQTApp();
	~CPQTApp();

	//
	// Members
	//
	CAppWnd		m_AppWnd;
	CAppCmds	m_AppCmds;
	CString		m_strConnection;
	CString		m_strDatabase;
	CPath		m_strQueryFile;
	CODBCSource	m_oConnection;
	CMDB		m_oMDB;
	CRect		m_rcLastPos;

protected:
	//
	// Startup and Shutdown template methods.
	//
	virtual	bool OnOpen();
	virtual	bool OnClose();

	//
	// Preferences.
	//
	CIniFile	m_oIniFile;

	//
	// Internal methods.
	//
	void LoadDefaults();
	void SaveDefaults();
};

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

//The application object.
extern CPQTApp App;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/


#endif //PQTAPP_HPP
