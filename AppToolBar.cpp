/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPTOOLBAR.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppToolBar class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppToolBar.hpp"

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

CAppToolBar::CAppToolBar()
{
	// Define the toolbar.
	DEFINE_TOOLBAR
		TBCTL(&m_btnOpenDB,		ID_DB_CONNECT)
		TBCTL(&m_btnCloseDB,	ID_DB_DISCONNECT)
		TBGAP()
		TBCTL(&m_btnNewQuery,	ID_QUERY_NEW)
		TBCTL(&m_btnOpenQuery,	ID_QUERY_OPEN)
		TBCTL(&m_btnSaveQuery,	ID_QUERY_SAVEAS)
		TBGAP()
		TBCTL(&m_btnExecQuery,	ID_EXEC_CURRENT)
		TBCTL(&m_btnExecFile,	ID_EXEC_FILE)
		TBGAP()
		TBCTL(&m_btnHelp,		ID_HELP_ABOUT)
	END_TOOLBAR
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

CAppToolBar::~CAppToolBar()
{
}
