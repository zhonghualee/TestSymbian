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
* Description:  Handle scrollbar and tab events
*
*/


// INCLUDE FILES
#include "BrCtlSampleAppContainer.h"
#include "BrCtlSampleAppLayoutObserver.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CBrCtlSampleAppLayoutObserver::UpdateBrowserVScrollBarL
// ---------------------------------------------------------
//
void CBrCtlSampleAppLayoutObserver::UpdateBrowserVScrollBarL(TInt aDocumentHeight, 
                                                             TInt aDisplayHeight,
                                                             TInt aDisplayPosY ) 
    {
    // Here we are creating a text string that will be displayed on the screen
    // But if you were implementing this method, you most likely would be implementing
    // your own scroll bars here.
    TBuf16<256> tgt;
    TPoint point(0, 220);

    _LIT(KUpdateScrollbar, "Doc Height=%d, Dis Height=%d, Dis Pos=%d");

    tgt.AppendFormat(KUpdateScrollbar, aDocumentHeight, aDisplayHeight, aDisplayPosY);

    iContainer->SetPoint(point);
    iContainer->SetText(tgt);

    iContainer->DrawNow();
    }

// ---------------------------------------------------------
// CBrCtlSampleAppLayoutObserver::UpdateBrowserHScrollBarL
// ---------------------------------------------------------
//
void CBrCtlSampleAppLayoutObserver::UpdateBrowserHScrollBarL(TInt aDocumentWidth, 
                                      TInt aDisplayWidth,
                                      TInt aDisplayPosX ) 
    {
    // Here we are creating a text string that will be displayed on the screen
    // But if you were implementing this method, you most likely would be implementing
    // your own scroll bars here.
    TBuf16<256> tgt;
    TPoint point(0, 130);

    _LIT(KUpdateScrollbar, "Doc Width=%d, Dis Width=%d, Dis Pos=%d");
        
    tgt.AppendFormat(KUpdateScrollbar, aDocumentWidth, aDisplayWidth, aDisplayPosX);

    iContainer->SetPoint(point);
    iContainer->SetText(tgt);

    iContainer->DrawNow();
    }

// ---------------------------------------------------------
// CBrCtlSampleAppLayoutObserver::NotifyLayoutChange
// ---------------------------------------------------------
//
void CBrCtlSampleAppLayoutObserver::NotifyLayoutChange( TBrCtlLayout aNewLayout ) 
    {
    TBuf16<256> tgt;
    TPoint point(0, 120);

    _LIT(KUpdateLayout, "New layout is  %d");

    tgt.AppendFormat(KUpdateLayout, aNewLayout);

    iContainer->SetPoint(point);
    iContainer->SetText(tgt);

    iContainer->DrawNow();
    }

// ---------------------------------------------------------
// CBrCtlSampleAppLayoutObserver::UpdateTitle
// ---------------------------------------------------------
//
void CBrCtlSampleAppLayoutObserver::UpdateTitleL( const TDesC& /*aTitle*/ )
    {
    }

//----------------------------------------------------------
// CBrCtlSampleAppLayoutObserver::ConstructL
// ---------------------------------------------------------
//
void CBrCtlSampleAppLayoutObserver::ConstructL(CBrCtlSampleAppContainer* aContainer)
    {
    iContainer = aContainer;
    }

// ---------------------------------------------------------
// CBrCtlSampleAppObserver::~CBrCtlSampleAppObserver
// ---------------------------------------------------------
//
CBrCtlSampleAppLayoutObserver::~CBrCtlSampleAppLayoutObserver()
    {
    }


// ---------------------------------------------------------
// CBrCtlSampleAppObserver::NewL
// ---------------------------------------------------------
//
CBrCtlSampleAppLayoutObserver* CBrCtlSampleAppLayoutObserver::NewL(CBrCtlSampleAppContainer* aContainer)
{
  CBrCtlSampleAppLayoutObserver* self = new(ELeave)CBrCtlSampleAppLayoutObserver();
  CleanupStack::PushL(self);
  self->ConstructL(aContainer);
  CleanupStack::Pop();
  return self;
}

// End of File  

