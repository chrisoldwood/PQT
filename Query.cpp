/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		QUERY.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CQuery class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

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

CQuery::CQuery(const CString& strQuery, CTable& oTable)
	: m_strQuery(strQuery)
	, m_pResults(&oTable)
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CQuery::~CQuery()
{
	delete m_pResults;
}
