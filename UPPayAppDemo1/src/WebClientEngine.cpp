/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/


// INCLUDE FILES
#include <avkon.hrh>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <http.h>
//#include <WebClient.rsg>
#include <stringloader.h>

//#include "WebClient.pan"
//#include "WebClient.hrh"
#include "WebClientEngine.h"
//#include "WebClientAppUi.h"
#include "UPPay.h"


// ----------------------------------------------------------------------------
// CWebClientEngine::NewL()
// Creates instance of CWebClientEngine.
// ----------------------------------------------------------------------------
//
CWebClientEngine* CWebClientEngine::NewL( MWebClientObserver& aObserver )
    {
    CWebClientEngine* self = CWebClientEngine::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CWebClientEngine::NewLC()
// Creates instance of CWebClientEngine.
// ----------------------------------------------------------------------------
//
CWebClientEngine* CWebClientEngine::NewLC( MWebClientObserver& aObserver )
    {
    CWebClientEngine* self = new (ELeave) CWebClientEngine( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ----------------------------------------------------------------------------
// CWebClientEngine::CWebClientEngine()
// First phase constructor.
// ----------------------------------------------------------------------------
//
CWebClientEngine::CWebClientEngine( MWebClientObserver& aObserver )
:    iObserver( aObserver ),
     iRunning( EFalse ),
     iApplicationUi( NULL )
    {
    // no implementation required
    }


// ----------------------------------------------------------------------------
// CWebClientEngine::~CWebClientEngine()
// Destructor.
// ----------------------------------------------------------------------------
//
CWebClientEngine::~CWebClientEngine()
    {
    iSession.Close();
    iConnection.Close();
    iSocketServ.Close();
    iApplicationUi = NULL;
    }


// ----------------------------------------------------------------------------
// CWebClientEngine::ConstructL()
// Second phase construction.
// ----------------------------------------------------------------------------
//
void CWebClientEngine::ConstructL()
    {
    // Open RHTTPSession with default protocol ("HTTP/TCP")
    TRAPD( err, iSession.OpenL() );
    if( err != KErrNone ) {
        // Most common error; no access point configured, and session creation
        // leaves with KErrNotFound.
        // Load a string from the resource file and add the error code to string
//        HBufC* textResource = StringLoader::LoadLC( R_WEBCLIENT_IAP_CONF_ERR, err );
//        CAknErrorNote* errorNote;
//        errorNote = new (ELeave) CAknErrorNote;
//        // Show the error Note with textResource loaded with StringLoader.
//        errorNote->ExecuteLD( *textResource);
//        // Pop HBuf from CleanUpStack and Destroy it.
//        CleanupStack::PopAndDestroy( textResource );
        User::Leave( err );
    }

    // Install this class as the callback for authentication requests. When 
    // page requires authentication the framework calls GetCredentialsL to get 
    // user name and password.
    InstallAuthenticationL( iSession );
    }


// ----------------------------------------------------------------------------
// CWebClientEngine::SetHeaderL()
// Used to set header value to HTTP request
// ----------------------------------------------------------------------------
//
void CWebClientEngine::SetHeaderL( RHTTPHeaders aHeaders, 
                                   TInt aHdrField, 
                                   const TDesC8& aHdrValue )
    {
    RStringF valStr = iSession.StringPool().OpenFStringL( aHdrValue );
    CleanupClosePushL( valStr );
    THTTPHdrVal val( valStr );
    aHeaders.SetFieldL( iSession.StringPool().StringF( aHdrField,
        RHTTPSession::GetTable() ), val );
    CleanupStack::PopAndDestroy( &valStr );
    }

// ----------------------------------------------------------------------------
// CWebClientEngine::DumpRespHeadersL(RHTTPTransaction& aTransaction)
// Used to display HTTP header field names and values
// ----------------------------------------------------------------------------
//
void CWebClientEngine::DumpRespHeadersL( RHTTPTransaction& aTransaction )
    {
    RHTTPResponse resp = aTransaction.Response();
    RStringPool strP = aTransaction.Session().StringPool();
    RHTTPHeaders hdr = resp.GetHeaderCollection();
    THTTPHdrFieldIter it = hdr.Fields();

    HBufC* headerField = HBufC::NewLC( KMaxHeaderNameLength + KMaxHeaderValueLength );
    HBufC* fieldValBuf = HBufC::NewLC( KMaxHeaderValueLength );

    while ( it.AtEnd() == EFalse )
        {
        RStringTokenF fieldName = it();
        RStringF fieldNameStr = strP.StringF( fieldName );
        THTTPHdrVal fieldVal;
        if ( hdr.GetField( fieldNameStr, 0, fieldVal ) == KErrNone )
            {
            const TDesC8& fieldNameDesC = fieldNameStr.DesC();
            headerField->Des().Copy( fieldNameDesC.Left( KMaxHeaderNameLength ));
            fieldValBuf->Des().Zero();
            switch ( fieldVal.Type() )
                {
            // the value is an integer
            case THTTPHdrVal::KTIntVal:
                fieldValBuf->Des().Num( fieldVal.Int() );
                break;
            // the value is a case-insensitive string
            case THTTPHdrVal::KStrFVal:
                {
                RStringF fieldValStr = strP.StringF( fieldVal.StrF() );
                const TDesC8& fieldValDesC = fieldValStr.DesC();
                fieldValBuf->Des().Copy( fieldValDesC.Left(KMaxHeaderValueLength ));
                }
                break;
            // the value is a case-sensitive string
            case THTTPHdrVal::KStrVal:
                {
                RString fieldValStr = strP.String( fieldVal.Str() );
                const TDesC8& fieldValDesC = fieldValStr.DesC();
                fieldValBuf->Des().Copy( fieldValDesC.Left(KMaxHeaderValueLength) );
                }
                break;
            // the value is a date/time
            case THTTPHdrVal::KDateVal:
                {
                TDateTime date = fieldVal.DateTime();
                TBuf<KMaxDateTimeStringLength> dateTimeString;
                TTime t( date );
                t.FormatL( dateTimeString,KDateFormat );
                fieldValBuf->Des().Copy( dateTimeString );
                } 
                break;
            // the value is type is unknown
            default:
                break;
                }

            // Display HTTP header field name and value
            headerField->Des().Append( KColon );
            headerField->Des().Append( *fieldValBuf );
            iObserver.ClientHeaderReceived( *headerField );
            
            // Display realm for WWW-Authenticate header
            RStringF wwwAuth = strP.StringF( HTTP::EWWWAuthenticate,RHTTPSession::GetTable() );
            if ( fieldNameStr == wwwAuth )
                {
                // check the auth scheme is 'basic'
                RStringF basic = strP.StringF( HTTP::EBasic,RHTTPSession::GetTable() );
                RStringF realm = strP.StringF( HTTP::ERealm,RHTTPSession::GetTable() );
                THTTPHdrVal realmVal;
                if (( fieldVal.StrF() == basic ) && 
                    ( !hdr.GetParam( wwwAuth, realm, realmVal )))
                    {
                    RStringF realmValStr = strP.StringF( realmVal.StrF() );
                    fieldValBuf->Des().Copy( realmValStr.DesC() );
                    headerField->Des().Copy( Krealm );
                    headerField->Des().Append( *fieldValBuf );
                    iObserver.ClientHeaderReceived( *headerField );
                    }
                }
            }
        ++it;
        }
        CleanupStack::PopAndDestroy( fieldValBuf );
        CleanupStack::PopAndDestroy( headerField );
    }

// ----------------------------------------------------------------------------
// CWebClientEngine::HandleRunErrorL()
// Called from MHFRunError() when *leave* occurs in handling of transaction event.
// ----------------------------------------------------------------------------
//
void CWebClientEngine::HandleRunErrorL( TInt aError )
    {
    // Load a string from the resource file and add the leave code to string
//    HBufC* textResource = StringLoader::LoadL( R_WEBCLIENT_MHFRUN_ERROR, aError );
//    // Notify about the error
//    iObserver.ClientEvent( *textResource );
//    CleanupStack ::PopAndDestroy( textResource );
//    if ( iApplicationUi )
//        iApplicationUi->RemoveWaitDialogL();
    }

// ----------------------------------------------------------------------------
// CWebClientEngine::IssueHTTPGetL()
// Start a new HTTP GET transaction.
// ----------------------------------------------------------------------------
//
void CWebClientEngine::IssueHTTPGetL( const TDesC8& aUri )
    {
    SetupConnectionL();

    // Parse string to URI (as defined in RFC2396)
    TUriParser8 uri;
    uri.Parse( aUri );

    // Get request method string for HTTP GET
    RStringF method = iSession.StringPool().StringF( HTTP::EGET,
        RHTTPSession::GetTable());

    // Open transaction with previous method and parsed uri. This class will
    // receive transaction events in MHFRunL and MHFRunError.
    iTransaction = iSession.OpenTransactionL( uri, *this, method );

    // Set headers for request; user agent and accepted content type
    RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
    SetHeaderL( hdr, HTTP::EUserAgent, KUserAgent );
    SetHeaderL( hdr, HTTP::EAccept, KAccept );

    // Submit the transaction. After this the framework will give transaction
    // events via MHFRunL and MHFRunError.
    iTransaction.SubmitL();

    iRunning = ETrue;

//    // Load a string from the resource file 
//    HBufC* textResource = StringLoader::LoadLC( R_WEBCLIENT_CONNECTING );
//    iObserver.ClientEvent( *textResource );
//    CleanupStack::PopAndDestroy( textResource );
   
   }


// ----------------------------------------------------------------------------
// CWebClientEngine::CancelTransactionL()
// Cancels currently running transaction and frees resources related to it.
// ----------------------------------------------------------------------------
//
void CWebClientEngine::CancelTransactionL()
    {
    if( !iRunning ) 
        return;

    // Close() also cancels transaction (Cancel() can also be used but 
    // resources allocated by transaction must be still freed with Close())
    iTransaction.Close();

    // Not running anymore
    iRunning = EFalse;

//    // Load a string from the resource file 
//    HBufC* textResource = StringLoader::LoadLC( R_WEBCLIENT_TRANSACTION_CANCEL );
//    iObserver.ClientEvent( *textResource );
//    CleanupStack::PopAndDestroy( textResource );
    }

// ----------------------------------------------------------------------------
// CWebClientEngine::SetCallBack()
// Set pointer to CApplicationUi instance
// ----------------------------------------------------------------------------
//
void CWebClientEngine::SetCallBack( CWebClientAppUi* aCallBack )
    {
    iApplicationUi = aCallBack;
    }

// ----------------------------------------------------------------------------
// CWebClientEngine::MHFRunL()
// Inherited from MHTTPTransactionCallback
// Called by framework to pass transaction events.
// ----------------------------------------------------------------------------
//
void CWebClientEngine::MHFRunL( RHTTPTransaction aTransaction, 
                                const THTTPEvent& aEvent )
    {

    switch ( aEvent.iStatus ) 
        {
        case THTTPEvent::EGotResponseHeaders:
            {
            // HTTP response headers have been received. Use
            // aTransaction.Response() to get the response. However, it's not
            // necessary to do anything with the response when this event occurs.

            // Get HTTP status code from header (e.g. 200)
//            RHTTPResponse resp = aTransaction.Response();
//            TInt status = resp.StatusCode();
//
//            // Get status text (e.g. "OK")
//            TBuf<KMaxStatusTextLength> statusText;
//            statusText.Copy( resp.StatusText().DesC() );

//            // Load a string from the resource file and add status and status text
//            HBufC* textResource = StringLoader::LoadLC( R_WEBCLIENT_HEADER_RECEIVED,
//                                                        statusText, status );
//            iObserver.ClientEvent( *textResource );
//            CleanupStack::PopAndDestroy( textResource );

            // Display header field names and value
//            DumpRespHeadersL( aTransaction );

            }
            break;

        case THTTPEvent::EGotResponseBodyData:
            {
            // Part (or all) of response's body data received. Use 
            // aTransaction.Response().Body()->GetNextDataPart() to get the actual
            // body data.

            // Get the body data supplier
            MHTTPDataSupplier* body = aTransaction.Response().Body();
            TPtrC8 dataChunk;

            // GetNextDataPart() returns ETrue, if the received part is the last 
            // one.
            TBool isLast = body->GetNextDataPart( dataChunk );
            iObserver.ClientBodyReceived( dataChunk );                
            
            	
            //CEikonEnv::Static()->InfoMsg(aData);
            
//            TBuf<128> url;
//            TUint32 iapid = 1;
//            url.Copy(dataChunk);               
            
//            // Load a string from the resource file and add data length to string
//            HBufC* textResource = StringLoader::LoadLC( R_WEBCLIENT_BODY_PART_RECEIVED,
//                                                                   dataChunk.Length() );
//            iObserver.ClientEvent(url );
            
//        	CAknInformationNote* informationNote;
//        	informationNote = new (ELeave) CAknInformationNote;
//        	// Show the information Note
//        	informationNote->ExecuteLD(url);
            
//            CEikonEnv::Static()->InfoMsg(_L("client event sent"));
//            CleanupStack::PopAndDestroy( textResource );   
            }
            break;

        case THTTPEvent::EResponseComplete:
            {
            // Indicates that header & body of response is completely received.
            // No further action here needed.
//            HBufC* textResource = StringLoader::LoadLC( R_WEBCLIENT_TRANSACTION_COMPLETE );
//            iObserver.ClientEvent( *textResource );
//            CleanupStack::PopAndDestroy( textResource );
            }
            break;

        case THTTPEvent::ESucceeded:
            {
            // Indicates that transaction succeeded. 
//            HBufC* textResource = StringLoader::LoadLC( R_WEBCLIENT_TRANSACTION_SUCCEED );
//            iObserver.ClientEvent( *textResource );
//            CleanupStack::PopAndDestroy( textResource );
//            if ( iApplicationUi )
//                iApplicationUi->RemoveWaitDialogL();

            // Transaction can be closed now. It's not needed anymore.
            aTransaction.Close();
            iRunning = EFalse;
            }
            break;

        case THTTPEvent::EFailed:
            {
            // Transaction completed with failure. 
//            HBufC* textResource = StringLoader::LoadLC( R_WEBCLIENT_TRANSACTION_FAILED );
//            iObserver.ClientEvent( *textResource );
//            CleanupStack::PopAndDestroy( textResource );
//            if ( iApplicationUi )
//                iApplicationUi->RemoveWaitDialogL();
            aTransaction.Close();
            iRunning = EFalse;
            }
            break;

        default:
            // There are more events in THTTPEvent, but they are not usually 
            // needed. However, event status smaller than zero should be handled 
            // correctly since it's error.
            {
//            HBufC* textResource;
            if ( aEvent.iStatus < 0 )
                {
//                // Load a string from the resource file and add status to string
//                textResource = StringLoader::LoadLC( R_WEBCLIENT_TRANSACTION_ERROR,
//                                                     aEvent.iStatus );
//                if ( iApplicationUi )
//                    iApplicationUi->RemoveWaitDialogL();
                // Just close the transaction on errors
                aTransaction.Close();
                iRunning = EFalse;
                } else {
//                // Other events are not errors (e.g. permanent and temporary redirections)
//                textResource = StringLoader::LoadLC( R_WEBCLIENT_UNRECOGNISED_EVENT,
//                                                     aEvent.iStatus );
                }
//            iObserver.ClientEvent( *textResource );
//            CleanupStack::PopAndDestroy( textResource );
            }
            break;
        }
    }

// ----------------------------------------------------------------------------
// CWebClientEngine::MHFRunError()
// Inherited from MHTTPTransactionCallback
// Called by framework when *leave* occurs in handling of transaction event.
// These errors must be handled, or otherwise HTTP-CORE 6 panic is thrown.
// ----------------------------------------------------------------------------
//
TInt CWebClientEngine::MHFRunError( TInt aError, 
                                    RHTTPTransaction /*aTransaction*/, 
                                    const THTTPEvent& /*aEvent*/)
    {
    // Handle error and return KErrNone.
    TRAPD( err, HandleRunErrorL( aError ) );
//    if( err )
//        Panic( EClientEngine );
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CWebClientEngine::GetCredentialsL()
//
// Inherited from MHTTPAuthenticationCallback
// Called by framework when we requested authenticated page and framework 
// needs to know username and password.
// ----------------------------------------------------------------------------
TBool CWebClientEngine::GetCredentialsL( const TUriC8& /*aUri*/,
                                               RString aRealm, 
                                               RStringF /*aAuthenticationType*/,
                                               RString& aUsername, 
                                               RString& aPassword) 
    {
    // aURI, aReal and aAuthenticationType are informational only. We only need 
    // to set aUsername and aPassword and return ETrue, if aUsername and 
    // aPassword are provided by user.

    // Query user name and password
//    TBuf<KMaxUserNameLength> userName;
//    TBuf<KMaxPasswordLength> password;
//    CAknMultiLineDataQueryDialog* dlg = 
//        CAknMultiLineDataQueryDialog::NewL( userName, password );
//
//    if ( !dlg->ExecuteLD( R_WEBCLIENT_DIALOG_USER_PASSWORD_QUERY ))
//        return EFalse; // No credentials given; must return EFalse
//
//    // Set aUsername and aPassword
//    TBuf8<KMaxUserNameLength> temp;
//    temp.Copy( userName );
//    TRAPD( err, aUsername = aRealm.Pool().OpenStringL( temp ));
//    if ( !err ) 
//        {
//        temp.Copy( password );
//        TRAP( err, aPassword = aRealm.Pool().OpenStringL( temp ));
//        if ( !err ) return ETrue;
//        }

    // Return ETrue if user has given credentials (username and password), 
    // otherwise EFlase
    return EFalse;
    }

// ----------------------------------------------------------------------------
// CWebClientEngine::SetupConnectionL()
//
// The method sets the connection setups.
// ----------------------------------------------------------------------------
void CWebClientEngine::SetupConnectionL()
    {
    if( iConnectionSetupDone )
        return;

    iConnectionSetupDone = ETrue;

    //open socket server and start the connection
    User::LeaveIfError(iSocketServ.Connect());
    User::LeaveIfError(iConnection.Open(iSocketServ));
    User::LeaveIfError(iConnection.Start());

    //set the sessions connection info
    RStringPool strPool = iSession.StringPool();
    RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();

    //to use our socket server and connection
    connInfo.SetPropertyL ( strPool.StringF(HTTP::EHttpSocketServ,
        RHTTPSession::GetTable() ), THTTPHdrVal (iSocketServ.Handle()) );

    connInfo.SetPropertyL ( strPool.StringF(HTTP::EHttpSocketConnection,
        RHTTPSession::GetTable() ),
        THTTPHdrVal (REINTERPRET_CAST(TInt, &(iConnection))) );
    }

