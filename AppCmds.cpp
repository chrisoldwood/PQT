/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppCmds class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppCmds.hpp"
#include "PQTApp.hpp"
#include "AboutDlg.hpp"
#include "ConnectDlg.hpp"
#include "QueryPrefsDlg.hpp"
#include "FindDlg.hpp"
#include "ParamsDlg.hpp"
#include <Legacy/FileFinder.hpp>
#include <WCL/Printer.hpp>
#include <WCL/PrinterDC.hpp>
#include <WCL/BusyCursor.hpp>
#include "Query.hpp"
#include <MDBL/SQLException.hpp>
#include <WCL/File.hpp>
#include <WCL/FileException.hpp>
#include <Core/AnsiWide.hpp>

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static tchar szSQLExts[] = {	TXT("SQL Scripts (*.sql)\0*.sql\0")
								TXT("All Files (*.*)\0*.*\0")
								TXT("\0\0")							};

static tchar szSQLDefExt[] = { TXT("sql") };

static tchar szTXTExts[] = {	TXT("Text Files (*.txt)\0*.txt\0")
								TXT("All Files (*.*)\0*.*\0")
								TXT("\0\0")							};

static tchar szTXTDefExt[] = { TXT("txt") };


/******************************************************************************
**
** Class members.
**
*******************************************************************************
*/

const tchar* CAppCmds::QUERY_MODIFIED_MSG = TXT("The current query has been modified.\n\nDo you want to save it first?");

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppCmds::CAppCmds()
{
	// Define the command table.
	DEFINE_CMD_TABLE
		// Database menu.
		CMD_ENTRY(ID_DB_CONNECT,			&CAppCmds::OnDBConnect,			NULL,							0 )
		CMD_ENTRY(ID_DB_DISCONNECT,			&CAppCmds::OnDBDisconnect,		&CAppCmds::OnUIDBDisconnect,	1 )
		CMD_RANGE(ID_DB_MRU_1,
				  ID_DB_MRU_5,				&CAppCmds::OnDBConnectMRU,		NULL,							-1) 
		CMD_ENTRY(ID_DB_EXIT,				&CAppCmds::OnDBExit,			NULL,							-1)
		// Query menu.
		CMD_ENTRY(ID_QUERY_NEW,				&CAppCmds::OnQueryNew,			NULL,							2 )
		CMD_ENTRY(ID_QUERY_OPEN,			&CAppCmds::OnQueryOpen,			NULL,							3 )
		CMD_ENTRY(ID_QUERY_SAVE,			&CAppCmds::OnQuerySave,			NULL,							4 )
		CMD_ENTRY(ID_QUERY_SAVEAS,			&CAppCmds::OnQuerySaveAs,		NULL,							4 )
		CMD_ENTRY(ID_QUERY_PRINT,			&CAppCmds::OnQueryPrint,		NULL,							4 )
		CMD_ENTRY(ID_QUERY_PREFS,			&CAppCmds::OnQueryPrefs,		NULL,							-1)
		// Execute menu.
		CMD_ENTRY(ID_EXEC_CURRENT,			&CAppCmds::OnExecCurrent,		&CAppCmds::OnUIExecCurrent,		5 )
		CMD_ENTRY(ID_EXEC_FILE,				&CAppCmds::OnExecFile,			&CAppCmds::OnUIExecFile,		6 )
		CMD_RANGE(ID_FIRST_SCRIPT_CMD,
				  ID_LAST_SCRIPT_CMD,		&CAppCmds::OnExecScript,		NULL,							-1)
		// Results menu.
		CMD_ENTRY(ID_RESULTS_FIND,			&CAppCmds::OnResultsFind,		&CAppCmds::OnUIResultsFind,		-1)
		CMD_ENTRY(ID_RESULTS_FINDNEXT,		&CAppCmds::OnResultsFindNext,	&CAppCmds::OnUIResultsFindNext,	-1)
		CMD_ENTRY(ID_RESULTS_SAVEAS,		&CAppCmds::OnResultsSaveAs,		&CAppCmds::OnUIResultsSaveAs,	-1)
		CMD_ENTRY(ID_RESULTS_PRINT,			&CAppCmds::OnResultsPrint,		&CAppCmds::OnUIResultsPrint,	-1)
		// Window menu.
		CMD_ENTRY(ID_WINDOW_QUERY,			&CAppCmds::OnWindowQuery,		NULL,							-1)
		CMD_ENTRY(ID_WINDOW_RESULTS,		&CAppCmds::OnWindowResults,		NULL,							-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_ABOUT,			&CAppCmds::OnHelpAbout,			NULL,							10)
	END_CMD_TABLE
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppCmds::~CAppCmds()
{
}

/******************************************************************************
** Method:		OnDBConnect()
**
** Description:	Prompt the user for a connection string and open the database
**				connection.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnDBConnect()
{
	CConnectDlg Dlg;

	Dlg.m_nConnection = App.m_nDefConnection;

	// Prompt the user.
	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
		Connect(Dlg.m_nConnection, Dlg.m_strLogin, Dlg.m_strPassword);
}

/******************************************************************************
** Method:		OnDBDisconnect()
**
** Description:	Close the current database connection.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnDBDisconnect()
{
	// Close connection, if open.
	if (App.m_oConnection.IsOpen())
		App.m_oConnection.Close();

	App.m_pCurrConn = NULL;

	// Reset the scripts menu.
	UpdateScriptsMenu();

	UpdateUI();
	App.m_AppWnd.UpdateTitle();
}

/******************************************************************************
** Method:		OnDBConnectMRU()
**
** Description:	Open a connection from the MRU list.
**
** Parameters:	nCmdID	The ID of the command.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnDBConnectMRU(int nCmdID)
{
	// Get the MRU item.
	CString strConn = App.m_oMRUList[nCmdID - ID_DB_MRU_1];

	uint i = 0;

	// Find the connection index.
	for (; i < App.m_apConConfigs.size(); ++i)
	{
		CConConfig* pConnCfg = App.m_apConConfigs[i];

		if (pConnCfg->m_strName == strConn)
			break;
	}

	// Connection invalid?
	if (i == App.m_apConConfigs.size())
	{
		App.AlertMsg(TXT("The connection is invalid."));
		return;
	}

	CConnectDlg Dlg;

	Dlg.m_nConnection = i;

	// Prompt the user.
	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
		Connect(Dlg.m_nConnection, Dlg.m_strLogin, Dlg.m_strPassword);
}

/******************************************************************************
** Method:		OnDBExit()
**
** Description:	Terminate the app.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnDBExit()
{
	App.m_AppWnd.Close();
}

/******************************************************************************
** Method:		OnQueryNew()
**
** Description:	Delete the contents of the query window.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnQueryNew()
{
	// If query modified, query to save.
	if (App.m_bModified)
	{
		int nResult = App.QueryMsg(QUERY_MODIFIED_MSG);

		if (nResult == IDCANCEL)
			return;

		if (nResult == IDYES)
		{
			OnQuerySave();
			return;
		}
	}

	// Empty the window contents.
	App.m_AppWnd.m_AppDlg.m_ebQuery.Text(TXT(""));

	// Switch to query window.
	App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::QUERY_TAB);

	// Reset query filename.
	App.m_strQueryFile = TXT("");
	App.m_bModified    = false;

	// Update UI.
	App.m_AppWnd.UpdateTitle();
}

/******************************************************************************
** Method:		OnQueryOpen()
**
** Description:	Load a query from a file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnQueryOpen()
{
	// If query modified, query to save.
	if (App.m_bModified)
	{
		int nResult = App.QueryMsg(QUERY_MODIFIED_MSG);

		if (nResult == IDCANCEL)
			return;

		if (nResult == IDYES)
		{
			OnQuerySave();
			return;
		}
	}

	const tchar* pszDefDir = NULL;

	// Use Scripts dir as default, if set.
	if ( (App.m_pCurrConn != NULL) && (App.m_pCurrConn->m_strSQLDir != TXT("")) )
		pszDefDir = App.m_pCurrConn->m_strSQLDir;

	CPath strPath;

	// Select the file to open.
	if (strPath.Select(App.m_AppWnd, CPath::OpenFile, szSQLExts, szSQLDefExt, pszDefDir))
		OnQueryOpen(strPath);
}

/******************************************************************************
** Method:		OnQueryOpen()
**
** Description:	Loads a query from a file.
**
** Parameters:	strtPath	The file to load.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnQueryOpen(const CPath& strPath)
{
	LoadQuery(strPath);

	// Switch to query window.
	App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::QUERY_TAB);
}

/******************************************************************************
** Method:		OnQuerySave()
**
** Description:	Save the current query.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnQuerySave()
{
	// If new query, do a "Save As".
	if (App.m_strQueryFile != TXT(""))
		SaveQuery(App.m_strQueryFile);
	else
		OnQuerySaveAs();
}

/******************************************************************************
** Method:		OnQuerySaveAs()
**
** Description:	Save the current query to a file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnQuerySaveAs()
{
	const tchar* pszDefDir = NULL;

	// Use Scripts dir as default, if set.
	if ( (App.m_pCurrConn != NULL) && (App.m_pCurrConn->m_strSQLDir != TXT("")) )
		pszDefDir = App.m_pCurrConn->m_strSQLDir;

	CPath strPath;

	// Select the filename.
	if (!strPath.Select(App.m_AppWnd, CPath::SaveFile, szSQLExts, szSQLDefExt, pszDefDir))
		return;

	// Warn if replacing.
	if ( (strPath.Exists()) && (App.QueryMsg(TXT("Replace existing file?\n\n%s"), strPath) != IDYES) )
		return;

	// Save it.
	SaveQuery(strPath);

	// Reload scripts menu.
	UpdateScriptsMenu();

	// Save query filename.
	App.m_strQueryFile = strPath;

	// Update UI.
	App.m_AppWnd.UpdateTitle();
}

/******************************************************************************
** Method:		OnQueryPrint()
**
** Description:	Print the query.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnQueryPrint()
{
	// Get the query text.
	CString strQuery = App.m_AppWnd.m_AppDlg.m_ebQuery.Text();

	// Nothing to print?
	if (strQuery == TXT(""))
		return;

	CPrinter oPrinter;

	// Select a printer.
	if (!oPrinter.Select(App.m_AppWnd))
		return;

	CBusyCursor oCursor;

	// Create the printer DC.
	CPrinterDC oDC(oPrinter);

	// Get printer attributes.
	CRect rcRect = oDC.PrintableArea();
	CSize dmFont = oDC.TextExtents(TXT("Wy"));

	// Calculate number of pages.
	int nPageSize  = rcRect.Height() / dmFont.cy;
	int nRptLines  = strQuery.Count(TXT('\r'));
	int nPages     = nRptLines / nPageSize;
	int nLineStart = 0;

	// Doesn't end on a page?
	if ((nRptLines % nPageSize) != 0)
		nPages++;

	// Start printing.
	oDC.Start(TXT("PQT Query ") + (CString)App.m_strQueryFile);

	// For all pages.
	for (int p = 0; p < nPages; ++p)
	{
		oDC.StartPage();

		// Calculate first line rect.
		CRect rcLine  = rcRect;
		rcLine.bottom = rcLine.top + dmFont.cy;

		// Calculate lines on this page.
		int nFirstLine = p * nPageSize;
		int nLastLine  = nFirstLine + nPageSize;

		// Adjust rows, if last page.
		if (nLastLine > nRptLines)
			nLastLine = nRptLines;

		// For all lines on the page.
		for (int l = nFirstLine; l < nLastLine; ++l)
		{
			// Find the end of the report line.
			int nLineEnd = strQuery.Find(TXT('\r'), nLineStart);

			// Extract report line.
			CString strLine = strQuery.Mid(nLineStart, nLineEnd-nLineStart-1);

			// Print it.
			oDC.DrawText(rcLine, strLine, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

			// Update rect to start of next line.
			rcLine.top    = rcLine.bottom;
			rcLine.bottom = rcLine.top + dmFont.cy;

			// Move to the next line.
			nLineStart = nLineEnd + 1;
		}

		oDC.EndPage();
	}

	// Done printing.
	oDC.End();
}

/******************************************************************************
** Method:		OnQueryPrefs()
**
** Description:	Show the query settings dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnQueryPrefs()
{
	CQueryPrefsDlg Dlg;

	// Initialise with current settings.
	Dlg.m_nMinWidth  = App.m_nMinWidth;
	Dlg.m_nMaxWidth  = App.m_nMaxWidth;
	Dlg.m_strNullVal = App.m_strNull;

	// Show the dialog.
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		// Save new settings.
		App.m_nMinWidth = Dlg.m_nMinWidth;
		App.m_nMaxWidth = Dlg.m_nMaxWidth;
		App.m_strNull   = Dlg.m_strNullVal;

		// Update grid.
		App.m_AppWnd.m_AppDlg.m_lvGrid.NullValue(App.m_strNull);
	}
}

/******************************************************************************
** Method:		OnExecCurrent()
**
** Description:	Execute the current query.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnExecCurrent()
{
	CAppDlg&  oAppDlg = App.m_AppWnd.m_AppDlg;
	CEditBox& ebQuery = oAppDlg.m_ebQuery;

	// Query empty?
	if (ebQuery.TextLength() == 0)
		return;

	int nStart, nEnd;

	// Get the text selection, if one.
	ebQuery.Selected(nStart, nEnd);

	// Get the full query text.
	CString strQuery = ebQuery.Text();

	// If a selection, extract selected query text.
	if (nEnd > nStart)
		strQuery = strQuery.Mid(nStart, nEnd-nStart);

	// Any parameters to provide?
	if (strQuery.Find(TXT('$')) != -1)
	{
		CStrArray astrParams, astrValues;

		int nParamStart = 0;
		int nParamEnd   = 0;

		// For all parameters.
		while ((nParamStart = strQuery.Find(TXT('$'), nParamStart)) != -1)
		{
			// Find parameter end marker.
			nParamEnd = strQuery.Find(TXT('$'), nParamStart+1);

			// Parameter end maker not found?
			if (nParamEnd == -1)
			{
				App.AlertMsg(TXT("Parameter marker ($) mismatch."));
				return;
			}

			// Extract parameter name.
			CString strParam = strQuery.Mid(nParamStart+1, nParamEnd-nParamStart-1);

			if (strParam.Empty())
			{
				App.AlertMsg(TXT("Empty parameter ($$) found.")); 
				return;
			}

			// Add to list, if a new parameter.
			if (astrParams.Find(strParam) == -1)
				astrParams.Add(strParam);

			// Skip passed parameter.
			nParamStart = nParamEnd+1;
		}

		CParamsDlg Dlg;

		Dlg.m_pastrParams = &astrParams;
		Dlg.m_pastrValues = &astrValues;

		// Query user for parameter values.
		if (Dlg.RunModal(App.m_AppWnd) != IDOK)
			return;

		ASSERT(astrParams.Size() == astrValues.Size());

		// Copy parameterised query.
		CString strTmpQuery = strQuery;

		nParamStart = 0;
		nParamEnd   = -1;

		// Clear query.
		strQuery = TXT("");

		// For all parameters.
		while ((nParamStart = strTmpQuery.Find(TXT('$'), nParamStart)) != -1)
		{
			// Copy query up to parameter start marker.
			strQuery += strTmpQuery.Mid(nParamEnd+1, nParamStart-nParamEnd-1);

			// Find parameter end marker.
			nParamEnd = strTmpQuery.Find(TXT('$'), nParamStart+1);

			ASSERT(nParamEnd != -1);

			// Extract parameter name.
			CString strParam = strTmpQuery.Mid(nParamStart+1, nParamEnd-nParamStart-1);

			// Find parameter index.
			int nParam = astrParams.Find(strParam);

			ASSERT(nParam != -1);

			// Substitute value.
			strQuery += astrValues[nParam];

			// Skip passed parameter.
			nParamStart = nParamEnd+1;
		}

		// Copy tail end of query.
		strQuery += strTmpQuery.Mid(nParamEnd+1, strTmpQuery.Length()-nParamEnd-1);
	}

	try
	{
		CBusyCursor	oBusy;

		// Remove the last query.
		oAppDlg.m_lvGrid.Clear();
		delete App.m_pQuery;
		App.m_pQuery = NULL;

		// Reset the "find row" values.
		App.m_strFindVal   = TXT("");
		App.m_nLastFindRow = -1;
		App.m_nLastFindCol = -1;

		// Execute the query.
		CTable& oTable = App.m_oMDB.CreateTable(TXT("Query"), App.m_oConnection, strQuery);

		// Save query results.
		App.m_pQuery = new CQuery(strQuery, oTable);

		// Load the table and switch to the results view.
		oAppDlg.DisplayTable(oTable);
		oAppDlg.m_tcTabCtrl.CurSel(CAppDlg::RESULTS_TAB);
	}
	catch(CSQLException& e)
	{
		// Translate % chars before reporting.
		e.m_strError.Replace(TXT('%'), TXT("%%"));

		// Notify user.
		App.AlertMsg(TXT("%s"), e.m_strError);
	}

	UpdateUI();
}

/******************************************************************************
** Method:		OnExecFile()
**
** Description:	Execute a query stored in an external file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnExecFile()
{
	// If query modified, query to save.
	if (App.m_bModified)
	{
		int nResult = App.QueryMsg(QUERY_MODIFIED_MSG);

		if (nResult == IDCANCEL)
			return;

		if (nResult == IDYES)
		{
			OnQuerySave();
			return;
		}
	}

	CPath strPath;

	// Select the file to open.
	if (strPath.Select(App.m_AppWnd, CPath::OpenFile, szSQLExts, szSQLDefExt))
		OnExecFile(strPath);
}

/******************************************************************************
** Method:		OnExecFile()
**
** Description:	Execute a query stored in an external file.
**
** Parameters:	strPath		The file to execute.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnExecFile(const CPath& strPath)
{
	if (LoadQuery(strPath))
		OnExecCurrent();
}

/******************************************************************************
** Method:		OnExecScript()
**
** Description:	Execute a favourite script.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnExecScript(int nCmdID)
{
	// If query modified, query to save.
	if (App.m_bModified)
	{
		int nResult = App.QueryMsg(QUERY_MODIFIED_MSG);

		if (nResult == IDCANCEL)
			return;

		if (nResult == IDYES)
		{
			OnQuerySave();
			return;
		}
	}

	// Locate the script by its command ID.
	CRow* pScript = App.m_oScripts.SelectRow(CScripts::ID, nCmdID);

	ASSERT(pScript != NULL);

	// Extract the path and filename.
	CPath strPath = pScript->Field(CScripts::PATH);
	CPath strFile = pScript->Field(CScripts::NAME);

	OnExecFile(strPath / strFile);
}

/******************************************************************************
** Method:		OnResultsFind()
**
** Description:	Finds the first occurence a value in the results view.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnResultsFind()
{
	CFindDlg Dlg;

	Dlg.m_strValue = App.m_strFindVal;

	// Get the value to search for.
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		// Save the value.
		App.m_strFindVal   = Dlg.m_strValue.ToLower();
		App.m_nLastFindRow = -1;
		App.m_nLastFindCol = -1;

		OnResultsFindNext();
	}
}

/******************************************************************************
** Method:		OnResultsFindNext()
**
** Description:	Finds the next occurence of a value in the results view.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnResultsFindNext()
{
	// Have a value to find.
	if (App.m_strFindVal != TXT(""))
	{
		ASSERT(App.m_pQuery != NULL);

		CTable& oRes = *App.m_pQuery->m_pResults;

		int nStartRow = (App.m_nLastFindCol == -1) ? App.m_nLastFindRow+1 : App.m_nLastFindRow;
		int nStartCol = App.m_nLastFindCol+1;

		// For all rows from the last found row.
		for (size_t r = nStartRow; r < oRes.RowCount(); r++)
		{
			CRow& oRow = oRes[r];

			// For all columns from the last found column.
			for (size_t c = nStartCol; c < oRes.ColumnCount(); c++)
			{
				CString strValue = oRow[c].Format().ToLower();

				if (strValue.Find(App.m_strFindVal) >= 0)
				{
					// Select the row.
					App.m_AppWnd.m_AppDlg.m_lvGrid.Select(r);

					// Save find results.
					App.m_nLastFindRow = r;
					App.m_nLastFindCol = c;

					return;
				}
			}

			// Reset column.
			nStartCol = 0;
		}
	}
}

/******************************************************************************
** Method:		OnResultsSaveAs()
**
** Description:	Save the results to a file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnResultsSaveAs()
{
	ASSERT(App.m_pQuery != NULL);

	// Get the results table.
	CTable& oTable = *App.m_pQuery->m_pResults;

	// Nothing to save?
	if (oTable.RowCount() == 0)
		return;

	CPath strPath;

	// Select the filename.
	if (!strPath.Select(App.m_AppWnd, CPath::SaveFile, szTXTExts, szTXTDefExt, CPath::ApplicationDir()))
		return;

	try
	{
		CFile oFile;

		oFile.Create(strPath);

		// For all rows...
		for (size_t r = 0; r < oTable.RowCount(); ++r)
		{
			CRow&   oRow = oTable[r];
			CString strLine;

			// Generate line of field values.
			for (size_t c = 0; c < oTable.ColumnCount(); ++c)
			{
				if (c != 0)
					strLine += TXT(',');

				CString strField = App.m_strNull;

				if (oRow[c] != null)
					strField = oRow[c].Format();

				strLine += strField;
			}

			strLine += TXT("\r\n");

			// Write the row out.
			oFile.Write(strLine, strLine.Length());
		}
		
		oFile.Close();
	}
	catch(CFileException& e)
	{
		// Notify user.
		App.AlertMsg(TXT("%s"), e.ErrorText());
	}
}

/******************************************************************************
** Method:		OnResultsPrint()
**
** Description:	Print the results.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnResultsPrint()
{
	ASSERT(App.m_pQuery != NULL);

	// Get the results table.
	CTable& oTable = *App.m_pQuery->m_pResults;

	// Nothing to print?
	if (oTable.RowCount() == 0)
		return;

	CPrinter oPrinter;

	// Select a printer.
	if (!oPrinter.Select(App.m_AppWnd))
		return;

	CBusyCursor oCursor;

	// Create the printer DC.
	CPrinterDC oDC(oPrinter);

	// Get printer attributes.
	CRect rcRect = oDC.PrintableArea();
	CSize dmFont = oDC.TextExtents(TXT("Wy"));

	// Calculate number of pages.
	int nPageSize  = rcRect.Height() / dmFont.cy;
	int nPages     = oTable.RowCount() / nPageSize;

	// Doesn't end on a page?
	if ((oTable.RowCount() % nPageSize) != 0)
		nPages++;

	// Start printing.
	oDC.Start(TXT("PQT Results ") + (CString)App.m_strQueryFile);

	// For all pages.
	for (int p = 0; p < nPages; ++p)
	{
		oDC.StartPage();

		// Calculate first line rect.
		CRect rcLine  = rcRect;
		rcLine.bottom = rcLine.top + dmFont.cy;

		// Calculate lines on this page.
		uint nFirstLine = p * nPageSize;
		uint nLastLine  = nFirstLine + nPageSize;

		// Adjust rows, if last page.
		if (nLastLine > oTable.RowCount())
			nLastLine = oTable.RowCount();

		// For all lines on the page.
		for (size_t l = nFirstLine; l < nLastLine; ++l)
		{
			CRow&   oRow = oTable[l];
			CString strLine;

			// Generate line of field values.
			for (size_t c = 0; c < oTable.ColumnCount(); ++c)
			{
				CString strField = App.m_strNull;

				if (oRow[c] != null)
					strField = oRow[c].Format();

				strLine += strField;
				strLine += TXT(' ');
			}

			// Print it.
			oDC.DrawText(rcLine, strLine, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

			// Update rect to start of next line.
			rcLine.top    = rcLine.bottom;
			rcLine.bottom = rcLine.top + dmFont.cy;
		}

		oDC.EndPage();
	}

	// Done printing.
	oDC.End();
}

/******************************************************************************
** Method:		OnWindowQuery()
**
** Description:	Switch to the query window.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnWindowQuery()
{
	App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::QUERY_TAB);
}

/******************************************************************************
** Method:		OnWindowResults()
**
** Description:	Switch to the results window.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnWindowResults()
{
	App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::RESULTS_TAB);
}

/******************************************************************************
** Method:		OnHelpAbout()
**
** Description:	Show the about dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnHelpAbout()
{
	CAboutDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnUI...()
**
** Description:	UI update handlers.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnUIDBDisconnect()
{
	bool bConnected = App.m_oConnection.IsOpen();

	App.m_AppWnd.m_Menu.EnableCmd(ID_DB_DISCONNECT, bConnected);
//	App.m_AppWnd.m_ToolBar.m_btnCloseDB.Enable(bConnected);
}

void CAppCmds::OnUIExecCurrent()
{
	bool bConnected = App.m_oConnection.IsOpen();

	App.m_AppWnd.m_Menu.EnableCmd(ID_EXEC_CURRENT, bConnected);
//	App.m_AppWnd.m_ToolBar.m_btnExecQuery.Enable(bConnected);
}

void CAppCmds::OnUIExecFile()
{
	bool bConnected = App.m_oConnection.IsOpen();

	App.m_AppWnd.m_Menu.EnableCmd(ID_EXEC_FILE, bConnected);
//	App.m_AppWnd.m_ToolBar.m_btnExecFile.Enable(bConnected);
}

void CAppCmds::OnUIResultsFind()
{
	bool bQuery = (App.m_pQuery != NULL);

	App.m_AppWnd.m_Menu.EnableCmd(ID_RESULTS_FIND, bQuery);
}

void CAppCmds::OnUIResultsFindNext()
{
	bool bQuery = (App.m_pQuery != NULL);

	App.m_AppWnd.m_Menu.EnableCmd(ID_RESULTS_FINDNEXT, bQuery);
}

void CAppCmds::OnUIResultsSaveAs()
{
	bool bQuery = (App.m_pQuery != NULL);

	App.m_AppWnd.m_Menu.EnableCmd(ID_RESULTS_SAVEAS, bQuery);
}

void CAppCmds::OnUIResultsPrint()
{
	bool bQuery = (App.m_pQuery != NULL);

	App.m_AppWnd.m_Menu.EnableCmd(ID_RESULTS_PRINT, bQuery);
}

/******************************************************************************
** Method:		CmdHintStr()
**
** Description:	Get the commands' hint text. If the command is one of the
**				scripts return a custom hint, otherwise foward to base class.
**
** Parameters:	iCmdID		The command.
**
** Returns:		The hint text.
**
*******************************************************************************
*/

CString CAppCmds::CmdHintStr(uint iCmdID) const
{
	// Is a favourite script?
	if ( (iCmdID >= ID_FIRST_SCRIPT_CMD) && (iCmdID <= ID_LAST_SCRIPT_CMD) )
	{
		return TXT("Execute this script");
	}

	return CCmdControl::CmdHintStr(iCmdID);
}

/******************************************************************************
** Method:		Connect()
**
** Description:	Open the supplied database connection.
**
** Parameters:	nConnection		The index of the connection.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::Connect(int nConnection, const CString& strLogin, const CString& strPassword)
{
	try
	{
		CBusyCursor oBusy;

		// Close current connection.
		OnDBDisconnect();

		// Get the selected connection.
		App.m_pCurrConn = App.m_apConConfigs[nConnection];

		// Create the connection string.
		CString strConnection = App.m_pCurrConn->ConnectionString(strLogin, strPassword);

		// Open the connection.
		App.m_oConnection.Open(strConnection);

		// Use as default connection next time, 
		App.m_nDefConnection = nConnection;

		// Update the MRU list.
		App.m_oMRUList.Add(App.m_pCurrConn->m_strName);
		App.m_oMRUList.UpdateMenu(*App.m_AppWnd.Menu(), ID_DB_MRU_1);

		// Load the scripts menu.
		UpdateScriptsMenu();

		UpdateUI();
		App.m_AppWnd.UpdateTitle();
	}
	catch(CSQLException& e)
	{
		// Notify user.
		App.AlertMsg(TXT("%s"), e.m_strError);

		// Cleanup connection.
		OnDBDisconnect();
	}
}

/******************************************************************************
** Method:		LoadQuery()
**
** Description:	Load the query into the query window.
**
** Parameters:	strPath		The filename.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppCmds::LoadQuery(const CPath& strPath)
{
	bool bOK = false;

	try
	{
		CString    strQuery;
		TextFormat eFormat;

		// Read the query file.
		CFile::ReadTextFile(strPath, strQuery, eFormat);

		// Load the query into the text editor.
		App.m_AppWnd.m_AppDlg.m_ebQuery.Text(strQuery);

		// Save query filename.
		App.m_strQueryFile = strPath;
		App.m_bModified    = false;

		// Update UI.
		App.m_AppWnd.UpdateTitle();

		bOK = true;
	}
	catch(CFileException& e)
	{
		// Notify user.
		App.AlertMsg(TXT("%s"), e.ErrorText());
	}

	return bOK;
}

/******************************************************************************
** Method:		SaveQuery()
**
** Description:	Save the query to the file specfied.
**
** Parameters:	strPath		The filename.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppCmds::SaveQuery(const CPath& strPath)
{
	bool bOK = false;

	// Get the query text.
	CString strQuery = App.m_AppWnd.m_AppDlg.m_ebQuery.Text();

	try
	{
		// Write the query file.
		CFile::WriteTextFile(strPath, strQuery, ANSI_TEXT);

		// Reset modified flag.
		App.m_bModified = false;

		// Update UI.
		App.m_AppWnd.UpdateTitle();

		bOK = true;
	}
	catch(CFileException& e)
	{
		// Notify user.
		App.AlertMsg(TXT("%s"), e.ErrorText());
	}

	return bOK;
}

/******************************************************************************
** Method:		UpdateScriptsMenu()
**
** Description:	Repopulate the scrips menu.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::UpdateScriptsMenu()
{
	// Get the scripts popup menu.
	CPopupMenu oMenu = App.m_AppWnd.m_Menu.GetItemPopup(SCRIPTS_MENU_POS);

	// Clear favourite scripts menu and table.
	for (size_t i = 0; i < App.m_oScripts.RowCount(); i++)
		oMenu.RemoveCmd(App.m_oScripts[i][CScripts::ID].GetInt());

	App.m_oScripts.Truncate();

	// Scripts dir set?
	if ( (App.m_pCurrConn != NULL) && (App.m_pCurrConn->m_strSQLDir != TXT("")) )
	{
		CPath strPath = App.m_pCurrConn->m_strSQLDir;

		// Find all favourite scripts.
		if (strPath.Exists())
		{
			CFileFinder oSQLFinder;
			CFileTree	oSQLFiles;

			// Find all scripts.
			oSQLFinder.Find(strPath, TXT("*.sql"), false, oSQLFiles);

			// Get shortcut to the filename array.
			CStrArray& astrFiles = oSQLFiles.Root()->m_oData.m_astrFiles;

			// Copy to the scripts table.
			for (size_t i = 0; i < astrFiles.Size(); i++)
				App.m_oScripts.Add(ID_FIRST_SCRIPT_CMD+i, strPath, astrFiles[i]);

			// Load favourite scripts menu.
			for (size_t i = 0; i < App.m_oScripts.RowCount(); i++)
			{
				CRow&   oRow    = App.m_oScripts[i];
				int     nID     = oRow[CScripts::ID];
				CPath   strName = oRow[CScripts::NAME];

				oMenu.AppendCmd(nID, strName.FileTitle());
			}
		}
	}
}
