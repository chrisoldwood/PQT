/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CONCONFIG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CConConfig class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CONCONFIG_HPP
#define CONCONFIG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
** 
** This class is used to store a connection configuration.
**
*******************************************************************************
*/

class CConConfig
{
public:
	//
	// Methods.
	//
	CString ConnectionString(const CString& strLogin, const CString& strPassword) const;

	//
	// Members.
	//
	CString	m_strName;
	CString	m_strDSN;
	CString	m_strDriver;
	CString	m_strServer;
	CString	m_strDatabase;
	CString	m_strFile;
	CString	m_strLogin;
	CString	m_strSQLDir;
};

// Template shorthands.
typedef std::vector<CConConfig*> CConConfigs;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CString CConConfig::ConnectionString(const CString& strLogin, const CString& strPassword) const
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

	// Append user name and password.
	str += TXT("UID=") + strLogin    + TXT(";");
	str += TXT("PWD=") + strPassword + TXT(";");

	// Append no-dialog option.
	str += TXT("DLG=0;");

	return str;
}

#endif //CONCONFIG_HPP
