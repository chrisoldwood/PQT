/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppCmds class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPCMDS_HPP
#define APPCMDS_HPP

/******************************************************************************
** 
** Application command controller.
**
*******************************************************************************
*/

class CAppCmds : public CCmdControl
{
public:
	//
	// Constructors/Destructor.
	//
	CAppCmds();
	~CAppCmds();

	//
	// Commands.
	//

	// Database menu.
	void OnDBConnect();
	void OnDBDisconnect();
	void OnDBExit();

	// Query menu.
	void OnQueryNew();
	void OnQueryOpen();
	void OnQuerySaveAs();

	// Execute menu.
	void OnExecCurrent();
	void OnExecFile();

	// Window menu.
	void OnWindowQuery();
	void OnWindowResults();

	// Help menu.
	void OnHelpAbout();

	//
	// UI handlers.
	//
	void OnUIDBDisconnect();
	void OnUIExecCurrent();
	void OnUIExecFile();

protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPCMDS_HPP
