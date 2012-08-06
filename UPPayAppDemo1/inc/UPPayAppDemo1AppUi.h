/*
 ============================================================================
 Name		: UPPayAppDemo1AppUi.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares UI class for application.
 ============================================================================
 */

#ifndef __UPPAYAPPDEMO1APPUI_h__
#define __UPPAYAPPDEMO1APPUI_h__

// INCLUDES
#include <aknappui.h>

#include <brctlinterface.h> 
#include <brctldefs.h>
#include <brctllayoutobserver.h> 
#include <brctllinkresolver.h>// FORWARD DECLARATIONS
class CUPPayAppDemo1AppView;

// FORWARD DECLARATIONS
class CBrCtlSampleAppContainer;


class CWebClientContainer;
class CWebClientEngine;// CLASS DECLARATION
/**
 * CUPPayAppDemo1AppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class CUPPayAppDemo1AppUi : public CAknAppUi
	{
public:
	// Constructors and destructor

	/**
	 * ConstructL.
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CUPPayAppDemo1AppUi.
	 * C++ default constructor. This needs to be public due to
	 * the way the framework constructs the AppUi
	 */
	CUPPayAppDemo1AppUi();

	/**
	 * ~CUPPayAppDemo1AppUi.
	 * Virtual Destructor.
	 */
	virtual ~CUPPayAppDemo1AppUi();

private:
	// Functions from base classes

	/**
	 * From CEikAppUi, HandleCommandL.
	 * Takes care of command handling.
	 * @param aCommand Command to be handled.
	 */
	void HandleCommandL(TInt aCommand);

	
	
    /**
      * From CEikAppUi, handles key events.
      * @param aKeyEvent Event to handled.
      * @param aType Type of the key event. 
      * @return Response code (EKeyWasConsumed, EKeyWasNotConsumed). 
      */
      virtual TKeyResponse HandleKeyEventL(
          const TKeyEvent& aKeyEvent,TEventCode aType);	/**
	 *  HandleStatusPaneSizeChange.
	 *  Called by the framework when the application status pane
	 *  size is changed.
	 */
	void HandleStatusPaneSizeChange();

	/**
	 *  From CCoeAppUi, HelpContextL.
	 *  Provides help context for the application.
	 *  size is changed.
	 */
	CArrayFix<TCoeHelpContext>* HelpContextL() const;

private:
	// Data

	/**
	 * The application view
	 * Owned by CUPPayAppDemo1AppUi
	 */
	CUPPayAppDemo1AppView* iAppView;

	
	// Pointer to the browser control interface
	CBrCtlInterface* iBrCtlInterface;
	// Desired capabilities of the browser control
	TUint iBrCtlCapabilities;
	// Command Base
	TInt iCommandBase;

private: //Data
	CBrCtlSampleAppContainer* iAppContainer; 	};

#endif // __UPPAYAPPDEMO1APPUI_h__
// End of File
