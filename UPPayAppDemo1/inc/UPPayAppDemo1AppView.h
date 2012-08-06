/*
 ============================================================================
 Name		: UPPayAppDemo1AppView.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __UPPAYAPPDEMO1APPVIEW_h__
#define __UPPAYAPPDEMO1APPVIEW_h__

// INCLUDES
#include <coecntrl.h>

#include <brctlinterface.h> 
#include <brctldefs.h>
#include <brctllayoutobserver.h> 
#include <brctllinkresolver.h>


#include "UPPayObserver.h"
#include "UPPay.h"
#include "WebClientEngine.h"
class MVIWebClientObserver;// CLASS DECLARATION

class CUPPayAppDemo1AppView : public CCoeControl,
public MUPPayObserver
	{
public:
	// New methods

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Create a CUPPayAppDemo1AppView object, which will draw itself to aRect.
	 * @param aRect The rectangle this view will be drawn to.
	 * @return a pointer to the created instance of CUPPayAppDemo1AppView.
	 */
	static CUPPayAppDemo1AppView* NewL(const TRect& aRect);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Create a CUPPayAppDemo1AppView object, which will draw itself
	 * to aRect.
	 * @param aRect Rectangle this view will be drawn to.
	 * @return A pointer to the created instance of CUPPayAppDemo1AppView.
	 */
	static CUPPayAppDemo1AppView* NewLC(const TRect& aRect);

	/**
	 * ~CUPPayAppDemo1AppView
	 * Virtual Destructor.
	 */
	virtual ~CUPPayAppDemo1AppView();

public:
	// Functions from base classes

	/**
	 * From CCoeControl, Draw
	 * Draw this CUPPayAppDemo1AppView to the screen.
	 * @param aRect the rectangle of this view that needs updating
	 */
	void Draw(const TRect& aRect) const;

	/**
	 * From CoeControl, SizeChanged.
	 * Called by framework when the view size is changed.
	 */
	virtual void SizeChanged();

	/**
	 * From CoeControl, HandlePointerEventL.
	 * Called by framework when a pointer touch event occurs.
	 * Note: although this method is compatible with earlier SDKs, 
	 * it will not be called in SDKs without Touch support.
	 * @param aPointerEvent the information about this event
	 */
	virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);

	void UPPayEvent(const TDesC8& resultStatus);
	
	TBool SetUPPayPAAInfo(const TDesC8& );
	
	TBool GetItemsNameL(const TDesC8& aDescriptor);
	
private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 * Perform the second phase construction of a
	 * CUPPayAppDemo1AppView object.
	 * @param aRect The rectangle this view will be drawn to.
	 */
	void ConstructL(const TRect& aRect);

	/**
	 * CUPPayAppDemo1AppView.
	 * C++ default constructor.
	 */
	CUPPayAppDemo1AppView();
void NotifyUser(const TDesC& aData);
	

private: //Data
//    CBrCtlSampleAppContainer* iAppContainer; 
	CUPPayPlugin* iUPPayApp;
	HBufC* submitTime;
	HBufC* orderId;
	HBufC* transAmount;
	HBufC* merchantName;
	HBufC* merchantId;
	HBufC* signature;
	
	};

#endif // __UPPAYAPPDEMO1APPVIEW_h__
// End of File
