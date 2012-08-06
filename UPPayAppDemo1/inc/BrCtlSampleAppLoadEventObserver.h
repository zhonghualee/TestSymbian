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
* Description:  Handle load progress events
*
*/



#ifndef BRCTLSAMPLEAPPLOADEVENTOBSERVER_H
#define BRCTLSAMPLEAPPLOADEVENTOBSERVER_H

// INCLUDES
#include <BrCtlInterface.h>

// FORWARD DECLARATIONS
class CBrCtlSampleAppContainer;

// CLASS DECLARATION

/**
* CBrCtlSampleAppLoadEventObserver class.
* This class inherits from the MBrCtlLoadEventObserver interface. 
* This class is used to receive load progress events.
*/
class CBrCtlSampleAppLoadEventObserver : public CBase, public MBrCtlLoadEventObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CBrCtlSampleAppLoadEventObserver* NewL(CBrCtlSampleAppContainer* aContainer);

        /**
        * Destructor.
        */
        ~CBrCtlSampleAppLoadEventObserver();

        /**
        * A load events notification
        * @since 2.8
        * @param aLoadEvent The load event   
        * @param aSize Size depends on the event
        * @param aTransactionId The transaction id of the transaction that had this event
        * @return void
        */
        void HandleBrowserLoadEventL(TBrCtlDefs::TBrCtlLoadEvent aLoadEvent, TUint aSize, TUint16 aTransactionId) ;

    private:
	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(CBrCtlSampleAppContainer* aContainer);

    private:   // data
        // Pointer to the container class associated with this observer
        CBrCtlSampleAppContainer* iContainer;
    };

#endif      // BRCTLSAMPLEAPPLOADEVENTOBSERVER_H
            
// End of File

