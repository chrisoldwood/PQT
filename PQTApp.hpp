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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/App.hpp>
#include "AppWnd.hpp"
#include "AppCmds.hpp"
#include <MDBL/ODBCSource.hpp>
#include <MDBL/MDB.hpp>
#include <WCL/MRUList.hpp>
#include <WCL/IniFile.hpp>
#include "Scripts.hpp"
#include "ConConfig.hpp"

// Forward declarations.
class CQuery;

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
	CPath		m_strQueryFile;
	bool		m_bModified;
	CODBCSource	m_oConnection;
	CMDB		m_oMDB;
	CQuery*		m_pQuery;
	CString		m_strFindVal;
	int			m_nLastFindRow;
	int			m_nLastFindCol;
	CScripts	m_oScripts;
	CMRUList	m_oMRUList;

	CConConfigs	m_apConConfigs;
	CConConfig* m_pCurrConn;
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
