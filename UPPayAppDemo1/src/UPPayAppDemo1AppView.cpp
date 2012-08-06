/*
 ============================================================================
 Name		: UPPayAppDemo1AppView.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Application view implementation
 ============================================================================
 */

// INCLUDE FILES
#include <BAUTILS.h>
#include <s32file.h> //RFileWriteStream 
#include <f32file.h> //RFs
#include <S32FILE.H> 
#include <coemain.h>
#include <aknnotewrappers.h>
#include "UPPayAppDemo1AppView.h"
#include "MVIWebClientObserver.h"

#include "UPPayHttpConnection.h"
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUPPayAppDemo1AppView* CUPPayAppDemo1AppView::NewL(const TRect& aRect)
	{
	CUPPayAppDemo1AppView* self = CUPPayAppDemo1AppView::NewLC(aRect);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUPPayAppDemo1AppView* CUPPayAppDemo1AppView::NewLC(const TRect& aRect)
	{
	CUPPayAppDemo1AppView* self = new (ELeave) CUPPayAppDemo1AppView;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUPPayAppDemo1AppView::ConstructL(const TRect& aRect)
	{
	// Create a window for this application view
	CreateWindowL();

	// Set the windows size
	SetRect(aRect);

	// Activate the window, which makes it ready to be drawn
	ActivateL();
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppView::CUPPayAppDemo1AppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CUPPayAppDemo1AppView::CUPPayAppDemo1AppView()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppView::~CUPPayAppDemo1AppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CUPPayAppDemo1AppView::~CUPPayAppDemo1AppView()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CUPPayAppDemo1AppView::Draw(const TRect& /*aRect*/) const
	{
	// Get the standard graphics context
	CWindowGc& gc = SystemGc();

	// Gets the control's extent
	TRect drawRect(Rect());

	// Clears the screen
	gc.Clear(drawRect);

	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CUPPayAppDemo1AppView::SizeChanged()
	{
	DrawNow();
	}

// -----------------------------------------------------------------------------
// CUPPayAppDemo1AppView::HandlePointerEventL()
// Called by framework to handle pointer touch events.
// Note: although this method is compatible with earlier SDKs, 
// it will not be called in SDKs without Touch support.
// -----------------------------------------------------------------------------
//
void CUPPayAppDemo1AppView::HandlePointerEventL(
		const TPointerEvent& aPointerEvent)
	{

	// Call base class HandlePointerEventL()
	CCoeControl::HandlePointerEventL(aPointerEvent);
	}

void CUPPayAppDemo1AppView::UPPayEvent(const TDesC8& aResult)
{
//	//success fail cancel
//	if( !aResult.Compare(_L8("success")) )
//		NotifyUser(_L("\u652F\u4ED8\u6210\u529F\uFF01"));
//	else if( !aResult.Compare(_L8("fail")) )
//		NotifyUser(_L("\u652F\u4ED8\u5931\u8D25\uFF01"));
//	else if( !aResult.Compare(_L8("cancel")) )
//		NotifyUser(_L("\u652F\u4ED8\u53D6\u6D88\uFF01"));
}
void CUPPayAppDemo1AppView::NotifyUser(const TDesC& aData)
{
	CAknInformationNote* informationNote;
	informationNote = new (ELeave) CAknInformationNote;
	// Show the information Note
	informationNote->ExecuteLD(aData);
	
//	CEikonEnv::Static()->InfoMsg(aData);
}

void stringToDescriptor(const char* aString, TDes& aDescriptor)
{
    TPtrC8 ptr(reinterpret_cast<const TUint8*>(aString));
    aDescriptor.Copy(ptr);
}
extern HBufC8* UPPayDemoNetRequest(const TDesC& aAddr,TUint32 aApid);

_LIT( KPaidFileName, "e:\\Others\\paidresult3.txt");

void Append8ToStreamL(RFs& aFs,
			const TDesC& aFileName, const TDesC8&);

void Append16ToStreamL(RFs& aFs,
		const TDesC& aFileName, const TDesC& aUrl);

// ============================ MEMBER FUNCTIONS ===============================
void Append8ToStreamL(RFs& aFs,
		const TDesC& aFileName, const TDesC8& aUrl)
	{
	// Open the file stream.
	RFileWriteStream writeStream;
	TInt err = KErrNone;
	if (BaflUtils::FileExists(CCoeEnv::Static()->FsSession(), aFileName))
		{
		err = writeStream.Open(aFs, aFileName, EFileWrite);
		}
	else
		{
		err = writeStream.Create(aFs, aFileName, EFileWrite);
		}
	User::LeaveIfError(err);
	writeStream.PushL();
	
	// Move the file pointer to the end of file.
	writeStream.Sink()->SeekL(MStreamBuf::EWrite, EStreamEnd, 0);
	
	// Write a TInt32 at the end of file.
	//writeStream.WriteUint32L(aValue);
	writeStream.WriteL(aUrl);
	
	// Commit the change and close the file.
	writeStream.CommitL();
	CleanupStack::Pop(&writeStream);
	writeStream.Release();
	}


void Append16ToStreamL(RFs& aFs,
		const TDesC& aFileName, const TDesC& aUrl)
	{
	// Open the file stream.
	RFileWriteStream writeStream;
	TInt err = KErrNone;
	if (BaflUtils::FileExists(CCoeEnv::Static()->FsSession(), aFileName))
		{
		err = writeStream.Open(aFs, aFileName, EFileWrite);
		}
	else
		{
		err = writeStream.Create(aFs, aFileName, EFileWrite);
		}
	User::LeaveIfError(err);
	writeStream.PushL();
	
	// Move the file pointer to the end of file.
	writeStream.Sink()->SeekL(MStreamBuf::EWrite, EStreamEnd, 0);
	
	// Write a TInt32 at the end of file.
	//writeStream.WriteUint32L(aValue);
	writeStream.WriteL(aUrl);
	
	// Commit the change and close the file.
	writeStream.CommitL();
	CleanupStack::Pop(&writeStream);
	writeStream.Release();
	}

TBool 
CUPPayAppDemo1AppView::GetItemsNameL(const TDesC8& aDescriptor)
    {
	 _LIT(KSubmitTime, "\"CreatedTime\":\"");
	 _LIT(KOrderId, "\"Id\":\"");
	 _LIT(KMerchantId, "\"SPId\":\"");
	 _LIT(KMerchantName, "\"SPName\":\"");				
	 _LIT(KTransAmount, "\"Sum\":\"");
	 _LIT(KSignature, "\"senderSignature\":\"");

	 _LIT(KSeparator, "\"");

	 TInt count;
	 TInt index = 0;
	 TBool drvLetter = EFalse;
	 TUint16 c;

	 TUint16 pos;
	 
	 TBuf<1024> buf;
	 buf.Copy(aDescriptor);
	 
	 // Verify the file scheme
	 TPtrC urlPtr(buf);
	
	 pos = urlPtr.FindF(KSubmitTime);


	 urlPtr.Set(urlPtr.Mid(pos));    
	 urlPtr.Set(urlPtr.Mid(KSubmitTime().Length()));   


	 pos = urlPtr.FindF(KSeparator);  
	 submitTime = HBufC16::NewL(pos);

	 c = urlPtr[index++];
	 while(c != '"'){
		 submitTime->Des().Append(&c, 1);    	
		 c = urlPtr[index++];
	 }
	 NotifyUser(_L("3"));
	 index =0;
	 pos = urlPtr.FindF(KOrderId);


	 urlPtr.Set(urlPtr.Mid(pos));    
	 urlPtr.Set(urlPtr.Mid(KOrderId().Length()));   


	 pos = urlPtr.FindF(KSeparator);  
	 orderId = HBufC16::NewL(pos);

	 c = urlPtr[index++];
	 while(c != '"'){
		 orderId->Des().Append(&c, 1);    	
		 c = urlPtr[index++];
	 }
	 
	
	 index =0;
	 pos = urlPtr.FindF(KMerchantId);


	 urlPtr.Set(urlPtr.Mid(pos));    
	 urlPtr.Set(urlPtr.Mid(KMerchantId().Length()));   


	 pos = urlPtr.FindF(KSeparator);  
	 merchantId = HBufC16::NewL(pos);

	 c = urlPtr[index++];
	 while(c != '"'){
		 merchantId->Des().Append(&c, 1);    	
		 c = urlPtr[index++];
	 }
	
	 index =0;
	 pos = urlPtr.FindF(KMerchantName);


	 urlPtr.Set(urlPtr.Mid(pos));    
	 urlPtr.Set(urlPtr.Mid(KMerchantName().Length()));   


	 pos = urlPtr.FindF(KSeparator);  
	 merchantName = HBufC16::NewL(pos);

	 c = urlPtr[index++];
	 while(c != '"'){
		 merchantName->Des().Append(&c, 1);    	
		 c = urlPtr[index++];
	 }
	

	 index =0;
	 pos = urlPtr.FindF(KTransAmount);


	 urlPtr.Set(urlPtr.Mid(pos));    
	 urlPtr.Set(urlPtr.Mid(KTransAmount().Length()));   


	 pos = urlPtr.FindF(KSeparator);  
	 transAmount = HBufC16::NewL(pos);

	 c = urlPtr[index++];
	 while(c != '"'){
		 transAmount->Des().Append(&c, 1);    	
		 c = urlPtr[index++];
	 }

	 index =0;
	 pos = urlPtr.FindF(KSignature);


	 urlPtr.Set(urlPtr.Mid(pos));    
	 urlPtr.Set(urlPtr.Mid(KSignature().Length()));   


	 pos = urlPtr.FindF(KSeparator);  
	 signature = HBufC16::NewL(pos);

	 c = urlPtr[index++];
	 while(c != '"'){
		 signature->Des().Append(&c, 1);    	
		 c = urlPtr[index++];
	 }
	
	 return true;	 
}


TBool CUPPayAppDemo1AppView::SetUPPayPAAInfo(const TDesC8& aDescriptor)
{			  
	  _LIT(KPrefix, "<?xml version=\"1.0\" encoding=\"UTF-8\"?><cupMobile application=\"ALL\" version=\"1.01\"><transaction type=\"Purchase.MARsp\"><submitTime>");
	  TBuf16<2048> buf(KPrefix);
	  TBuf8<2048>  uri8;	 	  
	  GetItemsNameL(aDescriptor);
#if 1
	  //buf.Append((const unsigned short*)submitTime->Des().PtrZ(), submitTime->Length());
	  buf+= submitTime->Des();
	  buf+=(_L("</submitTime><order id=\""));
	  buf += orderId->Des();
	  //buf.Append((const unsigned short*)orderId->Des().PtrZ(), orderId->Length());
	  buf+=(_L("\">order5</order><transAmount currency=\"156\">")); 
	  //buf.Append((const unsigned short*)transAmount->Des().PtrZ(), transAmount->Length());
	  buf += transAmount->Des();
	  
	  buf+=(_L("</transAmount><terminal id=\"01042900\"/><merchant name=\"")); 
	  buf.Append((const unsigned short*)merchantName->Des().PtrZ(), merchantName->Length());
	  
	  
	  buf+=(_L("\" id=\"")); 
	  buf.Append((const unsigned short*)merchantId->Des().PtrZ(), merchantId->Length());
		  
	  buf+=(_L("\"/></transaction><senderSignature>"));
	  
	  buf += signature->Des();
	  //buf.Append((const unsigned short*)signature->Des().PtrZ(), signature->Length());	 
	  
	  buf+=(_L("</senderSignature></cupMobile>")); 	 
	  
	  uri8.FillZ(2048);
	  uri8.Copy(buf);
	  uri8.ZeroTerminate();
	  uri8.SetLength(uri8.Length());
	  NotifyUser(_L("10"));


	  CSyncHttpConnection* httpConnection = new CSyncHttpConnection();
	  TBool error = EFalse;
	  //HBufC8* res = httpConnection->GetRespone(_L("www.baidu.com"), error);
	  //aApid = httpConnection->GetAccessPointId();
	  CommonSocketConnection *connect = new CommonSocketConnection();
	  TInt aApid = connect ->GetAccessPointId();

#endif
	_LIT(KSysProvide,"00000002");
	_LIT(KSpId,"0009");
	TRAPD(err0,iUPPayApp = CUPPayPlugin::NewL());
	if (iUPPayApp)
		iUPPayApp->test(REINTERPRET_CAST(CAknAppUi*, iCoeEnv->AppUi()), this,
				&uri8, KSysProvide, KSpId, aApid);
	else
		NotifyUser(_L("Error"));
	
//	NotifyUser(_L("11"));
//	
//	delete Paa;
//	Paa = NULL;
	
	delete httpConnection;
	delete connect;
	return true;
}// End of File
