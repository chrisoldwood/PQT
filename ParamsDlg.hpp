/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PARAMSDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CParamsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PARAMSDLG_HPP
#define PARAMSDLG_HPP

/******************************************************************************
** 
** The dialog used to enter the query parameters.
**
*******************************************************************************
*/

class CParamsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CParamsDlg();
	~CParamsDlg();
	
	//
	// Members.
	//
	CStrArray*	m_pastrParams;
	CStrArray*	m_pastrValues;

protected:
	// Template shorthands.
	typedef std::vector<CLabel*>   CLabels;
	typedef std::vector<CEditBox*> CEditBoxes;

	//
	// Controls.
	//
	CLabel*		m_ptxtParam;
	CEditBox*	m_pebValue;
	CButton		m_btnOK;
	CButton		m_btnCancel;
	CLabels		m_aoLabels;
	CEditBoxes	m_aoEditBoxes;

	//
	//	Constants.
	//
	enum { CONTROL_GAP = 10 };

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // PARAMSDLG_HPP
