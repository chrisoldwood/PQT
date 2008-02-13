/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SCRIPTS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CScripts class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SCRIPTS_HPP
#define SCRIPTS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <MDBL/Table.hpp>

/******************************************************************************
** 
** This table is used to store to favourite scripts for a connection.
**
*******************************************************************************
*/

class CScripts : public CTable
{
public:
	//
	// Constructors/Destructor.
	//
	CScripts(CMDB& oDB);
	~CScripts();

	// Columns.
	enum
	{
		ID,		// Unique ID.
		PATH,	// Path.
		NAME,	// Filename.
	};
	
	//
	// Methods.
	//
	void Add(int nID, const tchar* pszPath, const tchar* pszFileName);

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

inline void CScripts::Add(int nID, const tchar* pszPath, const tchar* pszFileName)
{
	CRow& oRow = CreateRow();

	oRow[ID]   = nID;
	oRow[PATH] = pszPath;
	oRow[NAME] = pszFileName;

	CTable::InsertRow(oRow);
}

#endif //SCRIPTS_HPP
