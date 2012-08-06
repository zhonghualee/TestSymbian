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


#ifndef WEBCLIENTENGINE_H
#define WEBCLIENTENGINE_H

// INCLUDES
#include <coecntrl.h>
#include <http.h>
#include <http\mhttpauthenticationcallback.h>
#include <es_sock.h>

// CONSTANTS
const TInt KMaxHeaderNameLength     = 32;
const TInt KMaxHeaderValueLength    = 128;
const TInt KMaxAuthTypeLength       = 128;
const TInt KMaxDateTimeStringLength = 40;
const TInt KMaxStatusTextLength     = 32;

// Used user agent for requests
_LIT8( KUserAgent, "WebClient 1.0" );

// This client accepts all content types.
// (change to e.g. "text/plain" for plain text only)
_LIT8( KAccept, "*/*" );

// Format for output of data/time values
_LIT( KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3" );

// Some texts for header output
_LIT( KColon, ": " );
_LIT( Krealm, "Realm: " );


// FORWARD DECLARATIONS
class CWebClientAppUi;

// CLASS DECLARATION

/**
* MWebClientObserver
* CWebClientEngine passes events and responses body data with this interface. 
* An instance of this class must be provided for construction of CWebClientEngine.
*/
class MWebClientObserver 
    {
    public:
        /**
        * ClientEvent()
        * Called when event occurs in CWebClientEngine.
        * @param aEventDescription: A event in textual format, e.g.
        *                           "Transaction Successful"
        */
        virtual void ClientEvent( const TDesC& aEventDescription ) = 0;

    public:
        /**
        * ClientHeaderReceived()
        * Called when HTTP header is received.
        * @param aHeaderData: Header field name and value
        */
        virtual void ClientHeaderReceived( const TDesC& aHeaderData ) = 0;

        /**
        * ClientBodyReceived()
        * Called when a part of the HTTP body is received.
        * @param aBodyData:  Part of the body data received. (e.g. part of
        *                    the received HTML page)
        */
        virtual void ClientBodyReceived( const TDesC8& aBodyData ) = 0;
    };

/**
* CWebClientEngine
* Provides simple interface to HTTP Client API.
*/
class CWebClientEngine : public CBase, 
                         public MHTTPTransactionCallback,
                         public MHTTPAuthenticationCallback
    {
    public:
        /**
        * NewL()
        * Create a CWebClientEngine object.
        * @param  iObserver: 
        * @return A pointer to the created instance of CWebClientEngine
        */
        static CWebClientEngine* NewL( MWebClientObserver& aObserver );

        /**
        * NewLC()
        * Create a CWebClientEngine object.
        * @param  iObserver:
        * @return A pointer to the created instance of CWebClientEngine
        */
        static CWebClientEngine* NewLC( MWebClientObserver& aObserver );

        /**
        * ~CWebClientEngine()
        * Destroy the object
        */
        ~CWebClientEngine();

        /**
        * IssueHTTPGetL()
        * Starts a new HTTP GET transaction.
        * @param aUri: URI to get. (e.g. http://host.org")
        */
        void IssueHTTPGetL( const TDesC8& aUri );

        /**
        * CancelTransactionL()
        * Closes currently running transaction and frees resources related to it.
        */
        void CancelTransactionL();

        /**
        * IsRunning()
        * Checks if the transaction is running.
        * @return ETrue, if transaction is currently running.
        */
        inline TBool IsRunning() { return iRunning; };

        /**
        * SetCallBack()
        * Sets the callback address.
        * @param aCallBack: A pointer to calling instance.
        */
        void SetCallBack( CWebClientAppUi* aCallBack );

    private:
        /**
        * ConstructL()
        * Perform the second phase construction of a CWebClientEngine object.
        */
        void ConstructL();

        /**
        * CWebClientEngine()
        * Perform the first phase of two phase construction.
        * @param iObserver: 
        */
        CWebClientEngine( MWebClientObserver& iObserver );

        /**
        * SetHeaderL()
        * Sets header value of an HTTP request.
        * @param aHeaders:  Headers of the HTTP request
        * @param aHdrField: Enumerated HTTP header field, e.g. HTTP::EUserAgent
        * @param aHdrValue: New value for header field
        */
        void SetHeaderL( RHTTPHeaders aHeaders, TInt aHdrField, 
                         const TDesC8& aHdrValue );

        /**
        * DumpRespHeadersL()
        * Called when HTTP header is received.
        * Displays HTTP header field names and values
        * @param aTransaction: The transaction that is processed.
        */
        void DumpRespHeadersL( RHTTPTransaction& aTransantion );

        /**
        * HandleRunErrorL()
        * Called from MHFRunError() when *leave* occurs in handling of transaction event. 
        * @param aError:       The leave code that occured.
        */
        void HandleRunErrorL( TInt aError );

        /**
        * SetupConnectionL()
        */
        void SetupConnectionL();

    /**
    * From MHTTPSessionEventCallback
    */
    private:
        /**
        * MHFRunL()
        * Called by framework to notify about transaction events.
        * @param aTransaction: Transaction, where the event occured.
        * @param aEvent:       Occured event.
        */
        void MHFRunL( RHTTPTransaction aTransaction, const THTTPEvent& aEvent );

        /**
        * MHFRunError()
        * Called by framework when *leave* occurs in handling of transaction event. 
        * @param aError:       The leave code that occured.
        * @param aTransaction: The transaction that was being processed when leave occured.
        * @param aEvent:       The event that was being processed when leave occured.
        * @return KErrNone,    if the error was handled. Otherwise the value of aError, or
        *                      some other error value. Returning error value causes causes 
        *                      HTTP-CORE 6 panic.
        */
        TInt MHFRunError( TInt aError, 
                          RHTTPTransaction aTransaction, 
                          const THTTPEvent& aEvent );

    /**
    * From MHTTPAuthenticationCallback (needed for HTTP authentication)
    */
    private:
        /**
        * GetCredentialsL()
        * Called by framework when username and password for requested URI is 
        * needed.
        * @param aURI: The URI being requested (e.g. "http://host.org")
        * @param aRealm: The realm being requested (e.g. "user@host.org")
        * @param aAuthenticationType: Authentication type. (e.g. "Basic")
        * @param aUsername: Given user name.
        * @param aPassword: Given password.
        * @return A pointer to the created document
        */
        TBool GetCredentialsL(  const TUriC8& aUri, 
                                RString aRealm, 
                                RStringF aAuthenticationType, 
                                RString& aUsername, 
                                RString& aPassword );

    private: // Data
        RSocketServ             iSocketServ;
        RConnection             iConnection;
        RHTTPSession            iSession;
        RHTTPTransaction        iTransaction;
        MWebClientObserver&     iObserver;      // Used for passing body data and
                                                // events to UI.
        TBool                   iRunning;       // ETrue, if transaction running
        TBool                   iConnectionSetupDone; // ETrue, if connection setup is done
        CWebClientAppUi*        iApplicationUi; // Pointer to AppUi instance
        
        CActiveSchedulerWait* iWait;
    };

#endif // WEBCLIENTENGINE_H
