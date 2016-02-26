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
#include "Query.hpp"
#include <WCL/BusyCursor.hpp>
#include <WCL/AppConfig.hpp>
#include <Core/ConfigurationException.hpp>

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

//! The configuration data publisher name.
const tchar* PUBLISHER = TXT("Chris Oldwood");
//! The configuration data application name.
const tchar* APPLICATION = TXT("PQT");
//! The configuration data format version.
const tchar* CONFIG_VERSION = TXT("1.0");

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
	, m_AppWnd()
	, m_AppCmds(m_AppWnd)
	, m_nDefConnection(Core::npos)
	, m_bModified(false)
	, m_pQuery(nullptr)
	, m_nLastFindRow(Core::npos)
	, m_oScripts()
	, m_oMRUList(5)
	, m_pCurrConn(nullptr)
	, m_nMinWidth(6)
	, m_nMaxWidth(25)
	, m_strNull(TXT("(null)"))
	, m_bGridlines(true)
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

	try
	{
		loadConfig();
	}
	catch (const Core::Exception& e)
	{
		FatalMsg(TXT("Failed to configure the application:-\n\n%s"), e.twhat());
		return false;
	}
	
	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Initialise the MRU list.
	m_oMRUList.UpdateMenu(*m_AppWnd.Menu(), ID_DB_MRU_1);

	// Show it.
	if (!m_rcLastPos.Empty())
		m_AppWnd.Move(m_rcLastPos);

	m_AppWnd.Show(m_iCmdShow);

	// Load SQL script, if passed on the command line.
	if (m_strCmdLine != TXT(""))
		App.m_AppCmds.OnQueryOpen(m_strCmdLine);

	// Update UI.
	m_AppCmds.InitialiseUI();

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

	try
	{
		saveConfig();
	}
	catch (const Core::Exception& e)
	{
		FatalMsg(TXT("Failed to save the application configuration:-\n\n%s"), e.twhat());
		return false;
	}

	return true;
}

/******************************************************************************
** Method:		loadConfig()
**
** Description:	Load the appliccation configuraion.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPQTApp::loadConfig()
{
	CBusyCursor    busyCursor;
	WCL::AppConfig appConfig(PUBLISHER, APPLICATION);

	// Read the config data version.
	tstring version = appConfig.readString(appConfig.DEFAULT_SECTION, TXT("Version"), CONFIG_VERSION);

	if (version != CONFIG_VERSION)
		throw Core::ConfigurationException(Core::fmt(TXT("The configuration data is incompatible - '%s'"), version.c_str()));

	// Read the UI settings.
	m_rcLastPos  = appConfig.readValue<CRect>(TXT("UI"), TXT("MainWindow"), m_rcLastPos);
	m_nMinWidth = appConfig.readValue<uint>(TXT("UI"), TXT("MinColumnWidth"), m_nMinWidth);
	m_nMaxWidth = appConfig.readValue<uint>(TXT("UI"), TXT("MaxColumnWidth"), m_nMaxWidth);
	m_strNull   = appConfig.readString(TXT("UI"), TXT("NullValue"), tstring(m_strNull));
	m_bGridlines = appConfig.readValue<bool>(TXT("UI"), TXT("Gridlines"), m_bGridlines);

	// Read the default connection.
	m_nDefConnection = appConfig.readValue<size_t>(TXT("Main"), TXT("LastConnection"), Core::npos);

	// Read the connection list.
	size_t nConnections = appConfig.readValue<size_t>(TXT("Main"), TXT("ConnectionCount"), 0);

	for (size_t i = 0; i != nConnections; i++)
	{
		CConConfigPtr pConConfig(new CConConfig);
		tstring       section = Core::fmt(TXT("Connection[%u]"), i);

		// Read the section.
		pConConfig->m_strName     = appConfig.readString(section, TXT("Name"),     TXT(""));
		pConConfig->m_strDSN      = appConfig.readString(section, TXT("DSN"),      TXT(""));
		pConConfig->m_strDriver   = appConfig.readString(section, TXT("Driver"),   TXT(""));
		pConConfig->m_strServer   = appConfig.readString(section, TXT("Server"),   TXT(""));
		pConConfig->m_strDatabase = appConfig.readString(section, TXT("Database"), TXT(""));
		pConConfig->m_strFile     = appConfig.readString(section, TXT("File"),     TXT(""));
		pConConfig->m_eSecurity   = static_cast<SecurityModel>(appConfig.readValue<int>(section, TXT("Security"), NONE));
		pConConfig->m_strLogin    = appConfig.readString(section, TXT("Login"),    TXT(""));
		pConConfig->m_strSQLDir   = appConfig.readString(section, TXT("Scripts"),  TXT(""));

		// Add to collection, if it is valid.
		if (!pConConfig->m_strName.Empty())
			m_apConConfigs.push_back(pConConfig);
	}

	// Validate settings.
	if ( (m_nDefConnection != Core::npos) && (m_nDefConnection >= m_apConConfigs.size()) )
		m_nDefConnection = Core::npos;

	// Load MRU list.
	m_oMRUList.Read(appConfig);
}

/******************************************************************************
** Method:		saveConfig()
**
** Description:	Save the application configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPQTApp::saveConfig()
{
	CBusyCursor    busyCursor;
	WCL::AppConfig appConfig(PUBLISHER, APPLICATION);

	// Write the config data version.
	appConfig.writeString(appConfig.DEFAULT_SECTION, TXT("Version"), CONFIG_VERSION);

	// Write the UI settings.
	appConfig.writeValue<CRect>(TXT("UI"), TXT("MainWindow"), m_rcLastPos);
	appConfig.writeValue<uint>(TXT("UI"), TXT("MinColumnWidth"), m_nMinWidth);
	appConfig.writeValue<uint>(TXT("UI"), TXT("MaxColumnWidth"), m_nMaxWidth);
	appConfig.writeString(TXT("UI"), TXT("NullValue"), tstring(m_strNull));
	appConfig.writeValue<bool>(TXT("UI"), TXT("Gridlines"), m_bGridlines);

	// Write the connection list.
	appConfig.writeValue<size_t>(TXT("Main"), TXT("ConnectionCount"), m_apConConfigs.size());

	for (size_t i = 0; i != m_apConConfigs.size(); i++)
	{
		CConConfigPtr pConConfig = m_apConConfigs[i];
		tstring       section = Core::fmt(TXT("Connection[%u]"), i);

		// Write the section.
		appConfig.writeString(section, TXT("Name"), tstring(pConConfig->m_strName));
		appConfig.writeString(section, TXT("DSN"), tstring(pConConfig->m_strDSN));
		appConfig.writeString(section, TXT("Driver"), tstring(pConConfig->m_strDriver));
		appConfig.writeString(section, TXT("Server"), tstring(pConConfig->m_strServer));
		appConfig.writeString(section, TXT("Database"), tstring(pConConfig->m_strDatabase));
		appConfig.writeString(section, TXT("File"), tstring(pConConfig->m_strFile));
		appConfig.writeValue<int>(section, TXT("Security"), pConConfig->m_eSecurity);
		appConfig.writeString(section, TXT("Login"), tstring(pConConfig->m_strLogin));
		appConfig.writeString(section, TXT("Scripts"), tstring(pConConfig->m_strSQLDir));
	}

	appConfig.writeValue<size_t>(TXT("Main"), TXT("LastConnection"), m_nDefConnection);

	// Save MRU list.
	m_oMRUList.Write(appConfig);
}
