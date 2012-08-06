/*
 ============================================================================
 Name		: UPPayAppDemo1Application.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "UPPayAppDemo1.hrh"
#include "UPPayAppDemo1Document.h"
#include "UPPayAppDemo1Application.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUPPayAppDemo1Application::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CUPPayAppDemo1Application::CreateDocumentL()
	{
	// Create an UPPayAppDemo1 document, and return a pointer to it
	return CUPPayAppDemo1Document::NewL(*this);
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1Application::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CUPPayAppDemo1Application::AppDllUid() const
	{
	// Return the UID for the UPPayAppDemo1 application
	return KUidUPPayAppDemo1App;
	}

// End of File
