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

#include "SecurityModel.hpp"

/******************************************************************************
** 
** This class is used to store a connection configuration.
**
*******************************************************************************
*/

class CConConfig
{
public:
	//! Default constructor.
	CConConfig();

	//
	// Methods.
	//
	CString FormatConnectionString(const CString& strLogin, const CString& strPassword) const;

	//
	// Members.
	//
	CString			m_strName;
	CString			m_strDSN;
	CString			m_strDriver;
	CString			m_strServer;
	CString			m_strDatabase;
	CString			m_strFile;
	SecurityModel	m_eSecurity;		//!< The security model in use.
	CString			m_strLogin;
	CString			m_strSQLDir;

	static const uint DEFAULT_TIMEOUT = 15;
};

//! The default CConConfig smart pointer type.
typedef Core::SharedPtr<CConConfig> CConConfigPtr;
//! The default CConConfig container type.
typedef std::vector<CConConfigPtr> CConConfigs;

#endif //CONCONFIG_HPP
