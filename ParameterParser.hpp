////////////////////////////////////////////////////////////////////////////////
//! \file   ParameterParser.hpp
//! \brief  .
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_PARAMETERPARSER_HPP
#define APP_PARAMETERPARSER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
//! Find all parameters embedded in the query text.

bool FindParameters(const CString& strQuery, CStrArray& astrParams, CString& strError);

////////////////////////////////////////////////////////////////////////////////
//! Replace all parameters with their values.

CString ReplaceParameters(const CString& strQuery, CStrArray astrParams, CStrArray astrValues);

#endif // APP_PARAMETERPARSER_HPP
