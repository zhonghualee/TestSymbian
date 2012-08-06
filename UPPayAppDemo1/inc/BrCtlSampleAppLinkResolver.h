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
* Description:  Observer interface which handles resolving embedded links
*
*/



#ifndef BRCTLSAMPLEAPPLINKRESOLVER_H
#define BRCTLSAMPLEAPPLINKRESOLVER_H

// INCLUDES
#include <BrCtlLinkResolver.h>
#include <BrCtlInterface.h>

#include "WebClientEngine.h"


class CWebClientEngine;
class CBrCtlSampleAppContainer;
class MVIWebClientObserver;
// CLASS DECLARATION

/**
* CBrCtlSampleAppLinkResolver class. 
* This class inherits from the MBrCtlLinkResolver interface.
* This class is used to provide a callback mechanism for receiving the content of an embedded 
* link or the content of a user-initiated load request.
*/
class CBrCtlSampleAppLinkResolver :
public CBase, 
public MBrCtlLinkResolver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CBrCtlSampleAppLinkResolver* NewL(CBrCtlSampleAppContainer* aContainer);
  
        /**
        * Destructor.
        */
        ~CBrCtlSampleAppLinkResolver();

    	/**
    	* Browser plug-in calls this method when embedded link is found. Used with ECapabilityClientResolveEmbeddedURL
        * @since 2.8
    	* @param aEmbeddedUrl The url of the embedded content
    	* @param aCurrentUrl The url of the current page
    	* @param aLoadContentType The expected type of the embedded content
    	* @param aEmbeddedLinkContent A callback interface to return the embedded content
    	* @return EFalse, if the browser will resolve the link
    	*/
        TBool ResolveEmbeddedLinkL(const TDesC& aEmbeddedUrl,
    		                       const TDesC& aCurrentUrl,
        	                       TBrCtlLoadContentType aLoadContentType, 
        	                       MBrCtlLinkContent& aEmbeddedLinkContent) ; 
		
    	/**
    	* Browser plug-in calls this method when the user requests to load content via selecting a link, or any other way. Used with ECapabilityClientNotifyURL 
        * @since 2.8
    	* @param aUrl The requested url
    	* @param aCurrentUrl The url of the current page
    	* @param aBrCtlLinkContent A callback interface to return the embedded content
    	* @return EFalse, if the browser will resolve the link
    	*/
        TBool ResolveLinkL(const TDesC& aUrl, const TDesC& aCurrentUrl,
                           MBrCtlLinkContent& aBrCtlLinkContent) ;

		/**
		* Cancel all outstanding resolving operations
        * @since 2.8
    	* @return void
    	*/
        void CancelAll() ;
//        
//    public: // Functions from base classes
//          /**
//          * From MWebClientObserver (see WebClientEngine.h)
//          */
//          void ClientEvent( const TDesC& aEventDescription );
//          void ClientHeaderReceived( const TDesC& aHeaderData );
//          void ClientBodyReceived( const TDesC8& aBodyData );
//          void HandleUrlRequestL();
        


    private:
	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(CBrCtlSampleAppContainer* aContainer);

	    /**
        * Find if the scheme if file://
        */
        TBool IsFileScheme(const TDesC& aFileName);

	    /**
        * Read the file.
        */
        HBufC8* ReadFileLC(const TDesC& aFileName);

	    /**
        * Rcognize the mime type.
        */
        HBufC* RecognizeLC(const TDesC& aFileName, const TDesC8& aData);

	            
        TBool GetItemsNameL(const TDesC& origin);
        

        void CreateOrders();
        
        void NotifyUser(const TDesC& aData);
        		

    private:   // data
        // The loaded file's name. This is different from the URL. It does not contain the scheme.
        HBufC* iFileName;
        

//        HBufC8
        HBufC* money;
        HBufC* SPName;
        HBufC* userName;
        HBufC* ID;
        HBufC* login;
        
        HBufC* orderLink;
        
        CWebClientEngine*       iClient;
        MVIWebClientObserver* iClientObserver;
    private:   // data
        // Pointer to the container class associated with this observer
        CBrCtlSampleAppContainer* iContainer;
};

#endif      // BRCTLSAMPLEAPPLINKRESOLVER_H
            
// End of File

