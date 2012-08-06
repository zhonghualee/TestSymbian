/*
* ============================================================================
*  Name        : BrCtlSampleAppCommandObserver.cpp
*  Part of     : BrCtlSampleApp
*  Interface   : Browser Control API
*  Description : The Ui class for the Browser Control Sample Application
*  Version     : %Version%
*
*  Copyright (c) 2005 Nokia Corporation.
*  This material, including documentation and any related 
*  computer programs, is protected by copyright controlled by 
*  Nokia Corporation. All rights are reserved. Copying, 
*  including reproducing, storing, adapting or translating, any 
*  or all of this material requires the prior written consent of 
*  Nokia Corporation. This material also contains confidential 
*  information which may not be disclosed to others without the 
*  prior written consent of Nokia Corporation.
* ============================================================================
*/

// INCLUDE FILES
#include "BrCtlSampleAppCommandObserver.h"
#include "BrCtlSampleAppContainer.h"

// ================= MEMBER FUNCTIONS =======================
//
void CBrCtlSampleAppCommandObserver::ConstructL(CBrCtlSampleAppContainer* aContainer)
{
    iContainer = aContainer;    
}

CBrCtlSampleAppCommandObserver::~CBrCtlSampleAppCommandObserver()
{
}

void CBrCtlSampleAppCommandObserver::HandleCommandL(TBrCtlDefs::TBrCtlClientCommands aCommand, 
const CArrayFix<TPtrC>& /*aAttributesNames*/,
const CArrayFix<TPtrC>& /*aAttributeValues*/)
{
    
    // Here we are creating a text string to be displayed on the screen, but if 
    // you were implementing this method, you would most likely be displaying a
    // progress bar.

    TBuf16<256> tgt; 

    _LIT(KHandleCommandEvent, "Command event = %d");//, size = %d, trId = %d");
     
    tgt.AppendFormat(KHandleCommandEvent, aCommand); //, aSize, aTransactionId);

    iContainer->SetText(tgt);

    iContainer->DrawNow();    
}

// ---------------------------------------------------------
// CBrCtlSampleAppCommandObserver::AddCommandObserver()
// ---------------------------------------------------------
//
void CBrCtlSampleAppCommandObserver::AddCommandObserver()
    {
	iBrCtlInterface->AddCommandObserverL(this);
    }

// ---------------------------------------------------------
// CBrCtlSampleAppCommandObserver::RemoveCommandObserver()
// ---------------------------------------------------------
//
void CBrCtlSampleAppCommandObserver::RemoveCommandObserver()
    {
	iBrCtlInterface->AddCommandObserverL(this);

	iBrCtlInterface->RemoveCommandObserver(this);

    }
// ---------------------------------------------------------
// CBrCtlSampleAppCommandObserver::NewL
// ---------------------------------------------------------
//
CBrCtlSampleAppCommandObserver* 
CBrCtlSampleAppCommandObserver::NewL(CBrCtlSampleAppContainer* aContainer)
{
  CBrCtlSampleAppCommandObserver* self = new(ELeave)CBrCtlSampleAppCommandObserver;
  CleanupStack::PushL(self);
  self->ConstructL(aContainer);
  CleanupStack::Pop();
  return self;
}


// End of File  

