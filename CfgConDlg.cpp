/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CFGCONDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CCfgConDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "CfgConDlg.hpp"
#include "SelDSNDlg.hpp"
#include "SelDriverDlg.hpp"

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

CCfgConDlg::CCfgConDlg()
	: CDialog(IDD_CFGCONNECTION)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_NAME,     &m_ebName    )
		CTRL(IDC_DSN,      &m_ebDSN     )
		CTRL(IDC_DRIVER,   &m_ebDriver  )
		CTRL(IDC_SERVER,   &m_ebServer  )
		CTRL(IDC_DATABASE, &m_ebDatabase)
		CTRL(IDC_FILENAME, &m_ebFile    )
		CTRL(IDC_LOGIN,    &m_ebLogin   )
		CTRL(IDC_SQLDIR,   &m_ebSQLDir  )
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_DSNS,      BN_CLICKED, OnDSNs     )
		CMD_CTRLMSG(IDC_DRIVERS,   BN_CLICKED, OnDrivers  )
		CMD_CTRLMSG(IDC_COMPUTERS, BN_CLICKED, OnComputers)
		CMD_CTRLMSG(IDC_FILES,     BN_CLICKED, OnFiles    )
		CMD_CTRLMSG(IDC_SQLDIRS,   BN_CLICKED, OnSQLDirs  )
	END_CTRLMSG_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CCfgConDlg::OnInitDialog()
{
	// Initialise the controls.
	m_ebName.Text(m_oConfig.m_strName);
	m_ebDSN.Text(m_oConfig.m_strDSN);
	m_ebDriver.Text(m_oConfig.m_strDriver);
	m_ebServer.Text(m_oConfig.m_strServer);
	m_ebDatabase.Text(m_oConfig.m_strDatabase);
	m_ebFile.Text(m_oConfig.m_strFile);
	m_ebLogin.Text(m_oConfig.m_strLogin);
	m_ebSQLDir.Text(m_oConfig.m_strSQLDir);
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	Validate the data and close the dialog.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CCfgConDlg::OnOk()
{
	// Extract data from controls.
	m_oConfig.m_strName     = m_ebName.Text();
	m_oConfig.m_strDSN      = m_ebDSN.Text();
	m_oConfig.m_strDriver   = m_ebDriver.Text();
	m_oConfig.m_strServer   = m_ebServer.Text();
	m_oConfig.m_strDatabase = m_ebDatabase.Text();
	m_oConfig.m_strFile     = m_ebFile.Text();
	m_oConfig.m_strLogin    = m_ebLogin.Text();
	m_oConfig.m_strSQLDir   = m_ebSQLDir.Text();

	// Have a name?
	if (m_oConfig.m_strName.Empty())
	{
		AlertMsg("You must supply a name for the connection.");
		return false;
	}

	// Have either a DSN or a Driver?
	if (m_oConfig.m_strDSN.Empty() && m_oConfig.m_strDriver.Empty())
	{
		AlertMsg("You must supply either a DSN or a Driver name.");
		return false;
	}

	// Have a Server or File to go with the Driver?
	if (!m_oConfig.m_strDriver.Empty() && (m_oConfig.m_strServer.Empty() && m_oConfig.m_strFile.Empty()))
	{
		AlertMsg("You must supply either a Server or File.");
		return false;
	}

	return true;
}

/******************************************************************************
** Method:		OnDSNs()
**
** Description:	Show the dialog to select a DSN.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CCfgConDlg::OnDSNs()
{
	CSelDSNDlg Dlg;

	if (Dlg.RunModal(*this) == IDOK)
		m_ebDSN.Text(Dlg.m_strDSN);
}

/******************************************************************************
** Method:		OnDrivers()
**
** Description:	Show the dialog to select a driver.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CCfgConDlg::OnDrivers()
{
	CSelDriverDlg Dlg;

	if (Dlg.RunModal(*this) == IDOK)
		m_ebDriver.Text(Dlg.m_strDriver);
}

/******************************************************************************
** Method:		OnComputers()
**
** Description:	Show the dialog to select a computer.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CCfgConDlg::OnComputers()
{
	CPath strPath;

	if (strPath.SelectComputer(*this, "Select the server."))
		m_ebServer.Text(strPath);
}

/******************************************************************************
** Method:		OnFiles()
**
** Description:	Show the dialog to select a file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CCfgConDlg::OnFiles()
{
	static char szExts[] = {	"All Files (*.*)\0*.*\0"
								"\0\0"					 };

	static char szDefExt[] = { "" };

	CPath strPath;

	if (strPath.Select(*this, CPath::OpenFile, szExts, szDefExt))
		m_ebFile.Text(strPath);
}

/******************************************************************************
** Method:		OnSQLDirs()
**
** Description:	Show the dialog to select a directory.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CCfgConDlg::OnSQLDirs()
{
	CPath strPath;

	if (strPath.SelectDir(*this, "Select the default SQL scripts folder."))
		m_ebSQLDir.Text(strPath);
}
