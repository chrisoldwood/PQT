/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SCRIPTS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CScripts class definition.
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

CScripts::CScripts(CMDB& oDB)
	: CTable(oDB, "Scripts")
{
	AddColumn("ID",   MDCT_INT,    0,        CColumn::UNIQUE  );
	AddColumn("Path", MDCT_FXDSTR, MAX_PATH, CColumn::DEFAULTS);
	AddColumn("Name", MDCT_FXDSTR, MAX_PATH, CColumn::DEFAULTS);
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

CScripts::~CScripts()
{
}
