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
#include <map>
#include "Query.hpp"

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

	typedef std::map<CString, CString> ParamValueMap;


	//
	// Members
	//
	CAppWnd		m_AppWnd;
	CAppCmds	m_AppCmds;

	size_t		m_nDefConnection;
	CPath		m_strQueryFile;
	bool		m_bModified;
	CODBCSource	m_oConnection;
	CMDB		m_oMDB;
	CQueryPtr	m_pQuery;
	CString		m_strFindVal;
	size_t		m_nLastFindRow;
	CScripts	m_oScripts;
	CMRUList	m_oMRUList;
	ParamValueMap	m_mapPrevValues;

	CConConfigs		m_apConConfigs;
	CConConfigPtr	m_pCurrConn;
	CRect		m_rcLastPos;
	size_t		m_nMinWidth;
	size_t		m_nMaxWidth;
	CString		m_strNull;
	bool		m_bGridlines;

protected:
	//
	// Startup and Shutdown template methods.
	//
	virtual	bool OnOpen();
	virtual	bool OnClose();

	//
	// Internal methods.
	//
	void loadConfig();
	void saveConfig();
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
