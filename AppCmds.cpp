/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppCmds class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "AboutDlg.hpp"
#include "ConnectDlg.hpp"
#include "QueryPrefsDlg.hpp"
#include "FindDlg.hpp"

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static char szExts[] = {	"SQL Scripts (*.sql)\0*.sql\0"
							"All Files (*.*)\0*.*\0"
							"\0\0"							};

static char szDefExt[] = { "sql" };

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
		CMD_ENTRY(ID_DB_CONNECT,		OnDBConnect,		NULL,					0 )
		CMD_ENTRY(ID_DB_DISCONNECT,		OnDBDisconnect,		OnUIDBDisconnect,		1 )
		CMD_ENTRY(ID_DB_EXIT,			OnDBExit,			NULL,					-1)
		// Query menu.
		CMD_ENTRY(ID_QUERY_NEW,			OnQueryNew,			NULL,					2 )
		CMD_ENTRY(ID_QUERY_OPEN,		OnQueryOpen,		NULL,					3 )
		CMD_ENTRY(ID_QUERY_SAVEAS,		OnQuerySaveAs,		NULL,					4 )
		CMD_ENTRY(ID_QUERY_PREFS,		OnQueryPrefs,		NULL,					-1)
		// Execute menu.
		CMD_ENTRY(ID_EXEC_CURRENT,		OnExecCurrent,		OnUIExecCurrent,		5 )
		CMD_ENTRY(ID_EXEC_FILE,			OnExecFile,			OnUIExecFile,			6 )
		CMD_RANGE(ID_FIRST_SCRIPT_CMD,
				  ID_LAST_SCRIPT_CMD,	OnExecScript,		NULL,					-1)
		// Results menu.
		CMD_ENTRY(ID_RESULTS_FIND,		OnResultsFind,		OnUIResultsFind,		-1)
		CMD_ENTRY(ID_RESULTS_FINDNEXT,	OnResultsFindNext,	OnUIResultsFindNext,	-1)
		// Window menu.
		CMD_ENTRY(ID_WINDOW_QUERY,		OnWindowQuery,		NULL,					-1)
		CMD_ENTRY(ID_WINDOW_RESULTS,	OnWindowResults,	NULL,					-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_ABOUT,		OnHelpAbout,		NULL,					7 )
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
	{
		try
		{
			CBusyCursor oBusy;

			// Close current connection.
			OnDBDisconnect();

			// Get the selected connection.
			CConConfig& oConn = App.m_aConConfigs[Dlg.m_nConnection];

			// Create the connection string.
			CString strConnection = oConn.ConnectionString(Dlg.m_strLogin, Dlg.m_strPassword);

			// Open the connection.
			App.m_oConnection.Open(strConnection);

			// Opened okay, 
			App.m_nDefConnection = Dlg.m_nConnection;
			App.m_strConnection  = oConn.m_strName;

		}
		catch(CSQLException& e)
		{
			// Notify user.
			App.AlertMsg(e.m_strError);

			// Cleanup connection.
			OnDBDisconnect();
			return;
		}

		// Get the selected connection.
		CConConfig& oConn = App.m_aConConfigs[Dlg.m_nConnection];

		// Scripts dir defined?
		if (!oConn.m_strSQLDir.Empty())
		{
			CPath strPath = oConn.m_strSQLDir;

			// Find all favourite scripts.
			if (strPath.Exists())
			{
				CFileFinder oSQLFinder;
				CFileTree	oSQLFiles;

				// Find all scripts.
				oSQLFinder.Find(strPath, "*.sql", false, oSQLFiles);

				// Get shortcut to the filename array.
				CStrArray& astrFiles = oSQLFiles.Root()->m_oData.m_astrFiles;

				// Copy to the scripts table.
				for (int i = 0; i < astrFiles.Size(); i++)
					App.m_oScripts.Add(ID_FIRST_SCRIPT_CMD+i, strPath, astrFiles[i]);

				// Get the scripts popup menu.
				CPopupMenu oMenu = App.m_AppWnd.m_Menu.GetItemPopup(SCRIPTS_MENU_POS);

				// Load favourite scripts menu.
				for (i = 0; i < App.m_oScripts.RowCount(); i++)
				{
					CRow&   oRow    = App.m_oScripts[i];
					int     nID     = oRow[CScripts::ID];
					CPath   strName = oRow[CScripts::NAME];

					oMenu.AppendCmd(nID, strName.FileTitle());
				}
			}
			else
			{
				// Notify user.
				App.AlertMsg("The SQL scripts path is invalid: \n\n" + (CString)strPath);
			}
		}

		UpdateUI();
		App.m_AppWnd.UpdateTitle();
	}
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

	// Get the scripts popup menu.
	CPopupMenu oMenu = App.m_AppWnd.m_Menu.GetItemPopup(SCRIPTS_MENU_POS);

	// Clear favourite scripts menu and table.
	for (int i = 0; i < App.m_oScripts.RowCount(); i++)
		oMenu.RemoveCmd(App.m_oScripts[i][CScripts::ID].GetInt());

	App.m_oScripts.Truncate();

	UpdateUI();
	App.m_AppWnd.UpdateTitle();
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
	// Empty the window contents.
	App.m_AppWnd.m_AppDlg.m_ebQuery.Text("");

	// Switch to query window.
	App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::QUERY_TAB);
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
	CPath strPath;

	// Select the file to open.
	if (strPath.Select(App.m_AppWnd, CPath::OpenFile, szExts, szDefExt))
		OnQueryOpen(strPath);
}

/******************************************************************************
** Method:		OnQueryOpen()
**
** Description:	Loads a query from a file.
**
** Parameters:	pszFileName		The file to load.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnQueryOpen(const char* pszFileName)
{
	ASSERT(pszFileName != NULL);

	try
	{
		CFile oFile;

		// Open, for reading.
		oFile.Open(pszFileName, CStream::ReadOnly);

		// Get the files' length.
		long lLength = oFile.Seek(0, CFile::End);
		oFile.Seek(0);

		// Allocate a read buffer + EOL char.
		char* pszQuery = (char*) _alloca(lLength+1);

		// Read the file and close it.
		oFile.Read(pszQuery, lLength);
		oFile.Close();

		// Ensure the query string has an EOL char.
		pszQuery[lLength] = '\0';

		// Load the query into the text editor.
		App.m_AppWnd.m_AppDlg.m_ebQuery.Text(pszQuery);

		// Switch to query window.
		App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::QUERY_TAB);
	}
	catch(CFileException& e)
	{
		// Notify user.
		App.AlertMsg(e.ErrorText());
	}
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
	CPath strPath;

	// Select the file to open.
	if (!strPath.Select(App.m_AppWnd, CPath::SaveFile, szExts, szDefExt))
		return;

	// Get the query text.
	CString strQuery = App.m_AppWnd.m_AppDlg.m_ebQuery.Text();

	try
	{
		CFile oFile;

		// Open, for reading.
		oFile.Create(strPath);

		// Write the file and close it.
		oFile.Write(strQuery, strQuery.Length());
		oFile.Close();
	}
	catch(CFileException& e)
	{
		// Notify user.
		App.AlertMsg(e.ErrorText());
	}
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

	try
	{
		CBusyCursor	oBusy;

		// Remove the last query.
		oAppDlg.m_lvGrid.Clear();
		delete App.m_pQuery;
		App.m_pQuery = NULL;

		// Reset the "find row" values.
		App.m_strFindVal   = "";
		App.m_nLastFindRow = -1;
		App.m_nLastFindCol = -1;

		// Execute the query.
		CTable& oTable = App.m_oMDB.CreateTable("Query", App.m_oConnection, strQuery);

		// Save query results.
		App.m_pQuery = new CQuery(strQuery, oTable);

		// Load the table and switch to the results view.
		oAppDlg.DisplayTable(oTable);
		oAppDlg.m_tcTabCtrl.CurSel(CAppDlg::RESULTS_TAB);
	}
	catch(CSQLException& e)
	{
		// Notify user.
		App.AlertMsg(e.m_strError);
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
	CPath strPath;

	// Select the file to open.
	if (strPath.Select(App.m_AppWnd, CPath::OpenFile, szExts, szDefExt))
		OnExecFile(strPath);
}

/******************************************************************************
** Method:		OnExecFile()
**
** Description:	Execute a query stored in an external file.
**
** Parameters:	pszFileName		The file containing the query.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnExecFile(const char* pszFileName)
{
	try
	{
		CFile oFile;

		// Open, for reading.
		oFile.Open(pszFileName, CStream::ReadOnly);

		// Get the files' length.
		long lLength = oFile.Seek(0, CFile::End);
		oFile.Seek(0);

		// Allocate a read buffer + EOL char.
		char* pszQuery = (char*) _alloca(lLength+1);

		// Read the file and close it.
		oFile.Read(pszQuery, lLength);
		oFile.Close();

		// Ensure the query string has an EOL char.
		pszQuery[lLength] = '\0';

		// Load the query into the text editor.
		App.m_AppWnd.m_AppDlg.m_ebQuery.Text(pszQuery);

		// Exec it.
		OnExecCurrent();
	}
	catch(CFileException& e)
	{
		// Notify user.
		App.AlertMsg(e.ErrorText());
	}
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
	// Locate the script by its command ID.
	CRow* pScript = App.m_oScripts.SelectRow(CScripts::ID, nCmdID);

	ASSERT(pScript != NULL);

	// Extract the path and filename.
	CPath strPath = pScript->Field(CScripts::PATH);
	CPath strFile = pScript->Field(CScripts::NAME);

	OnExecFile(strPath + strFile);
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
	if (App.m_strFindVal != "")
	{
		ASSERT(App.m_pQuery != NULL);

		CTable& oRes = *App.m_pQuery->m_pResults;

		int nStartRow = (App.m_nLastFindCol == -1) ? App.m_nLastFindRow+1 : App.m_nLastFindRow;
		int nStartCol = App.m_nLastFindCol+1;

		// For all rows from the last found row.
		for (int r = nStartRow; r < oRes.RowCount(); r++)
		{
			CRow& oRow = oRes[r];

			// For all columns from the last found column.
			for (int c = nStartCol; c < oRes.ColumnCount(); c++)
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
	App.m_AppWnd.m_ToolBar.m_btnCloseDB.Enable(bConnected);
}

void CAppCmds::OnUIExecCurrent()
{
	bool bConnected = App.m_oConnection.IsOpen();

	App.m_AppWnd.m_Menu.EnableCmd(ID_EXEC_CURRENT, bConnected);
	App.m_AppWnd.m_ToolBar.m_btnExecQuery.Enable(bConnected);
}

void CAppCmds::OnUIExecFile()
{
	bool bConnected = App.m_oConnection.IsOpen();

	App.m_AppWnd.m_Menu.EnableCmd(ID_EXEC_FILE, bConnected);
	App.m_AppWnd.m_ToolBar.m_btnExecFile.Enable(bConnected);
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
		return "Execute this script";
	}

	return CCmdControl::CmdHintStr(iCmdID);
}
