////////////////////////////////////////////////////////////////////////////////
//! \file   ManageDatabasesDialog.hpp
//! \brief  The ManageDatabasesDialog class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef MANAGEDATABASESDIALOG_HPP
#define MANAGEDATABASESDIALOG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "ConConfig.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to manage the set of database configurations.

class ManageDatabasesDialog : public CDialog
{
public:
	//! Default constructor.
	ManageDatabasesDialog();
	
	//
	// Members.
	//
	CConConfigs m_databases;

private:
	//
	// Members.
	//
	
	//
	// Controls.
	//
	CListView	m_view;

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();
	//! Add button handler.
	void onAddDatabase();
	//! Edit button handler.
	void onEditDatabase();
	//! Delete button handler.
	void onDeleteDatabase();
	//! Database view selection change handler.
	LRESULT onDatabaseSelected(NMHDR& header);

	//
	// Internal methods.
	//

	//! Update the state of the UI.
	void updateUi();
	//! Add a database row to the view.
	size_t addToView(CConConfigPtr config);
	//! Update a database row in the view.
	void updateView(size_t row, CConConfigPtr config);
};

#endif // MANAGEDATABASESDIALOG_HPP
