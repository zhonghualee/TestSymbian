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
* Description:  Implementation of the container control for Browser Control Sample application.
*
*/


// INCLUDE FILES
#include "BrCtlSampleAppContainer.h"
#include "BrCtlSampleAppSpecialLoadObserver.h"

#include "BrCtlSampleAppLayoutObserver.h"
#include "BrCtlSampleAppSoftkeysObserver.h"
#include "BrCtlSampleAppLoadEventObserver.h"
#include "BrCtlSampleAppCommandObserver.h"
#include "BrCtlSampleAppLinkResolver.h"
#include "BrCtlSampleAppStateChangeObserver.h"
//#include "BrCtlSampleAppDialogsProvider.h"
//#include "BrCtlSampleAppQueryDialog.h"
//#include "BrCtlSampleAppWidgetHandler.h"

#include <eiklabel.h>  // for example label control
#include <avkon.hrh>
/*
#include <BrCtlSampleApp.rsg>
#include "brctlsampleapp.hrh"
#include <BrCtlInterface.h>
*/
#include <CHARCONV.H>
#include <gdi.h>
#include <gulicon.h>
#include <bitdev.h>
#include <aknnotewrappers.h>


#include "UPPayAppDemo1AppView.h"
#include "WebClientEngine.h"
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CBrCtlSampleAppContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CBrCtlSampleAppContainer::ConstructL(CUPPayAppDemo1AppView* aContainer, const TRect& aRect)
    {
    CreateWindowL();
    SetRect(aRect);
    ActivateL();

    iCommandBase = TBrCtlDefs::ECommandIdBase;
    
    // Create the Observers    
    iBrCtlSampleAppSpecialLoadObserver = CBrCtlSampleAppSpecialLoadObserver::NewL();
//    iBrCtlSampleAppLayoutObserver = CBrCtlSampleAppLayoutObserver::NewL(this);
//    iBrCtlSampleAppSoftkeysObserver = CBrCtlSampleAppSoftkeysObserver::NewL(this);
    iBrCtlSampleAppLoadEventObserver = CBrCtlSampleAppLoadEventObserver::NewL(this);
    iBrCtlSampleAppLinkResolver = CBrCtlSampleAppLinkResolver::NewL(this);
    iBrCtlSampleAppStateChangeObserver = CBrCtlSampleAppStateChangeObserver::NewL(this);
    iBrCtlSampleAppCommandObserver = CBrCtlSampleAppCommandObserver::NewL(this);
//    iBrCtlSampleAppDialogsProvider = CBrCtlSampleAppDialogsProvider::NewL(this);
//    iBrCtlSampleAppWidgetHandler = CBrCtlSampleAppWidgetHandler::NewL(this);
//  
//    iBrCtlSampleAppLinkResolver->setView(iAppView);
    
    // Initialize memeber variables
    _LIT(KEmpty, "");
    iText.Set(KEmpty);
    iBrCtlCapabilities = TBrCtlDefs::ECapabilityDisplayScrollBar |
			   TBrCtlDefs::ECapabilityClientNotifyURL | 
			   TBrCtlDefs::ECapabilityGraphicalHistory |
			   TBrCtlDefs::ECapabilityGraphicalPage|
			   TBrCtlDefs::ECapabilityClientResolveEmbeddedURL|
			   TBrCtlDefs::ECapabilityCursorNavigation|
			   TBrCtlDefs::ECapabilityFitToScreen;   
										  

    
    // Create a font spec in order to retrieve a valid font
    _LIT(KFontFamily, "S60 Sans");
    TFontSpec fs(KFontFamily, 100);
    // Get the Screen device for use with fonts for writing text to the screen
    CEikonEnv::Static()->ScreenDevice()->GetNearestFontInTwips((CFont*&)iFont, fs);
    iRect = Rect();
    TRect rect(Rect());
    rect.iBr.iY -= 40;
//    iBrCtlInterface = CreateBrowserControlL( this, 
//    		 rect, 
//    		        iBrCtlCapabilities,
//    		        iCommandBase, 
//    		        iBrCtlSampleAppSoftkeysObserver, 
//    		        iBrCtlSampleAppLinkResolver, 
//    		        iBrCtlSampleAppSpecialLoadObserver,
//    		        iBrCtlSampleAppLayoutObserver,
//    		        NULL);
//        
    iBrCtlInterface = CreateBrowserControlL( this, 
  		            rect, 
  		            iBrCtlCapabilities,
  		            iCommandBase, 
  		            NULL, 
  		            iBrCtlSampleAppLinkResolver, 
  		            iBrCtlSampleAppSpecialLoadObserver
  		            );

// These observers can be added and removed dynamically
    iBrCtlInterface->AddLoadEventObserverL(iBrCtlSampleAppLoadEventObserver);
    iBrCtlInterface->AddStateChangeObserverL(iBrCtlSampleAppStateChangeObserver);
    iBrCtlInterface->AddCommandObserverL(iBrCtlSampleAppCommandObserver);
    
    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSmallScreen, 1);
    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSecurityWarnings, 1);
    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsAutoLoadImages, 1);
    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsECMAScriptEnabled, 1);
  
    //_LIT(KUrl, "file:///c:\\BrCtlSampleApp\\sample1.htm");
    //    iBrCtlInterface->LoadUrlL(_L("http://218.97.244.67"));
    
    //BasicBrowserControlL();
    iconFlag = EFalse;
    
    iAppView = aContainer;
    }

// Destructor
CBrCtlSampleAppContainer::~CBrCtlSampleAppContainer()
    {
    if (iBrCtlInterface)
        {
        iBrCtlInterface->RemoveLoadEventObserver(iBrCtlSampleAppLoadEventObserver);
        iBrCtlInterface->RemoveStateChangeObserver(iBrCtlSampleAppStateChangeObserver);
        iBrCtlInterface->RemoveCommandObserver(iBrCtlSampleAppCommandObserver);
        delete iBrCtlInterface;
        }

    // we are required to release the font
    CEikonEnv::Static()->ScreenDevice()->ReleaseFont(iFont);

    // delete the observers
    delete iBrCtlSampleAppSpecialLoadObserver;
//    delete iBrCtlSampleAppLayoutObserver;
//    delete iBrCtlSampleAppSoftkeysObserver;
    delete iBrCtlSampleAppLoadEventObserver;
    delete iBrCtlSampleAppCommandObserver;
    delete iBrCtlSampleAppLinkResolver;
//    delete iBrCtlSampleAppStateChangeObserver;
//    delete iBrCtlSampleAppDialogsProvider;
    }


// ---------------------------------------------------------
// CBrCtlSampleAppLinkResolver::NewL
// ---------------------------------------------------------
//
//CBrCtlSampleAppContainer* 
//CBrCtlSampleAppContainer::NewL(const TRect& aRect)
//    {
//	CBrCtlSampleAppContainer* self = new(ELeave)CBrCtlSampleAppContainer;
//    CleanupStack::PushL(self);   
//    self->ConstructL(aRect);
////    iAppView = aContainer;
//    CleanupStack::Pop();
//    return self;
//    }

// ---------------------------------------------------------
// CBrCtlSampleAppContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CBrCtlSampleAppContainer::SizeChanged()
    {
    if (iBrCtlInterface)
        {
        TRect rect(Rect());
        rect.iBr.iY -= 40;
        iBrCtlInterface->SetRect(rect);
        }
    }

// ---------------------------------------------------------
// CBrCtlSampleAppContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CBrCtlSampleAppContainer::CountComponentControls() const
    {
    if (iBrCtlInterface)
        return 1;
    return 0;
    }

// ---------------------------------------------------------
// CBrCtlSampleAppContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CBrCtlSampleAppContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            return iBrCtlInterface; // Could be NULL
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CBrCtlSampleAppContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CBrCtlSampleAppContainer::Draw(const TRect& aRect) const
    {
    if ( iRectChanged )
    	{
    	DrawBackground(iRect);
    	}
    TRect drawRect(aRect);
    drawRect.iTl.iY = drawRect.iBr.iY - 40;
    TPoint point(5,drawRect.iTl.iY+20);
     
    CWindowGc& gc = SystemGc();
    gc.SetPenStyle(CGraphicsContext::ESolidPen);
    gc.SetBrushColor(KRgbGray);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(drawRect);
    
    gc.UseFont(iFont);

    gc.DrawText(iText, point);      

    gc.DiscardFont();
    }

// ---------------------------------------------------------
// CBrCtlSampleAppContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CBrCtlSampleAppContainer::HandleControlEventL(
    CCoeControl* aControl,TCoeEvent aEventType)
    {
    // TODO: Add your control event handler code here
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::HandleKeyEventL(
//     const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
// ----------------------------------------------------
//
TKeyResponse CBrCtlSampleAppContainer::HandleKeyEventL(
    const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/)
    {
    return EKeyWasNotConsumed;
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::SetText(const TDesC& aText)
// ----------------------------------------------------
//
void
CBrCtlSampleAppContainer::SetText(const TDesC& aText)
    {
    iText.Set(aText);
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::SetPoint(const TPoint& aPoint)
// ----------------------------------------------------
//
void
CBrCtlSampleAppContainer::SetPoint(const TPoint& aPoint)
    {
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::CancelLoad()
// ----------------------------------------------------
//
void CBrCtlSampleAppContainer::CancelLoad()
    {
    iCancelInitDataLoad = ETrue;
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::HandleCommandL(TInt aCommand)
// ----------------------------------------------------
//
void CBrCtlSampleAppContainer::HandleCommandL(TInt aCommand)
    {
//    if ( aCommand != EBrCtlSampleAppCmdApp14 && iRectChanged )
//    	{
//    	iBrCtlInterface->SetExtent(TPoint(0,0),iRect.Size());
//    	iRectChanged = EFalse;
//    	}
    switch ( aCommand )
        {
//        case EBrCtlSampleAppCmdAppRestoreSetting:
//            {
//            if (iBrCtlInterface)
//                {
//                //TRect rect( Position(), Size() );
//                TRect rect(Rect());
//                rect.iBr.iY -= 40;
//                iBrCtlInterface->SetRect( rect );
//                iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSmallScreen, 1);
//                iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSecurityWarnings, 1);
//                iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsAutoLoadImages, 1);
//                iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsECMAScriptEnabled, 1);
//                }
//            break;
//            }
//        case EBrCtlSampleAppCmdAppReload:
//            {
//            if (iBrCtlInterface)
//                {
//                iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandReload);
//                }
//            break;
//            }
//        case EBrCtlSampleAppCmdAppBack:
//            {
////            if (iBrCtlInterface)
////                {
////                iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandBack);
////                }
////            iBrCtlSampleAppSoftkeysObserver->UpdateHistorySoftKey();
//            break;
//            }
//        case EBrCtlSampleAppCmdAppForward:
//            {
//            if (iBrCtlInterface)
//                {
//                iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandForward);
//                }
//            break;
//            }
//        case EBrCtlSampleAppSelect:
////        	if (iBrCtlInterface)
////				{
////				iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandOpen);
////				}
////        	iBrCtlSampleAppSoftkeysObserver->UpdateSoftkeyL(EKeyRight,KNullDesC,0,EChangeReasonIdle);
//        	break;
//        case EBrCtlSampleAppCancel:
////        	if (iBrCtlInterface)
////				{
////				iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandCancel);
////				}
////        	iBrCtlSampleAppSoftkeysObserver->UpdateSoftkeyL(EKeyRight,KNullDesC,0,EChangeReasonIdle);
//        	break;
//        case EBrCtlSampleAppCmdAppLoadUrl:
//        	{
//        	LoadurlL();
//        	break;
//        	}
//        case ECommand1:
//            {
//            BasicBrowserControlL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp1:
//            {
//            BrowserControlWithObserversL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp2:
//            {
//            BrowserControlPostWithObserversL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp3:
//            {
//            LoadingContentWithFileHandleL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp4:
//            {
//            LoadingContentWithBufferL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp5:
//            {
//            IncrementalLoadingL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp6:
//            {
//            SpecialLoadRequestsL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp7:
//            {
//            CustomizedDialogsL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp8:
//            {
//            CustomizedSoftKeysL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp9:
//            {
//            ResolvingEmbeddedLinksL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp10:
//            {
//            CustomizedScrollBarsL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp11:
//            {
//            HandleStateChangedL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp12:
//            {
//            ChangeSizeExtentsL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp13:
//            {
//            PageInfoL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp14:
//            {
//            ContentSizeImageCountL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp15:
//            {
//            setIconTest();
//            GetBitmapDataL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp16:
//            {
//            CommandObserverL();
//            break;
//            }
//        case EBrCtlSampleAppCmdApp17:
//            {
//            WidgetHandlerL();
//            break;
//            }
        case EAknSoftkeyCancel:
        	{
//        	iBrCtlSampleAppSoftkeysObserver->UpdateNormalSoftKey();
        	break;
        	}
        default:
            if (aCommand >= iCommandBase &&
                aCommand < iCommandBase + TBrCtlDefs::ECommandIdWMLBase &&
                iBrCtlInterface)
                {
                iBrCtlInterface->HandleCommandL(aCommand);
                }

            if (aCommand >= iCommandBase + TBrCtlDefs::ECommandIdWMLBase &&
                aCommand < iCommandBase + TBrCtlDefs::ECommandIdRange &&
                iBrCtlInterface)
                {
                iBrCtlInterface->HandleCommandL(aCommand);
                }              
            break;      
        }
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::OfferKeyEventL(const 
//               TKeyEvent& aKeyEvent,TEventCode aType)
// ----------------------------------------------------
//
TKeyResponse 
CBrCtlSampleAppContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    if (iBrCtlInterface)
        {
        return iBrCtlInterface->OfferKeyEventL(aKeyEvent, aType);
        }
    return EKeyWasConsumed;
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::DynInitMenuPaneL(
//           TInt aResourceId, CEikMenuPane* aMenuPane)
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {
//    if (iBrCtlInterface && aResourceId == R_BRCTLSAMPLEAPP_MENU )
        {
//        iBrCtlInterface->AddOptionMenuItemsL(*aMenuPane, aResourceId);
//        RPointerArray<TBrCtlWmlServiceOption>* options;
//        options = iBrCtlInterface->WMLOptionMenuItemsL();
//        TInt i;
//        TInt count = options->Count();
//        for (i = 0; i < count; i++)
//            {
//            TBrCtlWmlServiceOption* option = (*options)[i];
//            if (option != NULL)
//                {
//                CEikMenuPaneItem::SData item;
//                item.iText.Copy(option->Text());
//                item.iCommandId = option->ElemID();
//                item.iFlags = 0;
//                item.iCascadeId = 0;
//                aMenuPane->InsertMenuItemL(item, 0);
//                }
//            }
        }
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::CreateBasicBrowserControlL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::CreateBasicBrowserControlL()
    {
    if (iBrCtlInterface != NULL)
        {
        delete iBrCtlInterface;
        iBrCtlInterface = NULL;
        }
    else
        {
        TRect rect(Rect());
        rect.iBr.iY -= 40;
        
        iBrCtlInterface = CreateBrowserControlL( this, 
            rect, 
            TBrCtlDefs::ECapabilityDisplayScrollBar | TBrCtlDefs::ECapabilityLoadHttpFw |
            TBrCtlDefs::ECapabilityGraphicalHistory | TBrCtlDefs::ECapabilityGraphicalPage 
            | TBrCtlDefs::ECapabilityCursorNavigation,
            TBrCtlDefs::ECommandIdBase, 
            NULL, 
            NULL,
            NULL, 
            NULL);
        }
    
      iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSmallScreen, 1);
      iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSecurityWarnings, 1);
      iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsAutoLoadImages, 1);
      iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsECMAScriptEnabled, 1);
      iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsCSSFetchEnabled, 1);

    }


void CBrCtlSampleAppContainer::NotifyUser(const TDesC& aData)
{
//	CAknInformationNote* informationNote;
//	informationNote = new (ELeave) CAknInformationNote;
//	// Show the information Note
//	informationNote->ExecuteLD(aData);
	CEikonEnv::Static()->InfoWinL(aData, _L("test"));
//	CEikonEnv::Static()->InfoMsg(aData);
}

// ----------------------------------------------------
// CBrCtlSampleAppContainer::CreateBrowserControlWithObserversL(TUint aCapabilities)
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::CreateBrowserControlWithObserversL(TUint aCapabilities)
    {
//	NotifyUser(_L("1"));
//	if (iBrCtlCapabilities != aCapabilities)
//		{
//		delete iBrCtlInterface;
//		iBrCtlInterface = NULL;
//		}
	
//	NotifyUser(_L("2"));
	iBrCtlCapabilities = aCapabilities;
#if 0	
	if (iBrCtlInterface == NULL)
		{
		TRect rect(Rect());
		rect.iBr.iY -= 40;
		
		  NotifyUser(_L("3"));
//		  iBrCtlInterface = CreateBrowserControlL( this, 
//		            rect, 
//		            iBrCtlCapabilities,
//		            TBrCtlDefs::ECommandIdBase, 
//		            NULL, 
//		            iBrCtlSampleAppLinkResolver, 
//		            NULL,
//		            NULL
//		            );
		  
		  iBrCtlInterface = CreateBrowserControlL( this, 
		   		            rect, 
		   		            iBrCtlCapabilities,
		   		            iCommandBase, 
		   		            NULL, 
		   		            iBrCtlSampleAppLinkResolver, 
		   		            NULL
		   		            );

		  
			    

		 // These observers can be added and removed dynamically
		     iBrCtlInterface->AddLoadEventObserverL(iBrCtlSampleAppLoadEventObserver);
		     iBrCtlInterface->AddStateChangeObserverL(iBrCtlSampleAppStateChangeObserver);
		     iBrCtlInterface->AddCommandObserverL(iBrCtlSampleAppCommandObserver);
//		  NotifyUser(_L("5"));
		}
#endif
	NotifyUser(_L("6"));
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::ReadFileLC(const TDesC& aFileName)
// ----------------------------------------------------
//
HBufC8* 
CBrCtlSampleAppContainer::ReadFileLC(const TDesC& aFileName)
    {
    RFs rfs;
    RFile file;
    User::LeaveIfError(rfs.Connect());
    CleanupClosePushL(rfs);
    User::LeaveIfError(file.Open(rfs, aFileName, EFileRead));
    CleanupClosePushL(file);
    TInt size;
    User::LeaveIfError(file.Size(size));
    HBufC8* buf = HBufC8::NewLC(size);
    TPtr8 bufPtr(buf->Des());
    User::LeaveIfError(file.Read(bufPtr));
    CleanupStack::Pop(); // buf
    CleanupStack::PopAndDestroy(2); // file, rfs
    CleanupStack::PushL(buf);
    return buf;
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::BasicBrowserControlL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::BasicBrowserControlL()
    {
    if (iBrCtlInterface == NULL)
        {
        //TRect rect(Position(), Size());
        TRect rect(Rect());
        rect.iBr.iY -= 40;
//        iBrCtlInterface = CreateBrowserControlL( this, 
//            rect, 
//            TBrCtlDefs::ECapabilityLoadHttpFw,
//            TBrCtlDefs::ECapabilityDisplayScrollBar | TBrCtlDefs::ECapabilityLoadHttpFw |
//            TBrCtlDefs::ECapabilityGraphicalHistory | TBrCtlDefs::ECapabilityGraphicalPage | 
//            TBrCtlDefs::ECapabilityCursorNavigation |TBrCtlDefs::ECapabilityFitToScreen,
//            iCommandBase, 
//            NULL, 
//            NULL,
//            NULL           
//            );
//        
        iBrCtlInterface = CreateBrowserControlL( this, 
                   rect, 
                   TBrCtlDefs::ECapabilityDisplayScrollBar | TBrCtlDefs::ECapabilityLoadHttpFw |
                   TBrCtlDefs::ECapabilityGraphicalHistory | TBrCtlDefs::ECapabilityGraphicalPage|
                   TBrCtlDefs::ECapabilityCursorNavigation |TBrCtlDefs::ECapabilityFitToScreen,
                   iCommandBase, 
                   NULL, 
                   NULL,
                   NULL, 
                   NULL,
                   NULL);
        
        iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSmallScreen, 1);
    		    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSecurityWarnings, 1);
    		    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsAutoLoadImages, 0);
    		    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsECMAScriptEnabled, 1);
    		    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsCSSFetchEnabled, 1);
    	
        
        iBrCtlInterface->ActivateL();
        }

//    _LIT(KUrl, "http://www.google.com");
    _LIT(KUrl, "http://news.sina.com.cn");

	  iBrCtlInterface->LoadUrlL( KUrl );

    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::BrowserControlWithObserversL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::BrowserControlWithObserversL()
    {
//    CreateBrowserControlWithObserversL( TBrCtlDefs::ECapabilityDisplayScrollBar | 
//                                        TBrCtlDefs::ECapabilityLoadHttpFw |
//                                        TBrCtlDefs::ECapabilityGraphicalHistory |
//                                        TBrCtlDefs::ECapabilityGraphicalPage | TBrCtlDefs::ECapabilityCursorNavigation);
//    _LIT(KUrl, "file:///c:\\BrCtlSampleApp\\sample2.htm");
//
//    iBrCtlInterface->LoadUrlL( KUrl );
//    CAknInformationNote* dialog = new(ELeave)CAknInformationNote();		
//    dialog->ExecuteLD(_L("BrCtl has valid observers"));
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::BasicPostBrowserControlL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::BrowserControlPostWithObserversL()
    {
//    CreateBrowserControlWithObserversL( TBrCtlDefs::ECapabilityDisplayScrollBar | 
//                                        TBrCtlDefs::ECapabilityLoadHttpFw |
//                                        TBrCtlDefs::ECapabilityGraphicalHistory |
//                                        TBrCtlDefs::ECapabilityGraphicalPage | 
//                                        TBrCtlDefs::ECapabilityCursorNavigation);
//
//    _LIT(FileName, "c:\\BrCtlSampleApp\\sample12.htm");
//    
//    RFs rfs;
//    RFile file;
//    User::LeaveIfError(rfs.Connect());
//    CleanupClosePushL(rfs);
//    User::LeaveIfError(file.Open(rfs, FileName(), EFileShareReadersOnly));
//    CleanupClosePushL(file);    
//    
//    int size;
//    User::LeaveIfError(file.Size(size));
//    HBufC8* data = NULL;
//    data = HBufC8::NewLC(size);
//    TPtr8 dataPtr(data->Des());
//    User::LeaveIfError(file.Read(dataPtr));
//
//    //parse url
//    HBufC* url16 = NULL;
//    int pos = dataPtr.Find(_L8("\r\n"));                   
//    if (pos != KErrNotFound)
//    {
//        url16 = HBufC::NewLC(pos);                
//        url16->Des().Copy(dataPtr.Mid(0,pos)); 
//    }
//    else 
//    {
//        CleanupStack::PushL(url16);        
//    }
//    
//    //parse content type
//    HBufC8* contenttype = NULL;
//    int pos2 = dataPtr.Mid(pos+2).Find(_L8("\r\n"));                   
//    if (pos2 != KErrNotFound)
//    {
//        contenttype = HBufC8::NewLC(pos2);                
//        contenttype->Des().Copy(dataPtr.Mid(pos+2,pos2));             
//    }
//    else 
//    {
//        CleanupStack::PushL(contenttype);        
//    }
//
//    //parse post data
//    HBufC8* postdata = NULL;
//    int pos3 = dataPtr.Mid(pos+2+pos2+2).Find(_L8("\r\n"));                   
//    if (pos3 != KErrNotFound)
//    {
//        postdata = HBufC8::NewLC(pos3);                
//        postdata->Des().Copy(dataPtr.Mid(pos+2+pos2+2,pos3));             
//    }
//    else 
//    {
//        CleanupStack::PushL(postdata);        
//    }
//
//    //parse boundry
//    HBufC8* boundry = NULL;
//    int pos4 = dataPtr.Mid(pos+2+pos2+2+pos3+2).Find(_L8("\r\n"));                   
//    if (pos4 != KErrNotFound)
//    {
//        boundry = HBufC8::NewLC(pos4);                
//        boundry->Des().Copy(dataPtr.Mid(pos+2+pos2+2+pos3+2,pos4));             
//    }
//    else 
//    {
//        CleanupStack::PushL(boundry);        
//    }
//    
//    TAny* any = NULL;
//    
//    TRAPD(iError,iBrCtlInterface->PostUrlL ( *url16, 
//                                *contenttype, 
//                                *postdata, 
//                                boundry, 
//                                any))
//
//    if (iError != KErrNone) 
//    {        
//        CAknQueryDialog* dialog = new (ELeave) CAknQueryDialog();
//        dialog->PrepareLC( R_CONFIRM_QUERY );
//        dialog->SetPromptL(_L("error argument"));
//        dialog->RunLD();        
//    }
//    
//    CleanupStack::PopAndDestroy(boundry);
//    CleanupStack::PopAndDestroy(postdata);
//    CleanupStack::PopAndDestroy(contenttype);
//    CleanupStack::PopAndDestroy(url16);    
//    CleanupStack::PopAndDestroy(data);                        
//    CleanupStack::PopAndDestroy(2); // file, rfs        
        
    }


// CBrCtlSampleAppContainer::LoadingContentWithFileHandleL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::LoadingContentWithFileHandleL()
    {
//    CreateBrowserControlWithObserversL(TBrCtlDefs::ECapabilityDisplayScrollBar |
//        							   TBrCtlDefs::ECapabilityLoadHttpFw | 
//        							   TBrCtlDefs::ECapabilityGraphicalHistory |
//       								   TBrCtlDefs::ECapabilityGraphicalPage);
//    _LIT(KUrl, "c:\\BrCtlSampleApp\\sample3.htm");
//    RFs rfs;
//    RFile file;
//    User::LeaveIfError(rfs.Connect());
//    CleanupClosePushL(rfs);
//    User::LeaveIfError(file.Open(rfs, KUrl(), EFileShareReadersOnly));
//    CleanupClosePushL(file);
//    iBrCtlInterface->LoadFileL(file);
//    CleanupStack::PopAndDestroy(2); // file, rfs
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::LoadingContentWithBufferL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::LoadingContentWithBufferL()
    {
//    CreateBrowserControlWithObserversL(TBrCtlDefs::ECapabilityDisplayScrollBar |
//                                       TBrCtlDefs::ECapabilityLoadHttpFw |
//    								   TBrCtlDefs::ECapabilityGraphicalHistory |
//    								   TBrCtlDefs::ECapabilityGraphicalPage |
//    								   TBrCtlDefs::ECapabilityCursorNavigation);
//    _LIT(KUrl, "c:\\BrCtlSampleApp\\sample4.htm");
//    _LIT8(KDataType, "text/html");
//
//    HBufC8* data = ReadFileLC(KUrl);
//    TDataType dataType(KDataType());
//    TUid uid;
//    uid.iUid = KCharacterSetIdentifierIso88591;
//    iBrCtlInterface->LoadDataL(KUrl, *data, dataType, uid);
//    CleanupStack::PopAndDestroy(); // data
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::IncrementalLoadingL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::IncrementalLoadingL()
    {
//    CreateBrowserControlWithObserversL(TBrCtlDefs::ECapabilityDisplayScrollBar | 
//                                       TBrCtlDefs::ECapabilityLoadHttpFw |
//    								   TBrCtlDefs::ECapabilityGraphicalHistory |
//    								   TBrCtlDefs::ECapabilityGraphicalPage |
//    								   TBrCtlDefs::ECapabilityCursorNavigation);
//    _LIT(KUrl, "c:\\BrCtlSampleApp\\sample5.htm");
//    HBufC8* data = ReadFileLC(KUrl);
//    _LIT8(KDataType, "text/html");
//    TDataType dataType(KDataType());
//    MBrCtlDataLoadConsumer* brCtlDataLoadConsumer = NULL;
//    TUid uid;
//    uid.iUid = KCharacterSetIdentifierIso88591;
//    iBrCtlInterface->InitLoadDataL(KUrl, dataType, uid, data->Length(),
//        this, &brCtlDataLoadConsumer);
//
//    if (brCtlDataLoadConsumer)
//        {
//        brCtlDataLoadConsumer->HandleNextDataChunk(*data);
//        if (!iCancelInitDataLoad)
//            {
//            brCtlDataLoadConsumer->HandleLoadComplete();
//            }
//        }
//    iCancelInitDataLoad = EFalse;
//    CleanupStack::PopAndDestroy(); // data
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::SpecialLoadRequestsL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::SpecialLoadRequestsL()
    {
//    CreateBrowserControlWithObserversL(TBrCtlDefs::ECapabilityDisplayScrollBar |
//        							   TBrCtlDefs::ECapabilityClientNotifyURL | 
//        							   TBrCtlDefs::ECapabilityGraphicalHistory |
//        							   TBrCtlDefs::ECapabilityGraphicalPage|
//        							   TBrCtlDefs::ECapabilityClientResolveEmbeddedURL|
//        							   TBrCtlDefs::ECapabilityCursorNavigation|
//        							   TBrCtlDefs::ECapabilityFitToScreen);
//    _LIT(KUrl, "file:///c:\\UPPayAppDemo1\\sample6.htm");
    //_LIT(KUrl, "http://www.google.com");
    _LIT(KUrl, "http://218.97.244.67");
    
    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSmallScreen, 1);
	iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSecurityWarnings, 1);
	iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsAutoLoadImages, 1);
	iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsECMAScriptEnabled, 1);
	iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsCSSFetchEnabled, 1);
	
    iBrCtlInterface->LoadUrlL( KUrl );
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::CustomizedDialogsL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::CustomizedDialogsL()
    {
//    CreateBrowserControlWithObserversL(TBrCtlDefs::ECapabilityDisplayScrollBar |
//        							   TBrCtlDefs::ECapabilityLoadHttpFw | 
//        							   TBrCtlDefs::ECapabilityGraphicalHistory |
//        							   TBrCtlDefs::ECapabilityGraphicalPage |
//        							   TBrCtlDefs::ECapabilityCursorNavigation);
//    _LIT(KUrl, "file:///c:\\BrCtlSampleApp\\sample7.htm");
//
//    iBrCtlInterface->LoadUrlL( KUrl );
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::CustomizedSoftKeysL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::CustomizedSoftKeysL()
    {
//    CreateBrowserControlWithObserversL(TBrCtlDefs::ECapabilityDisplayScrollBar |
//        							   TBrCtlDefs::ECapabilityLoadHttpFw | 
//        							   TBrCtlDefs::ECapabilityGraphicalHistory |
//        							   TBrCtlDefs::ECapabilityGraphicalPage |
//        							   TBrCtlDefs::ECapabilityCursorNavigation);
//    _LIT(KUrl, "file:///c:\\BrCtlSampleApp\\sample8.htm");
//
//    iBrCtlInterface->LoadUrlL( KUrl );
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::ResolvingEmbeddedLinksL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::ResolvingEmbeddedLinksL()
    {
//    CreateBrowserControlWithObserversL(TBrCtlDefs::ECapabilityDisplayScrollBar |
//        							   TBrCtlDefs::ECapabilityClientResolveEmbeddedURL |
//        							   TBrCtlDefs::ECapabilityGraphicalHistory |
//        							   TBrCtlDefs::ECapabilityGraphicalPage);
//    _LIT(KUrl, "file:///c:\\BrCtlSampleApp\\sample9.htm");
//
//    iBrCtlInterface->LoadUrlL( KUrl );
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::CustomizedScrollBarsL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::CustomizedScrollBarsL()
    {
//    CreateBrowserControlWithObserversL(TBrCtlDefs::ECapabilityDisplayScrollBar | 
//                                       TBrCtlDefs::ECapabilityLoadHttpFw |
//                                       TBrCtlDefs::ECapabilityGraphicalHistory |
//                                       TBrCtlDefs::ECapabilityGraphicalPage |
//                                       TBrCtlDefs::ECapabilityCursorNavigation);
//    _LIT(KUrl, "file:///c:\\BrCtlSampleApp\\sample10.htm");
//
//    //iBrCtlInterface->SetExtent(Position(), TSize(300, 180));
//
//    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSmallScreen, 0);
//
//    iBrCtlInterface->LoadUrlL( KUrl );
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::HandleStateChangedL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::HandleStateChangedL()
    {
//    CreateBrowserControlWithObserversL(TBrCtlDefs::ECapabilityDisplayScrollBar |
//                                       TBrCtlDefs::ECapabilityLoadHttpFw |  
//    								   TBrCtlDefs::ECapabilityGraphicalHistory |
//    								   TBrCtlDefs::ECapabilityGraphicalPage |
//    								   TBrCtlDefs::ECapabilityCursorNavigation);
//    _LIT(KUrl, "file:///c:\\BrCtlSampleApp\\sample11.htm");
//
//    //iBrCtlInterface->SetExtent(Position(), TSize(176, 72));
//
//    iBrCtlInterface->LoadUrlL( KUrl );
//    iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandEnterFullscreenBrowsing);
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::ChangeSizeExtentsL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::ChangeSizeExtentsL()
    {
    if (iBrCtlInterface)
        {
//        _LIT(KTitle,"Query Dialog");
//        _LIT(KPromptWidth, "Enter new width");
//        _LIT(KPromptHeight, "Enter new height");
//        HBufC* returnedInput = NULL;
//        TUint width = iBrCtlInterface->Rect().Width();
//        TUint height = iBrCtlInterface->Rect().Height();
//        TBool ret;
//	    // defInput is not modified by the dialog.
//        TBuf<10>defInput;
//        defInput.Num(width);
//
//        CBrCtlSampleAppQueryDialog* dialog = new (ELeave) CBrCtlSampleAppQueryDialog( defInput, returnedInput );
//        CleanupStack::PushL(dialog);
//
//        dialog->PrepareLC( R_DATA_QUERY_WITH_HEADING );
//	    dialog->SetHeaderTextL( KTitle );
//        dialog->SetPromptL( KPromptWidth );
//        CleanupStack::Pop(); // dialog
//        ret = dialog->RunLD();
//        if (ret)
//            {
//            TInt i = 0;
//            for (width = 0; i < returnedInput->Length(); i++)
//                {
//                if (((*returnedInput)[i] > '9') || ((*returnedInput)[i] < '0'))
//                    break;
//                width *= 10;
//                width += ((*returnedInput)[i] - '0');
//                }
//
//            defInput.Num(height);
//
//            dialog = new (ELeave) CBrCtlSampleAppQueryDialog( defInput, returnedInput );
//            CleanupStack::PushL(dialog);
//            dialog->PrepareLC( R_DATA_QUERY_WITH_HEADING );
//	        dialog->SetHeaderTextL( KTitle );
//            dialog->SetPromptL( KPromptHeight );
//            CleanupStack::Pop(); // dialog
//            ret = dialog->RunLD();
//            i = 0;
//            for (height = 0; i < returnedInput->Length(); i++)
//                {
//                if ((*returnedInput)[i] > '9' || (*returnedInput)[i] < '0')
//                    break;
//                height *= 10;
//                height += ((*returnedInput)[i] - '0');
//                }
//            if (width && height)
//                {
//                
//                //SetRect(TRect(TPoint(0,0), TSize(width, height)));
//                iRectChanged = ETrue;
//                iPoint = TPoint(0,iRect.iTl.iY);
//          		iBrCtlInterface->SetExtent(TPoint(0,0), TSize(width, height));
//                }
//            }
        }
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::PageInfoL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::PageInfoL()
    {
//    if (iBrCtlInterface)
//        {
//        HBufC* title = iBrCtlInterface->PageInfoLC(TBrCtlDefs::EPageInfoTitle);
//        if (title)
//            {
//            CAknQueryDialog* dialog = new (ELeave) CAknQueryDialog();
//            dialog->PrepareLC( R_CONFIRM_QUERY );
//            dialog->SetPromptL( *title );
//            dialog->RunLD();
//            CleanupStack::PopAndDestroy();
//            }
//        HBufC* url = iBrCtlInterface->PageInfoLC(TBrCtlDefs::EPageInfoUrl);
//        if (url)
//            {
//            CAknQueryDialog* dialog = new (ELeave) CAknQueryDialog();
//            dialog->PrepareLC( R_CONFIRM_QUERY );
//            dialog->SetPromptL( *url );
//            dialog->RunLD();
//            CleanupStack::PopAndDestroy();
//            }
//        }
    }

// ----------------------------------------------------
// CBrCtlSampleAppContainer::ContentSizeImageCountL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::ContentSizeImageCountL()
    {
//    if (iBrCtlInterface)
//        {
//        TSize size(0, 0);
//        size = iBrCtlInterface->ContentSize();
//        TInt images = iBrCtlInterface->ImageCountL();
//        _LIT(KStr, "Width = %d, Height = %d.");
//
//        TBuf<100> str;
//        str.Format(KStr, size.iWidth, size.iHeight);
//        CAknQueryDialog* dialog = new (ELeave) CAknQueryDialog();
//        dialog->PrepareLC( R_CONFIRM_QUERY );
//        dialog->SetPromptL( str );
//        dialog->RunLD();
//        }
    }
    
// ----------------------------------------------------
// CBrCtlSampleAppContainer::CommandObserverL()
// ----------------------------------------------------
//	
void 
CBrCtlSampleAppContainer::CommandObserverL()
	{
//	//command observer are registered in the ConstructL and removed in the distruct call
//	_LIT(TestResult, "Command observer registered, Sending command event 1");
//	CArrayFix<TPtrC>* dummy1;
//	CArrayFix<TPtrC>* dummy2;
//	if ( iBrCtlSampleAppCommandObserver )
//		{
//		iBrCtlSampleAppCommandObserver->HandleCommandL(TBrCtlDefs::EClientCommandSubscribeToFeeds,*dummy1,*dummy2);
//		}
//    CAknQueryDialog* dialog = new (ELeave) CAknQueryDialog();
//    dialog->PrepareLC( R_CONFIRM_QUERY );
//    dialog->SetPromptL( TestResult );
//    dialog->RunLD();			
	}

// ----------------------------------------------------
// CBrCtlSampleAppContainer::WidgetHandlerL()
// ----------------------------------------------------
//	
void 
CBrCtlSampleAppContainer::WidgetHandlerL()	
	{
	if (iBrCtlInterface)
		{
//		iBrCtlSampleAppWidgetHandler->CreateWidgetExtension(iBrCtlInterface);
		}
    }
    
// ----------------------------------------------------
// CBrCtlSampleAppContainer::isIconTest()
// ----------------------------------------------------
//
TBool 
CBrCtlSampleAppContainer::isIconTest()
	{
		return iconFlag; 
	}

// ----------------------------------------------------
// CBrCtlSampleAppContainer::setIconTest()
// ----------------------------------------------------
//	
void 
CBrCtlSampleAppContainer::setIconTest()
	{
		iconFlag = ETrue;
	}
    
// ----------------------------------------------------
// CBrCtlSampleAppContainer::GetBitmapDataL()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::GetBitmapDataL()
    {
    if (iBrCtlInterface)
        {
        _LIT(KUrl, "http://www.yahoo.com");
        
        iBrCtlInterface->LoadUrlL( KUrl );

        }
    }
       
    
// ----------------------------------------------------
// CBrCtlSampleAppContainer::ProcessIcon()
// ----------------------------------------------------
//
void 
CBrCtlSampleAppContainer::ProcessIcon()
	{
//	    _LIT(KUrl, "http://www.yahoo.com");
//	    _LIT(TestResult1, "Favicon returned from Yahoo");
//	    _LIT(TestResult2, "Check the sample code");
//	    _LIT(TestResult3, "Icon data not retrieved");
//	    	    
//	    icon = iBrCtlInterface->GetBitmapData(KUrl, TBrCtlDefs::EBitmapFavicon);
//		if (icon)						
//		{	     
//            CAknQueryDialog* dialog = new (ELeave) CAknQueryDialog();
//            dialog->PrepareLC( R_CONFIRM_QUERY );
//            dialog->SetPromptL( TestResult1 );
//            dialog->RunLD();
//		}
//		else
//			{
//			CAknQueryDialog* dialog = new (ELeave) CAknQueryDialog();
//			dialog->PrepareLC( R_CONFIRM_QUERY );
//			dialog->SetPromptL( TestResult3 );
//			dialog->RunLD();
//			}
//		iconFlag = EFalse;
	}

void CBrCtlSampleAppContainer::LoadurlL()
	{
//	_LIT(KTitle,"Query Dialog");
//	_LIT(KPromptUrl, "Enter url");
//	TBuf<15> defaultUrl;
//	defaultUrl.Copy(_L("www.nokia.com"));
//	HBufC* inputUrl = NULL;
//	CBrCtlSampleAppQueryDialog* dialog = new (ELeave) CBrCtlSampleAppQueryDialog( defaultUrl, inputUrl );
//	CleanupStack::PushL(dialog);
//	_LIT(KHttp,"http://");
//	dialog->PrepareLC( R_DATA_QUERY_WITH_HEADING );
//	dialog->SetHeaderTextL( KTitle );
//	dialog->SetPromptL( KPromptUrl );
//	CleanupStack::Pop(); // dialog
//	TBool ret = dialog->RunLD();
//	if (ret)
//		{
//		HBufC* url = HBufC::NewLC(inputUrl->Length()+KHttp().Length());
//		url->Des().Copy(KHttp);
//		url->Des().Append(*inputUrl);
//		iBrCtlInterface->LoadUrlL(*url);
//		CleanupStack::PopAndDestroy(url);
//		}
	}

// ----------------------------------------------------------------------------
// CWebClientContainer::ClientEvent()
// Called by CWebClientEngine to notify events to user
// ----------------------------------------------------------------------------
//
void CBrCtlSampleAppContainer::ClientEvent( const TDesC& aEventDescription )
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
void CBrCtlSampleAppContainer::ClientHeaderReceived( const TDesC& aHeaderData ) 
    {
//    TRAPD( err, AddToHeaderWindowL( aHeaderData ));
//    if( err )
//        Panic( EClientView );
    }

// ----------------------------------------------------------------------------
// CWebClientContainer::ClientBodyReceived()
// Called by CWebClientEngine when part of response body received
// ----------------------------------------------------------------------------
//
void CBrCtlSampleAppContainer::ClientBodyReceived( const TDesC8& aBodyData ) 
    {
//    TRAPD( err, AddToSourceWindowL( aBodyData ));
//    if( err )
//        Panic( EClientView );
    }


// End of File  

