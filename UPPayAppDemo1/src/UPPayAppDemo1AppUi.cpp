/*
 ============================================================================
 Name		: UPPayAppDemo1AppUi.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : CUPPayAppDemo1AppUi implementation
 ============================================================================
 */

// INCLUDE FILES
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <hlplch.h>

#include <uppayappdemo1_0xe55dc470.rsg>

#ifdef _HELP_AVAILABLE_
#include "UPPayAppDemo1_0xe55dc470.hlp.hrh"
#endif
#include "UPPayAppDemo1.hrh"
#include "UPPayAppDemo1.pan"
#include "UPPayAppDemo1Application.h"
#include "UPPayAppDemo1AppUi.h"
#include "UPPayAppDemo1AppView.h"
#include "BrCtlSampleAppContainer.h" 

#include "WebClientEngine.h"
_LIT( KFileName, "C:\\private\\e55dc470\\UPPayAppDemo1.txt" );
_LIT( KText, "Hello World!");

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUPPayAppDemo1AppUi::ConstructL()
	{
	// Initialise app UI with standard value.
	BaseConstructL(CAknAppUi::EAknEnableSkin);

	// Create view object
	iAppView = CUPPayAppDemo1AppView::NewL(ClientRect());
	iAppContainer = new (ELeave) CBrCtlSampleAppContainer;
	iAppContainer->SetMopParent(this);
	iAppContainer->ConstructL(iAppView, ClientRect() );
	AddToStackL( iAppContainer );
	
	}
// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppUi::CUPPayAppDemo1AppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CUPPayAppDemo1AppUi::CUPPayAppDemo1AppUi()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppUi::~CUPPayAppDemo1AppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CUPPayAppDemo1AppUi::~CUPPayAppDemo1AppUi()
	{
	if (iAppView)
		{
		delete iAppView;
		iAppView = NULL;
		}

	if (iAppContainer)
	        {
	        RemoveFromStack( iAppContainer );
	        delete iAppContainer;
	        iAppContainer = NULL;
	        }
	}

TKeyResponse CUPPayAppDemo1AppUi::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    return iAppContainer->HandleKeyEventL(aKeyEvent, aType);;
    }// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CUPPayAppDemo1AppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EEikCmdExit:
		case EAknSoftkeyExit:
			Exit();
			break;

		case ECommand1:
			{
		iAppContainer->BasicBrowserControlL();
			
			}
			break;
		case ECommand2:
			{
			iAppContainer->SpecialLoadRequestsL();						
			}
			break;
		case EHelp:
			{

			CArrayFix < TCoeHelpContext > *buf = CCoeAppUi::AppHelpContextL();
			HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), buf);
			}
			break;
		case EAbout:
			{

			}
			break;
		default:
			Panic( EUPPayAppDemo1Ui);
			break;
		}
	}
// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CUPPayAppDemo1AppUi::HandleStatusPaneSizeChange()
	{
	iAppView->SetRect(ClientRect());
	}

CArrayFix<TCoeHelpContext>* CUPPayAppDemo1AppUi::HelpContextL() const
	{
#warning "Please see comment about help and UID3..."
	// Note: Help will not work if the application uid3 is not in the
	// protected range.  The default uid3 range for projects created
	// from this template (0xE0000000 - 0xEFFFFFFF) are not in the protected range so that they
	// can be self signed and installed on the device during testing.
	// Once you get your official uid3 from Symbian Ltd. and find/replace
	// all occurrences of uid3 in your project, the context help will
	// work. Alternatively, a patch now exists for the versions of 
	// HTML help compiler in SDKs and can be found here along with an FAQ:
	// http://www3.symbian.com/faq.nsf/AllByDate/E9DF3257FD565A658025733900805EA2?OpenDocument
#ifdef _HELP_AVAILABLE_
	CArrayFixFlat<TCoeHelpContext>* array = new(ELeave)CArrayFixFlat<TCoeHelpContext>(1);
	CleanupStack::PushL(array);
	array->AppendL(TCoeHelpContext(KUidUPPayAppDemo1App, KGeneral_Information));
	CleanupStack::Pop(array);
	return array;
#else
	return NULL;
#endif
	}

// End of File
