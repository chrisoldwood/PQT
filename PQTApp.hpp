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

// Template shorthands.
typedef TRefArray<CConConfig> TConConfigs;

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

	int			m_nDefConnection;
	CString		m_strConnection;
	CPath		m_strQueryFile;
	CODBCSource	m_oConnection;
	CMDB		m_oMDB;
	CQuery*		m_pQuery;
	CString		m_strFindVal;
	int			m_nLastFindRow;
	int			m_nLastFindCol;
	CScripts	m_oScripts;

	TConConfigs	m_aConConfigs;
	CRect		m_rcLastPos;
	int			m_nMinWidth;
	int			m_nMaxWidth;
	CString		m_strNull;

	//
	// Constants.
	//
	static const char* VERSION;

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
