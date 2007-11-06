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
	CQuery(const CString& strQuery, CTable& oTable);
	~CQuery();
	
	//
	// Members.
	//
	CString	m_strQuery;
	CTable*	m_pResults;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //QUERY_HPP
