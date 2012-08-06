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
* Description:  Handle special load requests like Non-HTTP(S) load requests,
*                and Non-HTML content. Control over network connections
*
*/


#ifndef BRCTLSAMPLEAPPSPECIALLOADOBSERVER_H
#define BRCTLSAMPLEAPPSPECIALLOADOBSERVER_H

//  INCLUDES
#include <BrCtlSpecialLoadObserver.h>
#include <BrCtlInterface.h>
//#include <DocumentHandler.h> 

// CLASS DECLARATION

/**
* CBrCtlSampleAppSpecialLoadObserver class.
* This class inherits from the MBrCtlSpecialLoadObserver interface which
* is used if the host application requires: Handling of non-HTTP(S) load requests;
* Handling of non-markup content; Control over network connections
*/
class CBrCtlSampleAppSpecialLoadObserver : public CBase, public MBrCtlSpecialLoadObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CBrCtlSampleAppSpecialLoadObserver* NewL();

        /**
        * Destructor.
        */
        ~CBrCtlSampleAppSpecialLoadObserver();

        /**
        * Request to create a network connection.
        * @since 2.8
        * @param aConnectionPtr A pointer to the new connection. If NULL, the proxy filter will automatically create a network connection
        * @param aSockSvrHandle A handle to the socket server.
        * @param aNewConn A flag if a new connection was created. If the connection is not new, proxy filter optimization will not read the proxy again from CommsBd
        * @param aBearerType The bearer type of the new connection
        * @return void
        */
        void NetworkConnectionNeededL(TInt* /*aConnectionPtr*/,
                                      TInt* /*aSockSvrHandle*/,
                                      TBool* /*aNewConn*/,
                                      TApBearerType* /*aBearerType*/) ;

        /**
        * Request the host applicaion to handle non-http request.
        * @since 2.8
        * @param aTypeArray Array of download parameter types. These types indicates the type of data contained within the DesArray
        * @param aDesArray Array of values associated with the types in the type array.
        * @return ETrue, If the request is handled by the host application. EFalse if not
        */
        TBool HandleRequestL(RArray<TUint>* aTypeArray, CDesCArrayFlat* aDesArray);

        /**
        * Request the host applicaion to handle downloads
        * @since 2.8
        * @param aTypeArray Array of download parameter types
        * @param aDesArray Array of values associated with the types in the type array
        * @return ETrue, If the request is handled by the host application. EFalse if not
        */
        TBool HandleDownloadL(RArray<TUint>* aTypeArray, CDesCArrayFlat* aDesArray);

    private:
	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:   // data
        // Used to send the content to the proper application.
//        CDocumentHandler* iHandler;
    };

#endif      // BRCTLSAMPLEAPPSPECIALLOADOBSERVER_H
            
// End of File

