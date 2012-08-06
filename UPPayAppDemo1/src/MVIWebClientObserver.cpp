/*
 * MVIWebClientObserver.cpp
 *
 *  Created on: 18.7.2012
 *      Author: Zhonghua
 */
#include <aknnotewrappers.h>
#include "MVIWebClientObserver.h"
#include "BrCtlSampleAppContainer.h"
#include "UPPayAppDemo1AppView.h"

MVIWebClientObserver::MVIWebClientObserver(CBrCtlSampleAppContainer* aContainer)
	{
	// TODO Auto-generated constructor stub
	iContainer = aContainer;
	}

MVIWebClientObserver::~MVIWebClientObserver()
	{
	// TODO Auto-generated destructor stub
	}


// ----------------------------------------------------------------------------
// CWebClientContainer::ClientEvent()
// Called by CWebClientEngine to notify events to user
// ----------------------------------------------------------------------------
//
void MVIWebClientObserver::ClientEvent( const TDesC& aEventDescription )
    {
	
//    TRAPD( err, AddToStatusWindowL( aEventDescription ));
//    if( err )
//        Panic( EClientView );
    }

// ----------------------------------------------------------------------------
// CWebClientContainer::ClientHeaderReceived()
// Called by CWebClientEngine when HTTP header received
// ----------------------------------------------------------------------------
//
void MVIWebClientObserver::ClientHeaderReceived( const TDesC& aHeaderData ) 
    {
//    TRAPD( err, AddToHeaderWindowL( aHeaderData ));
//    if( err )
//        Panic( EClientView );
    }


//extern void Append8ToStreamL(RFs& aFs,
//			const TDesC& aFileName, const TDesC8&);
// ----------------------------------------------------------------------------
// CWebClientContainer::ClientBodyReceived()
// Called by CWebClientEngine when part of response body received
// ----------------------------------------------------------------------------
//

_LIT( KPaidFileName, "e:\\Others\\paidresult2.txt");
void MVIWebClientObserver::ClientBodyReceived( const TDesC8& aBodyData ) 
    {	
//	CAknInformationNote* informationNote;
//		informationNote = new (ELeave) CAknInformationNote;
//	informationNote->ExecuteLD(_L("Write data!"));
//	
//	Append8ToStreamL(CCoeEnv::Static()->FsSession(), KPaidFileName,
//			aBodyData);
//	informationNote->ExecuteLD(_L("Write data End!"));

	//CUPPayAppDemo1AppView* 	iAppView = iContainer->GetView();
	if(iContainer->GetView())
		{
		//informationNote->ExecuteLD(_L("called!"));
		iContainer->GetView()->SetUPPayPAAInfo(aBodyData);
		}
//	else
//		informationNote->ExecuteLD(_L("not called!"));
    }

