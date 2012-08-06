#ifndef _F_HTTPCONNECTION_H_
#define _F_HTTPCONNECTION_H_
//#define F_DEBUG_HTTP
//#define F_ENCODE_URI
#include <e32std.h>
#include <e32base.h>				// CBase
#include <httpstringconstants.h>	// HTTP string table
#include <http\mhttpdatasupplier.h>	// MHTTPDataSupplier
#include <http/rhttpheaders.h>	    // RHTTPHeaders
#include <CommDbConnPref.h>
#include <utf.h>                    //CnvUtfConverter
#include <http\mhttptransactioncallback.h>		// MHTTPTransactionCallback
#include <http\RHTTPSession.h>					// RHTTPSession
#include <http\RHTTPTransaction.h>				// RHTTPTransaction
#include <e32def.h>
#include <commdb.h>
#include <es_sock.h> 
#include <uri8.h>
#include <http/mhttpdatasupplier.h>

#define INVALID_IAPID 1000

class CConnMon;
class CommonSocketConnection;

class CommonSocketConnection:public CActive
{
public:
	enum THttpConnectionState 
	    {
		ENotConnected,
        EConnecting,
        EConnected
	    };
	CommonSocketConnection(TUint32 aIapId = INVALID_IAPID);
	~CommonSocketConnection();
	TBool OpenConnection();
	void CancelConnection();
	void CloseConnection();
	THttpConnectionState GetConnectionState(){return iConnectionState;};
	void SetAccessPointId(TUint32 id)
	{
		iAccessPointId = id;
	}
	TUint32 GetAccessPointId()
	{
		return iAccessPointId;
	}
	;
protected:
	void RunL();
	void DoCancel();	
private:
	void SetupConnectionL();
public:
	RSocketServ iSocketServ;
	RConnection iConnection;	
	THttpConnectionState iConnectionState;
private:
	CActiveSchedulerWait iWait;
	TUint32    iIapId; 
	CConnMon* iMonitor;
	TUint32 iAccessPointId;
};

class CConnMon : public CActive {
public:
    static CConnMon* NewL(RConnection &conn, RSocketServ &sserver,CommonSocketConnection& aCommonHttpConnection)
    {
	CConnMon *self = new (ELeave) CConnMon(conn, sserver,aCommonHttpConnection);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
    }
    
    void Start() 
    {
	conn_.ProgressNotification(nif_progress_, iStatus);
	SetActive();
    }
    
    void Stop() 
    {
	Cancel();
    }
    
    ~CConnMon() 
    { 
    	Stop(); 
    }
    
private:
    CConnMon(RConnection &conn, RSocketServ &sserver,CommonSocketConnection& aCommonHttpConnection) : 
	CActive(EPriorityHigh), 
	conn_(conn), 
	sserver_(sserver),iCommonHttpConnection(aCommonHttpConnection)
    {
	CActiveScheduler::Add(this);
    }
    
    void ConstructL() {}

    void DoCancel() 
    {
	conn_.CancelProgressNotification();
    }

    void RunL()
    {
		if (nif_progress_().iStage == KLinkLayerClosed) 
		{
			iCommonHttpConnection.iConnectionState = CommonSocketConnection::ENotConnected;
		}
		else
		{
			Start();
		}
    }
    
private:
    RConnection& conn_;
    RSocketServ& sserver_;
    TNifProgressBuf nif_progress_;
    CommonSocketConnection& iCommonHttpConnection;
};

enum THttpEngineState
{
	ENotConnected, 
	EConnecting, 
	EGetHeader, 
	ERedirect, 
	EGetBody, 
	EHttpGetHeaderTimeOut,	
	EHttpGetBodyTimeOut,	
	EHttpError, 
	EHttpFinished
};

class MHttpObserver
{
public:
	virtual void ResponseReceivedL(HBufC8* aResponseBuffer) = 0;
	virtual void ProgressNotify(TInt aContentLength, TInt aDownloadLenth) = 0;

	virtual void StateChange(THttpEngineState aState) = 0;
};

class UPPayHttpConnection: public MHTTPTransactionCallback, public MHTTPDataSupplier//public CActive, 
{
public:

	UPPayHttpConnection();
	virtual ~UPPayHttpConnection();
	void SetObserver(MHttpObserver* aObserver)
	{
		iObserver = aObserver;
	};
	void GetRequestL(const TDesC& aUri, TInt aStartPos = 0);
	void PostRequestL(const TDesC& aUri, const TUint8 * aPostData, TInt aDataLen);
	HBufC8* GetResponeBuffer()
	{
		return iResponseBuffer;
	};
	HBufC8* GetResponeHeader()
	{
		return response_header;
	};
	void ResetVarible();
	void Stop();
	void setRequestProperty(const char* property, const char* value);
	bool sendRequest();
	TInt GetStatusCode()
	{
		return iStatusCode;
	}
	;
private:
	void ReGetRequestL();
	bool openConnection();
	void SetupConnectionL();
	TBool GetNextDataPart(TPtrC8& aDataPart);
	void ReleaseData();
	TInt OverallDataSize();
	TInt Reset();
	void AddProxy();
	//protected:
	//	void RunL();
	void DoCancel();
private:
	// from MHTTPTransactionCallback
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	void AddHeaderL(RHTTPHeaders aHeaders, TInt aHeaderField, const TDesC8& aHeaderValue);
	void AddTimer();
	static TInt Period(TAny * aPtr);
	void DoPeriodTask();
	void ParaseResponseHeaders(RHTTPResponse resp);
	void Close();
	void ParseUriL(const TDesC& aUri);

private:
	RHTTPSession iSession;
	RHTTPTransaction iTransaction;
	MHttpObserver* iObserver;
	HBufC8* iResponseBuffer;
	TInt iContentLength;
	TInt iContentStartPos;
	TBool iConnectionOpen;
	TInt iStatusCode;
	TBool iIsGetMethod;
#ifdef F_ENCODE_URI
	CUri8* iUriParser;
#else
	HBufC8* iUri;
	TUriParser8 iUriParser;
#endif
	RHTTPHeaders headers;
	RStringPool stringPool;
	THttpEngineState iEngineStatus;
	CPeriodic* iPeriodicTimer;
	char* iPostData;
	TInt iPostDataLength;
	TInt iCurPostDataLength;
	HBufC8* response_header;
	//	RTimer iTimer;
	
	CommonSocketConnection* iSocketConnection;
};


class CSyncHttpConnection: public CActive, public MHttpObserver
{
public:
	CSyncHttpConnection();
	~CSyncHttpConnection();
	HBufC8* GetRespone(const TDesC& aUri, TBool& IsHttpError);
	HBufC8* PostRespone(const TDesC& aUri, const TDesC8& aData, TBool& IsHttpError);
	TInt GetStatusCode();
	void CancelRequest();
	TBool IsCanceled()
	{
		return iIsCanceled;
	}
	;
private:
	void ResponseReceivedL(HBufC8* aResponseBuffer);
	void ProgressNotify(TInt aContentLength, TInt aDownloadLenth);
	void StateChange(THttpEngineState aState);
protected:
	void RunL();
	void DoCancel();
public:
	//	static TBool iNeedCancel;
private:
	
	UPPayHttpConnection* iConnection;
	CActiveSchedulerWait* iWait;
	RTimer iTimer;
	THttpEngineState iEngineStatus;
	TBool iIsCanceled;
};

extern "C" int CUPHttpRequest(int ok, const char* result, char** response);
extern "C" int CUPHttpsRequest(int ok, const char* result, char** response);
#endif
