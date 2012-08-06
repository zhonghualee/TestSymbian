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
* Description:  Handle load progress events
*
*/


// INCLUDE FILES
#include "BrCtlSampleAppContainer.h"
#include "BrCtlSampleAppLoadEventObserver.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CBrCtlSampleAppLoadEventObserver::HandleBrowserLoadEventL
// ---------------------------------------------------------
//
void 
CBrCtlSampleAppLoadEventObserver::HandleBrowserLoadEventL(TBrCtlDefs::TBrCtlLoadEvent aLoadEvent,
                                                          TUint aSize, 
                                                          TUint16 aTransactionId) 
    {
    // Here we are creating a text string to be displayed on the screen, but if 
    // you were implementing this method, you would most likely be displaying a
    // progress bar.

    TBuf16<256> tgt; 

    _LIT(KHandleBrowserLoadEvent, "Load event = %d, size = %d, trId = %d");
     
    tgt.AppendFormat(KHandleBrowserLoadEvent, aLoadEvent, aSize, aTransactionId);

    iContainer->SetText(tgt);

    iContainer->DrawNow();
    
    if (aLoadEvent == TBrCtlDefs::EEventLoadFinished && iContainer->isIconTest())
 		iContainer->ProcessIcon();
    }

// ---------------------------------------------------------
// CBrCtlSampleAppLoadEventObserver::ConstructL
// ---------------------------------------------------------
//
void 
CBrCtlSampleAppLoadEventObserver::ConstructL(CBrCtlSampleAppContainer* aContainer)
    {
    iContainer = aContainer;
    }

// ---------------------------------------------------------
// CBrCtlSampleAppLoadEventObserver::~CBrCtlSampleAppLoadEventObserver
// ---------------------------------------------------------
//
CBrCtlSampleAppLoadEventObserver::~CBrCtlSampleAppLoadEventObserver()
    {
    }


// ---------------------------------------------------------
// CBrCtlSampleAppLoadEventObserver::NewL
// ---------------------------------------------------------
//
CBrCtlSampleAppLoadEventObserver* 
CBrCtlSampleAppLoadEventObserver::NewL(CBrCtlSampleAppContainer* aContainer)
{
  CBrCtlSampleAppLoadEventObserver* self = new(ELeave)CBrCtlSampleAppLoadEventObserver;
  CleanupStack::PushL(self);
  self->ConstructL(aContainer);
  CleanupStack::Pop();
  return self;
}


// End of File  

