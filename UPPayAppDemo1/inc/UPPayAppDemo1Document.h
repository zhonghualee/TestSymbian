/*
 ============================================================================
 Name		: UPPayAppDemo1Document.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares document class for application.
 ============================================================================
 */

#ifndef __UPPAYAPPDEMO1DOCUMENT_h__
#define __UPPAYAPPDEMO1DOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CUPPayAppDemo1AppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CUPPayAppDemo1Document application class.
 * An instance of class CUPPayAppDemo1Document is the Document part of the
 * AVKON application framework for the UPPayAppDemo1 example application.
 */
class CUPPayAppDemo1Document : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CUPPayAppDemo1Document for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CUPPayAppDemo1Document.
	 */
	static CUPPayAppDemo1Document* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CUPPayAppDemo1Document for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CUPPayAppDemo1Document.
	 */
	static CUPPayAppDemo1Document* NewLC(CEikApplication& aApp);

	/**
	 * ~CUPPayAppDemo1Document
	 * Virtual Destructor.
	 */
	virtual ~CUPPayAppDemo1Document();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CUPPayAppDemo1AppUi object and return a pointer to it.
	 * The object returned is owned by the Uikon framework.
	 * @return Pointer to created instance of AppUi.
	 */
	CEikAppUi* CreateAppUiL();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CUPPayAppDemo1Document.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CUPPayAppDemo1Document(CEikApplication& aApp);

	};

#endif // __UPPAYAPPDEMO1DOCUMENT_h__
// End of File
