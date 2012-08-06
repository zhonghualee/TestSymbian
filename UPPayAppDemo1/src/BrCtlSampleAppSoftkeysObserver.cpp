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
* Description:  Handle soft key events
*
*/


// INCLUDE FILES
#include <eikbtgpc.h>
//#include <BrCtlSampleApp.rsg>
#include "BrCtlSampleAppContainer.h"
#include "BrCtlSampleAppSoftkeysObserver.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CBrCtlSampleAppSoftkeysObserver::UpdateSoftkeyL
// ---------------------------------------------------------
//
void 
CBrCtlSampleAppSoftkeysObserver::UpdateSoftkeyL(TBrCtlKeySoftkey /*aKeySoftkey*/,
                                                const TDesC& /*aLabel*/,
                                                TUint32 /*aCommandId*/,
                                                TBrCtlSoftkeyChangeReason /*aBrCtlSoftkeyChangeReason*/) 
    {
    CBrCtlInterface* brCtl = iContainer->BrCtlInterface();

    TBrCtlDefs::TBrCtlElementType type = brCtl->FocusedElementType();

    CEikButtonGroupContainer* current = CEikButtonGroupContainer::Current();
    switch (type)
        {
        case TBrCtlDefs::EElementInputBox:
//            current->SetCommandSetL( R_INPUT_ELEMENT_BUTTONS );
            break;
        
        default:
//            current->SetCommandSetL( R_BROWSER_DEFAULT_BUTTONS );
            break;
        }
    current->DrawNow();
    }

// ---------------------------------------------------------
// CBrCtlSampleAppSoftkeysObserver::ConstructL
// ---------------------------------------------------------
//
void 
CBrCtlSampleAppSoftkeysObserver::ConstructL(CBrCtlSampleAppContainer* aContainer)
    {
    iContainer = aContainer;
    }

// ---------------------------------------------------------
// CBrCtlSampleAppSoftkeysObserver::~CBrCtlSampleAppSoftkeysObserver
// ---------------------------------------------------------
//
CBrCtlSampleAppSoftkeysObserver::~CBrCtlSampleAppSoftkeysObserver()
    {
    }

// ---------------------------------------------------------
// CBrCtlSampleAppSoftkeysObserver::NewL
// ---------------------------------------------------------
//
CBrCtlSampleAppSoftkeysObserver* CBrCtlSampleAppSoftkeysObserver::NewL(CBrCtlSampleAppContainer* aContainer)
    {
    CBrCtlSampleAppSoftkeysObserver* self = new(ELeave)CBrCtlSampleAppSoftkeysObserver;
    CleanupStack::PushL(self);
    self->ConstructL(aContainer);
    CleanupStack::Pop();
    return self;
    }

void CBrCtlSampleAppSoftkeysObserver::UpdateHistorySoftKey()
	{
	CEikButtonGroupContainer* current = CEikButtonGroupContainer::Current();
//	current->SetCommandSetL( R_BROWSER_HISTORY_BUTTONS );
	}

void CBrCtlSampleAppSoftkeysObserver::UpdateNormalSoftKey()
	{
	CEikButtonGroupContainer* current = CEikButtonGroupContainer::Current();
//	current->SetCommandSetL( R_BROWSER_DEFAULT_BUTTONS );
	}
// End of File  

