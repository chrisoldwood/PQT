/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPHEADERS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	Wrapper to include all the main application headers.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPHEADERS_HPP
#define APPHEADERS_HPP

/******************************************************************************
**
** System headers.
**
*******************************************************************************
*/

#include <Core/Common.hpp>		// Core library common headers.
#include <WCL/wcl.hpp>			// Windows C++ library.
#include <MDBL/mdbl.hpp>		// Memory Database Library.

/******************************************************************************
**
** Application specific headers.
**
*******************************************************************************
*/

#include "Resource.h"

// App data classes.
#include "ConConfig.hpp"
#include "Query.hpp"
#include "Scripts.hpp"

// App GUI classes.
#include "AppCmds.hpp"
#include "AppToolBar.hpp"
#include "AppDlg.hpp"
#include "AppWnd.hpp"
#include "PQTApp.hpp"

#endif //APPHEADERS_HPP
