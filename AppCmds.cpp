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
		CMD_ENTRY(ID_DB_CONNECT,		OnDBConnect,		NULL,				-1)
		CMD_ENTRY(ID_DB_DISCONNECT,		OnDBDisconnect,		OnUIDBDisconnect,	-1)
		CMD_ENTRY(ID_DB_EXIT,			OnDBExit,			NULL,				-1)
		CMD_ENTRY(ID_QUERY_NEW,			OnQueryNew,			NULL,				-1)
		CMD_ENTRY(ID_QUERY_OPEN,		OnQueryOpen,		NULL,				-1)
		CMD_ENTRY(ID_QUERY_SAVEAS,		OnQuerySaveAs,		NULL,				-1)
		CMD_ENTRY(ID_EXEC_CURRENT,		OnExecCurrent,		OnUIExecCurrent,	-1)
		CMD_ENTRY(ID_EXEC_FILE,			OnExecFile,			OnUIExecFile,		-1)
		CMD_ENTRY(ID_WINDOW_QUERY,		OnWindowQuery,		NULL,				-1)
		CMD_ENTRY(ID_WINDOW_RESULTS,	OnWindowResults,	NULL,				-1)
		CMD_ENTRY(ID_HELP_ABOUT,		OnHelpAbout,		NULL,				-1)
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

	Dlg.m_strConnection = App.m_strConnection;

	// Prompt the user.
	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
	{
		try
		{
			CBusyCursor oBusy;

			// Close current connection, if open.
			if (App.m_oConnection.IsOpen())
				App.m_oConnection.Close();

			// Open the connection.
			App.m_oConnection.Open(Dlg.m_strConnection);

			// Opened okay, 
			App.m_strConnection = Dlg.m_strConnection;

			// Extract DSN.
			int nDSNStart = App.m_strConnection.Find('=') + 1;
			int nDSNEnd   = App.m_strConnection.Find(';');

			App.m_strDatabase = App.m_strConnection.Mid(nDSNStart, nDSNEnd - nDSNStart);
		}
		catch(CSQLException& e)
		{
			// Notify user.
			App.AlertMsg(e.m_strError);
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
	if (!strPath.Select(App.m_AppWnd, CPath::OpenFile, szExts, szDefExt))
		return;

	try
	{
		CFile oFile;

		// Open, for reading.
		oFile.Open(strPath, CStream::ReadOnly);

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
	// Get the query text.
	CString strQuery = App.m_AppWnd.m_AppDlg.m_ebQuery.Text();

	try
	{
		CBusyCursor	oBusy;

		// Execute the query.
		CTable& oTable = App.m_oMDB.CreateTable("Query", App.m_oConnection, strQuery);

		// Load the table and switch to the results view.
		App.m_AppWnd.m_AppDlg.DisplayTable(oTable);
		App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::RESULTS_TAB);

		// Cleanup.
		delete &oTable;
	}
	catch(CSQLException& e)
	{
		// Notify user.
		App.AlertMsg(e.m_strError);
	}
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
	if (!strPath.Select(App.m_AppWnd, CPath::OpenFile, szExts, szDefExt))
		return;

	try
	{
		CFile oFile;

		// Open, for reading.
		oFile.Open(strPath, CStream::ReadOnly);

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
}

void CAppCmds::OnUIExecCurrent()
{
	bool bConnected = App.m_oConnection.IsOpen();

	App.m_AppWnd.m_Menu.EnableCmd(ID_EXEC_CURRENT, bConnected);
}

void CAppCmds::OnUIExecFile()
{
	bool bConnected = App.m_oConnection.IsOpen();

	App.m_AppWnd.m_Menu.EnableCmd(ID_EXEC_FILE, bConnected);
}
