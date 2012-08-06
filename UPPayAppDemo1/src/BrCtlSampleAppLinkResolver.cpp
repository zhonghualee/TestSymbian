/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Observer interface which handles resolving embedded links
*
*/


// INCLUDE FILES
#include <apmrec.h>
#include <apgcli.h>
#include <eikappui.h>
#include <eikenv.h>
#include <aknnotewrappers.h>
#include "BrCtlSampleAppLinkResolver.h"
#include "WebClientEngine.h"
#include "MVIWebClientObserver.h"

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CBrCtlSampleAppLinkResolver::ResolveEmbeddedLinkL
// ---------------------------------------------------------
//
TBool 
CBrCtlSampleAppLinkResolver::ResolveEmbeddedLinkL(const TDesC& aEmbeddedUrl,
    		                                      const TDesC& /*aCurrentUrl*/,
        	                                      TBrCtlLoadContentType /*aLoadContentType*/, 
                                                  MBrCtlLinkContent& aEmbeddedLinkContent) 
    {        

   if (IsFileScheme(aEmbeddedUrl))
       {
//        GetFileNameL(aEmbeddedUrl);
//        HBufC8* buf = ReadFileLC(*iFileName);
//        HBufC* contentType = NULL;
//        TPtrC p(NULL, 0);
//        contentType = RecognizeLC(*iFileName, *buf);
//        aEmbeddedLinkContent.HandleResolveComplete(*contentType, p, buf);        
//        CleanupStack::PopAndDestroy(2); // contentType, buf
//        return ETrue;
       }
    return EFalse;
    }

// ---------------------------------------------------------
// CBrCtlSampleAppLinkResolver::ResolveLinkL
// ---------------------------------------------------------
//
TBool 
CBrCtlSampleAppLinkResolver::ResolveLinkL(const TDesC& aUrl, 
                                          const TDesC& /*aCurrentUrl*/,
                                          MBrCtlLinkContent& aBrCtlLinkContent) 
    {        
//	NotifyUser(_L("ResolveLinkL"));
    if (IsFileScheme(aUrl))
        {            
    	//NotifyUser(_L("FileScheme"));
        GetItemsNameL(aUrl);
        CreateOrders();        
        return ETrue;
        }
    return EFalse;
    //return ETrue;
    }

// ---------------------------------------------------------
// CBrCtlSampleAppLinkResolver::CancelAll
// ---------------------------------------------------------
//
void 
CBrCtlSampleAppLinkResolver::CancelAll() 
    {        
    }


// ---------------------------------------------------------
// CBrCtlSampleAppLinkResolver::ConstructL
// ---------------------------------------------------------
//
void 
CBrCtlSampleAppLinkResolver::ConstructL(CBrCtlSampleAppContainer* aContainer)
    {
//	iClient = CWebClientEngine::NewL( *this );
//	iClient->SetCallBack( *this );

	iContainer = aContainer;
	iClientObserver = new MVIWebClientObserver(iContainer);
	
	iClient = CWebClientEngine::NewL( *iClientObserver );	
    }

// ---------------------------------------------------------
// CBrCtlSampleAppLinkResolver::~CBrCtlSampleAppLinkResolver
// ---------------------------------------------------------
//
CBrCtlSampleAppLinkResolver::~CBrCtlSampleAppLinkResolver()
    {
    delete iFileName;    
    delete money;
    delete userName;
    delete SPName;
    delete ID;
    delete login; 
    }

////----------------------------------------------------------
//// CBrCtlSampleAppLayoutObserver::ConstructL
//// ---------------------------------------------------------
////
//void CBrCtlSampleAppLinkResolver::ConstructL(CBrCtlSampleAppContainer* aContainer)
//    {
//    iContainer = aContainer;
//    }

// ---------------------------------------------------------
// CBrCtlSampleAppLinkResolver::NewL
// ---------------------------------------------------------
//
CBrCtlSampleAppLinkResolver* 
CBrCtlSampleAppLinkResolver::NewL(CBrCtlSampleAppContainer* aContainer)
    {
    CBrCtlSampleAppLinkResolver* self = new(ELeave)CBrCtlSampleAppLinkResolver;
    CleanupStack::PushL(self);
    self->ConstructL(aContainer);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// CBrCtlSampleAppLinkResolver::IsFileScheme
// ---------------------------------------------------------
//
TBool 
CBrCtlSampleAppLinkResolver::IsFileScheme(const TDesC& aFileName)
    {
    _LIT(KFileScheme, "startPay://");
    if (aFileName.Length() > 0 && aFileName.Ptr() != NULL)
        {
        if (aFileName.FindF(KFileScheme) == 0)
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// ---------------------------------------------------------
// CBrCtlSampleAppLinkResolver::ReadFile
// ---------------------------------------------------------
//
HBufC8* 
CBrCtlSampleAppLinkResolver::ReadFileLC(const TDesC& aFileName)
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

// ---------------------------------------------------------
// CBrCtlSampleAppLinkResolver::RecognizeL
// ---------------------------------------------------------
//
HBufC* 
CBrCtlSampleAppLinkResolver::RecognizeLC(const TDesC& aFileName, const TDesC8& aData)
    {
    TDataRecognitionResult dataType;
    RApaLsSession apaSession;
    TInt ret;
    HBufC* contentTypeString = NULL;

    User::LeaveIfError(apaSession.Connect());
    // Ask the application architecture to find the file type
    ret = apaSession.RecognizeData(aFileName, aData, dataType);
    apaSession.Close();

    if (ret == KErrNone &&
        (dataType.iConfidence == CApaDataRecognizerType::ECertain) ||
        (dataType.iConfidence == CApaDataRecognizerType::EProbable))
        {
        // If the file type was found, try to match it to a known file type
        TPtrC8 mimeTypePtr = dataType.iDataType.Des8();
        TInt len = mimeTypePtr.Length() + 1;
        contentTypeString = HBufC::NewL(len); 
        contentTypeString->Des().Copy(mimeTypePtr);
        contentTypeString->Des().ZeroTerminate();
        }
    CleanupStack::PushL(contentTypeString);
    return contentTypeString;
    }

// -----------------------------------------------------------------------------
// CBrCtlSampleAppLinkResolver::GetFileNameL
// Translate the file name from a URL to a valid file name in the system.
// -----------------------------------------------------------------------------
//
TBool 
CBrCtlSampleAppLinkResolver::GetItemsNameL(const TDesC& aFileName)
    {
    _LIT(KFileScheme, "startpay://");
    _LIT(KSeparator, "-");
    _LIT(KLogin, "OK");
    
    TInt count;
    TInt index = 0;
    TBool drvLetter = EFalse;
    TUint16 c;
    
    // Verify the file scheme
    TPtrC urlPtr(aFileName);
    if (urlPtr.FindF(KFileScheme) != 0)
    	{
    	return EFalse;
    	}
    urlPtr.Set(urlPtr.Mid(KFileScheme().Length()));

    // make sure there are enough characters in the filename before
    // trying to check them
    count = urlPtr.Length();
    if(count == 0)
    	{
    	return EFalse;            // no filename, so can't look at urlPtr[0]
    	}

    // Skip the first '/' if there is one
    if (urlPtr[0] == '-')
    	{
    	urlPtr.Set(urlPtr.Mid(1));
    	}
    count = urlPtr.Length();
    TUint8 pos;
    pos = urlPtr.FindF(KSeparator);  

    money = HBufC16::NewL(pos);

    c = urlPtr[index++];
    while(c != '-' && index < urlPtr.Length()){
    	money->Des().Append(&c, 1);    	
    	c = urlPtr[index++];
    }
   
    urlPtr.Set(urlPtr.Mid(pos + 1));
    index =0;
    pos = urlPtr.FindF(KSeparator);  

    SPName = HBufC16::NewL(pos);
    c = urlPtr[index++];
    while(c != '-' && index < urlPtr.Length()){
    	SPName->Des().Append(&c, 1);
    	c = urlPtr[index++];
    }

   
    urlPtr.Set(urlPtr.Mid(pos +1));
    
    index =0;
    pos = urlPtr.FindF(KSeparator);  
    ID = HBufC16::NewL(pos);

    c = urlPtr[index++];
    while(c != '-' && index < urlPtr.Length()){
    	ID->Des().Append(&c, 1);
    	c = urlPtr[index++];
    }
  

    urlPtr.Set(urlPtr.Mid(pos +1));
    
    index =0;
    pos = urlPtr.FindF(KSeparator);  


    userName = HBufC16::NewL(pos);

    c = urlPtr[index++];
    while(c != '-' && index < urlPtr.Length()){
    	userName->Des().Append(&c, 1);
    	c = urlPtr[index++];
    }   

    urlPtr.Set(urlPtr.Mid(pos+1));
    index =0;
    
    c = urlPtr[index++];
    if(c == 'O')
    	{
    	c = urlPtr[index++];
    	if(c == 'K')
    		return ETrue; 
    	}
 
    return EFalse;
}


void 
CBrCtlSampleAppLinkResolver::CreateOrders()
{	
	//NotifyUser(_L("CreateOrders"));
	_LIT(KTxtHelloWorld,"http://218.97.244.67:8000/PaymentService.svc/CreateOrders?jsoncallback=?");
	TBuf16<256> buf(KTxtHelloWorld);
	TBuf8<256> uri8;

	buf+=(_L("&SPId=")); 
	buf.Append((const unsigned short*)ID->Des().PtrZ(), ID->Length());

	buf+=(_L("&SPName=")); 
	buf.Append((const unsigned short*)SPName->Des().PtrZ(), SPName->Length());
	
	buf+=(_L("&userId=")); 
	buf.Append((const unsigned short*)userName->Des().PtrZ(), userName->Length());
	
	buf+=(_L("&Sum=")); 
	buf.Append((const unsigned short*)money->Des().PtrZ(), money->Length());	    
    uri8.FillZ(256);
	uri8.Copy(buf);
    uri8.ZeroTerminate();
    uri8.SetLength(uri8.Length());
    iClient->CancelTransactionL();
    iClient->IssueHTTPGetL( uri8 );   
    //NotifyUser(_L("CreateOrders over"));
}

void CBrCtlSampleAppLinkResolver::NotifyUser(const TDesC& aData)
{
	CAknInformationNote* informationNote;
	informationNote = new (ELeave) CAknInformationNote;
	// Show the information Note
	informationNote->ExecuteLD(aData);
	
//	CEikonEnv::Static()->InfoMsg(aData);
}
// End of File  

