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
		CMD_ENTRY(ID_FILE_CONNECT,	OnFileConnect,	NULL,				-1)
		CMD_ENTRY(ID_FILE_EXIT,		OnFileExit,		NULL,				-1)
		CMD_ENTRY(ID_QUERY_EXECUTE,	OnQueryExecute,	OnUIQueryExecute,	-1)
		CMD_ENTRY(ID_HELP_ABOUT,	OnHelpAbout,	NULL,				-1)
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
** Method:		OnFileConnect()
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

void CAppCmds::OnFileConnect()
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
** Method:		OnFileExit()
**
** Description:	Terminate the app.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileExit()
{
	App.m_AppWnd.Close();
}

/******************************************************************************
** Method:		OnQueryExecute()
**
** Description:	Execute the current query.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnQueryExecute()
{
	// Get the query text.
	CString strQuery = App.m_AppWnd.m_AppDlg.m_ebQuery.Text();

	try
	{
		CBusyCursor	oBusy;

		CTable& oTable = App.m_oMDB.CreateTable("market");

		oTable.AddColumn("marketid",   MDCT_FXDSTR,    3, CColumn::NOT_NULLABLE);
		oTable.AddColumn("name",       MDCT_FXDSTR,   40, CColumn::NOT_NULLABLE);
		oTable.AddColumn("statusdate", MDCT_DATETIME,  0, CColumn::NULLABLE);
		oTable.AddColumn("rowid",      MDCT_INT,       0, CColumn::NOT_NULLABLE);

		oTable << App.m_oConnection;















/*
		// Execute the query.
		CTable& oTable = App.m_oMDB.CreateTable("Query", App.m_oConnection, strQuery);
*/

		// Load the table and switch to the results view.
		App.m_AppWnd.m_AppDlg.DisplayTable(oTable);
		App.m_AppWnd.m_AppDlg.m_tcTabCtrl.CurSel(CAppDlg::RESULTS_TAB);
		delete &oTable;
	}
	catch(CSQLException& e)
	{
		// Notify user.
		App.AlertMsg(e.m_strError);
	}
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

void CAppCmds::OnUIQueryExecute()
{
	bool bConnected = App.m_oConnection.IsOpen();

	App.m_AppWnd.m_Menu.EnableCmd(ID_QUERY_EXECUTE, bConnected);
//	App.m_AppWnd.m_ToolBar.m_?Btn.Enable(bConnected);
}
