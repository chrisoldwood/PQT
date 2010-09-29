////////////////////////////////////////////////////////////////////////////////
//! \file   SecurityModel.hpp
//! \brief  The SecurityModel enumeration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_SECURITYMODEL_HPP
#define APP_SECURITYMODEL_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
//! The types of ODBC security settings.

enum SecurityModel
{
	NONE	= 0,	//!< No security.
	LOGIN	= 1,	//!< Username & Password.
	TRUSTED	= 2,	//!< Windows authentication.
};

#endif // APP_SECURITYMODEL_HPP
