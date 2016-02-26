/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SCRIPTS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CScripts class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Scripts.hpp"

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

CScripts::CScripts()
	: CTable(TXT("Scripts"))
{
	AddColumn(TXT("ID"),   MDCT_INT,    0,        CColumn::UNIQUE  );
	AddColumn(TXT("Path"), MDCT_FXDSTR, MAX_PATH, CColumn::DEFAULTS);
	AddColumn(TXT("Name"), MDCT_FXDSTR, MAX_PATH, CColumn::DEFAULTS);
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
