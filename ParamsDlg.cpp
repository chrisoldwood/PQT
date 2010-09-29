/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PARAMSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CParamsDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ParamsDlg.hpp"
#include <Legacy/STLUtils.hpp>
#include <WCL/StrArray.hpp>
#include "PQTApp.hpp"

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
	m_aoLabels.push_back(m_ptxtParam);
	m_aoEditBoxes.push_back(m_pebValue);

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
	DeleteAll(m_aoLabels);
	DeleteAll(m_aoEditBoxes);
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
	::MapWindowPoints(HWND_DESKTOP, m_hWnd, reinterpret_cast<POINT*>(&rcParam), 2);
	::MapWindowPoints(HWND_DESKTOP, m_hWnd, reinterpret_cast<POINT*>(&rcValue), 2);

	// Get the OK/Cancel button positions in screen co-ordinates.
	CRect rcOK     = m_btnOK.WindowRect();
	CRect rcCancel = m_btnCancel.WindowRect();

	// Convert to dialog co-ordinates.
	::MapWindowPoints(HWND_DESKTOP, m_hWnd, reinterpret_cast<POINT*>(&rcOK),     2);
	::MapWindowPoints(HWND_DESKTOP, m_hWnd, reinterpret_cast<POINT*>(&rcCancel), 2);

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

	CString strLabel = m_pastrParams->At(0);

	// Set the 1st parameter label.
	m_ptxtParam->Text(strLabel);
	m_pebValue->Text(App.m_mapPrevValues[strLabel]);

	// Create the extra param and value controls.
	for (size_t i = 1; i < m_pastrParams->Size(); ++i)
	{
		CRect rcNewParam = rcParam;
		CRect rcNewValue = rcValue;

		// Calculate param control position.
		rcNewParam.top   += (i*CONTROL_GAP) + (i*rcParam.Height());
		rcNewParam.bottom = rcNewParam.top  + rcParam.Height();

		// Calculate value control position.
		rcNewValue.top   += (i*CONTROL_GAP) + (i*rcValue.Height());
		rcNewValue.bottom = rcNewValue.top  + rcValue.Height();

		strLabel = m_pastrParams->At(i);

		// Allocate control wrappers.
		CLabel*   pLabel   = new CLabel;
		CEditBox* pEditBox = new CEditBox;

		// Add to containers.
		m_aoLabels.push_back(pLabel);
		m_aoEditBoxes.push_back(pEditBox);

		DWORD dwLabelExStyle = m_ptxtParam->WindowExStyle();
		DWORD dwLabelStyle   = m_ptxtParam->WindowStyle();

		// Create and initialise the label control.
		pLabel->Create(*this, IDC_PARAM_NAME+i, rcNewParam, dwLabelExStyle, dwLabelStyle);
		pLabel->Text(strLabel);
		pLabel->Font(oDlgFont);

		DWORD dwEditExStyle = m_pebValue->WindowExStyle();
		DWORD dwEditStyle   = m_pebValue->WindowStyle();

		// Create and initialise the edit control.
		pEditBox->Create(*this, IDC_PARAM_VALUE+i, rcNewValue, dwEditExStyle, dwEditStyle );
		pEditBox->Font(oDlgFont);
		pEditBox->Text(App.m_mapPrevValues[strLabel]);
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
	for (size_t i = 0; i < m_pastrParams->Size(); ++i)
		m_pastrValues->Add(m_aoEditBoxes[i]->Text());

	return true;
}
