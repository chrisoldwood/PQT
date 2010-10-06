////////////////////////////////////////////////////////////////////////////////
//! \file   ManageDatabasesDialog.cpp
//! \brief  The ManageDatabasesDialog class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ManageDatabasesDialog.hpp"
#include "Resource.h"
#include "CfgConDlg.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

ManageDatabasesDialog::ManageDatabasesDialog()
	: CDialog(IDD_MANAGE)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_CONNECTIONS, &m_view)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ADD,         BN_CLICKED,      &ManageDatabasesDialog::onAddDatabase)
		CMD_CTRLMSG(IDC_EDIT,        BN_CLICKED,      &ManageDatabasesDialog::onEditDatabase)
		CMD_CTRLMSG(IDC_DELETE,      BN_CLICKED,      &ManageDatabasesDialog::onDeleteDatabase)
		NFY_CTRLMSG(IDC_CONNECTIONS, LVN_ITEMCHANGED, &ManageDatabasesDialog::onDatabaseSelected)
		NFY_CTRLMSG(IDC_CONNECTIONS, NM_DBLCLK,       &ManageDatabasesDialog::onDatabasaeDoubleClick)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Format the DSN/Driver properties into a single string.

tstring formatDsnOrDriver(CConConfigPtr config)
{
	tstring result;

	if (!config->m_strDSN.Empty())
		result = config->m_strDSN;
	else
		result = config->m_strDriver;

	return result;
}

////////////////////////////////////////////////////////////////////////////////
//! Format the server and database properties into a single string.

tstring formatServerDatabaseOrFile(CConConfigPtr config)
{
	tstring result = config->m_strServer;

	if (!result.empty() && !config->m_strDatabase.Empty())
		result += TXT(" ");

	result += config->m_strDatabase;

	if (!result.empty() && !config->m_strFile.Empty())
		result += TXT(" ");

	result += config->m_strFile;

	return result;
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void ManageDatabasesDialog::OnInitDialog()
{
	typedef CConConfigs::const_iterator iter;

	m_view.InsertColumn(0, TXT("Name"), 100);
	m_view.InsertColumn(1, TXT("DSN/Driver"), 200);
	m_view.InsertColumn(2, TXT("Server/Database"), 200);

	for (iter it = m_databases.begin(); it != m_databases.end(); ++it)
		addToView(*it);

	if (m_view.ItemCount() != 0)
		m_view.Select(0);

	updateUi();
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool ManageDatabasesDialog::OnOk()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Add button handler.

void ManageDatabasesDialog::onAddDatabase()
{
	CCfgConDlg dialog;

	if (dialog.RunModal(*this) == IDOK)
	{
		CConConfigPtr config(new CConConfig(dialog.m_oConfig));

		m_databases.push_back(config);

		size_t row = addToView(config);
		m_view.Select(row);
		updateUi();
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Edit button handler.

void ManageDatabasesDialog::onEditDatabase()
{
	ASSERT(m_view.IsSelection());

	size_t		  selection = m_view.Selection();
	CConConfigPtr config = m_databases[selection];

	CCfgConDlg dialog;

	dialog.m_oConfig = *config;

	if (dialog.RunModal(*this) == IDOK)
	{
		*config = dialog.m_oConfig;

		updateView(selection, config);
		updateUi();
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Delete button handler.

void ManageDatabasesDialog::onDeleteDatabase()
{
	ASSERT(m_view.IsSelection());

	size_t selection = m_view.Selection();

	m_view.DeleteItem(selection);
	m_databases.erase(m_databases.begin() + selection);

	size_t count = m_view.ItemCount();

	if (selection >= count)
		selection = count - 1;

	if (count != 0)
		m_view.Select(selection);

	updateUi();
}

////////////////////////////////////////////////////////////////////////////////
//! Database view selection change handler.

LRESULT ManageDatabasesDialog::onDatabaseSelected(NMHDR& header)
{
	NMLISTVIEW& message = reinterpret_cast<NMLISTVIEW&>(header);

	if (message.uChanged & LVIF_STATE)
		updateUi();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Double-clicked database.

LRESULT ManageDatabasesDialog::onDatabasaeDoubleClick(NMHDR& /*header*/)
{
	if (m_view.IsSelection())
		onEditDatabase();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Update the state of the UI.

void ManageDatabasesDialog::updateUi()
{
	bool isSelection = m_view.IsSelection();

	Control(IDC_EDIT).Enable(isSelection);
	Control(IDC_DELETE).Enable(isSelection);
}

////////////////////////////////////////////////////////////////////////////////
//! Add a database row to the view.

size_t ManageDatabasesDialog::addToView(CConConfigPtr config)
{
	size_t row = m_view.AppendItem(config->m_strName);

	m_view.ItemText(row, 1, formatDsnOrDriver(config));
	m_view.ItemText(row, 2, formatServerDatabaseOrFile(config));

	return row;
}

////////////////////////////////////////////////////////////////////////////////
//! Update a database row in the view.

void ManageDatabasesDialog::updateView(size_t row, CConConfigPtr config)
{
	m_view.ItemText(row, 0, config->m_strName);
	m_view.ItemText(row, 1, formatDsnOrDriver(config));
	m_view.ItemText(row, 2, formatServerDatabaseOrFile(config));
}
