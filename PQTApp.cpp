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
	m_strQueryFile = "Untitled";

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

	// Update UI.
	m_AppCmds.UpdateUI();

	// Prompt for connection.
	m_AppWnd.PostCommand(ID_FILE_CONNECT);

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
	m_strConnection = m_oIniFile.ReadString("Main", "Connection", "");

	m_rcLastPos.left   = m_oIniFile.ReadInt("Main", "Left",   0);
	m_rcLastPos.top    = m_oIniFile.ReadInt("Main", "Top",    0);
	m_rcLastPos.right  = m_oIniFile.ReadInt("Main", "Right",  0);
	m_rcLastPos.bottom = m_oIniFile.ReadInt("Main", "Bottom", 0);
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
	m_oIniFile.WriteString("Main", "Connection", m_strConnection);

	m_oIniFile.WriteInt("Main", "Left",   m_rcLastPos.left);
	m_oIniFile.WriteInt("Main", "Top",    m_rcLastPos.top);
	m_oIniFile.WriteInt("Main", "Right",  m_rcLastPos.right);
	m_oIniFile.WriteInt("Main", "Bottom", m_rcLastPos.bottom);
}
