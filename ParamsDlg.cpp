/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PARAMSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CParamsDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ParamsDlg.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CParamsDlg::CParamsDlg()
	: CDialog(IDD_PARAMETERS)
	, m_pastrParams(NULL)
	, m_pastrValues(NULL)
{
	// Create template control wrappers.
	m_ptxtParam = new CLabel;
	m_pebValue  = new CEditBox;

	// Add to containers.
	m_aoLabels.Add(m_ptxtParam);
	m_aoEditBoxes.Add(m_pebValue);

	DEFINE_CTRL_TABLE
		CTRL(IDC_PARAM_NAME,  m_ptxtParam )
		CTRL(IDC_PARAM_VALUE, m_pebValue  )
		CTRL(IDOK,            &m_btnOK    )
		CTRL(IDCANCEL,        &m_btnCancel)
	END_CTRL_TABLE
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

CParamsDlg::~CParamsDlg()
{
	// Free collections.
	m_aoLabels.DeleteAll();
	m_aoEditBoxes.DeleteAll();
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CParamsDlg::OnInitDialog()
{
	ASSERT(m_pastrParams != NULL);
	ASSERT(m_pastrValues != NULL);

	// Get the template control positions in screen co-ordinates.
	CRect rcParam = m_ptxtParam->WindowRect();
	CRect rcValue = m_pebValue->WindowRect();

	// Convert to dialog co-ordinates.
	::MapWindowPoints(HWND_DESKTOP, m_hWnd, (POINT*)&rcParam, 2);
	::MapWindowPoints(HWND_DESKTOP, m_hWnd, (POINT*)&rcValue, 2);

	// Get the OK/Cancel button positions in screen co-ordinates.
	CRect rcOK     = m_btnOK.WindowRect();
	CRect rcCancel = m_btnCancel.WindowRect();

	// Convert to dialog co-ordinates.
	::MapWindowPoints(HWND_DESKTOP, m_hWnd, (POINT*)&rcOK,     2);
	::MapWindowPoints(HWND_DESKTOP, m_hWnd, (POINT*)&rcCancel, 2);

	// Calculate how much to grow the dialog by.
	int nExtraHeight = (rcParam.Height() * (m_pastrParams->Size()-1))
					 + (CONTROL_GAP      * (m_pastrParams->Size()-1));

	// Grow the dialog.
	CRect rcDialog = WindowRect();

	rcDialog.bottom += nExtraHeight;

	Move(rcDialog, false);
	Centre();

	// Move the OK button down.
	rcOK.top     += nExtraHeight;
	rcOK.bottom  += nExtraHeight;

	m_btnOK.Move(rcOK);

	// Move the Cancel button down.
	rcCancel.top    += nExtraHeight;
	rcCancel.bottom += nExtraHeight;

	m_btnCancel.Move(rcCancel);

	// Get the dialog font.
	CFont oDlgFont = Font();

	// Set the 1st parameter label.
	m_ptxtParam->Text(m_pastrParams->At(0));

	// Create the extra param and value controls.
	for (int i = 1; i < m_pastrParams->Size(); ++i)
	{
		CRect rcNewParam = rcParam;
		CRect rcNewValue = rcValue;

		// Calculate param control position.
		rcNewParam.top   += (i*CONTROL_GAP) + (i*rcParam.Height());
		rcNewParam.bottom = rcNewParam.top  + rcParam.Height();

		// Calculate value control position.
		rcNewValue.top   += (i*CONTROL_GAP) + (i*rcValue.Height());
		rcNewValue.bottom = rcNewValue.top  + rcValue.Height();

		// Allocate control wrappers.
		CLabel*   pLabel   = new CLabel;
		CEditBox* pEditBox = new CEditBox;

		// Add to containers.
		m_aoLabels.Add(pLabel);
		m_aoEditBoxes.Add(pEditBox);

		DWORD dwLabelExStyle = 0;
		DWORD dwLabelStyle   = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | SS_SIMPLE | SS_CENTERIMAGE;

		// Create and initialise the label control.
		pLabel->Create(*this, IDC_PARAM_NAME+i, rcNewParam, dwLabelExStyle, dwLabelStyle);
		pLabel->Text(m_pastrParams->At(i));
		pLabel->Font(oDlgFont);

		DWORD dwEditExStyle = WS_EX_CLIENTEDGE;
		DWORD dwEditStyle   = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP;

		// Create and initialise the edit control.
		pEditBox->Create(*this, IDC_PARAM_VALUE+i, rcNewValue, dwEditExStyle, dwEditStyle );
		pEditBox->Font(oDlgFont);
	}

	// Fix the tabbing order.
	for (int i = m_pastrParams->Size()-1; i >= 0; --i)
	{
		::BringWindowToTop(m_aoLabels[i]->Handle());
		::BringWindowToTop(m_aoEditBoxes[i]->Handle());
	}
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	The OK button was pressed.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CParamsDlg::OnOk()
{
	// Get the parameter values.
	for (int i = 0; i < m_pastrParams->Size(); ++i)
		m_pastrValues->Add(m_aoEditBoxes[i]->Text());

	return true;
}
