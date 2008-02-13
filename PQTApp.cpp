/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PQTAPP.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPQTApp class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "PQTApp.hpp"
#include <Legacy/STLUtils.hpp>
#include "Query.hpp"

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
const tchar* CPQTApp::VERSION = TXT("v1.1 [Debug]");
#else
const tchar* CPQTApp::VERSION = TXT("v1.1");
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
	, m_bModified(false)
	, m_pQuery(NULL)
	, m_nLastFindRow(-1)
	, m_nLastFindCol(-1)
	, m_oScripts(m_oMDB)
	, m_oMRUList(5)
	, m_pCurrConn(NULL)
	, m_nMinWidth(6)
	, m_nMaxWidth(25)
	, m_strNull(TXT("(null)"))
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
	m_strTitle = TXT("P.Q.T.");

	// Load settings.
	LoadDefaults();
	
	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Initialise the MRU list.
	m_oMRUList.UpdateMenu(*m_AppWnd.Menu(), ID_DB_MRU_1);

	// Show it.
	if (ShowNormal() && !m_rcLastPos.Empty())
		m_AppWnd.Move(m_rcLastPos);

	m_AppWnd.Show(m_iCmdShow);

	// Load SQL script, if passed on the command line.
	if (m_strCmdLine != TXT(""))
		App.m_AppCmds.OnQueryOpen(m_strCmdLine);

	// Update UI.
	m_AppCmds.UpdateUI();

	// Prompt for connection, if running SQL script.
	if (m_strCmdLine != TXT(""))
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
	DeleteAll(m_apConConfigs);

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
	m_nDefConnection = m_oIniFile.ReadInt(TXT("Main"), TXT("Connection"), -1);

	// Read the window pos and size.
	m_rcLastPos.left   = m_oIniFile.ReadInt(TXT("Main"), TXT("Left"),   0);
	m_rcLastPos.top    = m_oIniFile.ReadInt(TXT("Main"), TXT("Top"),    0);
	m_rcLastPos.right  = m_oIniFile.ReadInt(TXT("Main"), TXT("Right"),  0);
	m_rcLastPos.bottom = m_oIniFile.ReadInt(TXT("Main"), TXT("Bottom"), 0);

	// Read the results preferences.
	m_nMinWidth = m_oIniFile.ReadInt   (TXT("Results"), TXT("MinWidth"),  m_nMinWidth);
	m_nMaxWidth = m_oIniFile.ReadInt   (TXT("Results"), TXT("MaxWidth"),  m_nMaxWidth);
	m_strNull   = m_oIniFile.ReadString(TXT("Results"), TXT("NullValue"), m_strNull);

	// Read the connection list.
	int nConnections = m_oIniFile.ReadInt(TXT("Main"), TXT("NumConnections"), 0);

	for (int i = 0; i < nConnections; i++)
	{
		CConConfig* pConConfig = new CConConfig;
		CString     strSection;

		// Create the section name.
		strSection.Format(TXT("Connection%d"), i);

		// Read the section.
		pConConfig->m_strName     = m_oIniFile.ReadString(strSection, TXT("Name"),     TXT(""));
		pConConfig->m_strDSN      = m_oIniFile.ReadString(strSection, TXT("DSN"),      TXT(""));
		pConConfig->m_strDriver   = m_oIniFile.ReadString(strSection, TXT("Driver"),   TXT(""));
		pConConfig->m_strServer   = m_oIniFile.ReadString(strSection, TXT("Server"),   TXT(""));
		pConConfig->m_strDatabase = m_oIniFile.ReadString(strSection, TXT("Database"), TXT(""));
		pConConfig->m_strFile     = m_oIniFile.ReadString(strSection, TXT("File"),     TXT(""));
		pConConfig->m_strLogin    = m_oIniFile.ReadString(strSection, TXT("Login"),    TXT(""));
		pConConfig->m_strSQLDir   = m_oIniFile.ReadString(strSection, TXT("Scripts"),  TXT(""));

		// Add to collection, if it is valid.
		if (!pConConfig->m_strName.Empty())
			m_apConConfigs.push_back(pConConfig);
		else
			delete pConConfig;
	}

	// Validate settings.
	if (static_cast<size_t>(m_nDefConnection) >= m_apConConfigs.size())
		m_nDefConnection = -1;

	// Load MRU list.
	m_oMRUList.Load(m_oIniFile);
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
	m_oIniFile.WriteInt(TXT("Main"), TXT("Connection"), m_nDefConnection);

	// Write the window pos and size.
	m_oIniFile.WriteInt(TXT("Main"), TXT("Left"),   m_rcLastPos.left);
	m_oIniFile.WriteInt(TXT("Main"), TXT("Top"),    m_rcLastPos.top);
	m_oIniFile.WriteInt(TXT("Main"), TXT("Right"),  m_rcLastPos.right);
	m_oIniFile.WriteInt(TXT("Main"), TXT("Bottom"), m_rcLastPos.bottom);

	// Write the results preferences.
	m_oIniFile.WriteInt   (TXT("Results"), TXT("MinWidth"),  m_nMinWidth);
	m_oIniFile.WriteInt   (TXT("Results"), TXT("MaxWidth"),  m_nMaxWidth);
	m_oIniFile.WriteString(TXT("Results"), TXT("NullValue"), m_strNull  );

	// Write the connection list.
	m_oIniFile.WriteInt(TXT("Main"), TXT("NumConnections"), m_apConConfigs.size());

	for (uint i = 0; i < m_apConConfigs.size(); i++)
	{
		CConConfig* pConConfig = m_apConConfigs[i];
		CString     strSection;

		// Create the section name.
		strSection.Format(TXT("Connection%d"), i);

		// Write the section.
		m_oIniFile.WriteString(strSection, TXT("Name"),     pConConfig->m_strName);
		m_oIniFile.WriteString(strSection, TXT("DSN"),      pConConfig->m_strDSN);
		m_oIniFile.WriteString(strSection, TXT("Driver"),   pConConfig->m_strDriver);
		m_oIniFile.WriteString(strSection, TXT("Server"),   pConConfig->m_strServer);
		m_oIniFile.WriteString(strSection, TXT("Database"), pConConfig->m_strDatabase);
		m_oIniFile.WriteString(strSection, TXT("File"),     pConConfig->m_strFile);
		m_oIniFile.WriteString(strSection, TXT("Login"),    pConConfig->m_strLogin);
		m_oIniFile.WriteString(strSection, TXT("Scripts"),  pConConfig->m_strSQLDir);
	}

	// Save MRU list.
	m_oMRUList.Save(m_oIniFile);
}
