/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PQTAPP.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPQTApp class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// "The" application object.
CPQTApp App;

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

#ifdef _DEBUG
const char* CPQTApp::VERSION      = "v1.0 Alpha [Debug]";
#else
const char* CPQTApp::VERSION      = "v1.0 Alpha";
#endif

/******************************************************************************
** Method:		Constructor
**
** Description:	Default constructor.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CPQTApp::CPQTApp()
	: CApp(m_AppWnd, m_AppCmds)
	, m_nDefConnection(-1)
	, m_pQuery(NULL)
	, m_nLastFindRow(-1)
	, m_nLastFindCol(-1)
	, m_oScripts(m_oMDB)
{
}

/******************************************************************************
** Method:		Destructor
**
** Description:	Cleanup.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CPQTApp::~CPQTApp()
{
}

/******************************************************************************
** Method:		OnOpen()
**
** Description:	Initialises the application.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CPQTApp::OnOpen()
{
	// Set the app title.
	m_strTitle = "P.Q.T.";

	// Set the query file name.
//	m_strQueryFile = "Untitled";

	// Load settings.
	LoadDefaults();
	
	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	if ( (m_iCmdShow == SW_SHOWNORMAL) && (m_rcLastPos.Empty() == false) )
		m_AppWnd.Move(m_rcLastPos);

	m_AppWnd.Show(m_iCmdShow);

	// Load SQL script, if passed on the command line.
	if (m_strCmdLine != "")
		App.m_AppCmds.OnQueryOpen(m_strCmdLine);

	// Update UI.
	m_AppCmds.UpdateUI();

	// Prompt for connection, if running SQL script.
	if (m_strCmdLine != "")
		m_AppWnd.PostCommand(ID_DB_CONNECT);

	return true;
}

/******************************************************************************
** Method:		OnClose()
**
** Description:	Cleans up the application resources.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CPQTApp::OnClose()
{
	// Close current connection, if open.
	if (App.m_oConnection.IsOpen())
		App.m_oConnection.Close();

	// Save settings.
	SaveDefaults();

	// Cleanup.	
	delete m_pQuery;
	m_aConConfigs.DeleteAll();

	return true;
}

/******************************************************************************
** Method:		LoadDefaults()
**
** Description:	Load the default settings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPQTApp::LoadDefaults()
{
	// Read the default connection.
	m_nDefConnection = m_oIniFile.ReadInt("Main", "Connection", -1);

	// Read the window pos and size.
	m_rcLastPos.left   = m_oIniFile.ReadInt("Main", "Left",   0);
	m_rcLastPos.top    = m_oIniFile.ReadInt("Main", "Top",    0);
	m_rcLastPos.right  = m_oIniFile.ReadInt("Main", "Right",  0);
	m_rcLastPos.bottom = m_oIniFile.ReadInt("Main", "Bottom", 0);

	// Read the results preferences.
	m_nMinWidth = m_oIniFile.ReadInt   ("Results", "MinWidth",  6);
	m_nMaxWidth = m_oIniFile.ReadInt   ("Results", "MaxWidth",  25);
	m_strNull   = m_oIniFile.ReadString("Results", "NullValue", "(null)");

	// Read the connection list.
	int nConnections = m_oIniFile.ReadInt("Main", "NumConnections", 0);

	for (int i = 0; i < nConnections; i++)
	{
		CConConfig* pConConfig = new CConConfig;
		CString     strSection;

		// Create the section name.
		strSection.Format("Connection%d", i);

		// Read the section.
		pConConfig->m_strName     = m_oIniFile.ReadString(strSection, "Name",     "");
		pConConfig->m_strDSN      = m_oIniFile.ReadString(strSection, "DSN",      "");
		pConConfig->m_strDriver   = m_oIniFile.ReadString(strSection, "Driver",   "");
		pConConfig->m_strServer   = m_oIniFile.ReadString(strSection, "Server",   "");
		pConConfig->m_strDatabase = m_oIniFile.ReadString(strSection, "Database", "");
		pConConfig->m_strFile     = m_oIniFile.ReadString(strSection, "File",     "");
		pConConfig->m_strLogin    = m_oIniFile.ReadString(strSection, "Login",    "");
		pConConfig->m_strSQLDir   = m_oIniFile.ReadString(strSection, "Scripts",  "");

		// Add to collection, if it is valid.
		if (!pConConfig->m_strName.Empty())
			m_aConConfigs.Add(*pConConfig);
		else
			delete pConConfig;
	}

	// Validate settings.
	if (m_nDefConnection >= m_aConConfigs.Size())
		m_nDefConnection = 0;
}

/******************************************************************************
** Method:		SaveDefaults()
**
** Description:	Save the default settings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPQTApp::SaveDefaults()
{
	// Write the default connection.
	m_oIniFile.WriteInt("Main", "Connection", m_nDefConnection);

	// Write the window pos and size.
	m_oIniFile.WriteInt("Main", "Left",   m_rcLastPos.left);
	m_oIniFile.WriteInt("Main", "Top",    m_rcLastPos.top);
	m_oIniFile.WriteInt("Main", "Right",  m_rcLastPos.right);
	m_oIniFile.WriteInt("Main", "Bottom", m_rcLastPos.bottom);

	// Write the results preferences.
	m_oIniFile.WriteInt   ("Results", "MinWidth",  m_nMinWidth);
	m_oIniFile.WriteInt   ("Results", "MaxWidth",  m_nMaxWidth);
	m_oIniFile.WriteString("Results", "NullValue", m_strNull  );

	// Write the connection list.
	m_oIniFile.WriteInt("Main", "NumConnections", m_aConConfigs.Size());

	for (int i = 0; i < m_aConConfigs.Size(); i++)
	{
		CConConfig& oConConfig = m_aConConfigs[i];
		CString     strSection;

		// Create the section name.
		strSection.Format("Connection%d", i);

		// Write the section.
		m_oIniFile.WriteString(strSection, "Name",     oConConfig.m_strName);
		m_oIniFile.WriteString(strSection, "DSN",      oConConfig.m_strDSN);
		m_oIniFile.WriteString(strSection, "Driver",   oConConfig.m_strDriver);
		m_oIniFile.WriteString(strSection, "Server",   oConConfig.m_strServer);
		m_oIniFile.WriteString(strSection, "Database", oConConfig.m_strDatabase);
		m_oIniFile.WriteString(strSection, "File",     oConConfig.m_strFile);
		m_oIniFile.WriteString(strSection, "Login",    oConConfig.m_strLogin);
		m_oIniFile.WriteString(strSection, "Scripts",  oConConfig.m_strSQLDir);
	}
}
