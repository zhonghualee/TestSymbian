#include "UPPayHttpConnection.h"
#include <commdb.h>
#include <eikenv.h>  //CEikonEnv
#include <es_sock.h> 
#include <CommDbConnPref.h>
#include <apselect.h> 
#include <apdatahandler.h> 
#include <apaccesspointitem.h>
#include <aknappui.h>	// iAvkonAppUi
#include <aknquerydialog.h>
#include <bautils.h>
#ifdef __SERIES60_3X__
#include <e32debug.h>
#endif
#include <f32file.h>
#include <eikenv.h>
#include <ImageConversion.h>
#include <utf.h>
#include <UriUtils.h>
#include <string.h>


CommonSocketConnection::CommonSocketConnection(TUint32 aIapId) :
	CActive(CActive::EPriorityHigh), iAccessPointId(aIapId)
{
	iConnectionState = ENotConnected;
	CActiveScheduler::Add(this);
	iMonitor = NULL;
#ifdef __WINS__
#ifdef __S60_50__
	iAccessPointId = 1;
#else
	iAccessPointId = 3;
#endif
#endif
}

CommonSocketConnection::~CommonSocketConnection()
{
	CloseConnection();
	iConnectionState = ENotConnected;
	if (iMonitor)
	{
		delete iMonitor;
	}
}

TBool CommonSocketConnection::OpenConnection()
{
	switch(iConnectionState)
	{
		case EConnecting:
			return EFalse;
			break;
		case EConnected:
		{
			TUint iConnectionCount = 0;
			iConnection.EnumerateConnections(iConnectionCount);
			if (iConnectionCount>0)
			{
				return ETrue;
			}
			else
			{
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection OpenConnection start");
#endif	
				SetupConnectionL();
				if(!iWait.IsStarted())
				iWait.Start();
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection OpenConnection iWait end");
#endif	
				if(iConnectionState==EConnected)
				{
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection OpenConnection iConnectionState==EConnected");
#endif	
//					if(iMonitor)
//					{
//						delete iMonitor;
//						iMonitor = NULL;
//					}
					iMonitor = CConnMon::NewL(iConnection,iSocketServ,*this);
					iMonitor->Start();
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection OpenConnection return  true");
#endif	
					return ETrue;
				}
				else
				{
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection OpenConnection delete iMonitor ");
#endif	
//					if(iMonitor)
//					{
//						delete iMonitor;
//						iMonitor = NULL;
//					}
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection OpenConnection return  false");
#endif	
					return EFalse;
				}			
			}		
			break;
		}
		case ENotConnected:
		{

#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection OpenConnection start2");
#endif	
				SetupConnectionL();
				if(!iWait.IsStarted())
				iWait.Start();
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection OpenConnection iWait end2");
#endif	
				if(iConnectionState==EConnected)
				{
//					if(iMonitor)
//					{
//						delete iMonitor;
//						iMonitor = NULL;
//					}
					iMonitor = CConnMon::NewL(iConnection,iSocketServ,*this);
					iMonitor->Start();
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection OpenConnection return  true2");
#endif	
					return ETrue;
				}
				else
				{
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection OpenConnection return  false2");
#endif	
					return EFalse;
				}			
			break;
		}
		default:
		{
			return EFalse;
			break;
		}
	}
}

void CommonSocketConnection::CloseConnection()
{
	if (iMonitor)
	{
		delete iMonitor;
		iMonitor = NULL;
	}
	if (iConnectionState == EConnected)
	{
		
		iConnection.Close();
		iSocketServ.Close();
	}
	iConnectionState = ENotConnected;

}

void CommonSocketConnection::CancelConnection()
{
#ifdef XF_DEBUG_HTTP
XF_IO_printf("________________CommonHttpConnection CancelConnection \n");
#endif		
	Cancel();
	iConnectionState = ENotConnected;
	if(iWait.IsStarted())
	{
#ifdef XF_DEBUG_HTTP
XF_IO_printf("________________CommonHttpConnection CancelConnection AsyncStop \n");
#endif	
		iWait.AsyncStop();
	}	
}

void CommonSocketConnection::RunL()
{
	if (iStatus.Int() == KErrNone)
	{
		TUint32 iiapid = iAccessPointId;
		
		User::LeaveIfError(iConnection.GetIntSetting(_L("IAP\\Id"), iiapid));
		//		iConnection.GetIntSetting(_L("IAP\\Id"), iiapid);
		if (iiapid != iAccessPointId)
		{
			iAccessPointId = iiapid;
		}
		Cancel();
		iConnectionState = EConnected;
		if(iWait.IsStarted())
		iWait.AsyncStop();
	}
	else
	{
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection RunL iStatus.Int() == %d\n",iStatus.Int());
#endif	
//		if(iStatus.Int()==KErrCancel)
//		{
			iConnectionState = ENotConnected;
			if(iWait.IsStarted())
			{
#ifdef XF_DEBUG_HTTP
		XF_IO_printf("________________CommonHttpConnection RunL AsyncStop \n");
#endif	
				iWait.AsyncStop();
			}
//		}
//		else
//		{
//			SetupConnectionL();
//		}
	}	
}

void CommonSocketConnection::DoCancel()
{

}

void CommonSocketConnection::SetupConnectionL()
{
	iSocketServ.Connect();
	iSocketServ.ShareAuto();
	iConnection.Open(iSocketServ);
	iStatus = KRequestPending;
	TCommDbConnPref pref;	
	pref.SetDirection(ECommDbConnectionDirectionOutgoing);
	if(iAccessPointId!=INVALID_IAPID)
	{
		pref.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);	
		pref.SetIapId(iAccessPointId);  
		iConnection.Start(pref,iStatus);		
	}
	else
	{
	    //SetDefaultIap(0); //cmnet
		pref.SetDialogPreference(ECommDbDialogPrefPrompt);	
		iConnection.Start(pref,iStatus); 		
	}
	iConnectionState = EConnecting;
	SetActive();
}

const TInt KMaxHeaderValueLength = 128;
_LIT(add1,": ");
_LIT(add2,"\r\n");

void UPPayHttpConnection::ParaseResponseHeaders(RHTTPResponse resp)
{
	RHTTPHeaders headers = resp.GetHeaderCollection();
	THTTPHdrVal aHeaderValue;
	iStatusCode = resp.StatusCode();
	if (iStatusCode >= 200 && iStatusCode < 300)
	{
		
		RStringF contLength = stringPool.OpenFStringL(_L8("Content-Length"));
		headers.GetField(contLength, 0, aHeaderValue);
		contLength.Close();
		if (aHeaderValue.Type() == THTTPHdrVal::KTIntVal)
		{
			iContentLength = aHeaderValue.Int();
		}
		else
		{
			iContentLength = 200 * 1024;
		}
		//		if(iContentStartPos != 0)
		//		{
		//			HBufC8* fieldValBuf = HBufC8::NewLC(KMaxHeaderValueLength);
		//			RStringF  contentrange= stringPool.OpenFStringL(_L8("Content-Range"));
		//			TPtrC8 rawField(fieldValBuf->Des());
		//			if(headers.GetRawField(contentrange,rawField)==KErrNone)
		//			{
		//				fieldValBuf->Des().Zero();
		//			}
		//			contentrange.Close ( );
		//			CleanupStack::PopAndDestroy(fieldValBuf);
		//		}
	}
	//
	//	else
	//	{
	//		Stop(); 
	//		iObserver.StateChange(EHttpError);
	//	}

	if (response_header)
	{
		delete response_header;
	}
	response_header = HBufC8::NewL(2048);
	Mem::FillZ((void*) response_header->Ptr(), 2048);
	TPtr8 headPtr = response_header->Des();
	TVersion ver = resp.Version();
	headPtr.AppendFormat(_L8("HTTP/%d.%d %d "), ver.iMajor, ver.iMinor, iStatusCode);
	headPtr.Append(resp.StatusText().DesC());
	headPtr.Append(add2);

	RHTTPHeaders hdr = resp.GetHeaderCollection();
	THTTPHdrFieldIter it = hdr.Fields();
	THTTPHdrVal fieldVal;
	HBufC8* fieldValBuf = HBufC8::NewLC(KMaxHeaderValueLength);
	while (it.AtEnd() == EFalse)
	{
		RStringTokenF fieldName = it();
		RStringF fieldNameStr = stringPool.StringF(fieldName);
		TPtrC8 rawField(fieldValBuf->Des());
		if (hdr.GetRawField(fieldNameStr, rawField) == KErrNone)
		{
			headPtr.Append(fieldNameStr.DesC());
			headPtr.Append(add1);
			headPtr.Append(rawField);
			headPtr.Append(add2);
			fieldValBuf->Des().Zero();
		}
		++it;
	}
	CleanupStack::PopAndDestroy(fieldValBuf);
	
	if (iStatusCode == 301 || iStatusCode == 302)
	{
		if (iObserver)
		{
			iObserver->StateChange(ERedirect);
		}
	}
}

void UPPayHttpConnection::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
{
	
	switch (aEvent.iStatus)
	{
	case THTTPEvent::ERedirectedPermanently:
	case THTTPEvent::ERedirectedTemporarily:

		break;
	case THTTPEvent::EGotResponseHeaders:
	{
		RHTTPResponse resp = aTransaction.Response();
		ParaseResponseHeaders(resp);
		iEngineStatus = EGetHeader;
		if (iObserver)
		{
			iObserver->StateChange(EGetHeader);
		}
		
		AddTimer();
		//iObserver.ProgressNotify(iContentLength,0);
	}
		break;

	case THTTPEvent::EGotResponseBodyData:
	{
		MHTTPDataSupplier* dataSupplier = aTransaction.Response().Body();
		TPtrC8 ptr;
		TBool isLast = dataSupplier->GetNextDataPart(ptr);
		if (iContentLength > 0)
		{
			if (!iResponseBuffer)
			{
				iResponseBuffer = HBufC8::NewL(iContentLength);
			}
			iResponseBuffer->Des().Append(ptr);
		}
		else
		{
			if (!iResponseBuffer)
			{
				iResponseBuffer = ptr.Alloc();
			}
			else
			{
				iResponseBuffer = iResponseBuffer->ReAllocL(iResponseBuffer->Length() + ptr.Length());
				iResponseBuffer->Des().Append(ptr);
			}
		}
		dataSupplier->ReleaseData();
		if (iResponseBuffer && iObserver)
		{
			iObserver->ProgressNotify(iContentLength, iResponseBuffer->Length());
		}
		if (isLast)
		{
			
			iEngineStatus = EHttpFinished;
			Stop();
			if (iObserver)
			{
				iObserver->ResponseReceivedL(iResponseBuffer);
			}
			if (iObserver)
			{
				iObserver->StateChange(EHttpFinished);
			}

		}
		else
		{
			
			iEngineStatus = EGetBody;
			AddTimer();
		}
	}
		break;

	case THTTPEvent::EResponseComplete:
	{
		iEngineStatus = EHttpFinished;

		if (iResponseBuffer && iObserver)
		{
			iObserver->ProgressNotify(iContentLength, iResponseBuffer->Length());
		}
		if (iObserver)
		{
			iObserver->StateChange(EHttpFinished);
		}
		Stop();

	}
		break;
	case THTTPEvent::ESucceeded:
	{
		iEngineStatus = EHttpFinished;
		if (iResponseBuffer && iObserver)
		{
			iObserver->ProgressNotify(iContentLength, iResponseBuffer->Length());
		}
		if (iObserver)
		{
			iObserver->StateChange(EHttpFinished);
		}
		Stop();
	}
		break;
	case THTTPEvent::EFailed:
	{
		Stop();
		if (iObserver)
		{
			iObserver->StateChange(EHttpError);
		}
	}
		break;
	default:
		break;
	}
}

void UPPayHttpConnection::Close()
{
	//	CommonUtils::WriteLogL(_L("UPPayHttpConnection"), _L("Close()"));
	if (iPeriodicTimer->IsActive())
	{
		iPeriodicTimer->Cancel();
	}
	//	iTransaction.Close();
	iSession.Close();
	//iConnection.Close();
	//iSocketServ.Close(); 	
}

void UPPayHttpConnection::AddHeaderL(RHTTPHeaders aHeaders, TInt aHeaderField, const TDesC8& aHeaderValue)
{
	RStringPool stringPool = iSession.StringPool();
	RStringF valStr = stringPool.OpenFStringL(aHeaderValue);
	THTTPHdrVal headerVal(valStr);
	aHeaders.SetFieldL(stringPool.StringF(aHeaderField, RHTTPSession::GetTable()), headerVal);
	valStr.Close();
}
void UPPayHttpConnection::AddProxy()
{
	RHTTPTransactionPropertySet transactionProperties = iTransaction.PropertySet();
	_LIT8(KAddress,"10.0.0.172:80");
	TBuf8<20> aProxy8(KAddress);
	RStringF prxAddr = iSession.StringPool().OpenFStringL(aProxy8);
	CleanupClosePushL(prxAddr);

	THTTPHdrVal prxUsage(iSession.StringPool().StringF(HTTP::EUseProxy, RHTTPSession::GetTable()));
	transactionProperties.SetPropertyL(iSession.StringPool().StringF(HTTP::EProxyUsage, RHTTPSession::GetTable()), prxUsage);
	transactionProperties.SetPropertyL(iSession.StringPool().StringF(HTTP::EProxyAddress, RHTTPSession::GetTable()), prxAddr);

	CleanupStack::PopAndDestroy(); // prxAddr
}

TInt UPPayHttpConnection::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
{
	
	return KErrNone;
}

void UPPayHttpConnection::SetupConnectionL()
{
	//		iTimer.After(iStatus,10); 
	//		SetActive();
	if ( iSocketConnection->OpenConnection())
	{
#ifdef F_ENCODE_URI
		if(iUriParser->Uri().Validate())
#else
		if (iUriParser.Validate())
#endif
		{
			RStringPool strPool = iSession.StringPool();
			RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
			TInt err = 0;
			//to use our socket server and connection
			TRAP(err,connInfo.SetPropertyL(strPool.StringF(HTTP::EHttpSocketServ, RHTTPSession::GetTable()),
					THTTPHdrVal(iSocketConnection->iSocketServ.Handle())));
			if (!err)
			{
				TRAP(err,connInfo.SetPropertyL(strPool.StringF(HTTP::EHttpSocketConnection, RHTTPSession::GetTable()),
						THTTPHdrVal(REINTERPRET_CAST(TInt, &(iSocketConnection->iConnection)))));
				if (!err)
				{
					TRAP(err,openConnection());
					if (!err)
					{
						return;
					}
					else
					{
						//CommonUtils::WriteLogL(_L("openConnection() false err = "), err);
					}
				}
			}

		}
	}
	//CommonUtils::WriteLogL(_L("OpenConnection false "), _L("EHttpError"));
	if (iObserver)
	{
		iObserver->StateChange(EHttpError);
	}
}

UPPayHttpConnection::UPPayHttpConnection() :
	iObserver(NULL), iResponseBuffer(NULL)//,CActive(CActive::EPriorityHigh)
{
	//	CActiveScheduler::Add(this);
	//	iTimer.CreateLocal();
#ifndef F_ENCODE_URI
	iUri = NULL;
#else
	iUriParser = NULL;
#endif

	TRAPD(err,iSession.OpenL());
	if (err)
	{
		//CommonUtils::WriteLogL(_L("iSession.OpenL() false err ="), err);
	}
	stringPool = iSession.StringPool();
	iResponseBuffer = NULL;
	iEngineStatus = ENotConnected;
	iContentStartPos = 0;
	iContentLength = 0;
	iConnectionOpen = EFalse;
	iPostData = NULL;
	iCurPostDataLength = 0;
	iPostDataLength = 0;
	response_header = NULL;
	iPeriodicTimer = CPeriodic::NewL(CActive::EPriorityStandard);
	iStatusCode = -1;
	iIsGetMethod = ETrue;
	iSocketConnection = new CommonSocketConnection();
}

UPPayHttpConnection::~UPPayHttpConnection()
{
	iStatusCode = -1;
	iObserver = NULL;
	Close();
	if (iPeriodicTimer->IsActive())
	{
		iPeriodicTimer->Cancel();
	}
	delete iPeriodicTimer;
	iPeriodicTimer = NULL;
	//	Cancel();
	//	iTimer.Close();

	if (iResponseBuffer)
	{
		delete iResponseBuffer;
		iResponseBuffer = NULL;
	}
	if (response_header)
	{
		delete response_header;
		response_header = NULL;
	}
	if (iPostData)
	{
		delete iPostData;
		iPostData = NULL;
	}
#ifdef F_ENCODE_URI
	if(iUriParser)
	{
		delete iUriParser;
		iUriParser = NULL;
	}
#else
	delete iUri;
#endif

	if(iSocketConnection)
	{
		delete iSocketConnection;
		iSocketConnection = NULL;
	}
}

bool UPPayHttpConnection::openConnection()
{
	RStringF method = iSession.StringPool().StringF(HTTP::EGET, RHTTPSession::GetTable());
	if (!iIsGetMethod)
	{
		method = iSession.StringPool().StringF(HTTP::EPOST, RHTTPSession::GetTable());
	}
#ifdef F_ENCODE_URI
	iTransaction = iSession.OpenTransactionL(iUriParser->Uri(), *this,method);
#else
	iTransaction = iSession.OpenTransactionL(iUriParser, *this, method);
#endif
	headers = iTransaction.Request().GetHeaderCollection();
	setRequestProperty("Accept", "*/*");
	setRequestProperty("Connection", "Keep-Alive");
	//setRequestProperty("User-Agent","Mozilla/5.0 (SymbianOS/9.1;U;[en];NokiaN73-1/3.0638.0.0.1 Series60/3.0) AppleWebKit/413 (KHTML, like Gecko) Safari/413");

	//#endif		
	if (iContentStartPos != 0)
	{
		TBuf8<100> range;
		range.Append(_L8("bytes="));
		range.AppendNum(iContentStartPos);
		range.Append(_L8("-\0\0"));
		char* temp = new char[range.Length() + 1];
		memset(temp, 0, range.Length() + 1);
		memcpy(temp, range.Ptr(), range.Length());
		setRequestProperty("RANGE", (const char *) temp);
		//CommonUtils::WriteLogL(_L8("RANGE:"), range);
		delete temp;
		
	}
	return true;
}


bool UPPayHttpConnection::sendRequest()
{

	if (iResponseBuffer)
	{
		delete iResponseBuffer;
		iResponseBuffer = NULL;
	}

	if (/*iPostData != NULL && */iPostDataLength != 0)//post
	{
		MHTTPDataSupplier* dataSupplier = this;
		iTransaction.Request().SetBody(*dataSupplier);
	}
	iConnectionOpen = ETrue;
	iTransaction.SubmitL();
	iEngineStatus = EConnecting;
	//CommonUtils::WriteLogL(_L("F_HttpConnection::sendRequest SubmitL iCurPostDataLength="), iCurPostDataLength);
	return true;
}

void UPPayHttpConnection::setRequestProperty(const char *property, const char *value)
{
	TInt len = User::StringLength((const TUint8*) property);
	if (!len)
		return;
	TPtr8 namePtr((TUint8 *) property, len, len);
	len = User::StringLength((const TUint8*) value);
	if (!len)
		return;
	TPtr8 valuePtr((TUint8 *) value, len, len);
	//CommonUtils::WriteLogL(namePtr, valuePtr);
	RStringF val;
	val = stringPool.OpenFStringL(namePtr);
	headers.SetRawFieldL(val, valuePtr, _L8(";"));
	val.Close();
}

void UPPayHttpConnection::AddTimer()
{
	if (iPeriodicTimer->IsActive())
	{
		iPeriodicTimer->Cancel();
	}
	iPeriodicTimer->Start(60000000, 60000000, TCallBack(UPPayHttpConnection::Period, this));
}

TInt UPPayHttpConnection::Period(TAny * aPtr)
{
	((UPPayHttpConnection*) aPtr)->DoPeriodTask();
	return ETrue;
}

void UPPayHttpConnection::DoPeriodTask()
{
	if (iPeriodicTimer->IsActive())
	{
		iPeriodicTimer->Cancel();
	}
	switch (iEngineStatus)
	{
	case EConnecting:
	case ENotConnected:
	case EGetHeader:		
		Stop();
		if (iObserver)
		{
			iObserver->StateChange(EHttpGetHeaderTimeOut);
		}
		break;
	case EGetBody:
		Stop();
		if (iObserver)
		{
			iObserver->StateChange(EHttpGetBodyTimeOut);
		}
		break;
	default:		
//		UPPAY_printf("HttpConnection::DoPeriodTask error state: %d, at %s line %d\n", 
//							iEngineStatus, __FILE__, __LINE__);
		break;
	}
}

TBool UPPayHttpConnection::GetNextDataPart(TPtrC8& aDataPart)
{
	if (iPostData)
	{
		aDataPart.Set((const TUint8 *) iPostData, iCurPostDataLength);
	}
	return ETrue;
}
void UPPayHttpConnection::ReleaseData()
{
	if (iPostData)
	{
		delete iPostData;
		iPostData = NULL;
	}
}
TInt UPPayHttpConnection::OverallDataSize()
{
	if (iPostData)
		return iPostDataLength;
	else
		return KErrNotFound;
}
TInt UPPayHttpConnection::Reset()
{
	return KErrNone;
}

void UPPayHttpConnection::ParseUriL(const TDesC& aUri)
{
#ifdef F_ENCODE_URI
	TUriParser16 uriparser;
	uriparser.Parse(aUri);
	iUriParser = UriUtils::ConvertToInternetFormL(uriparser);
#else
	delete iUri;
	iUri = NULL;
	iUri = HBufC8::NewL(aUri.Length());
	iUri->Des().Copy(aUri);
	User::LeaveIfError(iUriParser.Parse(*iUri));
#endif
}

void UPPayHttpConnection::GetRequestL(const TDesC& aUri, TInt aStartPos)
{
	iContentStartPos = aStartPos;
	iContentLength = 0;
	iStatusCode = -1;
	//CommonUtils::WriteLogL(_L("F_HttpConnection::GetRequestL:"), aUri);
	iIsGetMethod = ETrue;
	ParseUriL(aUri);
	AddTimer();
	
	SetupConnectionL();

}

void UPPayHttpConnection::PostRequestL(const TDesC& aUri, const TUint8 * aPostData, TInt aDataLen)
{
	iContentStartPos = 0;
	iContentLength = 0;
	iStatusCode = -1;
	if (aPostData && aDataLen > 0)
	{
		iPostData = new char[aDataLen];
		iPostDataLength = aDataLen;
		iCurPostDataLength = aDataLen;
		Mem::Copy(iPostData, aPostData, aDataLen);
		iIsGetMethod = EFalse;
	}
	//CommonUtils::WriteLogL(_L("F_HttpConnection::PostRequestL:"), aUri);
	//CommonUtils::WriteLogL(_L("F_HttpConnection::PostRequestL:"), aDataLen);
	ParseUriL(aUri);
	AddTimer();
	
	SetupConnectionL();
}


void UPPayHttpConnection::ReGetRequestL()
{
	iContentLength = 0;
	AddTimer();
	
	SetupConnectionL();
	sendRequest();
}

void UPPayHttpConnection::Stop()
{
	if (iPeriodicTimer->IsActive())
	{
		iPeriodicTimer->Cancel();
	}
	if (iConnectionOpen)
	{
		iTransaction.Close();
		iConnectionOpen = EFalse;
	}
}

void UPPayHttpConnection::ResetVarible()
{
	iIsGetMethod = ETrue;
	iContentLength = 0;
	iContentStartPos = 0;
	if (iResponseBuffer)
	{
		delete iResponseBuffer;
		iResponseBuffer = NULL;
	}
}


/*
 * 
 * 
 * 
 * 	CSyncHttpConnection
 * 
 * 
 * 
 */
CSyncHttpConnection::CSyncHttpConnection() :
	CActive(CActive::EPriorityStandard)
{
	iConnection = new UPPayHttpConnection();
	iConnection->SetObserver(this);
	CActiveScheduler::Add(this);
	iTimer.CreateLocal();
	iWait = new CActiveSchedulerWait();
	iIsCanceled = EFalse;
}

CSyncHttpConnection::~CSyncHttpConnection()
{
	if (iConnection)
	{
		iConnection->SetObserver(NULL);
		delete iConnection;
		iConnection = NULL;
	}
	if(IsActive())
	Cancel();
	iTimer.Close();
	if (iWait && iWait->IsStarted())
	{
		iWait->AsyncStop();
	}
	delete iWait;
	iWait = NULL;
	iIsCanceled = EFalse;
}

void CSyncHttpConnection::RunL()
{
//	if(iWait->IsStarted())
//	{
//		iWait->AsyncStop();
//	}
}

void CSyncHttpConnection::DoCancel()
{

}

void CSyncHttpConnection::CancelRequest()
{
	//CommonUtils::WriteLogL(_L("CSyncHttpConnection::CancelRequest()"), _L("Cancelled"));
	iConnection->Stop();
	iEngineStatus = EHttpError;
	iIsCanceled = ETrue;
	if (iWait->IsStarted())
	{
		iWait->AsyncStop();
	}
}

TInt CSyncHttpConnection::GetStatusCode()
{
	if(iConnection)
	{
		return iConnection->GetStatusCode();		
	}
	return 404;
}

HBufC8* CSyncHttpConnection::GetRespone(const TDesC& aUri, TBool& IsHttpError)
{
	//CommonUtils::WriteLogL(_L("CSyncHttpConnection::GetRespone start"), aUri);
	iConnection->GetRequestL(aUri, 0);
	//SetDefaultHeaders();
	iConnection->sendRequest();
//	iTimer.After(iStatus, 150000000);
//	SetActive();
	if (!iWait->IsStarted())
	{
		iWait->Start();
	}
	IsHttpError = ETrue;
	switch (iEngineStatus)
	{
	case EHttpError:
	case EHttpGetHeaderTimeOut:
	case EHttpGetBodyTimeOut:
		break;
	case EHttpFinished:
	{
		
		if (iConnection->GetStatusCode() >= 200 && iConnection->GetStatusCode() < 300)
		{
			IsHttpError = EFalse;
			return iConnection->GetResponeBuffer();
		}
		else
		{
		}
	}
		break;
	default:
		break;
	}
	

	return NULL;
}

HBufC8* CSyncHttpConnection::PostRespone(const TDesC& aUri, const TDesC8& aData, TBool& IsHttpError)
{
	//CommonUtils::WriteLogL(_L("CSyncHttpConnection::GetRespone start"), aUri);
	iConnection->PostRequestL(aUri, aData.Ptr(), aData.Length());
	//iConnection->setRequestProperty("Content-Type", "application/octet-stream");
	//iConnection->setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
	iConnection->setRequestProperty("Content-Type", "text/plain");
	//SetDefaultHeaders();
	iConnection->sendRequest();
//	iTimer.After(iStatus, 150000000);
//	SetActive();
	if (!iWait->IsStarted())
	{
		iWait->Start();
	}
	IsHttpError = ETrue;
	switch (iEngineStatus)
	{
	case EHttpError:
	case EHttpGetHeaderTimeOut:
	case EHttpGetBodyTimeOut:
		break;
	case EHttpFinished:
	{
		
		if (iConnection->GetStatusCode() >= 200 && iConnection->GetStatusCode() < 300)
		{
			//CommonUtils::WriteLogL(_L("CSyncHttpConnection::GetRespone end"), aUri);
			IsHttpError = EFalse;
			return iConnection->GetResponeBuffer();
		}
		else
		{
		}
	}
		break;
	default:
		break;
	}
	

	
	return NULL;
}

void CSyncHttpConnection::ResponseReceivedL(HBufC8* /*aResponseBuffer*/)
{
	//	TBuf<500> buf;
	//	if(aResponseBuffer->Length()<500)
	//	{
	//		buf.Copy(aResponseBuffer->Des());
	//		RDebug::RawPrint(buf);
	//	}
}

void CSyncHttpConnection::ProgressNotify(TInt /*aContentLength*/, TInt /*aDownloadLenth*/)
{

}

void CSyncHttpConnection::StateChange(THttpEngineState aState)
{
	iEngineStatus = aState;
	//CommonUtils::WriteLogL(_L("CSyncHttpConnection::StateChange  iEngineStatus = "), iEngineStatus);


	
	switch (aState)
	{
	case EHttpError:
	case EHttpGetHeaderTimeOut:
	case EHttpGetBodyTimeOut:
	case EHttpFinished:
		if (iWait->IsStarted())
		{
			iWait->AsyncStop();
		}
		break;
	case EGetHeader:
		break;
	case ERedirect:
	{
		HBufC8* head = iConnection->GetResponeHeader();
		TPtrC8 redirectUrl;
		if (head)
		{
			_LIT8(GOHREF, "Location:");
			TInt redirectUrlIndex = head->FindC(GOHREF);
			if (redirectUrlIndex >= 0)
			{
				redirectUrl.Set(head->Right(head->Length() - redirectUrlIndex - GOHREF().Length()));
				_LIT8(SPACE, " ");
				redirectUrlIndex = redirectUrl.FindC(SPACE);
				while (redirectUrlIndex == 0)
				{
					redirectUrl.Set(redirectUrl.Right(redirectUrl.Length() - redirectUrlIndex - SPACE().Length()));
					redirectUrlIndex = redirectUrl.FindC(SPACE);
				}
				redirectUrlIndex = redirectUrl.FindC(_L8("\r\n"));
				if (redirectUrlIndex >= 0)
				{
					redirectUrl.Set(redirectUrl.Left(redirectUrlIndex));
				}
			}
		}
		if (redirectUrl.Length() > 0)
		{
			TBuf<200> aUri;
			aUri.Copy(redirectUrl);
			//CommonUtils::WriteLogL(_L("CSyncHttpConnection::StateChange redirectUrl = "), aUri);
			iConnection->ResetVarible();
			iConnection->Stop();
			iConnection->GetRequestL(aUri, 0);
			iConnection->sendRequest();
		}
		else
		{
			//CommonUtils::WriteLogL(_L("CSyncHttpConnection::StateChange ERedirect error"), _L(""));
			iEngineStatus = EHttpError;
			if (iWait->IsStarted())
			{
				iWait->AsyncStop();
			}
		}

	}
		break;
	default:
		if (iWait->IsStarted())
		{
			iWait->AsyncStop();
		}
		break;
	}
}
//#include "UPCardMemory.h"

int UPPayNetRequest(const TDesC& aAddr,const char* result, char** response)
{
	int ret = 0; 
	CSyncHttpConnection* httpConnection = new CSyncHttpConnection();
	int len = strlen(result);
	TPtr8 ptr((TUint8 *)result,len,len);
	//CommonUtils::WriteLogL(_L("send!!!!!!!!!!!!!!!!!!!!!:\n"));
	//CommonUtils::WriteLogL(ptr,len);
	TBool error = EFalse;
	//httpConnection->GetRespone(_L("http://www.baidu.com"),error);//http://202.101.25.178:8080/Gateway/MobilePayment  //http://222.66.233.198:8080/Gateway/MobilePayment
	HBufC8* res = httpConnection->PostRespone(aAddr,ptr,error);////
	if(res)
	{
		len = res->Length()+1;
		*response = (char*)User::Alloc(len);
		if(*response)
		{
			memset(*response,0,len);
			Mem::Copy(*response,res->Ptr(),len-1);
			ret = 1;
			//CommonUtils::WriteLogL(_L("\nreceive!!!!!!!!!!!!!!!!!!!!!:\n"));
			//CommonUtils::WriteLogL(res->Des(),ret);
			//CommonUtils::WriteLogL(_L("\nend!!!!!!!!!!!!!!!!!!!!!\n"));
		}
	}
	delete httpConnection;
	return ret;
}

extern "C" int CUPHttpRequest(int ok, const char* result, char** response)
{

	if( 1 == ok && result)
	{
		return UPPayNetRequest(_L("http://222.66.233.198:8080/cmsp2"),result,response);
	}
	else
	{
		//UPPAY_printf("[UPPAY]CUPHttpRequest error!!!!!!1 == ok || result== NULL\n");
		return 0;
	}
}

extern "C" int CUPHttpsRequest(int ok, const char* result, char** response)
{
	if( 1 == ok && result)
	{//
		return UPPayNetRequest(_L("https://58.246.226.97:8443/app/entry"),result,response);//https://172.17.248.34:8442
	}
	else
	{
		//UPPAY_printf("[UPPAY]CUPHttpsRequest error!!!!!!1 == ok || result== NULL\n");
		return 0;
	}	
}
