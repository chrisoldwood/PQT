/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		QUERY.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CQuery class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef QUERY_HPP
#define QUERY_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <MDBL/FwdDecls.hpp>

//! The default CTable smart pointer type.
typedef Core::SharedPtr<CTable> CTablePtr;

/******************************************************************************
** 
** This is used to hold the query text and resulting table.
**
*******************************************************************************
*/

class CQuery
{
public:
	//
	// Constructors/Destructor.
	//
	CQuery(const CString& strQuery, CTablePtr pTable);
	
	//
	// Members.
	//
	CString		m_strQuery;
	CTablePtr	m_pResults;
};

//! The default CQuery smart pointer type.
typedef Core::SharedPtr<CQuery> CQueryPtr;

#endif //QUERY_HPP
