/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CONCONFIG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CConConfig class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ConConfig.hpp"
#include <Core/StringUtils.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

CConConfig::CConConfig()
	: m_strName()
	, m_strDSN()
	, m_strDriver()
	, m_strServer()
	, m_strDatabase()
	, m_strFile()
	, m_eSecurity(NONE)
	, m_strLogin()
	, m_strSQLDir()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Format the connection string for the given configuration.

CString CConConfig::FormatConnectionString(const CString& strLogin, const CString& strPassword) const
{
	CString str;

	// Create basic string.
	if (!m_strDSN.Empty())
		str += TXT("DSN=") + m_strDSN + TXT(";");

	if (!m_strDriver.Empty())
		str += TXT("DRIVER=") + m_strDriver + TXT(";");

	if (!m_strServer.Empty())
		str += TXT("SERVER=") + m_strServer + TXT(";");

	if (!m_strDatabase.Empty())
		str += TXT("DATABASE=") + m_strDatabase + TXT(";");

	if (!m_strFile.Empty())
		str += TXT("FILE=") + m_strFile + TXT(";");

	if (m_eSecurity == LOGIN)
	{
		// Append user name and password.
		str += TXT("UID=") + strLogin    + TXT(";");
		str += TXT("PWD=") + strPassword + TXT(";");
	}
	else if (m_eSecurity == TRUSTED)
	{
		str += TXT("Trusted_Connection=yes;");
	}

	return str;
}
