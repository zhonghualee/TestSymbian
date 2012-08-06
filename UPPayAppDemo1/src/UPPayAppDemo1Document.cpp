/*
 ============================================================================
 Name		: UPPayAppDemo1Document.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : CUPPayAppDemo1Document implementation
 ============================================================================
 */

// INCLUDE FILES
#include "UPPayAppDemo1AppUi.h"
#include "UPPayAppDemo1Document.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUPPayAppDemo1Document::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUPPayAppDemo1Document* CUPPayAppDemo1Document::NewL(CEikApplication& aApp)
	{
	CUPPayAppDemo1Document* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1Document::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUPPayAppDemo1Document* CUPPayAppDemo1Document::NewLC(CEikApplication& aApp)
	{
	CUPPayAppDemo1Document* self = new (ELeave) CUPPayAppDemo1Document(aApp);

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1Document::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUPPayAppDemo1Document::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1Document::CUPPayAppDemo1Document()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CUPPayAppDemo1Document::CUPPayAppDemo1Document(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CUPPayAppDemo1Document::~CUPPayAppDemo1Document()
// Destructor.
// ---------------------------------------------------------------------------
//
CUPPayAppDemo1Document::~CUPPayAppDemo1Document()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CUPPayAppDemo1Document::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CUPPayAppDemo1Document::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new (ELeave) CUPPayAppDemo1AppUi;
	}

// End of File
