/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ROWDATADLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CRowDataDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ROWDATADLG_HPP
#define ROWDATADLG_HPP

/******************************************************************************
** 
** This is the dialog used to display a single rows' data.
**
*******************************************************************************
*/

class CRowDataDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CRowDataDlg(CRow& oRow);
	
protected:
	//
	// Members.
	//
	CRow&		m_oRow;
	CListView 	m_lvGrid;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //ROWDATADLG_HPP
