/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppCmds class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPCMDS_HPP
#define APPCMDS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CmdCtrl.hpp>

// Forward declarations.
class CAppWnd;
class CPath;

/******************************************************************************
** 
** Application command controller.
**
*******************************************************************************
*/

class CAppCmds : public CCmdControl
{
public:
	//
	// Constructors/Destructor.
	//
	CAppCmds(CAppWnd& appWnd);
	~CAppCmds();

	//
	// Commands.
	//

	// Database menu.
	void OnDBConnect();
	void OnDBDisconnect();
	void OnDBManage();
	bool ManageDatabases();
	void OnDBConnectMRU(int nCmdID);
	void OnDBExit();

	// Query menu.
	void OnQueryNew();
	void OnQueryOpen();
	void OnQueryOpen(const CPath& strPath);
	void OnQuerySave();
	void OnQuerySaveAs();
	void OnQueryPrint();

	// Execute menu.
	void OnExecCurrent();
	void OnExecFile();
	void OnExecFile(const CPath& strPath);
	void OnExecScript(int nCmdID);

	// Results menu.
	void OnResultsFind();
	void OnResultsFindNext();
	void OnResultsSaveAs();
	void OnResultsPrint();

	// Tools menu.
	void OnToolsOptions();

	// Window menu.
	void OnWindowQuery();
	void OnWindowResults();

	// Help menu.
	void OnHelpContents();
	void OnHelpAbout();

	//
	// UI handlers.
	//
	void OnUIDBDisconnect();
	void OnUIExecCurrent();
	void OnUIExecFile();
	void OnUIResultsFind();
	void OnUIResultsFindNext();
	void OnUIResultsSaveAs();
	void OnUIResultsPrint();

	//
	// Command property methods.
	//
	virtual CString CmdHintStr(uint iCmdID) const;

	//
	// Constants.
	//
	static const tchar* QUERY_MODIFIED_MSG;

protected:
	//
	// Members.
	//

	// Position of scripts menu.
	enum { SCRIPTS_MENU_POS = 2 };

	// Command ranges.
	enum
	{
		ID_FIRST_SCRIPT_CMD = 305,
		ID_LAST_SCRIPT_CMD  = 399,
	};

	//
	// Internal methods.
	//
	void Connect(size_t nConnection, const CString& strLogin, const CString& strPassword);
	bool LoadQuery(const CPath& strPath);
	bool SaveQuery(const CPath& strPath);
	void UpdateScriptsMenu();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPCMDS_HPP
