/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Handle state changed events
*
*/


// INCLUDE FILES
#include "BrCtlSampleAppContainer.h"
#include "BrCtlSampleAppStateChangeObserver.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CBrCtlSampleAppStateChangeObserver::StateChanged
// ---------------------------------------------------------
//
void 
CBrCtlSampleAppStateChangeObserver::StateChanged(TBrCtlDefs::TBrCtlState aState, 
                                                 TInt aValue)
    {
    // Here we are creating a string to be displayed on the screen, but you would
    // implement this method to have code that would be executed when the observer
    // receives state changed events.
    TBuf16<256> tgt; 
    TPoint point(0, 120);

    _LIT(KHandleBrowserStateChange, "State Change event = %d, value = %d");
     
    tgt.AppendFormat(KHandleBrowserStateChange, aState, aValue);

    iContainer->SetPoint(point);
    iContainer->SetText(tgt);

    iContainer->DrawNow();
    }

// ---------------------------------------------------------
// CBrCtlSampleAppStateChangeObserver::ConstructL
// ---------------------------------------------------------
//
void 
CBrCtlSampleAppStateChangeObserver::ConstructL(CBrCtlSampleAppContainer* aContainer)
    {
    iContainer = aContainer;
    }

// ---------------------------------------------------------
// CBrCtlSampleAppStateChangeObserver::~CBrCtlSampleAppStateChangeObserver
// ---------------------------------------------------------
//
CBrCtlSampleAppStateChangeObserver::~CBrCtlSampleAppStateChangeObserver()
    {
    }


// ---------------------------------------------------------
// CBrCtlSampleAppStateChangeObserver::NewL
// ---------------------------------------------------------
//
CBrCtlSampleAppStateChangeObserver* 
CBrCtlSampleAppStateChangeObserver::NewL(CBrCtlSampleAppContainer* aContainer)
{
  CBrCtlSampleAppStateChangeObserver* self = new(ELeave)CBrCtlSampleAppStateChangeObserver;
  CleanupStack::PushL(self);
  self->ConstructL(aContainer);
  CleanupStack::Pop();
  return self;
}

// End of File  

