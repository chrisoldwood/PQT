/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPTOOLBAR.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppToolBar class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPTOOLBAR_HPP
#define APPTOOLBAR_HPP

/******************************************************************************
** 
** This is the applications toolbar.
**
*******************************************************************************
*/

class CAppToolBar : public CToolBar
{
public:
	//
	// Constructors/Destructor.
	//
	CAppToolBar();
	~CAppToolBar();
	
	//
	// Members.
	//
	CCmdButton	m_btnOpenDB;
	CCmdButton	m_btnCloseDB;
	CCmdButton	m_btnNewQuery;
	CCmdButton	m_btnOpenQuery;
	CCmdButton	m_btnSaveQuery;
	CCmdButton	m_btnExecQuery;
	CCmdButton	m_btnExecFile;
	CCmdButton	m_btnHelp;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPTOOLBAR_HPP
