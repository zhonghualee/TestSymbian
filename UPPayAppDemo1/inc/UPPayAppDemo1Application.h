/*
 ============================================================================
 Name		: UPPayAppDemo1Application.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __UPPAYAPPDEMO1APPLICATION_H__
#define __UPPAYAPPDEMO1APPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "UPPayAppDemo1.hrh"

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidUPPayAppDemo1App =
	{
	_UID3
	};

// CLASS DECLARATION

/**
 * CUPPayAppDemo1Application application class.
 * Provides factory to create concrete document object.
 * An instance of CUPPayAppDemo1Application is the application part of the
 * AVKON application framework for the UPPayAppDemo1 example application.
 */
class CUPPayAppDemo1Application : public CAknApplication
	{
public:
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID (KUidUPPayAppDemo1App).
	 */
	TUid AppDllUid() const;

protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates CUPPayAppDemo1Document document object. The returned
	 * pointer in not owned by the CUPPayAppDemo1Application object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	};

#endif // __UPPAYAPPDEMO1APPLICATION_H__
// End of File
